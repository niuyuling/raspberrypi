/*
 * 超声波距离传感器
 *
 */

#include <wiringPi.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int is_num(int n, char *argv[])
{
    int j = 0;
    int i;
    char parameter[10];
    strcpy(parameter, argv[n]);
    int len = strlen(parameter);

    for (i = 0; i < len; i++) {
        if (parameter[i] <= 57 && parameter[i] >= 48) //0~9的ASCII码是48~57
        {
            j++;
        }
    }
    if (j == len) {
        return 1;
    } else {
        return 0;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Parameter error.\n");
        exit(1);
    }
    if (is_num(1, argv) == 0) {
        printf("Parameter error, Parameter 1 is the tring pin\n");
        exit(1);
    }
    if (is_num(2, argv) == 0) {
        printf("Parameter error, Parameter 2 is the echo pin\n");
        exit(1);
    }    

    int pin_tring = atol(argv[1]);
    int pin_echo = atol(argv[2]);
    struct timeval tv1;
    struct timeval tv2;
    long start_time, stop_time;
    float distance;

    if (wiringPiSetup() == -1) {
        exit(0);
        return 0;
    }
    digitalWrite(pin_tring, LOW);
    digitalWrite(pin_tring, HIGH);
    delayMicroseconds(10);
    digitalWrite(pin_tring, LOW);

    while (!(digitalRead(pin_echo) == 1)) ;
    gettimeofday(&tv1, NULL);

    while (!(digitalRead(pin_echo) == 0)) ;
    gettimeofday(&tv2, NULL);

    start_time = tv1.tv_sec * 1000000 + tv1.tv_usec;
    stop_time = tv2.tv_sec * 1000000 + tv2.tv_usec;

    distance = (float)(stop_time - start_time) / 1000000 * 34000 / 2;

    printf("%0.0f\n", distance);
    return distance;
}
