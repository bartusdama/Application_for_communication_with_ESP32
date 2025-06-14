#ifndef WIFISTRATEGY_H
#define WIFISTRATEGY_H

#include "communicationstrategy.h"
#include <QTcpSocket>

class WiFiStrategy : public CommunicationStrategy
{
public:
    WiFiStrategy();
    ~WiFiStrategy();

    bool connectToDevice(QString ipAndPort) override;
    void disconnect() override;
    void sendMessage(const QString &message) override;
    bool isConnected() const override;

private:
    QTcpSocket *socket;
};

#endif // WIFISTRATEGY_H
