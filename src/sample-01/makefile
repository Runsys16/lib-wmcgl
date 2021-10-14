BIN=sample-01
GPP = g++

INCS     =  -I"/usr/include" -I"/usr/include/freetype2"
CFLAGS   = $(INCS) -g  -O2 -fpermissive -mtune=core2 -Wno-deprecated -Wno-unused-result

PATHLIBS = -L"/usr/local/lib" -L"/usr/lib"
LIBS	 =  $(PATHLIBS) -lGL -lglut -lGLU -lGLEW -lIL -lwmcgl -rdynamic




.PHONY: all
all: sample-01

clean:
	rm -f *.o
	rm -f $(BIN)

sample-01.o: main.cpp
	@echo ---- Compilation : main.cpp -----
	$(GPP) -c main.cpp -o $@  $(CFLAGS) 
	
$(BIN): sample-01.o
	@echo -- Edition des liens -----
	$(GPP) $< -o $@ $(LIBS) 

