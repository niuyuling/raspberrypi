#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

/*
void pinMode (int pin, int mode) ;
void pullUpDnControl (int pin, int pud) ;
void digitalWrite (int pin, int value) ;
void pwmWrite (int pin, int value) ;
int digitalRead (int pin) ;
analogRead (int pin) ;
analogWrite (int pin, int value) ;
*/
int main(int argc, char *argv[])
{
    if( argc != 2) {
        exit(0);
    }

    int pin = atol(argv[1]);
    wiringPiSetup();
    pinMode(pin, OUTPUT);
    printf("%d\n", digitalRead(pin));
}
