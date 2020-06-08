#pragma once
#include<iostream>
#include<time.h>
#define row 3
#define col 3
using namespace std;

void init(char chess[row][col]);
void print(char chess[row][col]);
char judge_win(char chess[row][col]);
int judge_full(char chess[row][col]);
void player_server(char chess[row][col]);
void player_client(char chess[row][col]);

