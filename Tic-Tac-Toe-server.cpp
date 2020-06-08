
#include "stdafx.h"
#include "Tic-Tac-Toe-game.h"
#include<winsock2.h>
#include <WS2tcpip.h>
#include<iostream>
#pragma comment (lib,"ws2_32.lib")

using namespace std;

int main()
{
	WSADATA wsadata;
	int WSAresult = WSAStartup(MAKEWORD(2, 2), &wsadata);
	if (WSAresult != 0)
	{
		cout << "WSAStartup failed with error" << WSAGetLastError << endl;
		return 1;
	}

	SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server == INVALID_SOCKET)
	{
		cout << "socket failed with error" << WSAGetLastError << endl;
		WSACleanup();
		return 1;
	}

	struct sockaddr_in serveradd;
	memset(&serveradd, 0, sizeof(serveradd));
	serveradd.sin_family = AF_INET;
	serveradd.sin_addr.s_addr = htonl(INADDR_ANY);
	serveradd.sin_port = htons(2345);

	int bindresult = bind(server, (sockaddr*)&serveradd, sizeof(serveradd));
	if (bindresult == SOCKET_ERROR)
	{
		cout << "bind failed with error" << WSAGetLastError << endl;
		closesocket(server);
		WSACleanup();
		return 1;
	}

	int listenresult = listen(server, SOMAXCONN);
	if (listenresult == SOCKET_ERROR)
	{
		cout << "listen failed with error" << WSAGetLastError << endl;
		closesocket(server);
		WSACleanup();
		return 1;
	}

	SOCKET conn;
	sockaddr_in connadd;
	int connaddlen;
	char chess[row][col];
	while (1)
	{
		cout << "waiting for connection..." << endl;
		connaddlen = sizeof(connadd);
		conn = accept(server, (sockaddr*)&connadd, &connaddlen);
		if (conn == INVALID_SOCKET)
		{
			cout << "accept failed with error" << WSAGetLastError << endl;
			continue;
		}
		else {
			char connIP[20] = { 0 };
			char message[1024] = { 0 };
			inet_ntop(AF_INET, &connadd.sin_addr, connIP, 16);
			cout << connIP << " " << ntohs(connadd.sin_port) << " connected" << endl;
			//init(chess);
			//print(chess);
			while (1)
			{
				init(chess);
				print(chess);
				int over = 0;
				while (1)
				{
					int recvresult = recv(conn, message, sizeof(message), 0);
					if (recvresult == SOCKET_ERROR)
					{
						cout << "recv failed with error" << WSAGetLastError << endl;
						over = 1;
						break;
					}
					else if (recvresult == 0)
					{
						cout << "connection closed" << endl;
						over = 1;
						break;
					}
					else {
						for (int i = 0; i < row; i++)
						{
							for (int j = 0; j < col; j++)
							{
								if (message[col*i + j] != '0' && message[col*i + j] != 'X')
								{
									chess[i][j] = ' ';
								}
								else {
									chess[i][j] = message[col*i + j];
								}
							}
						}
						print(chess);
						if (judge_win(chess) == 'X')
						{
							cout << "client胜利" << endl;
							break;
						}
						else if (judge_full(chess) == 1)
						{
							cout << "无人获胜" << endl;
							break;
						}
						player_server(chess);
						print(chess);
						for (int i = 0; i < row; i++)
						{
							for (int j = 0; j < col; j++)
							{
								if (chess[i][j] == '0' || chess[i][j] == 'X')
								{
									message[col*i + j] = chess[i][j];
								}
								else {
									message[col*i + j] = ' ';
								}
							}
						}
						int sendresult = send(conn, message, strlen(message), 0);
						if (sendresult == SOCKET_ERROR)
						{
							cout << "send failed with error" << WSAGetLastError << endl;
							break;
						}
						if (judge_win(chess) == '0')
						{
							cout << "server胜利" << endl;
							break;
						}
						else if (judge_full(chess) == 1)
						{
							cout << "无人获胜" << endl;
							break;
						}
					}
					memset(message, 0, sizeof(message));
				}
				if (over == 1)	break;
			}
		}
		closesocket(conn);
	}

	closesocket(server);
	WSACleanup();

	return 0;
}