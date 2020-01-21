#include "server.h"

Server::Server(QObject *parent) : QObject(parent)
{
    myPort = SERVERPORT;
    socket = new QUdpSocket(this);
    if (!socket->bind(QHostAddress(IPADDRESS), SERVERPORT)) {
        qDebug("Server bind failed.");
    } else {
        qDebug("Server bind done.");
    }
    connect(socket, SIGNAL(readyRead()), this, SLOT(Recv()));
}

void Server::SendInit(int id){
    QByteArray Data = MakeInitMsg(id);
    if (socket->writeDatagram(Data, QHostAddress(IPADDRESS),   CLIENTINITPORT) <= 0)
    {
        qDebug("Did not send data");
    }else{
        qDebug()<<"Poruka poslata od servera";
    }

}

void Server::Send(QByteArray Data, int sendToPort){


    if (socket->writeDatagram(Data, QHostAddress(IPADDRESS),   sendToPort) <= 0)
    {
        qDebug("Did not send data");
    }else{
        qDebug()<<"Poruka poslata od servera na port"<<sendToPort;
    }

}

void Server::Recv(){
    QByteArray Buffer;
    Buffer.resize(socket->pendingDatagramSize());

    QHostAddress sender;
    quint16 senderPort;
    socket->readDatagram(Buffer.data(), Buffer.size(), &sender, &senderPort);

    qDebug()<<"Message from: "<< sender.toString();
    qDebug()<<"Message port: "<< senderPort;
    qDebug()<<"Message: "<< Buffer;

}
QByteArray Server::MakeInitMsg(int id)
{
    QByteArray Data;
    QString s = QString::number(id);
    Data.append(s);
    Data.append(":");
    s = QString::number(CLIENTINITPORT + id);
    Data.append(s);
    Data.append(":");
    Data.append("initial");
    Data.append(":");
    s = QString::number(clientNumber);
    Data.append(s);

    return Data;
}

void Server::AddClient(int id)
{
    clients[id].id = id;
    clients[id].assignedPort = CLIENTINITPORT + id;
}

QByteArray Server::MakeToken(int salje, int prima, QString poruka)
{
    struct Token tok;

    QString s = QString::number(salje);
    tok.source_addr = s;
    s = QString::number(prima);
    tok.dest_addr = s;
    tok.poruka = poruka;
    qDebug()<<"napravljen token";
    this->PrintToken(tok);
    QByteArray Data;
    whoIsSendig = clients[salje].assignedPort;
    Data.append(qUtf8Printable(tok.poruka));
    Data.append(":");
    Data.append(qUtf8Printable(tok.dest_addr));
    Data.append(":");
    Data.append(qUtf8Printable(tok.source_addr));
    Data.append(":");
    Data.append(qUtf8Printable(tok.port));

    return Data;
}

void Server::PrintToken(struct Token tok)
{
    qDebug()<<tok.poruka;
    qDebug()<<tok.dest_addr;
    qDebug()<<tok.source_addr;
    qDebug()<<tok.port;

}


QByteArray Server::MakeTokenFromUI()
{
    int salje,prima;
    QString poruka;
    qDebug()<<"Odakle krece token? 1-"<<clientNumber;
    QTextStream qtin(stdin);
    QString line = qtin.readLine();
    salje = line.toInt();
    qDebug()<< "clients["<<salje<<"].assigned_port = "<< clients[salje].assignedPort;
    this->whoIsSendig = clients[salje].assignedPort;

    qDebug()<<"Ko prima token? "<<clientNumber;
    line = qtin.readLine();
    prima = line.toInt();
    qDebug()<<"Sta je poruka?";
    line = qtin.readLine();
    poruka = line;
    return MakeToken(salje, prima, poruka);

}
























