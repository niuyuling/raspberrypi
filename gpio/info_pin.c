/* 测试PIN脚是否1或0
 *
 *
 */


#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if( argc != 2) {
        exit(0);
    }

    int pin = atol(argv[1]);
    wiringPiSetup();
    pinMode(pin, OUTPUT);
    printf("%d\n", digitalRead(pin));
	
	return pin;
}
