#pragma once

#include <QByteArray>

class Parser
{
    static QByteArray lastData;
    static int lastWeight;

public:
    static int ParseXK3(const QByteArray& data);
    static int ParseZemicA9(const QByteArray& data);
    static int ParseT7E(const QByteArray& data);

    static void Zeroing() { lastData.clear(); lastWeight = 0; }
};