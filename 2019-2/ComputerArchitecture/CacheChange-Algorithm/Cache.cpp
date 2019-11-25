#include "Cache.h"

Cache::Cache()
{
	Input();
	Menu();
}

Cache::~Cache()
{
}

void Cache::Input()
{
	data.clear();

	cout << "슬롯의 개수를 입력하세요 : ";
	cin >> this->size;

	int data_size;
	cout << "데이터의 개수를 입력하세요 : ";
	cin >> data_size;

	//cout << "임의의 데이터 세트를 입력하세요 : ";
	for (int i = 0; i < data_size; i++)
	{
		/*int num;
		cin >> num;
		data.push_back(num);*/
		data.push_back(rand() % 10 + 1);
	}
	/*LRU();
	FIFO();
	LFU();
	RAND();*/
}

void Cache::Menu()
{
	int select = -1;
	while (true)
	{
		cout << "1. LRU  2. FIFO  3. LFU  4. RANDOM  5. Test Case 초기화  6. 종료  ==>";
		cin >> select;
		switch (select)
		{
		case 1:
			LRU();
			break;
		case 2:
			FIFO();
			break;
		case 3:
			LFU();
			break;
		case 4:
			RAND();
			break;
		case 5:
			Input();
			break;
		}
		if (select < 1 || select>5)
			break;
	}
	cout << "프로그램을 종료합니다." << endl;
}

double Cache::Print(const int& count, const int& type, const long long& running_time)
{
	cout << "=============================================" << endl;
	switch (type)
	{
	case 0:
		cout << "LRU Algorithm" << endl;
		break;
	case 1:
		cout << "FIFO Algorithm" << endl;
		break;
	case 2:
		cout << "LFU Algorithm" << endl;
		break;
	case 3:
		cout << "Random Change Algorithm" << endl;
		break;
	}
	cout << "데이터 세트 =>" << endl;
	for (int i : data) cout << i << "  ";
	cout << endl;
	cout << "데이터 개수 : " << data.size() << endl;
	cout << "적중 횟수 : " << count << endl;
	cout.precision(6);
	cout << "적중률 : " << (double)count / (double)data.size() << endl;
	cout << "수행 시간 : " << running_time << "ns" << endl;
	cout << "=============================================" << endl;

	return (double)count / (double)data.size();
}

double Cache::LRU_2way()
{
	chrono::system_clock::time_point start = chrono::system_clock::now();

	// true -> 첫 번째 슬롯에 적재		/ false -> 두 번재 슬롯에 적재
	bool use = true;
	int slot[2] = { -1,-1 };
	int count = 0;

	for (int i = 0; i < data.size(); i++)
	{
		// 첫 번째 슬롯에 위치한 값에 적중 -> 적중 횟수 +1, 두 번째 슬롯에 적재 준비
		if (data[i] == slot[0])
		{
			count++;
			use = false;
		}
		// 두 번째 슬롯에 위치한 값에 적중 -> 적중 횟수 +1, 첫 번째 슬롯에 적재 준비
		else if (data[i] == slot[1])
		{
			count++;
			use = true;
		}
		// 슬롯에 값이 존재하지 않음 -> use bit 값에 따라서 슬롯에 값 적재
		else
		{
			(use) ? (slot[0] = data[i]) : (slot[1] = data[i]);
			use != use;
		}
	}

	chrono::system_clock::time_point end = chrono::system_clock::now();
	return Print(count, 0, (end - start).count());
}

