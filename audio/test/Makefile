CC = gcc
CPPFLAGS = `pkg-config --cflags sdl gtk+-3.0` -MMD
CFLAGS = -g -Wall -Wextra -std=c99 -O3
LDFLAGS =
LDLIBS = `pkg-config --libs sdl gtk+-3.0` -lSDL_image -lm #-lSDL_gfx

EXE = shazam

all: $(EXE)

test:

$(foreach f, $(EXE), $(eval $(f):))

clean:
	${RM} $(EXE)
	${RM} *.o
	${RM} *.d

#END
