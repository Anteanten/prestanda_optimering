#include "Virtual.h"

Virtual::Virtual(int duration, std::mt19937 random)
{
	cpu.d = 0.0394;			// 0.0394
	disks[0].d = 0.0771;	// 0.0771
	disks[1].d = 0.1238;	// 0.1238
	disks[2].d = 0.0804;	// 0.0804
	disks[3].d = 0.2050;	// 0.2050
	cpuUtil = 0.90;			// 0.90
	aps = cpuUtil / cpu.d;
	ips = 1000;
	this->duration = duration;	
	this->random = random;
}

void Virtual::reset()
{
	iterations = 0;
	maximumConcurrentSteady = 0;
	maximumConcurrent = 0;
	maxResponseTime = 0;
	totalQueueLength = 0;
	totalDequeLength = 0;
	clients = 0;
	clientsServed = 0;
	cpu.util = 0;
	cpu.queue = 0;
	cpu.deque.clear();
	cpu.st = 0;
	for (int i = 0; i < 4; i++) {
		disks[i].util = 0;
		disks[i].queue = 0;
		disks[i].deque.clear();
		disks[i].st = 0;
	}
}

void Virtual::run(int brokerChoice)
{
	int broker = 0;
	while (iterations < ips * duration) {
		if (random() % (int)(ips / aps) == 0) {
			cpu.queue++;
			cpu.deque.push_back(iterations);
			clients++;
		}
		//if (!cpu.deque.empty())
		if (cpu.queue > 0)
		{
			totalQueueLength += cpu.queue;//(float)cpu.deque.size();
			cpu.st++;
			cpu.util++;
			if (cpu.st >= cpu.d * ips) {
				cpu.st = 0;
				cpu.queue--;
				int disk;
				float lowest;
				int client = cpu.deque.front();
				switch (brokerChoice)
				{
					case 0:
					{
						int tempBroker = broker;
						disks[broker++].queue++;
						disks[tempBroker].deque.push_back(client);
						broker = broker % 4;
						break;
					}
					case 1:
						lowest = 1000;
						disk = 0;
						for (int i = 0; i < 4; i++) {
							if (disks[i].getUtil(iterations) < lowest) {
								lowest = disks[i].getUtil(iterations);
								disk = i;
							}
						}
						disks[disk].queue++;
						disks[disk].deque.push_back(client);
						break;
					case 2:
						lowest = 1000;
						disk = 0;
						for (int i = 0; i < 4; i++) {
							if (disks[i].d * disks[i].queue < lowest) {
								lowest = disks[i].d * disks[i].queue;
								disk = i;
							}
						}
						disks[disk].queue++;
						disks[disk].deque.push_back(client);
						break;
				}
				cpu.deque.pop_front();

				int disksQueue = 0;
				for (int i = 0; i < 4; i++) {
					disksQueue += disks[i].queue;
				}

				// Steady state, assume cpu queue is 0
				if (disksQueue > maximumConcurrentSteady) {
					maximumConcurrentSteady = disksQueue;
				}

				// Not steady state
				if (disksQueue + cpu.queue > maximumConcurrent)
				{
					maximumConcurrent = disksQueue + cpu.queue;
				}
			}
		}
		for (int j = 0; j < 4; j++) {
			// if (!disks[j].deque.empty())
			if(disks[j].queue > 0)
			{
				totalQueueLength += (float)disks[j].queue;
				disks[j].st++;
				disks[j].util++;
				if (disks[j].st >= disks[j].d * ips) {
					disks[j].st = 0;
					disks[j].queue--;
					int responseTime = iterations - disks[j].deque.front();
					totalDequeLength += responseTime;
					if (responseTime > maxResponseTime)
					{
						maxResponseTime = responseTime;
					}
					disks[j].deque.pop_front();
					clientsServed++;
				}
			}
		}
		
		iterations++;
	}
}

int Virtual::getMaxConcurrentSteady()
{
	return maximumConcurrentSteady;
}

int Virtual::getMaxConcurrent()
{
	return maximumConcurrent;
}

int Virtual::getMaxResponseTime()
{
	return maxResponseTime;
}

float Virtual::getCpuUtil()
{
	return cpu.getUtil(iterations);
}

int Virtual::getIps()
{
	return ips;
}

void Virtual::incLoad()
{
	cpuUtil += 0.01;
	aps = cpuUtil / cpu.d;
}
void Virtual::decLoad()
{
	cpuUtil -= 0.01;
	aps = cpuUtil / cpu.d;
}

std::string Virtual::toString()
{
	std::string str = "";
	str += "Clients: " + std::to_string(clients) + "\n";
	str += "Clients served: " + std::to_string(clientsServed) + "\n";
	str += "avg Response Time (queue): " + std::to_string((float)totalQueueLength / (clients*ips)) + "\n";
	str += "avg Response Time (deque): " + std::to_string((float)totalDequeLength / (clientsServed*ips)) + "\n";

	str += "\n";

	str += "CPU\n";
	str += "Queue: " + std::to_string(cpu.queue) + "\n";
	str += "Util: " + std::to_string(cpu.getUtil(iterations)) + "%\n";
	str += "avg Service Demand: " + std::to_string(cpu.d) + "\n";
	for (int j = 0; j < 4; j++) {
		str += "\nDisk " + std::to_string(j + 1) + "\n";
		str += "Queue: " + std::to_string(disks[j].queue) + "\n";
		str += "Util: " + std::to_string(disks[j].getUtil(iterations)) + "%\n";
		str += "avg Service Demand: " + std::to_string(disks[j].d) + "\n";
	}
	return str;
}
