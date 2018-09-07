#include <iostream>
using namespace std;

/* Linux */
#include <linux/types.h>
#include <linux/input.h>
#include <linux/hidraw.h>

/* Unix */
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/* C */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <getopt.h>

#include <time.h>
//#include <math.h>

//Leo
#include "PWMSettings.h"

/*
 * Ugly hack to work around failing compilation on systems that don't
 * yet populate new version of hidraw.h to userspace.
 */
#ifndef HIDIOCSFEATURE
#warning Please have your distro update the userspace kernel headers
#define HIDIOCSFEATURE(len)    _IOC(_IOC_WRITE|_IOC_READ, 'H', 0x06, len)
#define HIDIOCGFEATURE(len)    _IOC(_IOC_WRITE|_IOC_READ, 'H', 0x07, len)
#endif

#define HIDIOCGRAWNAME(len)     _IOC(_IOC_READ, 'H', 0x04, len)

const char *bus_str(int bus);

int main(int argc, char **argv)
{
      cout << "Leo Bodnar BU0836A PWM Demo Code" << endl;
      
      int fd;
      int i, res, desc_size = 0;
      u_int8_t buf[256];

      struct hidraw_devinfo info;

      PWMSettings *currentSettings = new PWMSettings;

   /* Open the Device with non-blocking reads. In real life,
      don't use a hard coded path; use libudev instead. 
   */
      if (argc == 1)
      {
            printf("usage: lb-bu0836a-pwm /dev/hidraw?? [--value\n");
            return -1;
      }

      printf("Trying to open device %s\n", argv[1]);

      fd = open(argv[1], O_RDWR|O_NONBLOCK);

      if (fd < 0) 
      {
            perror("Unable to open device");
            //return 1;
      }

      //Device connected, setup report structs
      //currentSettings.reportDescriptor??
      //memset(&rpt_desc, 0x0, sizeof(rpt_desc));
      memset(&info, 0x0, sizeof(info));

      /* Get Feature */
      res = ioctl(fd, HIDIOCGRAWINFO, &info);
      
      if (res < 0) 
      {
            perror("HIDIOCGRAWINFO");
      } 
      else
      {
            printf("Raw Info:\n");
            printf("\tbustype: %d (%s)\n",
            info.bustype, bus_str(info.bustype));
            printf("\tvendor: 0x%04hx\n", info.vendor);
            printf("\tproduct: 0x%04hx\n", info.product);
      }

      /* Get Raw Name */
      res = ioctl(fd, HIDIOCGRAWNAME(256), buf);

      if (res < 0)
            perror("HIDIOCGRAWNAME");
      else
            printf("Raw Name: %s\n", buf);

            currentSettings->processCommandLineArguments(argc, argv);
	    currentSettings->sendValue();
            currentSettings->printParameters();

            /* Write Report */
            res = write(fd, currentSettings->buf, 17);
            if (res < 0)
                  perror("HIDIOCSFEATURE");
            else {
                  printf("pre");
                  printf("write buf returned: %d\n", res);
                  printf("post");
            }

      close(fd);

      return 0;
      }

const char * bus_str(int bus)
{
   switch (bus) 
   {
      case BUS_USB:       return "USB";       break;
      case BUS_HIL:       return "HIL";       break;
      case BUS_BLUETOOTH: return "Bluetooth"; break;
      case BUS_VIRTUAL:   return "Virtual";   break;
      default: return "Other"; break;
   }
}

