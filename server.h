#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QUdpSocket>
#include "initial_str.h"
#include "token.h"
#include "clients_str.h"

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    QUdpSocket *socket;
    int client_number;
    int myPort;
    int clientPortInit;
    int whoIsSendig;
    QByteArray make_init_msg(int id);
    void add_client(int id);
    QByteArray make_token_from_ui();
    QByteArray make_token(int salje, int prima, QString poruka);
    void ispisi_token(struct Token tok);

signals:

private:
    struct Clients_str clients[10];


public slots:
    void send(QByteArray Data, int);
    void send_init(int id);
    void recv();

};

#endif // SERVER_H
