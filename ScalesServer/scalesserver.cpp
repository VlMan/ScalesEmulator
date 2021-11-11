#include "scalesserver.h"

#include <QValidator>

ScalesServer::ScalesServer(QWidget *parent)
    : QMainWindow(parent), socWeight(new QTcpSocket(this)), server(new ScalesTcp(3331)), ui(new Ui::ScalesServerClass)
{
    ui->setupUi(this);

    QIntValidator* intValidator = new QIntValidator(0, 65535, this);
    ui->le_port_scales->setValidator(intValidator);
    ui->le_port_server->setValidator(intValidator);

    ui->le_port_server->setText(QString::number(server->GetPort()));
    connect(socWeight.data(), &QTcpSocket::readyRead, this, [&]() {
        //server->WriteToAllClient(QByteArray(QString::number(Parser::Parse(dynamic_cast<QTcpSocket*>(sender())->readAll())).toLocal8Bit()));
        server->WriteToAllClient(dynamic_cast<QTcpSocket*>(sender())->readAll());
        });

    connect(ui->btn_connect_scales, &QPushButton::clicked, this, [&]() {
        if (socWeight->state() == QAbstractSocket::ConnectedState)
            socWeight->disconnectFromHost();
        socWeight->connectToHost(ui->le_ip_scales->text(), ui->le_port_scales->text().toInt());
        });

    connect(ui->btn_disconnect_scales, &QPushButton::clicked, this, [&]() {
        if (socWeight->state() == QAbstractSocket::ConnectedState)
            socWeight->disconnectFromHost();
        });

    connect(ui->btn_start_server, &QPushButton::clicked, this, [&]() {
        if (!server->IsStarting())
            if (!server->Start(ui->le_port_server->text().toInt()))
                ui->brs_log->append(QTime::currentTime().toString("[hh.mm.ss.zzz] ") + "Server not start - " + server->LastError() + "\n");
            else
                ui->brs_log->append(QTime::currentTime().toString("[hh.mm.ss.zzz] ") + "Server start\n");
        });

    connect(ui->btn_stop_server, &QPushButton::clicked, this, [&]() {
        if (server->IsStarting())
        {
            server->Stop();
            ui->brs_log->append(QTime::currentTime().toString("[hh.mm.ss.zzz] ") + "Server stop\n");
        }
        });
}
