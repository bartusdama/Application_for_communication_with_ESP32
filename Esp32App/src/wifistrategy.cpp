#include "wifistrategy.h"
#include <QStringList>

WiFiStrategy::WiFiStrategy()
{
    socket = new QTcpSocket();
}

WiFiStrategy::~WiFiStrategy()
{
    disconnect();
    delete socket;
}

bool WiFiStrategy::connectToDevice(QString ipAndPort)
{
    QStringList parts = ipAndPort.split(":");
    if (parts.size() != 2)
        return false;

    QString ip = parts[0];
    quint16 port = parts[1].toUInt();

    socket->connectToHost(ip, port);
    return socket->waitForConnected(3000);
}

void WiFiStrategy::disconnect()
{
    if (socket->isOpen())
        socket->disconnectFromHost();
}

void WiFiStrategy::sendMessage(const QString &message)
{
    if (socket->state() == QAbstractSocket::ConnectedState) {
        socket->write(message.toUtf8());
    }
}

bool WiFiStrategy::isConnected() const
{
    return socket->state() == QAbstractSocket::ConnectedState;
}

