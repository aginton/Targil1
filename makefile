CC=gcc
#CFLAGS=-Wall -fPIC
#LDFLAGS = -shared
#SRCS = 

SHCFLAGS = -c -fPIC -Wall
SHLDFLAGS = -shared   # linking flags
RM = rm -f   # rm command
TARGET_LIB = libcoviddb.so  # target lib

SHSRCS = countryReader.c datebaseAPI.c   # source files
SHOBJS = $(SHSRCS:.c=.o)


.PHONY: all
all: ${TARGET_LIB}

$(TARGET_LIB): $(SHOBJS)
	$(CC) ${SHLDFLAGS} -o $@ $^
	
$(SHOBJS):$(SHSRCS)
	$(CC) $(SHCFLAGS) -o $@ $^
	
countryReader.o: countryReader.h
databaseAPI.o: countryReader.h databaseAPI.h

.PHONY: clean
clean:
	-${RM} ${TARGET_LIB} ${OBJS} $(SRCS:.c=.d)


