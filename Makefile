CC = gcc
CPPFLAGS = `pkg-config --cflags sdl gtk+-3.0` -MMD
CFLAGS = -g -Wall -Wextra -std=c99 -O3 
LDFLAGS =
LDLIBS = `pkg-config --libs sdl gtk+-3.0` -lSDL_image -lm #-lSDL_gfx

EXE = in recommendation reco audio/test/shazam

all: $(EXE)

interface:
recommendation:
reco:
#audio:

$(foreach f, $(EXE), $(eval $(f):))

clean:
	${RM} $(EXE)
	${RM} *.o
	${RM} *.d
	${RM} audio/test/*.d
	${RM} audio/test/*.o
	${RM} audio/test/son.wav
	${RM} recommendation.txt newrecommendation.txt recording.mp3 favoris.txt test/son.wav
#END
