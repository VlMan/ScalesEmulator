#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_scalesserver.h"
#include "ScalesTcp.h"

class ScalesServer : public QMainWindow
{
    Q_OBJECT

public:
    ScalesServer(QWidget *parent = Q_NULLPTR);
    ~ScalesServer() {}

private:
    Ui::ScalesServerClass ui;
    QScopedPointer<QTcpSocket> socWeight;
    QScopedPointer<ScalesTcp> server;
};
