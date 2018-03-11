#include <stdio.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define N 80

int main(int argc, char *argv[])
{
    char path[50] = "/sys/bus/w1/devices/";
    char rom[20];
    char buffer[N+1];
    DIR *dirp;
    struct dirent *direntp;
    FILE *fp;
    char *temp;
    float value;
	
    system("sudo modprobe w1-gpio");
    system("sudo modprobe w1-therm");
    if((dirp = opendir(path)) == NULL) {
        exit(0);
    }

    while((direntp = readdir(dirp)) != NULL) {
        if(strstr(direntp->d_name, "28-0")) {
            strcpy(rom, direntp->d_name);
        }
    }
    closedir(dirp);

    strcat(path, rom);
    strcat(path, "/w1_slave");
    //printf("%s\n", path);

    if ((fp = fopen(path, "r")) < 0) {
        exit(0);    /* 文件不存在,则退出. */
    }
    while(fgets(buffer, N, fp) != NULL){
        ;
    }

    temp = strchr(buffer, 't');
    sscanf(temp, "t=%s", temp);
    value = atof(temp) / 1000;
    printf("%.0f\n", value);

    fclose(fp);
    return 0;
}

