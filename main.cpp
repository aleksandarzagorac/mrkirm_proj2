#include <QCoreApplication>
#include <QTextStream>
#include "client.h"
#include "server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug()<<"1.Server";
    qDebug()<<"2.Client";
    QTextStream qtin(stdin);
    QString line = qtin.readLine();


    if(line == '1')
    {
         Server server;
         qDebug()<<"Koliko clienata zelite? ";
         line = qtin.readLine();
         server.client_number = line.toInt();
         for(int i = 0; i < line.toInt(); i++){
            qDebug()<<"Report client"<< i+1;
            server.socket->waitForReadyRead();
            server.send_init(i+1);
            server.add_client(i+1);
         }
         QByteArray Data = server.make_token_from_ui();
         qDebug()<<"whoIsSendig"<<server.whoIsSendig;
         server.send(Data, server.whoIsSendig);
    }else
    {
         Client client;
         client.send_init() ;




    }
    return a.exec();
}
