/*
 *	read temperature and humidity from DHT11 or DHT22 sensor
 *
 */

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MAX_TIMINGS	85

int data[5] = { 0, 0, 0, 0, 0 };

int read_dht_data(float *h, float *c, float *f, int PIN)
{
    uint8_t laststate = HIGH;
    uint8_t counter = 0;
    uint8_t j = 0, i;

    data[0] = data[1] = data[2] = data[3] = data[4] = 0;

    /* pull pin down for 18 milliseconds */
    pinMode(PIN, OUTPUT);
    digitalWrite(PIN, LOW);
    delay(18);

    /* prepare to read the pin */
    pinMode(PIN, INPUT);

    /* detect change and read data */
    for (i = 0; i < MAX_TIMINGS; i++) {
        counter = 0;
        while (digitalRead(PIN) == laststate) {
            counter++;
            delayMicroseconds(1);
            if (counter == 255) {
                break;
            }
        }
        laststate = digitalRead(PIN);

        if (counter == 255)
            break;

        /* ignore first 3 transitions */
        if ((i >= 4) && (i % 2 == 0)) {
            /* shove each bit into the storage bytes */
            data[j / 8] <<= 1;
            if (counter > 16)
                data[j / 8] |= 1;
            j++;
        }
    }

    /*
     * check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
     * print it out if data is good
     */
    if ((j >= 40) &&
        (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF))) {
        *h = (float)((data[0] << 8) + data[1]) / 10;
        if (*h > 100) {
            *h = data[0];        // for DHT11
        }
        *c = (float)(((data[2] & 0x7F) << 8) + data[3]) / 10;
        if (*c > 125) {
            *c = data[2];        // for DHT11
        }
        if (data[2] & 0x80) {
            *c = -(*c);
        }
        *f = *c * 1.8f + 32;
        return 0;
    } else {
        return 1;
    }
}

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
    
    if (wiringPiSetup() == -1)
        exit(1);
    int PIN = atol(argv[1]);
    float h;
    float c;
    float f;
    read_dht_data(&h, &c, &f, PIN);
    printf("Humidity = %.1f Temperature = %.1f ℃  ( %.1f ℉ )\n", h, c, f);

    return (0);
}
