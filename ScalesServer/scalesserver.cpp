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
        QByteArray ba = dynamic_cast<QTcpSocket*>(sender())->readAll();
        if (ui->chk_parse_data->isChecked())
            server->WriteToAllClient(Parser::Parse(ba));
        else
            server->WriteToAllClient(ba);
        ui->brs_log_scales->append(QTime::currentTime().toString("[hh.mm.ss.zzz] ") + QString(ba));
        });

    connect(ui->btn_connect_scales, &QPushButton::clicked, this, [&]() {
        if (socWeight->state() == QAbstractSocket::ConnectedState)
            socWeight->disconnectFromHost();
        socWeight->connectToHost(ui->le_ip_scales->text(), ui->le_port_scales->text().toInt());
        socWeight->waitForConnected(10000);
        if (socWeight->state() == QAbstractSocket::ConnectedState)
        {
            ui->brs_log->append(QTime::currentTime().toString("[hh.mm.ss.zzz] ") + "Socket connect\n");
        }
        });

    connect(ui->btn_disconnect_scales, &QPushButton::clicked, this, [&]() {
        if (socWeight->state() == QAbstractSocket::ConnectedState)
        {
            socWeight->disconnectFromHost();
            ui->brs_log->append(QTime::currentTime().toString("[hh.mm.ss.zzz] ") + "Socket disconnect\n");
        }
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