double Cache::LRU()
{
	chrono::system_clock::time_point start = chrono::system_clock::now();

	// pair 의 first -> use bit		/ pair 의 second -> data
	vector<pair<int, int>> slot(this->size, make_pair(0, -1));
	int count = 0;

	for (int i = 0; i < data.size(); i++)
	{
		int index = -1;
		// 슬롯 탐색
		for (int k = 0; k < slot.size(); k++)
		{
			if (data[i] == slot[k].second)
			{
				index = k;
				count++;
				break;
			}
		}

		int max_index = -1;
		int max = slot[0].first;
		/*
		data[i] 가 캐시에 이미 적재되어 있는 값이라면, 
		index 번째를 제외한 나머지의 use bit 값을 +1 해주고 index 번째 use bit 는 0으로 초기화 한다.
		
		data[i] 가 캐시에 이미 적재되어 있는 값이 아니라면,
		모든 슬롯의 use bit 를 +1 한 후에, use bit 값이 최소인 것을 제거하고 새롭게 적재한다.
		*/
		for (int k = 0; k < slot.size(); k++)
		{
			if (k != index)
			{
				slot[k].first++;
				if (max < slot[k].first)
				{
					max = slot[k].first;
					max_index = k;
				}
			}
			else
				slot[k].first = 0;
		}

		// 슬롯에 원하는 데이터가 없는 경우 데이터를 새로 적재
		if (index == -1)
		{
			slot[max_index].first = 0;
			slot[max_index].second = data[i];
		}
	}

	chrono::system_clock::time_point end = chrono::system_clock::now();
	return Print(count, 0, (end - start).count());
}

double Cache::FIFO()
{
	chrono::system_clock::time_point start = chrono::system_clock::now();

	// pair 의 first -> use bit		/ pair 의 second -> data
	vector<int> slot(this->size, -1);
	int count = 0;

	int insert = 0;
	for (int i = 0; i < data.size(); i++)
	{
		int index = -1;
		// 슬롯 탐색
		for (int k = 0; k < slot.size(); k++)
		{
			if (data[i] == slot[k])
			{
				count++;
				index = k;
				break;
			}
		}

		// 캐시 적중 실패한 경우
		if (index == -1)
		{
			// 슬롯이 포화 상태인 경우
			if (insert == slot.size())
			{
				for (int k = 1; k < slot.size(); k++)
				{
					slot[k - 1] = slot[k];
				}
				slot[insert - 1] = data[i];
			}
			else
			{
				slot[insert++] = data[i];
			}
		}
	}

	chrono::system_clock::time_point end = chrono::system_clock::now();
	return Print(count, 1, (end - start).count());
}

double Cache::LFU()
{
	chrono::system_clock::time_point start = chrono::system_clock::now();

	// pair 의 first -> use bit		/ pair 의 second -> data
	vector<pair<int, int>> slot(this->size, make_pair(0, -1));
	int count = 0;

	for (int i = 0; i < data.size(); i++)
	{
		int index = -1;
		// 슬롯 탐색
		for (int k = 0; k < slot.size(); k++)
		{
			if (data[i] == slot[k].second)
			{
				index = k;
				count++;
				break;
			}
		}

		// 캐시 적중 실패한 경우
		if (index == -1)
		{
			int min_index = 0;
			int min = slot[0].first;
			// use bit 값이 제일 작은 인덱스를 찾음
			for (int k = 0; k < slot.size(); k++)
			{
				if (min > slot[k].first)
				{
					min = slot[k].first;
					min_index = k;
				}
			}
			// 찾은 인덱스에 새로운 값을 적재, use bit 를 1 로 초기화 함
			slot[min_index].first = 1;
			slot[min_index].second = data[i];
		}
		// 캐시 적중한 경우
		else
		{
			slot[index].first++;
		}
	}

	chrono::system_clock::time_point end = chrono::system_clock::now();
	return Print(count, 2, (end - start).count());
}

double Cache::RAND()
{
	chrono::system_clock::time_point start = chrono::system_clock::now();

	// pair 의 first -> use bit		/ pair 의 second -> data
	vector<int> slot(this->size, -1);
	int count = 0;

	int num = 0;
	for (int i = 0; i < data.size(); i++)
	{
		int index = -1;
		// 슬롯 탐색
		for (int k = 0; k < slot.size(); k++)
		{
			if (data[i] == slot[k])
			{
				index = k;
				count++;
				break;
			}
		}

		// 캐시 적중 실패한 경우
		if (index == -1)
		{
			// 슬롯에 초기값 -1 이 아닌, 사용자가 적재한 값으로 포화된 경우
			if (num == slot.size())
				slot[rand() % slot.size()] = data[i];
			// 불포화 상태 -> 슬롯에 새롭게 적재 가능한 경우
			else
				slot[num++] = data[i];
		}
	}

	chrono::system_clock::time_point end = chrono::system_clock::now();
	return Print(count, 3, (end - start).count());
}
