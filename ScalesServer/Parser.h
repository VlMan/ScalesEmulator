#pragma once

#include <QString>
#include <QDateTime>
#include <QByteArray>
#include <QObject>

class Parser :
    public QObject
{
    static QByteArray lastData;
    static int lastWeight;

public:
    static int Parse(const QByteArray& ba);
};