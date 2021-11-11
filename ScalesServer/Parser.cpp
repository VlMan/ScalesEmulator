#include "Parser.h"

QByteArray Parser::lastData = QByteArray();
int Parser::lastWeight = 0;

int Parser::ParseXK3(const QByteArray& data)
//3d 2d 30 30 31 33 32 30 28 6b 67 29 0d 0a 
{
	lastData += data;
	if (lastData.length() >= 14 && lastData.endsWith("(kg)"))
	{
		QByteArray ba = lastData;
		char* chararray = ba.data();

		auto startPtr = strstr(chararray, "=");
		auto endPtr = strstr(chararray, "(kg)");
		if ((startPtr != NULL) && (endPtr != NULL))
		{
			startPtr++;
			*(char*)endPtr = '\0';
			lastWeight = atoi(startPtr);
		}
		lastData.clear();
	}
	else if (lastData.length() > 29)
	{
		lastData.clear();
	}
	return lastWeight;
}

int Parser::ParseZemicA9(const QByteArray& data)
// Zemic A9 (без запроса, tf0) // шлет вес на 10 больше (koef д.б. 0.1)
			//02 2d 30 30 30 30 30 36 30 4b 47 20
			//02 20 30 30 30 30 31 31 30 4b 47 20
{
	lastData += data;
	if (lastData.startsWith("\0x2") && lastData.length() >= 12 && lastData.endsWith("\0x3"))
	{
		int znak = 1;
		lastData.remove(0, 1);
		if (lastData.startsWith('-'))
		{
			znak = -1;
			lastData.remove(0, 1);
		}
		else
		if (lastData.startsWith(' ') || lastData.startsWith('+'))
		{
			znak = 1;
			lastData.remove(0, 1);
		}

		lastData.truncate(7);
		bool ok;
		int temp_weight = lastData.toInt(&ok) * znak * 0.1;
		if (ok)
			lastWeight = temp_weight;
		lastData.clear();
	}
	else if (lastData.length() >= 30)
	{
		lastData.clear();
	}
	return lastWeight;
}
constexpr int size = 7;
int Parser::ParseT7E(const QByteArray& data)
// T7E
{
	//QString data = "=00.0050"; //=00.005-
	lastData += data;
	if (lastData.startsWith('=') && lastData.length() == size + 1)
	{
		// result = reverse data string
		char result[size];
		for (int i = 0; i < size; i++)
		{
			result[i] = lastData[size - i];
		}
		lastWeight = atoi(result);

		lastData.clear();
	}
	else if (lastData.length() > 9)
	{
		lastData.clear();
	}
	return lastWeight;
}
