#include "Virtual.h"

int main() {
	std::mt19937 random(time(0));
	Virtual v(180, random);

	char q;
	bool exit = false;

	do
	{
	std::cout << "\nChoose question: ";
	std::cin >> q;

	switch (q)
	{
	case '1':
		// Question 1
		v.setCpuUtil(0.90);
		for (int j = 0; j < 3; j++)
		{
			std::cout << "Broker " << j << std::endl;
			float con = 0;
			float util = 0;
			for (int i = 0; i < 100; i++) {
				v.reset();
				v.run(j);
				con += v.getMaxConcurrentSteady();
				util += v.getCpuUtil();
			}
			std::cout << "Maximum Concurrent: " << con / 100.0 << std::endl;
			std::cout << "CPU util%: " << util / 100 << "%\n\n";

			std::cout << v.toString() << "____________________\n" << std::endl;
		}
		break;
	case '2':
	case '3':
		// Question 2 & 3
		for (int j = 0; j < 3; j++)
		{
			v.setCpuUtil(2.50);
			std::cout << "Broker " << j << std::endl;
			float maxCon = 0;
			float maxRes = 0;
			float con = 0;
			float res = 0;
			float util = 0;
			int num = 0;
			int totNum = 0;
			int ips;
			while (num < 100) {
				v.reset();
				v.run(j);
				maxRes = v.getMaxResponseTime();
				res += maxRes;
				util += v.getCpuUtil();
				ips = v.getIps();
				if (maxRes < 20 * ips)
				{
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
				totNum++;
			}

			std::cout << "Maximum Concurrent: " << maxCon << std::endl;
			std::cout << "avg Maximum Concurrent: " << con / num << std::endl;
			std::cout << "avg Maximum Response Time: " << res / (totNum * ips) << std::endl;
			std::cout << "CPU util%: " << util / totNum << "%\n\n";

			// std::cout << v.toString();
			std::cout << "____________________\n" << std::endl;
		}
		break;
	default:
		exit = true;
		break;
	}
	} while(!exit);

	return 0;
}