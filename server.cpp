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

void Server::send(QByteArray Data, int send_to_port){


    if (socket->writeDatagram(Data, QHostAddress("192.168.56.1"),   send_to_port) <= 0)
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
QByteArray Server::make_init_msg(int id)
{
    QByteArray Data;
    QString s = QString::number(id);
    Data.append(s);
    Data.append(":");
    s = QString::number(clientPortInit + id);
    Data.append(s);
    Data.append(":");
    Data.append("initial");
    Data.append(":");
    s = QString::number(client_number);
    Data.append(s);

    return Data;
}

void Server::add_client(int id)
{
    clients[id].id = id;
    clients[id].assigned_port = clientPortInit + id;
}

QByteArray Server::make_token(int salje, int prima, QString poruka)
{
    struct Token tok;

    QString s = QString::number(salje);
    tok.source_addr = s;
    s = QString::number(prima);
    tok.dest_addr = s;
    tok.poruka = poruka;
    qDebug()<<"napravljen token";
    this->ispisi_token(tok);
    QByteArray Data;

    Data.append(qUtf8Printable(tok.poruka));
    Data.append(":");
    Data.append(qUtf8Printable(tok.dest_addr));
    Data.append(":");
    Data.append(qUtf8Printable(tok.source_addr));
    Data.append(":");
    Data.append(qUtf8Printable(tok.port));

    return Data;
}

void Server::ispisi_token(struct Token tok)
{
    qDebug()<<tok.poruka;
    qDebug()<<tok.dest_addr;
    qDebug()<<tok.source_addr;
    qDebug()<<tok.port;

}


QByteArray Server::make_token_from_ui()
{
    int salje,prima;
    QString poruka;
    qDebug()<<"Odakle krece token? 1-"<<client_number;
    QTextStream qtin(stdin);
    QString line = qtin.readLine();
    salje = line.toInt();

    this->whoIsSendig = clients[salje].assigned_port;

    qDebug()<<"Ko prima token? "<<client_number;
    line = qtin.readLine();
    prima = line.toInt();
    qDebug()<<"Sta je poruka?";
    line = qtin.readLine();
    poruka = line;
    return make_token(salje, prima, poruka);

}
























