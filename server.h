#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QUdpSocket>
#include "initial_str.h"
#include "token.h"
#include "clients_str.h"
#include "inc.h"


class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    QUdpSocket *socket;
    int clientNumber;
    int myPort;
    int whoIsSendig;
    QByteArray MakeInitMsg(int id);
    void AddClient(int id);
    QByteArray MakeTokenFromUI();
    QByteArray MakeToken(int salje, int prima, QString poruka);
    void PrintToken(struct Token tok);

signals:

private:
    struct Clients_str clients[10];


public slots:
    void Send(QByteArray Data, int);
    void SendInit(int id);
    void Recv();

};

#endif // SERVER_H
