#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_scalesserver.h"
#include "ScalesTcp.h"

class ScalesServer : public QMainWindow
{
    Q_OBJECT

public:
    ScalesServer(QWidget *parent = Q_NULLPTR);
    ~ScalesServer() { delete ui; }

private:
    Ui::ScalesServerClass *ui;
    QScopedPointer<QTcpSocket> socWeight;
    QScopedPointer<ScalesTcp> server;

    QStringList GetListScales() const {
        return {
            "XK3",
            "Zemic A9",
            "T7E"
        };
    }
};
