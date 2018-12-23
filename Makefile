ELFFILE=qlink_demo

TARGET=arm-linux-androideabi-
CPP=$(TARGET)g++
CC=$(TARGET)gcc
AR=$(TARGET)ar
LD=$(TARGET)ld
STRIP=$(TARGET)strip
OBJS+=qlink_demo.o
HEADERS+=Qlink_API.h


LDFLAGS = -L. -L./libcoap-4.1.1\
			   -lqlink -pthread


$(ELFFILE) : $(OBJS)
	cd ./libcoap-4.1.1;make -f Makefile_QLink CC=$(CC) AR=$(AR) LD=$(LD) BUILD_SO=1
	$(CPP) -O2 -o $@ $^ $(LDFLAGS) 

qlink_demo.o : qlink_demo.cpp  Qlink_API.h
	$(CPP) -O2 $(INCLUDE) -c -o $@ $<

strip:
	$(STRIP) $(ELFFILE)

.PHONY: clean 
clean:
	-rm -f $(ELFFILE) *.o
	cd ./libcoap-4.1.1;make -f Makefile_QLink clean
