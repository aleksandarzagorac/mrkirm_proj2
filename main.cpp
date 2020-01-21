#include <QCoreApplication>
#include <QTextStream>
#include "client.h"
#include "server.h"

void FunctionServer();
void FunctionClient();
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug()<<"Ovaj terminal se ponasa kao:";
    qDebug()<<"1.Server";
    qDebug()<<"2.Client";

    QTextStream qtin(stdin);
    QString line = qtin.readLine();

    if(line == '1')
    {
        FunctionServer();
    }else if(line == '2')
    {
        FunctionClient();
    }
    return a.exec();
}

void FunctionServer()
{
    Server server;

    qDebug()<<"Koliko clienata zelite? ";
    QTextStream qtin1(stdin);
    QString line = qtin1.readLine();

    server.clientNumber = line.toInt();
    qDebug()<<server.clientNumber;
    for(int i = 0; i < line.toInt(); i++){
       qDebug()<<"Report client"<< i+1;
       server.socket->waitForReadyRead();
       server.SendInit(i+1);
       server.AddClient(i+1);
    }


    QByteArray Data = server.MakeTokenFromUI();
    qDebug()<<"whoIsSendig"<<server.whoIsSendig;
    server.Send(Data, server.whoIsSendig);


}

void FunctionClient()
{
    Client client;
    client.SendInit();
}
