#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QtCharts>

#include "communicationstrategy.h"
#include "uartstrategy.h"
#include "wifistrategy.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void dataReceived(double value);

private slots:
    void on_pushButtonSearch_clicked();

    void on_pushButtonConnect_clicked();

    void on_pushButtonDisconnect_clicked();

    void readFromPort();

    void sendMessageTodevice(QString message);

    void on_pushButtonLedOn_clicked();

    void on_pushButtonLedOff_clicked();

    void addToLogs(QString message);

    void on_pushButtonConnectWifi_clicked();

private:
    Ui::MainWindow *ui;
    QChart *chart;
    QLineSeries *series;
    QValueAxis *axisX;
    QValueAxis *axisY;
    int chartCounter = 0;

    CommunicationStrategy *comm = nullptr;

    void setupChart();
};
#endif // MAINWINDOW_H
