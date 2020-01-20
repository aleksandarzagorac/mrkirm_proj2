#include "client.h"

Client::Client(QObject *parent) : QObject(parent)
{
    serverPort = 1233;
    myInitPort = 1234;
    br = 0;
    socket = new QUdpSocket(this);
    if (!socket->bind(QHostAddress(ipAddres), quint16(myInitPort))) {
        qDebug("Client bind failed.");
    } else {
        qDebug("Client bind done.");
    }
    connect(socket, SIGNAL(readyRead()), this, SLOT(recv()));
}

void Client::send_init(){
    QByteArray Data;
    Data.append("initial");
    if (socket->writeDatagram(Data, QHostAddress(ipAddres), quint16(serverPort)) <= 0)
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

    if (socket->writeDatagram(Data, QHostAddress(ipAddres), quint16(serverPort)) <= 0)
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
    if(br == 0){
        qDebug()<<"1 PUT U RECV";
        Buffer.resize(socket->pendingDatagramSize());

    }else{
        qDebug()<<"2 PUT U RECV";
        Buffer.resize(socket2->pendingDatagramSize());
    }
    QHostAddress sender;
    quint16 senderPort;


    if(br == 0){
        qDebug()<<"1 PUT U RECV";
        socket->readDatagram(Buffer.data(), Buffer.size(), &sender, &senderPort);
        br++;
    }else{
        qDebug()<<"2 PUT U RECV";
        socket2->readDatagram(Buffer.data(), Buffer.size(), &sender, &senderPort);
    }
    qDebug()<<"Message from: "<< sender.toString();
    qDebug()<<"Message port: "<< senderPort;
    qDebug()<<"Message: "<< Buffer;

    QStringList lista =  split_msg(Buffer);
    if(lista[2] == "initial"){
        processing_init_msg(lista);
    }else{
        processing_msg(Buffer);
    }
    qDebug()<< "Da li da klient ceka ili da posalje nesto?";
    QTextStream qtin(stdin);
    QString line = qtin.readLine();
    if(line == '1'){
        qDebug()<< "Client ceka!";
        socket2->waitForReadyRead();
    }else{
        qDebug("------------------------------------------------------------------");
        qDebug()<<"IP for sending datagram is " << ipAddres << " and port " << 1235;
        qDebug("------------------------------------------------------------------");
        Buffer.append("idemo malena");
        if (socket2->writeDatagram(Buffer, QHostAddress(ipAddres), 1235) < 0)
        {
            qDebug("Did not send data");
        }else{
            qDebug()<<"Poruka poslata od clienta!";
        }
//        /socket2->waitForBytesWritten();
    }
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

    socket2 = new QUdpSocket(this);
    if (!socket2->bind(QHostAddress(ipAddres), myPort)) {
        qDebug("Client bind failed.");
    } else {
        qDebug()<<"Client bind done. Binded on port"<<myPort;
    }
    socket2->open(QAbstractSocket::ReadWrite);
    if (!socket2->isOpen()) qDebug("Could not open socket2!!!");
    if (bool(connect(socket2, &QIODevice::readyRead, this, &Client::recv)))
        qDebug()<<"connect success!!!!!!!!";
    else
        qDebug()<<"connect failed!!!!!!!!";
}

void Client::processing_msg(QByteArray Buffer)
{
    QStringList lista =  split_msg(Buffer);
    if(lista[1].toInt() == myId){
        qDebug()<<"Poruka je za mene!";
        qDebug()<<lista[0];
    }else{
        qDebug()<<"Poruka NIJE za mene! Saljem dalje na port"<< myInitPort + (nubmerOfClients + myId + 1)%nubmerOfClients;
        if (socket2->writeDatagram(Buffer, QHostAddress(ipAddres), quint16(myInitPort +(nubmerOfClients + myId + 1)%nubmerOfClients)) <= 0)
        {
            qDebug("Processing_msg --> Did not send data");
        }else{
            qDebug()<<"Poruka poslata od clienta!";
        }
        if (socket2->writeDatagram(Buffer, QHostAddress(ipAddres), quint16(myInitPort +(nubmerOfClients + myId + 1)%nubmerOfClients)) <= 0)
        {
            qDebug("Did not send data");
        }else{
            qDebug()<<"Poruka poslata od clienta!";
        }

    }
}


void print_token(struct Token tok){


}















