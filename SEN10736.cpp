/*
 * SEN10736.cpp
 *
 *  Created on: Mar 28, 2015
 *      Author: Batuhan Buyukguzel
 */

#include "SEN10736.hpp"


int SEN_10736::open_port() {

	int fd; // File descriptor for the port

	/* O_RDWR: Opens the port for reading and writing
	 * O_NOCTTY: The port never becomes the controlling terminal of the process.
	 * O_NDELAY: Use non-blocking I/O. On some systems this also means the RS232 DCD signal line is ignored. */
	fd = open(port_name.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);

	if (fd == -1) {
		//Could not open the port.
		perror("open_port: Unable to open /dev/tty- ");
	} else {
		//Port opened.
		fcntl(fd, F_SETFL, 0);
	}

	return (fd);
}


int SEN_10736::start_read() {

	int mainfd = 0; //file descriptor
	char ch;
	struct termios options;

	mainfd = open_port();

	fcntl(mainfd, F_SETFL, FNDELAY); //configure port reading

	//Get the current options for the port...
	tcgetattr(mainfd, &options);

	//Set the baud rates
	cfsetispeed(&options, B57600); //set line-in speed
	cfsetospeed(&options, B57600); //set line-out speed

	//Enable the receiver and set local mode...
	options.c_cflag |= (CLOCAL | CREAD);

	//Set the new options for the port...
	//TCSANOW: the configuration is changed immediately.
	tcsetattr(mainfd, TCSANOW, &options);

	//No parity (8N1):
	options.c_cflag &= ~PARENB; //no parity bit
	options.c_cflag &= ~CSTOPB; //only one stop bit
	options.c_cflag &= ~CSIZE; //mask the character size bits
	options.c_cflag |= CS8; //select 8 data bits

	//To disable hardware flow control:
	options.c_cflag &= ~CRTSCTS;
	//To disable software flow control simply mask those bits:
	options.c_iflag &= ~(IXON | IXOFF | IXANY);

	while (1) {

		read(mainfd, &ch, sizeof(ch));
		if (ch != 0) {
			printf("%c", ch);
		}
		ch = 0;

	}

	close(mainfd); //close the serial port

	return 0;
}
