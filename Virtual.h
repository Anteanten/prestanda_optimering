#pragma once
#include <string>
#include <random>
#include <time.h>
#include <deque>
#include <algorithm>
#include <iostream>

class Virtual {
private:
	class Hardware {
	public:
		float d; //Average Service demand
		int util = 0; //Number of iterations hardware was active
		int queue = 0;
		std::deque<int> deque;
		int st = 0; //Service time

		float getUtil(int currTime) {
			return ((float)util / (float)currTime) * 100.0;
		}
	};

	Hardware cpu;
	Hardware disks[4];
	float cpuUtil;
	float aps; //arrival of clients per second

	int ips; //iterations per second
	int duration; //Simulations in seconds
	int iterations = 0;
	int maximumConcurrentSteady = 0;
	int maximumConcurrent = 0;
	int maxResponseTime = 0;
	int clients = 0;
	int clientsServed = 0;
	int totalQueueLength = 0;
	int totalDequeLength = 0;

	std::mt19937 random;
	
public:
	Virtual(int duration, std::mt19937 random);
	void reset();

	void run(int brokerChoice);

	int getMaxConcurrentSteady();
	int getMaxConcurrent();
	int getMaxResponseTime();
	float getCpuUtil();
	int getIps();
	int getClientsServed();

	void setCpuUtil(float value)
	{
		cpuUtil = value;
		aps = cpuUtil / cpu.d;
	}
	void setCpuD(float value)
	{
		cpu.d = value;
		aps = cpuUtil / cpu.d;
	}
	void setDiskD(int disk, float value)
	{
		disks[disk].d = value;
	}

	void incLoad();
	void decLoad();

	std::string toString();
};
