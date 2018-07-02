CC=gcc
libs=info_infrared.o info_light.o info_pin.o info_sound.o info_ultrasound.o

all: infrared light pin sound ultrasound wind_

infrared: info_infrared
	$(CC) info_infrared.c -o info_infrared -lwiringPi
light: info_light
	$(CC) info_light.c -o info_light -lwiringPi
pin: info_pin
	$(CC) info_pin.c -o info_pin -lwiringPi
sound: info_sound
	$(CC) info_sound.c -o info_sound -lwiringPi
ultrasound: info_ultrasound
	$(CC) info_ultrasound.c -o info_ultrasound -lwiringPi
wind_: wind
	${CC} wind.c -o wind

.PHONY : clean
clean :
	rm -f $(libs) info_infrared info_light info_sound info_ultrasound
