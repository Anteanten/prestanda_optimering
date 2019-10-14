#include "Virtual.h"

void run(Virtual v)
{
	v.setCpuUtil(0.90);
	for (int j = 0; j < 3; j++)
	{
		std::cout << "Broker " << j << std::endl;
		float con = 0;
		float util = 0;
		float tps = 0;
		for (int i = 0; i < 100; i++) {
			v.reset();
			v.run(j);
			con += v.getMaxConcurrentSteady();
			util += v.getCpuUtil();
			tps += v.getClientsServed() / 180.0;
		}
		std::cout << "avg Maximum Concurrent: " << con / 100.0 << std::endl;
		std::cout << "avg CPU util%: " << util / 100 << "%\n";
		std::cout << "avg Throughput: " << tps / 100 << " tps\n\n";

		std::cout << v.toString() << "____________________\n" << std::endl;
	}
}
void runMaxResponse(Virtual v)
{
	for (int j = 0; j < 3; j++)
	{
		v.setCpuUtil(2.50);
		std::cout << "Broker " << j << std::endl;
		float maxCon = 0;
		float maxRes = 0;
		float con = 0;
		float res = 0;
		float util = 0;
		float tps = 0;
		int num = 0;
		int ips;
		while (num < 100) {
			v.reset();
			v.run(j);
			maxRes = v.getMaxResponseTime();
			ips = v.getIps();
			if (maxRes < 20 * ips)
			{
				res += maxRes;
				util += v.getCpuUtil();
				tps += v.getClientsServed() / 180.0;
				con += v.getMaxConcurrent();
				num++;
				v.incLoad();
				if (v.getMaxConcurrent() > maxCon)
					maxCon = v.getMaxConcurrent();
			}
			else
			{
				v.decLoad();
			}
		}

		std::cout << "Maximum Concurrent: " << maxCon << std::endl;
		std::cout << "avg Maximum Concurrent: " << con / num << std::endl;
		std::cout << "avg Maximum Response Time: " << res / (num * ips) << std::endl;
		std::cout << "CPU util%: " << util / num << "%\n";
		std::cout << "avg Throughput: " << tps / 100 << " tps\n\n";

		// std::cout << v.toString();
		std::cout << "____________________\n" << std::endl;
	}
}

int main() {
	std::mt19937 random(time(0));
	Virtual v(180, random);

	char q, imp;
	bool exit = false;

	do
	{
	std::cout << "\nChoose question (0 to exit): ";
	std::cin >> q;

	switch (q)
	{
	case '1':
		// Question 1
		v.setDiskD(3, 0.2050);
		v.setCpuD(0.0394);
		run(v);
		break;
	case '2':
	case '3':
		// Question 2 & 3
		v.setDiskD(3, 0.2050);
		v.setCpuD(0.0394);
		runMaxResponse(v);
		break;
	case '4':
		// Question 4
		std::cout << "Choose improvement: \n1. CPU\n2. Disk\n3. Both\n";
		std::cin >> imp;
		switch (imp)
		{
			case '1':
				v.setCpuD(0.0394/2);
				v.setDiskD(3, 0.2050);
				break;
			case '2':
				v.setCpuD(0.0394);
				v.setDiskD(3, 0.0771);
				break;
			case '3':
				v.setCpuD(0.0394/2);
				v.setDiskD(3, 0.0771);
				break;
		}
		runMaxResponse(v);
		break;
	case '5':
		// Question 5
		v.setDiskD(3, 0.2050);
		v.setCpuD(0.137);
		runMaxResponse(v);
		break;
	case '6':
		// Question 6
		v.setDiskD(3, 0.1771);
		v.setCpuD(0.0394);
		runMaxResponse(v);
		break;
	case '0':
		exit = true;
		break;
	default:
		std::cout << "Not valid\n";
		break;
	}
	} while(!exit);

	return 0;
}