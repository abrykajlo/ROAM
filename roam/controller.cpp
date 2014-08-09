#include <sys/ioctl.h>
#include <sys/types.h>
#include <linux/input.h>
#include <linux/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>


#include <iostream>
using namespace std;

int main() {
	int controller = open("/dev/input/js0", O_NONBLOCK);
	char name[100];
	name[99] = '\0';
	while (1) {
	read(controller, (void*)name, 99);
	cout << name;
}
	return 0;
}

