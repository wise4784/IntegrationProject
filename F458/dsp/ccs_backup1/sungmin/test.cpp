#include <iostream>

using namespace std;

extern char* dev0;

int main (void){
	int fd=serial_config(dev0);

	return 0;
}
