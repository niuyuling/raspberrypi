CC=gcc
libs=info_infrared.o info_light.o info_pin.o info_sound.o info_ultrasound.o

all: infrared light pin sound ultrasound

infrared: info_infrared.o
	$(CC) -c info_infrared.c
	$(CC) info_infrared.o -o info_infrared -lwiringPi
light: info_light.o
	$(CC) -c info_light.c
	$(CC) info_light.o -o info_light -lwiringPi
pin: info_pin.o
	$(CC) -c info_pin.c
	$(CC) info_pin.o -o info_pin -lwiringPi
sound: info_sound.o
	$(CC) -c info_sound.c
	$(CC) info_sound.o -o info_sound -lwiringPi
ultrasound: info_ultrasound.o
	$(CC) -c info_ultrasound.c
	$(CC) info_ultrasound.o -o info_ultrasound -lwiringPi

.PHONY : clean
clean :
	rm -f $(libs) info_infrared info_light info_sound info_ultrasound
