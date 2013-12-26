BUILD = build/
SOURCE = src/
OBJS := $(patsubst $(SOURCE)%.cc,$(BUILD)%.o,$(wildcard $(SOURCE)*.cc))

CC = g++
CPPFLAGS = -Wall -g -ansi

# The first LIBS is static. The second LIBS is dynamic. Choose ONE.
LIBS = `pkg-config --libs --static allegro-static-5 allegro_image-static-5\
 allegro_audio-static-5 allegro_acodec-static-5`
#LIBS = -lallegro -lallegro_image -lallegro_audio -lallegro_acodec

gg2: $(OBJS)
	$(CC) -o $@ $(OBJS) $(CPPFLAGS) $(LIBS)

$(BUILD)main.o: $(patsubst %,$(SOURCE)%, main.cc init.h constants.h game.h\
render.h audio.h)
	$(CC) -o $@ -c $(CPPFLAGS) $<
$(BUILD)init.o: $(patsubst %,$(SOURCE)%,init.cc init.h constants.h)
	$(CC) -o $@ -c $(CPPFLAGS) $<
$(BUILD)game.o: $(patsubst %,$(SOURCE)%,game.cc game.h gamedata.h render.h\
audio.h gamestate.h)
	$(CC) -o $@ -c $(CPPFLAGS) $<
$(BUILD)render.o: $(patsubst %,$(SOURCE)%,render.cc render.h init.h\
	constants.h renderHelper.h)
	$(CC) -o $@ -c $(CPPFLAGS) $<
$(BUILD)renderHelper.o: $(patsubst %,$(SOURCE)%,renderHelper.cc renderHelper.h)
	$(CC) -o $@ -c $(CPPFLAGS) $<
$(BUILD)audio.o: $(patsubst %,$(SOURCE)%,audio.cc audio.h init.h)
	$(CC) -o $@ -c $(CPPFLAGS) $<
$(BUILD)gamedata.o: $(patsubst %,$(SOURCE)%,gamedata.cc gamedata.h mapdata.h\
entity.h)
	$(CC) -o $@ -c $(CPPFLAGS) $<
$(BUILD)input.o: $(patsubst %,$(SOURCE)%,input.cc input.h constants.h)
	$(CC) -o $@ -c $(CPPFLAGS) $<
$(BUILD)entity.o: $(patsubst %,$(SOURCE)%,entity.cc entity.h audio.h render.h\
mapdata.h physics.h)
	$(CC) -o $@ -c $(CPPFLAGS) $<
$(BUILD)mapdata.o: $(patsubst %,$(SOURCE)%,mapdata.cc mapdata.h)
	$(CC) -o $@ -c $(CPPFLAGS) $<
$(BUILD)gamestate.o: $(patsubst %,$(SOURCE)%,gamestate.cc gamestate.h audio.h\
render.h)
	$(CC) -o $@ -c $(CPPFLAGS) $<
$(BUILD)physics.o: $(patsubst %,$(SOURCE)%, physics.cc physics.h constants.h\
entity.h)
	$(CC) -o $@ -c $(CPPFLAGS) $<


.PHONY: clean

clean:
	rm -rf $(OBJS)
