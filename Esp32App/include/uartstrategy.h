#ifndef UARTSTRATEGY_H
#define UARTSTRATEGY_H

#include "communicationstrategy.h"
#include <QSerialPort>

class UARTStrategy : public CommunicationStrategy
{
public:
    UARTStrategy();
    ~UARTStrategy();

    QSerialPort* getSerialPort() const;

    bool connectToDevice(QString portName) override;
    void disconnect() override;
    void sendMessage(const QString &message) override;
    bool isConnected() const override;

private:
    QSerialPort *serial;
};

#endif // UARTSTRATEGY_H
