#include <wiringPi.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char * argv[])
{
    int i;
    if( argc != 3) {
        exit(0);
    }

    int pin_tring = atol(argv[1]);
    int pin_echo  = atol(argv[2]); 
    struct timeval tv1;
    struct timeval tv2;
    long start_time, stop_time;
    float distance;

    if(wiringPiSetup() == -1) {
        exit(0);
        return 0;
    }
    digitalWrite(pin_tring, LOW);
    digitalWrite(pin_tring, HIGH);
    delayMicroseconds(10);
    digitalWrite(pin_tring, LOW);

    while(!(digitalRead(pin_echo) == 1));
        gettimeofday(&tv1, NULL);
    
    while(!(digitalRead(pin_echo) == 0));
        gettimeofday(&tv2, NULL);

    start_time = tv1.tv_sec * 1000000 + tv1.tv_usec;
    stop_time  = tv2.tv_sec * 1000000 + tv2.tv_usec;

    distance = (float)(stop_time - start_time) / 1000000 * 34000 / 2;

    printf("%0.0f\n", distance);
    return distance; 
}
