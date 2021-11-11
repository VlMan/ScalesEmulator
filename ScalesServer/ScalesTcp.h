#pragma once
#include "Parser.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>

class ScalesTcp :
    public QObject
{
    Q_OBJECT;

    QMap<int, QTcpSocket*> listClient;
    QScopedPointer<QTcpServer> server;
    int port;
    QString last_error;

    void NewConnection();
    void ReadClient();
    void Disconnection();
    quint64 WriteToClient(QTcpSocket* soc, const QByteArray& ba) { 
        if (soc->state() == QAbstractSocket::ConnectedState) 
            return soc->write(ba); 
        return static_cast<quint64>(0);
    }

    int CountConnections() const { return listClient.size(); }
    void RemoveClient(QTcpSocket* soc);

public:
    explicit ScalesTcp() : server(new QTcpServer), port(0) {}
    explicit ScalesTcp(const int _port) : server(new QTcpServer), port(_port) {}
    ~ScalesTcp() {}

    bool Start(const int _port = 3331) {
        if (IsStarting()) return true;

        SetPort(_port);
        if (!server->listen(QHostAddress::Any, GetPort()))
        {
            last_error = server->errorString();
            return false;
        }
        connect(server.data(), &QTcpServer::newConnection, this, &ScalesTcp::NewConnection);
        return true;
    }

    void Stop() { if (IsStarting()) server->close(); }

    bool IsStarting() { return server->isListening(); }

    void SetPort(const int _port) { port = _port; }
    int GetPort() const { return port; }

    QString LastError() const { return last_error; }

    void WriteToAllClient(const QByteArray& ba);
};

