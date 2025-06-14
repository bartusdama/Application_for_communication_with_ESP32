#include <QtTest/QtTest>
#include "uartstrategy.h"

class TestUARTStrategy : public QObject {
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testConnectToInvalidPort();
    void testConnectToValidPort();
    void testIsConnectedBeforeAndAfterConnect();
    void testSendMessageWhenDisconnected();
    void testDisconnectBehavior();
};

void TestUARTStrategy::initTestCase() {
    qDebug() << "Starting tests UARTStrategy...";
}

void TestUARTStrategy::cleanupTestCase() {
    qDebug() << "Tests ended";
}

void TestUARTStrategy::testConnectToInvalidPort() {
    UARTStrategy uart;
    QVERIFY(!uart.connectToDevice("COM_fake"));
    QVERIFY(!uart.connectToDevice("/dev/fake"));
}

void TestUARTStrategy::testIsConnectedBeforeAndAfterConnect() {
    UARTStrategy uart;
    QVERIFY(!uart.isConnected());

    QString realPort = "";
    if (!realPort.isEmpty()) {
        bool connected = uart.connectToDevice(realPort);
        QVERIFY(connected);
        QVERIFY(uart.isConnected());
    } else {
        QSKIP("Skip");
    }
}

void TestUARTStrategy::testSendMessageWhenDisconnected() {
    UARTStrategy uart;
    QVERIFY(!uart.isConnected());

    QVERIFY_NO_THROW(uart.sendMessage("test"));
}

void TestUARTStrategy::testDisconnectBehavior() {
    UARTStrategy uart;
    QVERIFY(!uart.isConnected());

    QString realPort = "";
    if (!realPort.isEmpty()) {
        QVERIFY(uart.connectToDevice(realPort));
        QVERIFY(uart.isConnected());

        uart.disconnect();
        QVERIFY(!uart.isConnected());
    } else {
        QSKIP("Skip");
    }
}

void TestUARTStrategy::testConnectToValidPort() {
    UARTStrategy uart;
    QString realPort = "";

    if (!realPort.isEmpty()) {
        QVERIFY(uart.connectToDevice(realPort));
        QVERIFY(uart.isConnected());
    } else {
        QSKIP("Skip");
    }
}

QTEST_MAIN(TestUARTStrategy)
#include "test_uartstrategy.moc"
