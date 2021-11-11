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
    ui->cbox_scales_type->addItems(GetListScales());

    connect(socWeight.data(), &QTcpSocket::readyRead, this, [&]() {
        QByteArray ba = dynamic_cast<QTcpSocket*>(sender())->readAll();
        if (ui->chk_parse_data->isChecked())
        {
            if (ui->cbox_scales_type->currentText() == "")
            {
                server->WriteToAllClient(ba);
                ui->brs_log_scales->append(QTime::currentTime().toString("[hh.mm.ss.zzz] ") + QString(ba));
            }
            else
            {
                int parsedData = 0;
                if (ui->cbox_scales_type->currentText() == "XK3")
                    parsedData = Parser::ParseXK3(ba);
                else if (ui->cbox_scales_type->currentText() == "Zemic A9")
                    parsedData = Parser::ParseZemicA9(ba);
                else if (ui->cbox_scales_type->currentText() == "T7E")
                    parsedData = Parser::ParseT7E(ba);

                server->WriteToAllClient(parsedData);
                ui->brs_log_scales->append(QTime::currentTime().toString("[hh.mm.ss.zzz] ") + QString::number(parsedData));
            }
        }
        else
        {
            server->WriteToAllClient(ba);
            ui->brs_log_scales->append(QTime::currentTime().toString("[hh.mm.ss.zzz] ") + QString(ba));
        }
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
        else
            ui->brs_log->append(QTime::currentTime().toString("[hh.mm.ss.zzz] ") + "Socket not connect - " + socWeight->errorString() + "\n");
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
