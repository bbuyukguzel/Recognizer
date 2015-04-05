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
	std::string pwm_firmware;
	std::fstream fs; // File stream

public:

	// echo am33xx_pwm > /sys/devices/bone_capemgr.?/slots
	// cd /sys/devices/ocp.?/pwm_test_P8_13.??/
	void load_pwm_firmware(){

		//std::string BEGIN = "/sys/devices/bone_capemgr.";
		std::string BEGIN = "/sys/devices/ocp.3/pwm_test_P8_45.";
		std::string END = "/slots";

		char pwm_firmware[64];
		std::cout << find_related_number(BEGIN);

		//sprintf(pwm_firmware, "%s%d/slots", BEGIN.c_str(),number);
		std::cout << pwm_firmware;

		// fs.open((pwm_firmware), std::ios::out);
		// if(!fs.is_open()){
		// 	std::cerr << "Error: opening file";
		// 	return;
		// }

		// fs << "am33xx_pwm";
		// fs.close();
	}

	std::string find_related_number(std::string pattern){
		std::string result;
		std::string ret ;

		glob_t glob_result;
		glob((pattern+"*").c_str(), GLOB_TILDE, NULL, &glob_result);

		if(glob_result.gl_pathc > 0){
			result = std::string(glob_result.gl_pathv[0]);
			globfree(&glob_result);

			std::size_t pos = result.rfind(".");
			if(pos!=std::string::npos){ // non-position  (not found)
				return result.substr(pos+1, result.length());
			}
			else{
				std::cerr << "Error: '.' is not found";
				return "-1";
			}
		}
		else{
			std::cerr << "Error: finding related number";
			return "-1";
		}
	}


};

int main(){
	PWM p;
	p.load_pwm_firmware();

}
