/*
 * 声音传感器
 *
 */

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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
    if (argc != 2) {
        printf("Parameter error.\n");
        exit(1);
    }
    if (is_num(1, argv) == 0) {
        printf("Parameter error, parameter 1 is PIN pin value\n");
        exit(1);
    }
    
    int pin = atol(argv[1]);
    int status = 0;

    wiringPiSetup();

    pinMode(pin, INPUT);

    //pullUpDnControl(pin, PUD_DOWN);

    status = digitalRead(pin);
    printf("%d\n", status);

    return status;
}
