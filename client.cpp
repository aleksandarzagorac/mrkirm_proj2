#include "client.h"

Client::Client(QObject *parent) : QObject(parent)
{
    serverPort = 1233;
    myInitPort = 1234;
    socket = new QUdpSocket(this);
    if (!socket->bind(QHostAddress("192.168.56.1"), quint16(myInitPort))) {
        qDebug("Client bind failed.");
    } else {
        qDebug("Client bind done.");
    }
    connect(socket, SIGNAL(readyRead()), this, SLOT(recv()));
}

void Client::send_init(){
    QByteArray Data;
    Data.append("initial");
    if (socket->writeDatagram(Data, QHostAddress("192.168.56.1"), quint16(serverPort)) <= 0)
    {
        qDebug("Did not send data");
    }else{
        qDebug()<<"Poruka poslata od clienta!";
    }
    qDebug()<< "Client ceka!";
    socket->waitForReadyRead();
}

void Client::send(){
    QByteArray Data;

    if (socket->writeDatagram(Data, QHostAddress("192.168.56.1"), quint16(serverPort)) <= 0)
    {
        qDebug("Did not send data");
    }else{
        qDebug()<<"Poruka poslata od clienta!";
    }
    qDebug()<< "Client ceka!";
    socket->waitForReadyRead();
}


void Client::recv(){
    QByteArray Buffer;
    Buffer.resize(socket->pendingDatagramSize());

    QHostAddress sender;
    quint16 senderPort;
    socket->readDatagram(Buffer.data(), Buffer.size(), &sender, &senderPort);

    qDebug()<<"Message from: "<< sender.toString();
    qDebug()<<"Message port: "<< senderPort;
    qDebug()<<"Message: "<< Buffer;

    QStringList lista =  split_msg(Buffer);
    if(lista[2] == "initial"){
        processing_init_msg(lista);
    }else{
        processing_msg(Buffer);
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

void Client::processing_init_msg(QStringList lista){
    myId = lista[0].toInt();
    myPort = lista[1].toInt();
    nubmerOfClients = lista[3].toInt();
    qDebug()<<"MY ID is "<< myId;
    socket->close();
    if (!socket->bind(QHostAddress("192.168.56.1"), myPort)) {
        qDebug("Client bind failed.");
    } else {
        qDebug()<<"Client bind done. Binded on port"<<myPort;
    }
    connect(socket, SIGNAL(readyRead()), this, SLOT(recv()));
}

void Client::processing_msg(QByteArray Buffer)
{
    QStringList lista =  split_msg(Buffer);
    if(lista[1].toInt() == myId){
        qDebug()<<"Poruka je za mene!";
        qDebug()<<lista[0];
    }else{
        qDebug()<<"Poruka NIJE za mene! Saljem dalje na port"<< myInitPort + (nubmerOfClients + myId + 1)%nubmerOfClients;
        if (socket->writeDatagram(Buffer, QHostAddress("192.168.56.1"), quint16(myInitPort +(nubmerOfClients + myId + 1)%nubmerOfClients)) <= 0)
        {
            qDebug("Did not send data");
        }else{
            qDebug()<<"Poruka poslata od clienta!";
        }
        if (socket->writeDatagram(Buffer, QHostAddress("192.168.56.1"), quint16(myInitPort +(nubmerOfClients + myId + 1)%nubmerOfClients)) <= 0)
        {
            qDebug("Did not send data");
        }else{
            qDebug()<<"Poruka poslata od clienta!";
        }

    }
}


void print_token(struct Token tok){


}















