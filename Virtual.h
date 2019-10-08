#pragma once
#include <string>
#include <random>
#include <time.h>

class Virtual {
private:
	class Hardware {
	public:
		float d; //Average Service demand
		int util = 0; //Number of iterations hardware was active
		int queue = 0;
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
	int maximumConcurrent = 0;
	int clients = 0;
	float totalServiceTime = 0;


	std::mt19937 random;
public:
	Virtual(int duration, std::mt19937 random);
	void reset();

	void run(int brokerChoice);

	int getMaxConcurrent() const;
	float getCpuUtil();

	std::string toString();
};
