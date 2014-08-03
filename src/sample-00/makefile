
GPP = g++

PATHLIBS = -L"/usr/local/lib" -L"/usr/lib"
LIBS	 =  $(PATHLIBS) -lfreetype -lGL -lglut -lGLU -lGLEW -lIL -lwmcgl

INCS     =  -I"/usr/include"
CFLAGS   = $(INCS) -g  -O2 -fpermissive -mtune=core2 -Wno-deprecated -Wno-unused-result




.PHONY: all
all: sample-00

clean:
	rm -f *.o
	rm -f sample-00

sample-00.o: main.cpp
	@echo ---- obj -----
	$(GPP) -c main.cpp -o sample-00.o  $(CFLAGS) 
	
sample-00: sample-00.o
	@echo -- Edition des liens -----
	$(GPP) sample-00.o -o sample-00 $(LIBS) 

