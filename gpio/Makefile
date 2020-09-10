CROSS_COMPILE ?= 
CC := $(CROSS_COMPILE)gcc
STRIP := $(CROSS_COMPILE)strip
CFLAGS += -g -O2 -Wall
LIBS = -lwiringPi
BIN = infrared light pin sound ultrasound wind dht11 dht12 helement key_light

all: $(BIN)

infrared: infrared.o
	$(CC) $(CFLAGS) -o infrared $^ $(LIBS)
light: light.o
	$(CC) $(CFLAGS) -o light $^ $(LIBS)
pin: pin.o
	$(CC) $(CFLAGS) -o pin $^ $(LIBS)
sound: sound.o
	$(CC) $(CFLAGS) -o sound $^ $(LIBS)
ultrasound: ultrasound.o
	$(CC) $(CFLAGS) -o ultrasound $^ $(LIBS)
wind: wind.o
	$(CC) $(CFLAGS) -o wind $^ $(LIBS)
dht11: dht11.o
	$(CC) $(CFLAGS) -o dht11 $^ $(LIBS)
dht12: dht12.o
	$(CC) $(CFLAGS) -o dht12 $^ $(LIBS)
helement: helement.o
	$(CC) $(CFLAGS) -o helement $^ $(LIBS)
key_light: key_light.o
	$(CC) $(CFLAGS) -o key_light $^ $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c $< $(LIBS)

clean:
	rm -rf *.o
	rm $(BIN)

