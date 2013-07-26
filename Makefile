PROGRAM = CurrentCost_dump

############# Main application #################
all:    $(PROGRAM)
.PHONY: all

# source files
#DEBUG_INFO = YES
SOURCES = CurrentCost_dump.c
OBJECTS = $(SOURCES:.c=.o)

######## compiler- and linker settings #########
CXXFLAGS = -I/usr/include -I/usr/local/include -W -Wall -Werror -pipe
LIBSFLAGS = -L/usr/lib -L/usr/local/lib
ifdef DEBUG_INFO
 CXXFLAGS += -g -DDEBUG
 LIBSFLAGS += -lctbd-0.16
else
 CXXFLAGS += -O
 LIBSFLAGS += -lctb-0.16
endif

%.o: %.c
	g++ $(CXXFLAGS) -o $@ -c $<

############# Main application #################
$(PROGRAM):	$(OBJECTS)
	g++ -o $@ $(OBJECTS) $(LIBSFLAGS)

################### Clean ######################
clean:
	find . -name '*~' -delete
	rm -f $(PROGRAM) $(OBJECTS)

install:
	strip -s $(PROGRAM) && cp $(PROGRAM) /usr/local/bin/
