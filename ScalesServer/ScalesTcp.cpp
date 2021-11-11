#include "ScalesTcp.h"

void ScalesTcp::NewConnection()
{
	auto p_socket = server->nextPendingConnection();

	connect(p_socket, &QTcpSocket::disconnected, this, &ScalesTcp::Disconnection);
	connect(p_socket, &QTcpSocket::readyRead, this, &ScalesTcp::ReadClient);

	if(CountConnections() != 0)
		listClient.insert(listClient.lastKey() + 1, p_socket);
	else
		listClient.insert(1, p_socket);
}

void ScalesTcp::ReadClient()
{
	const auto p_socket = (QTcpSocket*)sender();
	if (!p_socket->bytesAvailable()) {
		return;
	}
	// Clients must somth send? No.
}

void ScalesTcp::Disconnection()
{
	const auto p_socket = (QTcpSocket*)sender();

	RemoveClient(p_socket);
}

void ScalesTcp::RemoveClient(QTcpSocket* soc)
{
	listClient.remove(listClient.key(soc));
	soc->deleteLater();
}

void ScalesTcp::WriteToAllClient(const QByteArray& ba)
{
	for (const auto& value : listClient.values())
		WriteToClient(value, ba);
}
