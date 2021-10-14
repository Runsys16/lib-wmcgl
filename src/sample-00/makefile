
GPP = g++

INCS     =  -I"/usr/include" -I"/usr/include/freetype2"
CFLAGS   = $(INCS) -g  -O2 -fpermissive -mtune=core2 -Wno-deprecated -Wno-unused-result

PATHLIBS = -L"/usr/local/lib" -L"/usr/lib"
LIBS	 =  $(PATHLIBS) -lGL -lglut -lGLU -lGLEW -lIL -lwmcgl -rdynamic




.PHONY: all
all: sample-00

clean:
	rm -f *.o
	rm -f sample-00

sample-00.o: main.cpp
	@echo ---- Compilation : main.cpp -----
	$(GPP) -c main.cpp -o sample-00.o  $(CFLAGS) 
	
sample-00: sample-00.o
	@echo -- Edition des liens -----
	$(GPP) sample-00.o -o sample-00 $(LIBS) 

