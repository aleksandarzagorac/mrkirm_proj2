#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QUdpSocket>
class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    QStringList split_msg(QByteArray Buffer);
    QUdpSocket *socket;



private:
    int myId;
    int myPort;
    int serverPort;



public slots:
    void send();
    //void recv();
    void recv_init();
};

#endif // CLIENT_H
