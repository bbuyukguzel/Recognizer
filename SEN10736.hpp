/*
 * SEN10736.hpp
 *
 *  Created on: Mar 29, 2015
 *      Author: Batuhan Buyukguzel
 */

#ifndef SEN10736_HPP_
#define SEN10736_HPP_

#include <stdio.h>   /* Standard input/output definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <string>


class SEN_10736 {

private:
	std::string port_name; // It should be UART1

public:
	SEN_10736(std::string s) :
			port_name(s) {
	}

	int open_port();
	int start_read();

};


#endif /* SEN10736_HPP_ */
