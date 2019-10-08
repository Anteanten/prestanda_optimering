#include <iostream>
#include <string>
#include <random>
#include <time.h>
#include "Virtual.h"

int main() {
	std::mt19937 random(time(0));
	Virtual v(180, random);

	for (int j = 0; j < 3; j++)
	{
		float con = 0;
		float util = 0;
		for (int i = 0; i < 100; i++) {
			v.reset();
			v.run(j);
			con += v.getMaxConcurrent();
			util += v.getCpuUtil();
		}
		std::cout << "Maximum Concurrent: " << con / 100.0 << std::endl;
		std::cout << "CPU util%: " << util / 100 << "%\n\n";

		std::cout << v.toString() << std::endl;
	}
	return 0;
}