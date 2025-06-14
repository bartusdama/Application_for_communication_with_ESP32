#include "uartstrategy.h"
#include <QDebug>

UARTStrategy::UARTStrategy()
{
    serial = new QSerialPort();
}

UARTStrategy::~UARTStrategy()
{
    disconnect();
    delete serial;
}

bool UARTStrategy::connectToDevice(QString portName)
{
    serial->setPortName(portName);
    if (serial->open(QIODevice::ReadWrite)) {
        serial->setBaudRate(QSerialPort::Baud9600);
        serial->setDataBits(QSerialPort::Data8);
        serial->setParity(QSerialPort::NoParity);
        serial->setStopBits(QSerialPort::OneStop);
        serial->setFlowControl(QSerialPort::NoFlowControl);
        return true;
    }
    return false;
}

void UARTStrategy::disconnect()
{
    if (serial->isOpen())
        serial->close();
}

void UARTStrategy::sendMessage(const QString &message)
{
    if (serial->isOpen() && serial->isWritable()) {
        serial->write(message.toStdString().c_str());
    }
}

bool UARTStrategy::isConnected() const
{
    return serial->isOpen();
}

QSerialPort* UARTStrategy::getSerialPort() const {
    return serial;
}
