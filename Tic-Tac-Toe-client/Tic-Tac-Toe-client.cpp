﻿// Tic-Tac-Toe-client.cpp: 定义控制台应用程序的入口点。
//

#include "Tic-Tac-Toe-game.h"
#include<winsock2.h>
#include <Ws2tcpip.h>
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

	SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (client == INVALID_SOCKET)
	{
		cout << "socket failed with error" << WSAGetLastError << endl;
		WSACleanup();
		return 1;
	}

	struct sockaddr_in serveradd;
	memset(&serveradd, 0, sizeof(serveradd));
	serveradd.sin_family = AF_INET;
	//serveradd.sin_addr.s_addr = inet_addr("127.0.0.1");
	inet_pton(AF_INET, "127.0.0.1", &serveradd.sin_addr.s_addr);
	serveradd.sin_port = htons(2345);

	int connectresult = connect(client, (sockaddr*)&serveradd, sizeof(serveradd));
	if (connectresult == SOCKET_ERROR)
	{
		cout << "connect failed with error" << WSAGetLastError << endl;
		closesocket(client);
		WSACleanup();
		return 1;
	}
	cout << "connection succeed" << endl;

	char chess[row][col];
	char sendmessage[1024] = { 0 };
	char recvmessage[1024] = { 0 };
	//init(chess);
	//print(chess);

	while (1)
	{
		cout << "游戏开始" << endl;
		init(chess);
		print(chess);
		cout << "client先落子，server后落子" << endl;
		while (1)
		{
			//cout << "please write the message" << endl;
			//cin >> sendmessage;
			//if (strcmp(sendmessage, "end") == 0)
			//{
			//	cout << "close the connection..." << endl;
			//	break;
			//}
			player_client(chess);
			print(chess);
			for (int i = 0; i < row; i++)
			{
				for (int j = 0; j < col; j++)
				{
					if (chess[i][j] == '0' || chess[i][j] == 'X')
					{
						sendmessage[col*i + j] = chess[i][j];
					}
					else {
						sendmessage[col*i + j] = ' ';
					}
				}
			}
			int sendresult = send(client, sendmessage, strlen(sendmessage), 0);
			if (sendresult == SOCKET_ERROR)
			{
				cout << "send failed with error" << WSAGetLastError << endl;
				break;
			}
			else {
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
				int recvresult = recv(client, recvmessage, sizeof(recvmessage), 0);
				if (recvresult == SOCKET_ERROR)
				{
					cout << "recv failed with error" << WSAGetLastError << endl;
					break;
				}
				else if (recvresult == 0)
				{
					cout << "connection closed" << endl;
					break;
				}
				else {
					//cout << "receive the message" << endl;
					//cout << recvmessage << endl;
					for (int i = 0; i < row; i++)
					{
						for (int j = 0; j < col; j++)
						{
							if (recvmessage[col*i + j] != '0' && recvmessage[col*i + j] != 'X')
							{
								chess[i][j] = ' ';
							}
							else {
								chess[i][j] = recvmessage[col*i + j];
							}
						}
					}
					print(chess);
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
			}
			memset(sendmessage, 0, sizeof(sendmessage));
			memset(recvmessage, 0, sizeof(recvmessage));
		}
		cout << "继续请输入1，结束请输入0" << endl;
		int a;
		cin >> a;
		if (a == 0)
		{
			cout << "游戏结束" << endl;
			break;
		}
	}

	closesocket(client);
	WSACleanup();

	return 0;
}
