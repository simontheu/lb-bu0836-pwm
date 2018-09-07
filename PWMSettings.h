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

/* Flag set by ‘--verbose’. */
static int verbose_flag;

//Limits for parameters
static const uint32_t kPWMValueLowerLimit = 0;
static const uint32_t kPWMValueUpperLimit = 1023;
static const uint8_t reportID = 0;
static const uint8_t reportTag = 1;

class PWMSettings {
    public:
    uint16_t PWMValue;
    uint8_t buf[17];
    //HID Specific Settings
    struct hidraw_report_descriptor rpt_desc;
    struct hidraw_devinfo info;

    
    uint8_t sendValue () {
           //Clear the buffer to 0
//	uint8_t buf[17]
	uint8_t length = 17;
 	  memset(&buf, 0x0, length);

   	buf[0] = reportID; // Report Number
    
  	memcpy(&buf[0x0], &reportID,           sizeof(u_int8_t));
 	memcpy(&buf[0x1], &reportTag,          sizeof(u_int8_t));
	memcpy(&buf[0x2], &PWMValue,       sizeof(u_int16_t));
    }

    void printParameters() {
        printf("PWM Value = %u\n", this->PWMValue);
    }

    int processCommandLineArguments(int argc, char **argv)
    {
      int c;

      while (1)
      {
            static struct option long_options[] =
            {
                  /* These options set a flag. */
                  {"verbose", no_argument,       &verbose_flag, 1},
                  {"brief",   no_argument,       &verbose_flag, 0},
                  /* These options don’t set a flag.
                        We distinguish them by their indices. */
                  {"value",    required_argument, 0, 'a'},
                  {0, 0, 0, 0}
            };
            /* getopt_long stores the option index here. */
            int option_index = 0;

            c = getopt_long (argc, argv, "a",
                        long_options, &option_index);

            /* Detect the end of the options. */
            if (c == -1)
            break;

            switch (c)
            {
                case 0:
                    /* If this option set a flag, do nothing else now. */
                    if (long_options[option_index].flag != 0)
                            break;
                    printf ("option %s", long_options[option_index].name);
                    if (optarg)
                            printf (" with arg %s", optarg);
                    printf ("\n");
                    break;
                
                case 'a'://N31
                    this->PWMValue = atoi(optarg);
		    printf("PWM Set");
                    break;

                case '?':
                    /* getopt_long already printed an error message. */
                    break;

                default:
                    abort ();
            }
        } 
    }

    void verifyParameters()
    {
        if (this->PWMValue < kPWMValueLowerLimit || this->PWMValue > kPWMValueUpperLimit) {
            printf("\nInvalid Parameter: PWMValue\n");
            printf("PWM Value Must be in the range from %i to %i\n", kPWMValueLowerLimit, kPWMValueUpperLimit);
        }
    }

} ;

