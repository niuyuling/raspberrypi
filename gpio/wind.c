/*
 * DS18B20 温度传感器(Debian jessie)
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define BUF 270

int _read_ds18b20(char *device)
{
    char path[50] = "/sys/bus/w1/devices/";
    char rom[20];
    char buffer[BUF + 1];
    DIR *dirp;
    struct dirent *direntp;
    FILE *fp;
    char *temp;
    float value;

    system("sudo modprobe w1-gpio");
    system("sudo modprobe w1-therm");
    if ((dirp = opendir(path)) == NULL) {
        exit(0);
    }

    while ((direntp = readdir(dirp)) != NULL) {
        if (strstr(direntp->d_name, device)) {
            strcpy(rom, direntp->d_name);
        }
    }
    closedir(dirp);

    strcat(path, rom);
    strcat(path, "/w1_slave");
    //printf("%s\n", path);

    if ((fp = fopen(path, "r")) < 0) {
        exit(0);                /* 文件不存在,则退出. */
    }
    while (fgets(buffer, BUF, fp) != NULL) {
        ;
    }

    temp = strchr(buffer, 't');
    sscanf(temp, "t=%s", temp);
    value = atof(temp) / 1000;
    printf("%.0f\n", value);

    fclose(fp);
    return value;
}

int main(int argc, char *argv[])
{
    int ch;
    opterr = 0;
    while ((ch = getopt(argc, argv, "d:h?")) != -1)
        switch (ch) {
        case 'd':
            _read_ds18b20(optarg);
            break;
        case 'h': case '?':
            printf("wind -d device\n");
            exit(0);
            break;
        default:
            ;
        }
    return 1;
}

