#include "Parser.h"

QByteArray Parser::lastData = QByteArray();
int Parser::lastWeight = 0;

int Parser::Parse(const QByteArray& data)
//3d 2d 30 30 31 33 32 30 28 6b 67 29 0d 0a 
{
	lastData += data;
	if (lastData.length() >= 14 && lastData.endsWith('\n'))
	{
		QByteArray ba = data;
		char* chararray = ba.data();

		auto startPtr = strstr(chararray, "=");
		auto endPtr = strstr(chararray, "(kg)");
		if ((startPtr != NULL) && (endPtr != NULL))
		{
			startPtr++;
			*(char*)endPtr = '\0';
			lastWeight = atoi(startPtr);
		}
	}
	else if (lastData.length() > 29)
	{
		lastData.clear();
	}
	return lastWeight;
}