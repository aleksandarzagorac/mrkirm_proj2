#include "client.h"

Client::Client(QObject *parent) : QObject(parent)
{
    serverPort = 1233;
    myPort = 1234;
    socket = new QUdpSocket(this);
    if (!socket->bind(QHostAddress("192.168.56.1"), quint16(myPort))) {
        qDebug("Client bind failed.");
    } else {
        qDebug("Client bind done.");
    }
    connect(socket, SIGNAL(readyRead()), this, SLOT(recv_init()));
}

void Client::send(){
    QByteArray Data;
    Data.append("Poruka od clienta");
    if (socket->writeDatagram(Data, QHostAddress("192.168.56.1"), quint16(serverPort)) <= 0)
    {
        qDebug("Did not send data");
    }else{
        qDebug()<<"Poruka poslata od clienta!";
    }
    qDebug()<< "Client ceka!";
    socket->waitForReadyRead();
}

void Client::recv_init(){
    QByteArray Buffer;
    Buffer.resize(socket->pendingDatagramSize());

    QHostAddress sender;
    quint16 senderPort;
    socket->readDatagram(Buffer.data(), Buffer.size(), &sender, &senderPort);

    qDebug()<<"Message from: "<< sender.toString();
    qDebug()<<"Message port: "<< senderPort;
    qDebug()<<"Message: "<< Buffer;

    QStringList lista =  split_msg(Buffer);
    myId = lista[0].toInt();
    myPort = lista[1].toInt();
    qDebug()<<"MY ID is "<< myId;

    socket->close();
    if (!socket->bind(QHostAddress("192.168.56.1"), myPort)) {
        qDebug("Client bind failed.");
    } else {
        qDebug()<<"Client bind done. Binded on port"<<myPort;
    }
    qDebug()<< "Client ceka!";
    socket->waitForReadyRead();
}

QStringList Client::split_msg(QByteArray Buffer)
{

    QVector<QString> vector;

    QString s = "" + Buffer;

    QRegExp rx("(\\ |\\,|\\.|\\:|\\t)"); //RegEx for ' ' or ',' or '.' or ':' or '\t'
    QStringList query = s.split(rx);

    return query;

}






















