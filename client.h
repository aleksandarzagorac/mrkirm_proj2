#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QUdpSocket>
#include "token.h"
#include "inc.h"
#include <QCoreApplication>
#include <QTime>

class Client : public QObject
{
    Q_OBJECT

private:
    int myId;
    int myPort;
    int nubmerOfClients;
    int br;


public:
    explicit Client(QObject *parent = nullptr);
    QStringList SplitMsg(QByteArray Buffer);
    QUdpSocket *socket;
    QUdpSocket *socket2;
    void ProcessingInitMsg(QStringList lista);
    void ProcessingMsg(QByteArray Buffer);
    void Delay(int);
    void Send(QByteArray, int);
    void SendInit();


public slots:

    void Recv();

};

#endif // CLIENT_H
