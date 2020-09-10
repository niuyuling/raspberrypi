/*
 * raspberrypi GPIO key light 
 * 功能：key等于1时，亮的灯灭、灭的灯亮
 */
 
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LIGHT 3                 // 灯PIN引脚
#define KEY 4                   // 按键PIN引脚

void buttonpressed(void);

int main()
{
    if (-1 == wiringPiSetup()) {
        exit(-1);
    }
    daemon(1, 1);
    pinMode(LIGHT, OUTPUT);     // 灯PIN输出模式
    pinMode(KEY, INPUT);        // 按键PIN输入模式
    pullUpDnControl(KEY, PUD_DOWN); // 按键启用下拉电阻，引脚电平拉到GND

    //注册中断处理函数, INT_EDGE_RISING是高电平触发
    if (0 > wiringPiISR(KEY, INT_EDGE_RISING, buttonpressed)) {
        exit(-1);
    }

    while (1) ;

    return 0;
}

//中断处理函数：反转LED的电平
void buttonpressed(void)
{
    //printf("%d\n", digitalRead(KEY));
    delay(250);                 // 模拟人手按下开关的时间
    int light_status = digitalRead(LIGHT); // 灯状态
    int key_status = digitalRead(KEY); // 按键状态
    if (key_status == 1) {
        if (light_status == 0) {
            digitalWrite(LIGHT, HIGH); // 开灯
            return ;
        }
        if (light_status == 1) {
            digitalWrite(LIGHT, LOW); // 关灯
            return ;
        }
    }
}
