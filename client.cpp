#include "client.h"

Client::Client(QObject *parent) : QObject(parent)
{
    myPort = CLIENTINITPORT;
    br = 0;
    socket = new QUdpSocket(this);
    if (!socket->bind(QHostAddress(IPADDRESS), quint16(CLIENTINITPORT))) {
        qDebug("Client bind failed.");
    } else {
        qDebug("Client bind done.");
    }
    connect(socket, SIGNAL(readyRead()), this, SLOT(Recv()));
}

void Client::SendInit(){
    QByteArray Data;
    Data.append("initial");
    if (socket->writeDatagram(Data, QHostAddress(IPADDRESS), quint16(SERVER_PORT)) <= 0)
    {
        qDebug("Did not send data");
    }else{
        qDebug()<<"Poruka poslata od clienta!";
    }
    qDebug()<< "Client ceka!";
    socket->waitForReadyRead();
}

void Client::Send(QByteArray Data, int sendToPort){
    if (socket->writeDatagram(Data, QHostAddress(IPADDRESS), quint16(sendToPort)) <= 0)
    {
        qDebug("Did not send data");
    }else{
        qDebug()<<"Poruka poslata od clienta!";
    }
    qDebug()<< "Client ceka!";
    socket->waitForReadyRead();
}


void Client::Recv(){
    QByteArray Buffer;
    if(br == 0){
        Buffer.resize(socket->pendingDatagramSize());

    }else{
        Buffer.resize(socket2->pendingDatagramSize());
    }
    QHostAddress sender;
    quint16 senderPort;

    if(br == 0){
        socket->readDatagram(Buffer.data(), Buffer.size(), &sender, &senderPort);
        br++;
    }else{

        socket2->readDatagram(Buffer.data(), Buffer.size(), &sender, &senderPort);
    }
    qDebug()<<"Message from: "<< sender.toString();
    qDebug()<<"Message port: "<< senderPort;
    qDebug()<<"Message: "<< Buffer;

    QStringList lista =  SplitMsg(Buffer);
    if(lista[2] == "initial"){
        ProcessingInitMsg(lista);
    }else{
        ProcessingMsg(Buffer);
    }

    qDebug()<< "Client ceka!";
    socket2->waitForReadyRead();

}

QStringList Client::SplitMsg(QByteArray Buffer)
{

    QVector<QString> vector;

    QString s = "" + Buffer;

    QRegExp rx("(\\ |\\,|\\.|\\:|\\t)"); //RegEx for ' ' or ',' or '.' or ':' or '\t'
    QStringList query = s.split(rx);

    return query;

}

void Client::ProcessingInitMsg(QStringList lista){
    myId = lista[0].toInt();
    myPort = lista[1].toInt();
    nubmerOfClients = lista[3].toInt();
    qDebug()<<"MY ID is "<< myId;
    socket->close();

    socket2 = new QUdpSocket(this);
    if (!socket2->bind(QHostAddress(IPADDRESS), myPort)) {
        qDebug("Client bind failed.");
    } else {
        qDebug()<<"Client bind done. Binded on port"<<myPort;
    }
    socket2->open(QAbstractSocket::ReadWrite);
    if (!socket2->isOpen()) qDebug("Could not open socket2!!!");
    if (bool(connect(socket2, &QIODevice::readyRead, this, &Client::Recv)))
        qDebug()<<"connect success!!!!!!!!";
    else
        qDebug()<<"connect failed!!!!!!!!";
}

void Client::ProcessingMsg(QByteArray Buffer)
{
    QStringList lista =  SplitMsg(Buffer);
    if(lista[1].toInt() == myId){
        qDebug()<<"Poruka je za mene!";
        qDebug()<<lista[0];
    }else{

        qDebug()<<"Poruka NIJE za mene! Saljem dalje na port"<< CLIENTINITPORT + 1 + ((myId) % nubmerOfClients);
        Delay(1500);
        Send(Buffer, CLIENTINITPORT + 1 + ((myId) % nubmerOfClients));

    }
}


void Client::Delay( int millisecondsToWait )
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}















