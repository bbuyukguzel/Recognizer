/*
 * PWM.cpp
 *
 *  Created on: Apr 4, 2015
 *      Author: Batuhan Buyukguzel
 */


#include <fstream>
#include <iostream>
#include <string>
#include <glob.h>

/*
* TODO: look GLOB_TILDE parameter
*/

class PWM{

private:
	std::string capemgr;
	std::string ocp;
	std::fstream fs; // File stream

public:
	void set_pwm(std::string pin);
	void load_pwm_firmware();
	void add_suffix(std::string& pattern);
	void set_period(int period);
	void set_duty(int duty);
	int get_period();
	int get_duty();

};


int PWM::get_duty(){
	int duty;
	std::string d_dir = ocp + "/duty";

	fs.open(d_dir.c_str(), std::ios::in);
	fs >> duty;
	fs.close();
	return duty;
	
}

int PWM::get_period(){
	int period;
	std::string p_dir = ocp + "/period";

	fs.open(p_dir.c_str(), std::ios::in);
	fs >> period;
	fs.close();
	return period;

}


void PWM::set_duty(int duty){
	std::string d_dir = ocp + "/duty";

	fs.open((d_dir.c_str()), std::ios::out);
	if(!fs.is_open()){
		std::cerr << "Error: opening file";
		return;
	}

	fs << duty;
	fs.close();

}


void PWM::set_period(int period){
	std::string p_dir = ocp + "/period";

	fs.open((p_dir.c_str()), std::ios::out);
	if(!fs.is_open()){
		std::cerr << "Error: opening file";
		return;
	}

	fs << period;
	fs.close();

}



// echo bone_pwm_P8_13 > /sys/devices/bone_capemgr.?/slots
void PWM::set_pwm(std::string pin){

	fs.open((capemgr.c_str()), std::ios::out);
	if(!fs.is_open()){
		std::cerr << "Error: opening file";
		return;
	}

	fs << "bone_pwm_" + pin;
	fs.close();

	// cd /sys/devices/ocp.?/pwm_test_P8_45.??/
	ocp = "/sys/devices/ocp.";
	add_suffix(ocp);
	ocp += ("/pwm_test_" + pin);
	add_suffix(ocp);

}


// echo am33xx_pwm > /sys/devices/bone_capemgr.?/slots
void PWM::load_pwm_firmware(){

	capemgr = "/sys/devices/bone_capemgr.";
	add_suffix(capemgr);
	capemgr += "/slots";

	fs.open((capemgr.c_str()), std::ios::out);
	if(!fs.is_open()){
		std::cerr << "Error: opening file";
		return;
	}

	fs << "am33xx_pwm";
	fs.close();

}


void PWM::add_suffix(std::string& pattern){

	glob_t glob_result;
	glob((pattern+"*").c_str(), GLOB_TILDE, NULL, &glob_result);

	if(glob_result.gl_pathc > 0){
		pattern = std::string(glob_result.gl_pathv[0]);
		globfree(&glob_result);
	}
	else{
		std::cerr << "Error: finding related number";
		return;
	}

}




int main(){
	PWM m1,m2,m3,m4;

	m1.load_pwm_firmware();
	m2.load_pwm_firmware();
	m3.load_pwm_firmware();
	m4.load_pwm_firmware();

	m1.set_pwm("P8_45");
	m2.set_pwm("P8_46");
	m3.set_pwm("P8_34");
	m4.set_pwm("P8_36");
	std::cout << "Pinler set edildi" << std::endl << std::endl;
	usleep(4000000);

	m1.set_period(10000000);
	m2.set_period(10000000);
	m3.set_period(10000000);
	m4.set_period(10000000);
	std::cout <<  "Periodlar 10M'e ayarlandi" << std::endl << std::endl;
	usleep(4000000);


	m1.set_duty(1500000);
	m2.set_duty(1500000);
	m3.set_duty(1500000);
	m4.set_duty(1500000);
	std::cout << "Dutyler 1.5M'e ayarlandi" << std::endl << std::endl;
	usleep(4000000);


	m1.set_duty(1000000);
	m2.set_duty(1000000);
	m3.set_duty(1000000);
	m4.set_duty(1000000);
	std::cout << "Dutyler 1M" << std::endl;
	usleep(4000000);

	std::cout << "Motorlar calisacak" << std::endl;
	m1.set_duty(1066500);
	m2.set_duty(1066500);
	m3.set_duty(1066500);
	m4.set_duty(1066500);
	usleep(10000000);

	m1.set_duty(1060500);
	m2.set_duty(1060500);
	m3.set_duty(1060500);
	m4.set_duty(1060500);
	std::cout << "Motorlar durdu" << std::endl;

}
