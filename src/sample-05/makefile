BIN=sample-05
GPP = g++

INCS     =  -I"/usr/include" -I"/usr/include/freetype2"
CFLAGS   = $(INCS) -g  -O2 -fpermissive -mtune=core2 -Wno-deprecated -Wno-unused-result

PATHLIBS = -L"/usr/local/lib" -L"/usr/lib"
LIBS	 =  $(PATHLIBS) -lGL -lglut -lGLU -lGLEW -lIL -lwmcgl -rdynamic




.PHONY: all
all: $(BIN)

clean:
	rm -f *.o
	rm -f $(BIN)

$(BIN).o: main.cpp
	@echo ---- Compilation : main.cpp -----
	$(GPP) -c main.cpp -o $@  $(CFLAGS) 
	
$(BIN): $(BIN).o
	@echo -- Edition des liens -----
	$(GPP) $< -o $@ $(LIBS) 

