#include <QCoreApplication>
#include <QTextStream>
#include "client.h"
#include "server.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug()<< "1.Server\n 2. Client";
    QTextStream qtin(stdin);
    QString line = qtin.readLine();


    if(line == '1')
    {
         Server server;
         qDebug()<<"Koliko clienata zelite? ";
         line = qtin.readLine();
         server.client_number = line.toInt();
         for(int i = 0; i < line.toInt(); i++){
            server.socket->waitForReadyRead();
            server.send_init(i+1);
         }

    }else
    {
         Client client;
         client.send();
         client.socket->waitForReadyRead();

    }
    return a.exec();
}
