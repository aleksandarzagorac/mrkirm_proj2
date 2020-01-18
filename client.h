#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QUdpSocket>
#include "token.h"

class Client : public QObject
{
    Q_OBJECT

private:
    int myId;
    int myPort;
    int myInitPort;
    int serverPort;
    int nubmerOfClients;


public:
    explicit Client(QObject *parent = nullptr);
    QStringList split_msg(QByteArray Buffer);
    QUdpSocket *socket;
    void processing_init_msg(QStringList lista);
    void processing_msg(QByteArray Buffer);
    void print_token(struct Token tok);



public slots:
    void send();
    void send_init();
    void recv();

};

#endif // CLIENT_H
