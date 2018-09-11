/* DTH11 温度湿度传感器
 *
 *
 */


#include <wiringPi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#define MAX_TIME 85
#define MAX_TRIES 100

int dht11_val[5]={0,0,0,0,0};

int dht11_read_val(int *h, int *t, int pin) {
    uint8_t lststate=HIGH;
    uint8_t counter=0;
    uint8_t j=0,i;
    
    for (i=0;i<5;i++) {
         dht11_val[i]=0;
    }

    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    delay(18);
    digitalWrite(pin, HIGH);
    delayMicroseconds(40);
    pinMode(pin, INPUT);

    for (i=0;i<MAX_TIME;i++) {
        counter=0;
        while (digitalRead(pin)==lststate){
            counter++;
            delayMicroseconds(1);
            if (counter==255)
                break;
        }
        lststate=digitalRead(pin);
        if (counter==255)
             break;
        // top 3 transitions are ignored
        if ((i>=4) && (i%2==0)) {
            dht11_val[j/8]<<=1;
            if(counter>16)
                dht11_val[j/8]|=1;
            j++;
        }
    }

    // verify cheksum and print the verified data
    if ((j>=40) && (dht11_val[4]==((dht11_val[0]+dht11_val[1]+dht11_val[2]+dht11_val[3])& 0xFF))) {
        // Only return the integer part of humidity and temperature. The sensor
        // is not accurate enough for decimals anyway 
        *h = dht11_val[0];
        *t = dht11_val[2];
        return 0;
    }
    else {
        // invalid data
        return 1;
    }
}

int _main(int n, char *argv[])
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

int main(int argc, char *argv[]) {
    if(argc != 2){
        exit(1);
    }
	if(_main(1, argv) == 0) {
		printf("参数错误, 参数1为PIN引脚值\n");
		exit(1);
	}

	int pin = atol(argv[1]);
	
    int i;
    int h; //humidity
    int t; //temperature in degrees Celsius
	
	
    wiringPiSetup();
    
    // throw away the first 3 measurements
    for (i=0; i<3; i++) {
        dht11_read_val(&h, &t, pin);
        delay(3000);
    }

    // read the sensor until we get a pair of valid measurements
    // but bail out if we tried too many times
    int retval=1;
    int tries=0;
    while (retval != 0 && tries < MAX_TRIES)
    {
        retval = dht11_read_val(&h, &t, pin);
        if (retval == 0) {
            printf("%d %d\n", h, t);
        } else {
            delay(3000);
    }
        tries += 1;
    }
    if (tries < MAX_TRIES)
        return 0;
    else
        return 1;

}