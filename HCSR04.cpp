/*
 * HCSR04.cpp
 *
 *  Created on: Mar 28, 2015
 *      Author: Batuhan Buyukguzel
 */
 
#include "GPIO.hpp"
#include <sys/time.h>

class HCSR04 {
 
private:
	GPIO trigger;
	GPIO echo;
 
public:
	HCSR04() :
		trigger("60", "out"), echo("48", "in") {
	}
	void start();
 
};
 
void HCSR04::start() {

	struct timeval start, end;
	double elapsed;

	while(1){	

		trigger.set_value(0);
		usleep(2);
		trigger.set_value(1);
		usleep(10);
		trigger.set_value(0);
		//usleep(2);
	

		while (echo.get_value() == 0) {

		}

		gettimeofday(&start, NULL);

		while (echo.get_value() != 0) {

		}

		gettimeofday(&end, NULL);

		elapsed = end.tv_usec-start.tv_usec;
		std::cout<<(elapsed/2/29.1)<<std::endl;

		usleep(100000);

	}
 
}
 

int main() {
	HCSR04 sonic;
	sonic.start();
}
