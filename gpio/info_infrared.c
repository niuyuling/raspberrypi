#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    if(argc != 2) {
        exit(0);
    }
    int pin = atol(argv[1]);
    int status = 0;

    wiringPiSetup();

    pinMode(pin, INPUT);

    pullUpDnControl(pin, PUD_DOWN);

    status = digitalRead(pin);
    printf("%d\n", status);
        
}
