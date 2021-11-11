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
    static int ParseXK3(const QByteArray& ba);
};