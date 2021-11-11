#include "scalesserver.h"

ScalesServer::ScalesServer(QWidget *parent)
    : QMainWindow(parent), socWeight(new QTcpSocket(this)), server(new ScalesTcp(3331))
{
    ui.setupUi(this);
    server->Start();

    socWeight->connectToHost("192.168.0.211", 9761);

    connect(socWeight.data(), &QTcpSocket::readyRead, this, [&]() {
        server->WriteToAllClient(QByteArray(QString::number(Parser::Parse(dynamic_cast<QTcpSocket*>(sender())->readAll())).toLocal8Bit()));
        });
}
