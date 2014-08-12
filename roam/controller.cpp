#include <sys/ioctl.h>
#include <sys/types.h>
#include <linux/input.h>
#include <linux/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/kernel.h>
#define __KERNEL__
#include "xpad.h"
#include <iostream>
using namespace std;

int main() {
	int controller = open("/dev/input/js0", O_NONBLOCK);
	struct usb_xpad x;
	int err = read(controller, &x, sizeof(struct usb_xpad));
	cout << x.phys;
	return 0;
}

