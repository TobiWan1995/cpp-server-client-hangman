#include "Header.h"

//Not good Practice in here - change later
using namespace std;

void main() {

	// Initialize Winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);
	hangMan* game1 = new hangMan();

	int wsOK = WSAStartup(ver, &wsData);

	if (wsOK != 0)
	{
		cerr << "Can't initialize winsock! Quitting" << endl;
		return;
	}

	// Create a Socket 
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "Can't create a socket! Quitting" << endl;
		return;
	}

	// Bind the socket to an ip adress and port
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY; // Could also use ine_pton ....

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	// Tell Winsock the socket is for listening
	listen(listening, SOMAXCONN);

	// Wait for a connection
	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
	// could check if clientSocket == INVALID_SOCKET

	char host[NI_MAXHOST]; // Client's remote name
	char service[NI_MAXHOST]; // Service (i.e port) the client is connected on

	ZeroMemory(host, NI_MAXHOST); // same as memset(host, 0, NI_MAXHOST);
	ZeroMemory(service, NI_MAXHOST);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		cout << host << " connected on port " << service << endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << " connected on port " <<
			ntohs(client.sin_port) << endl;
	}

	// Close listening socket

	closesocket(listening);

	// While loop: accept and echo message back to client
	char buf[4096];
	int bytesReceived;
	
	//normmally should read till no more bytes an not infinite loop
	while (true)
	{	
		ZeroMemory(buf, 4096);
		// Wait for to send data
		bytesReceived = recv(clientSocket, buf, 4096, 0);
		cout << "Bytes received: " << bytesReceived << endl;

		if (bytesReceived == SOCKET_ERROR)
		{
			cerr << "Error in recv(). Quitting" << endl;
			break;
		}

		if (bytesReceived == 0)
		{
			cout << "Client disconnected " << endl;
			break;
		}

		// play hangMan

		string guess = string(buf, 0, bytesReceived);

		// set buf to 0 again
		ZeroMemory(buf, 4096);

		switch (int response = game1->game(guess)) {
		case 0: sprintf_s(buf, "You won in %i tries!", game1->tries); break;
		case -1: sprintf_s(buf, "You have %i lifes left.. You died!", response + 1); break;
		default: sprintf_s(buf, "You have %i lifes left..", response ); break;
		}

		// Echo message to Client

		send(clientSocket, buf, strlen(buf) , 0);

		ZeroMemory(buf, 4096);
	}

	// Close the socket
	closesocket(clientSocket);

	// Cleanup Winsock
	WSACleanup();

}

