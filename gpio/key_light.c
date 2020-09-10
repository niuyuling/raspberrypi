/*
 * raspberrypi key light 
 * 功能：key等于1时，亮的灯灭、灭的灯亮
 */

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LIGHT 3                 // 灯PIN引脚
#define KEY 4                   // 按键PIN引脚

int main(int argc, char **argv) {
    
    daemon(1, 1);
    
    if(-1 == wiringPiSetup())
    {
        printf("wiringPi setup error\n");
        exit(-1);
    }
    
    pinMode(KEY, INPUT);            // 按键输入模式
    pullUpDnControl(KEY, PUD_DOWN); // 按键启用下拉电阻，引脚电平拉到GND
    pinMode(LIGHT, OUTPUT);         // 灯PIN引脚输出模式
    
    int light_status;
    int key_status;
    
    
    while(1){
        light_status = digitalRead(LIGHT);  // 灯状态
        key_status = digitalRead(KEY);      // 按键状态
        if (key_status == 1) {
            if (light_status == 0) {
                digitalWrite(LIGHT, HIGH);  // 开灯
            }
            if (light_status == 1) {
                digitalWrite(LIGHT, LOW);   // 关灯
            }
        }
        printf("%d\n", digitalRead(KEY));
        delay(200);
        
    }
}