#include "Virtual.h"

Virtual::Virtual(int duration, std::mt19937 random)
{
	cpu.d = 0.0394; // 0.0394
	disks[0].d = 0.0771;
	disks[1].d = 0.1238;
	disks[2].d = 0.0804;
	disks[3].d = 0.2050;
	cpuUtil = 0.90;
	aps = cpuUtil / cpu.d;
	ips = 1000;
	this->duration = duration;	
	this->random = random;
}

void Virtual::reset()
{
	iterations = 0;
	maximumConcurrent = 0;
	totalServiceTime = 0;
	clients = 0;
	cpu.util = 0;
	cpu.queue = 0;
	cpu.st = 0;
	for (int i = 0; i < 4; i++) {
		disks[i].util = 0;
		disks[i].queue = 0;
		disks[i].st = 0;
	}
}

void Virtual::run(int brokerChoice)
{
	int broker = 0;
	while (iterations < ips * duration) {
		if (random() % (int)(ips / aps) == 0) {
			cpu.queue++;
			clients++;
		}
		if (cpu.queue > 0) {
			totalServiceTime += (float)cpu.queue * 1.0 / (float)ips;
			cpu.st++;
			cpu.util++;
			if (cpu.st >= cpu.d * ips) {
				cpu.st = 0;
				cpu.queue--;
				int disk;
				float lowest;
				switch (brokerChoice)
				{
					case 0:
						disks[broker++].queue++;
						broker = broker % 4;
						break;
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
						break;
				}

				int disksQueue = 0;
				for (int i = 0; i < 4; i++) {
					disksQueue += disks[i].queue;
				}
				if (disksQueue > maximumConcurrent) {
					maximumConcurrent = disksQueue;
				}
			}
		}
		for (int j = 0; j < 4; j++) {
			if (disks[j].queue > 0) {
				totalServiceTime += (float)disks[j].queue * 1.0 / (float)ips;
				disks[j].st++;
				disks[j].util++;
				if (disks[j].st >= disks[j].d * ips) {
					disks[j].st = 0;
					disks[j].queue--;
				}
			}
		}
		
		iterations++;
	}
}

int Virtual::getMaxConcurrent() const
{
	return maximumConcurrent;
}

float Virtual::getCpuUtil()
{
	return cpu.getUtil(iterations);
}

std::string Virtual::toString()
{
	std::string str = "";
	str += "Clients: " + std::to_string(clients) + "\n";
	str += "avg Service Time: " + std::to_string(totalServiceTime / clients) + "\n";

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
