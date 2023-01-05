CC = gcc
CFLAGS = -W -Wall  #windows


INCLUDES = -I ../include
LIBS = -L ../lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
EXEC = ../bin/prog

OBJ= $(wildcard *.c) #tout tes .o
all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(INCLUDES) $(LIBS)


%.o: %.c 
	$(CC) -o $@ -c $< $(INCLUDES) $(LIBS)

clean:
	del /f *.o