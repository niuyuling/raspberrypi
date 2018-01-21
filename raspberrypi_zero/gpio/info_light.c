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
    if( argc != 3) {
        exit(0);
    }

    int pin = atol(argv[1]);
    wiringPiSetup();

    pinMode(pin, OUTPUT);
    if(atol(argv[2]) == 1) {
        digitalWrite(pin, HIGH);
    }
    if(atol(argv[2]) == 0) {
        digitalWrite(pin, LOW);
    }

}
