#pragma once
#include<iostream>
#include<vector>
#include<queue>
#include<chrono>

using namespace std;

class Cache
{
private:
	int size;
	vector<int> data;

public:
	Cache();
	~Cache();

	void Input();
	void Menu();
	double Print(const int& count, const int& type, const long long& running_time);
	double LRU_2way();
	double LRU();
	double FIFO();
	double LFU();
	double RAND();
};
