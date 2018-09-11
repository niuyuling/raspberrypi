/* 设置PIN脚为1或0电位(电压)
 *
 *
 */


#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
void pinMode (int pin, int mode) ;
void pullUpDnControl (int pin, int pud) ;
void digitalWrite (int pin, int value) ;
void pwmWrite (int pin, int value) ;
int digitalRead (int pin) ;
analogRead (int pin) ;
analogWrite (int pin, int value) ;
*/

int _main(char *argv[], int n)
{
    int j = 0;
	int i;
    char parameter[10];
    strcpy(parameter, argv[n]);
    int len = strlen(parameter);

    for(i=0; i<len; i++)
    {
        if(parameter[i]<=57 && parameter[i]>=48)  //0~9的ASCII码是48~57
        {j++;}
    }
    if(j==len){
        return 1;
    } else {
        return 0;
    }
}

int main(int argc, char *argv[])
{
    if( argc != 3) {
        exit(0);
    }
    if(_main(argv, 1) == 0) {
        printf("参数错误, 参数1为PIN引脚值\n");
        exit(1);
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
    return atol(argv[2]);
}

