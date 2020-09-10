/*
 * 设置PIN脚为1或0电位(电压)
 *
 */

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
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
        printf("Parameter error, parameter 1 is PIN pin value\n");
        exit(1);
    }
    if (is_num(2, argv) == 0) {
        printf("Parameter error, parameter 2 is true or false\n");
        exit(1);
    }

    int pin = atol(argv[1]);
    wiringPiSetup();

    pinMode(pin, OUTPUT);
    if (atol(argv[2]) == 1) {
        digitalWrite(pin, HIGH);
    }
    if (atol(argv[2]) == 0) {
        digitalWrite(pin, LOW);
    }
    return atol(argv[2]);
}
