#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>


#include <iostream>

#include "xpad.h"

using namespace std;
struct usb_xpad;
int main() {
	int controller = open("/dev/input/js0", O_NONBLOCK);
	struct usb_xpad * desc;
	read(controller, (void*)desc, sizeof(struct usb_xpad));
	cout << desc->isConnected;
	return 0;
}

struct usb_xpad {
	struct input_dev *dev;			/* input device interface */
	struct usb_device *udev;		/* usb device */

	struct urb *irq_in;			/* urb for int. in report */
	unsigned char *idata;			/* input data */
	dma_addr_t idata_dma;

	char phys[65];				/* physical input dev path */

	unsigned char offsetset_compensation;
	int left_offset_x;
	int left_offset_y;
	int right_offset_x;
	int right_offset_y;

	int isMat;				/* is this a dancepad/mat? */
	int is360;				/* is this a Xbox 360 Controller */
	int isWireless;			/* is this a Xbox 360 _Wireless_ Controller
						   NOTE: if this is set, is360 should be also */
	int isConnected;			/* is this controller connected */

#ifdef CONFIG_USB_XPAD_RUMBLE
	int rumble_enabled;			/* ioctl can toggle rumble */

	int ep_out_adr;				/* number of out endpoint */
	unsigned char tx_data[XPAD_PKT_LEN_FF];	/* output data (rumble) */
	int strong_rumble, play_strong;		/* strong rumbling */
	int weak_rumble, play_weak;		/* weak rumbling */
	struct timer_list rumble_timer;		/* timed urb out retry */
	wait_queue_head_t wait;			/* wait for URBs on queue */

	spinlock_t tx_lock;
	struct circ_buf tx;
	unsigned char tx_buf[XPAD_TX_BUFSIZE];
	long tx_flags[1];			/* transmit flags */
#endif
};