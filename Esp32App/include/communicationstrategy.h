#ifndef COMMUNICATIONSTRATEGY_H
#define COMMUNICATIONSTRATEGY_H

#include <QString>

class CommunicationStrategy
{
public:
    virtual ~CommunicationStrategy() {}

    virtual bool connectToDevice(QString addressOrPort) = 0;
    virtual void disconnect() = 0;
    virtual void sendMessage(const QString &message) = 0;
    virtual bool isConnected() const = 0;
};

#endif // COMMUNICATIONSTRATEGY_H
