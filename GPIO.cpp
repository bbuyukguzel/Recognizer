/*
 * GPIO.cpp
 *
 *  Created on: Mar 21, 2015
 *      Author: Batuhan Buyukguzel
 *     Version: 0.2
 */

#include "GPIO.hpp"


void GPIO::setup() {

	GPIO_DIR = "/sys/class/gpio/";

	// Open a file for writing.
	fs.open((GPIO_DIR + "export").c_str(), std::ios::out);
	// Write pin number to file
	fs << pin;
	// Close file stream
	fs.close();

	// Set pin direction (in/out)
	GPIO_DIR += "gpio" + pin + "/";
	fs.open((GPIO_DIR + "direction").c_str(), std::ios::out);
	if (direction.compare("out") == 0 || direction.compare("in") == 0) {
		fs << direction;
	} else {
		std::cout << "Wrong direction";
		return;
	}

	fs.close();
}


void GPIO::set_value(int state) {
	GPIO_DIR = "/sys/class/gpio/gpio" + pin + "/value";

	fs.open((GPIO_DIR).c_str(), std::ios::out);
	fs << state;
	fs.close();

}


int GPIO::get_value() {
	int value;
	GPIO_DIR = "/sys/class/gpio/gpio" + pin + "/value";

	// Open a file for reading.
	fs.open((GPIO_DIR).c_str(), std::ios::in);
	fs >> value;
	fs.close();
	return value;
}


std::string GPIO::get_direction() {
	std::string direction;
	GPIO_DIR = "/sys/class/gpio/gpio" + pin + "/direction";

	fs.open((GPIO_DIR).c_str(), std::ios::in);
	fs >> direction;
	fs.close();
	return direction;
}


void GPIO::remove() {
	GPIO_DIR = "/sys/class/gpio/unexport";

	fs.open((GPIO_DIR).c_str(), std::ios::out);
	fs << pin;
	fs.close();
}
