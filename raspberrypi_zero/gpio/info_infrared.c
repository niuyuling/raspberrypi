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

    //设置为输入模式
    pinMode(pin, INPUT);

    //很重要。控制默认情况下此PIN处于下拉低电平状态
    pullUpDnControl(pin, PUD_DOWN);

/*
    while(true)
    {
        delay(1000);
        //读取当前pin的输入状态
        if(digitalRead(pin) == 1) {
            printf("There is somebody here\n");
        }
        else {
            printf("There is no one here\n");
        }
    }
*/
    status = digitalRead(pin);
    printf("%d\n", status);
        
}
