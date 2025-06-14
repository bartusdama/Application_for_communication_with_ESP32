#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QList>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupChart();

    connect(this, &MainWindow::dataReceived, this, [=](double value) {
        series->append(chartCounter++, value);
        if (chartCounter > 100) {
            axisX->setRange(chartCounter - 100, chartCounter);
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonSearch_clicked()
{
    this->addToLogs("Looking for devices...");
    QList<QSerialPortInfo> devices;
    devices = QSerialPortInfo::availablePorts();
    for (int i = 0; i < devices.count(); i++) {
        this->addToLogs("Found device: " + devices.at(i).portName() + " " + devices.at(i).description());
        ui->comboBoxDevices->addItem(devices.at(i).portName() + " " + devices.at(i).description());
    }
}

void MainWindow::on_pushButtonConnect_clicked()
{
    if (ui->comboBoxDevices->count() == 0) {
        this->addToLogs("No devices detected!");
        return;
    }

    QString portName = ui->comboBoxDevices->currentText().split(" ").first();
    this->addToLogs(portName);

    if (comm)
        delete comm;

    comm = new UARTStrategy();

    if (comm->connectToDevice(portName)) {
        this->addToLogs("UART connected successfully.");
        UARTStrategy* uart = dynamic_cast<UARTStrategy*>(comm);
        if (uart) {
            connect(uart->getSerialPort(), SIGNAL(readyRead()), this, SLOT(readFromPort()));
        }
    } else {
        this->addToLogs("UART connection failed.");
        delete comm;
        comm = nullptr;
    }
}


void MainWindow::on_pushButtonDisconnect_clicked()
{
    if (comm) {
        comm->disconnect();
        this->addToLogs("Disconnect.");
        delete comm;
        comm = nullptr;
    } else {
        this->addToLogs("No active connection.");
    }
}

void MainWindow::readFromPort()
{
    auto *uart = dynamic_cast<UARTStrategy*>(comm);
    if(!uart)
        return;

    QSerialPort* port = uart->getSerialPort();

    while (port->canReadLine()) {
        QString line = port->readLine().trimmed();

        if (line.startsWith("TEMP:")) {
            bool ok;
            double value = line.mid(5).toDouble(&ok);
            if (ok) {
                emit dataReceived(value);
            }
        }
    }
}

void MainWindow::sendMessageTodevice(QString message)
{
    if (comm && comm->isConnected()) {
        this->addToLogs("Sending: " + message);
        comm->sendMessage(message);
    } else {
        this->addToLogs("Not connected. Cannot send!");
    }
}


void MainWindow::on_pushButtonLedOn_clicked()
{
    sendMessageTodevice("1");
}


void MainWindow::on_pushButtonLedOff_clicked()
{
    sendMessageTodevice("0");
}

void MainWindow::addToLogs(QString message)
{
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
    ui->textEditLogs->append(currentDateTime + "\t" + message);
}


void MainWindow::on_pushButtonConnectWifi_clicked()
{
    QString address = ui->lineEditWifiAddress->text();
    if (address.isEmpty()) {
        this->addToLogs("Please enter IP:PORT to connect");
        return;
    }

    this->addToLogs("Trying to connect to: " + address);

    if (comm)
        delete comm;

    comm = new WiFiStrategy();

    if (comm->connectToDevice(address)) {
        this->addToLogs("WiFi connected successfully.");
    } else {
        this->addToLogs("WiFi conncection failed.");
        delete comm;
        comm = nullptr;
    }
}

void MainWindow::setupChart()
{
    series = new QLineSeries();
    chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Dane z czujnika");
    chart->legend()->hide();

    axisX = new QValueAxis();
    axisX->setRange(0, 100);
    axisX->setLabelFormat("%d");
    axisX->setTitleText("Czas");

    axisY = new QValueAxis();
    axisY->setRange(0, 100);
    axisY->setTitleText("Wartość");

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(ui->layoutChart->layout());
    if (layout)
        layout->addWidget(chartView);
}

