## Build Instructions ##

To build the executable run the following

gcc -o lb-gps lb-bu0836a-pwm.cpp -lstdc++

## Usage ##
```
usage: lb-bu0836a-pwm /dev/hidraw?? [--value]
      --value:     PWM Value: integer within  the range 0 to 1023
```
