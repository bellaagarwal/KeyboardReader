TARGET = main
CROSS_COMPILE = arm-linux-gnueabihf-
CFLAGS = -g -Wall -std=gnu++11 -I ${SOCEDS_DEST_ROOT}/ip/altera/hps/altera_hps/hwlib/include 
LDFLAGS =  -g -Wall  -lstdc++
SOC=De1.h
CC = $(CROSS_COMPILE)g++
ARCH= arm

$(TARGET): main.o parse.h $(SOC)

% : %.o
	$(CC) $(LDFLAGS) $^ -o $@ 
	
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -rf $(TARGET) *.a *.o *~