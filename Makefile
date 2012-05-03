CC=gcc
CFLAGS=
OS := $(shell uname)
ifeq ($(OS), Linux)
LDFLAGS= -lGL -lglut -lGLU
else
LDFLAGS= glut32.lib -lopengl32 -lglu32
endif
SOURCES=main.c world.c draw.c helper.c pick.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=Proje

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
	
clean: 
	rm $(OBJECTS) Proje.exe