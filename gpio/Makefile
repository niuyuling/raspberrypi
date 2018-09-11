CC = gcc
RM = rm
libs = info_infrared.o info_light.o info_pin.o info_sound.o info_ultrasound.o wind.o dth11.o
CFLAGS = -g -Wall -Werror
LIB = -lwiringPi

all: infrared light pin sound ultrasound wind dth11

infrared: infrared.o
	$(CC) $(CFLAGS) info_infrared.o -o info_infrared $(LIB)
	$(RM) info_infrared.o
infrared.o:
	$(CC) $(CFLAGS) -c info_infrared.c $(LIB)

	
light: light.o
	$(CC) $(CFLAGS) info_light.o -o info_light $(LIB)
	$(RM) info_light.o
light.o:
	$(CC) $(CFLAGS) -c info_light.c $(LIB)

	
pin: pin.o
	$(CC) $(CFLAGS) info_pin.o -o info_pin $(LIB)
	$(RM) info_pin.o
pin.o:
	$(CC) $(CFLAGS) -c info_pin.c $(LIB)

	
sound: sound.o
	$(CC) $(CFLAGS) info_sound.o -o info_sound $(LIB)
	$(RM) info_sound.o
sound.o:
	$(CC) $(CFLAGS) -c info_sound.c $(LIB)

	
ultrasound: ultrasound.o
	$(CC) $(CFLAGS) info_ultrasound.o -o info_ultrasound $(LIB)
	$(RM) info_ultrasound.o
ultrasound.o:
	$(CC) $(CFLAGS) -c info_ultrasound.c $(LIB)


wind: wind.o
	$(CC) $(CFLAGS) wind.o -o wind
	$(RM) wind.o
wind.o:
	$(CC) $(CFLAGS) -c wind.c


dth11: dth11.o
	$(CC) $(CFLAGS) dth11.o -o dth11 $(LIB)
	$(RM) dth11.o
dth11.o:
	$(CC) $(CFLAGS) -c dth11.c $(LIB)


.PHONY : clean
clean :
	rm -f $(libs) info_infrared info_light info_sound info_ultrasound wind dth11
