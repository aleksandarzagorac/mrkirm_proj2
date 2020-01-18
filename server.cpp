#include "server.h"

Server::Server(QObject *parent) : QObject(parent)
{
    myPort = 1233;
    clientPortInit = 1234;
    socket = new QUdpSocket(this);
    if (!socket->bind(QHostAddress("192.168.56.1"), myPort)) {
        qDebug("Server bind failed.");
    } else {
        qDebug("Server bind done.");
    }
    connect(socket, SIGNAL(readyRead()), this, SLOT(recv()));
}

void Server::send_init(int id){
    QByteArray Data = make_init_msg(id);
    if (socket->writeDatagram(Data, QHostAddress("192.168.56.1"),   clientPortInit) <= 0)
    {
        qDebug("Did not send data");
    }else{
        qDebug()<<"Poruka poslata od servera";
    }

}

void Server::send(){
    QByteArray Data;

    if (socket->writeDatagram(Data, QHostAddress("192.168.56.1"),   clientPortInit) <= 0)
    {
        qDebug("Did not send data");
    }else{
        qDebug()<<"Poruka poslata od servera";
    }

}

void Server::recv(){
    QByteArray Buffer;
    Buffer.resize(socket->pendingDatagramSize());

    QHostAddress sender;
    quint16 senderPort;
    socket->readDatagram(Buffer.data(), Buffer.size(), &sender, &senderPort);

    qDebug()<<"Message from: "<< sender.toString();
    qDebug()<<"Message port: "<< senderPort;
    qDebug()<<"Message: "<< Buffer;

}
QByteArray Server::make_init_msg(int id){
    QByteArray Data;
    QString s = QString::number(id);
    Data.append(s);
    Data.append(":");
    s = QString::number(clientPortInit + id);
    Data.append(s);
    return Data;
}
































