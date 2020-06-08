

#include"Tic-Tac-Toe-game.h"

void init(char chess[row][col])
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			chess[i][j] = ' ';
		}
	}
}

void print(char chess[row][col])
{
	cout << " — — —" << endl;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			cout << "| " << chess[i][j];
		}
		cout << "|" << endl;
		cout << " — — —" << endl;
	}
}

char judge_win(char chess[row][col])
{
	for (int i = 0; i < row; i++)
	{
		if ((chess[i][0] == chess[i][1]) && (chess[i][1] == chess[i][2]) && (chess[i][0] != ' '))
		{
			return chess[i][0];
		}
	}
	for (int j = 0; j < col; j++)
	{
		if ((chess[0][j] == chess[1][j]) && (chess[1][j] == chess[2][j]) && (chess[0][j] != ' '))
		{
			return chess[0][j];
		}
	}
	if ((chess[0][0] == chess[1][1]) && (chess[1][1] == chess[2][2]) && (chess[0][0] != ' '))
	{
		return chess[0][0];
	}
	else if ((chess[0][2] == chess[1][1]) && (chess[1][1] == chess[2][0]) && (chess[0][2] != ' '))
	{
		return chess[0][2];
	}
	return ' ';
}

int judge_full(char chess[row][col])
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if (chess[i][j] == ' ')
			{
				return 0;
			}
		}
	}
	return 1;
}

void player_server(char chess[row][col])
{
	int x, y;
	while (1)
	{
		cout << "玩家请落子（输入x和y坐标）" << endl;
		cin >> x >> y;
		if (chess[x - 1][y - 1] == ' ' && x >0 && x <= row && y > 0 && y <= col)
		{
			chess[x - 1][y - 1] = '0';
			break;
		}
		else {
			cout << "落子失败，请重新落子" << endl;
		}
	}
}

void player_client(char chess[row][col])
{
	int x, y;
	while (1)
	{
		cout << "玩家请落子（输入x和y坐标）" << endl;
		cin >> x >> y;
		if (chess[x - 1][y - 1] == ' ' && x >0 && x <= row && y > 0 && y <= col)
		{
			chess[x - 1][y - 1] = 'X';
			break;
		}
		else {
			cout << "落子失败，请重新落子" << endl;
		}
	}
}
