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
    static int ParseXK3(const QByteArray& data);
    static int ParseZemicA9(const QByteArray& data);
    static int ParseT7E(const QByteArray& data);
};