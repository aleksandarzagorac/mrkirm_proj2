#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QUdpSocket>
#include "initial_str.h"

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    QUdpSocket *socket;
    int client_number;
    int myPort;
    int clientPortInit;
    QByteArray make_init_msg(int id);

signals:

private:

public slots:
    void send();
    void send_init(int id);
    void recv();

};

#endif // SERVER_H
