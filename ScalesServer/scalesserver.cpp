#include "scalesserver.h"

#include <QValidator>
#include <QTime>
#include <QSpinBox>

ScalesServer::ScalesServer(QWidget *parent)
    : QMainWindow(parent), socWeight(new QTcpSocket(this)), server(new ScalesTcp(3331)), ui(new Ui::ScalesServerClass), dataToSend(""), t_SendData(new QTimer(this)), currentScales("")
{
    ui->setupUi(this);

    QIntValidator* intValidator = new QIntValidator(0, 65535, this);
    ui->le_port_scales->setValidator(intValidator);
    ui->le_port_scales->setText("9761");
    ui->le_port_server->setValidator(intValidator);
    ui->sp_interval_send->setMinimum(0);
    ui->sp_interval_send->setMaximum(100000);
    ui->le_ip_scales->setText("127.0.0.1");
    ui->le_port_server->setText(QString::number(server->GetPort()));
    ui->cbox_scales_type->addItems(GetListScales());
    t_SendData->start(1000);
    ui->sp_interval_send->setValue(1000);

    connect(ui->sp_interval_send, &QSpinBox::valueChanged, this, [=](int value) {
        t_SendData->stop();
        t_SendData->setInterval(value);
        t_SendData->start();
        });

    connect(ui->le_data_to_send, &QLineEdit::textChanged, this, [=](QString str) {
        dataToSend = str;
        });

    connect(t_SendData.data(), &QTimer::timeout, this, [=]() {
        if (ui->chk_send_data->isChecked() && !dataToSend.isEmpty())
        {
            server->WriteToAllClient(dataToSend.toLocal8Bit());
            ui->brs_log_scales->append(QTime::currentTime().toString("[hh.mm.ss.zzz] Send - ") + dataToSend);
        }
        });

    connect(ui->cbox_scales_type, &QComboBox::currentTextChanged, this, [=](QString text) { // При переключении вкладки обнуляем вес
        currentScales = text;
        Parser::Zeroing();
        });

    connect(socWeight.data(), &QTcpSocket::readyRead, this, [&]() {
        QByteArray ba = dynamic_cast<QTcpSocket*>(sender())->readAll();
        QString rawDataString;

        for (char byte : ba) {
            rawDataString += QString("\\x%1").arg(static_cast<unsigned char>(byte), 2, 16, QChar('0'));
        }
        ui->brs_log_scales->append(QTime::currentTime().toString("[hh.mm.ss.zzz] ") + "Receipt - " + rawDataString);
        if (ui->chk_parse_data->isChecked())
        {
            if (ui->cbox_scales_type->currentText() == "") // Не выбраны весы - шлём сырые данные
            {
                server->WriteToAllClient(ba);
                ui->brs_log_scales->append(QTime::currentTime().toString("[hh.mm.ss.zzz] Send - ") + QString(ba));
            }
            else
            {
                int parsedData = 0;
                if (currentScales == "XK3")
                    parsedData = Parser::ParseXK3(ba);
                else if (currentScales == "Zemic A9")
                    parsedData = Parser::ParseZemicA9(ba);
                else if (currentScales == "T7E")
                    parsedData = Parser::ParseT7E(ba);

                server->WriteToAllClient(parsedData);
                ui->brs_log_scales->append(QTime::currentTime().toString("[hh.mm.ss.zzz] Send - ") + QString::number(parsedData));
            }
        }
        else
        {
            server->WriteToAllClient(ba);
            ui->brs_log_scales->append(QTime::currentTime().toString("[hh.mm.ss.zzz] Send - ") + QString(ba));
        }
        });

    connect(ui->btn_connect_scales, &QPushButton::clicked, this, [&]() {
        DisconnectSocketScales();
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
        DisconnectSocketScales();
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

void ScalesServer::DisconnectSocketScales()
{
    if (socWeight->state() == QAbstractSocket::ConnectedState)
    {
        socWeight->disconnectFromHost();
        ui->brs_log->append(QTime::currentTime().toString("[hh.mm.ss.zzz] ") + "Socket disconnect\n");
    }
}
