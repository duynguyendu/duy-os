# Generate list of sources
C_SOURCES = $(wildcard ./*.c)
HEADERS = $(wildcard ../include/*.h)

# List of object files
OBJ = ${C_SOURCES:.c=.o}
OBJ_DEBUG = ${C_SOURCES:.c=.o.debug}

# Gcc flags
CFLAGS = -I${PROJECT_DIR}/include -I${PROJECT_DIR}/kernel/arch/i386/include -fno-pie -ffreestanding -Wall -Wno-int-conversion
CC = i386-elf-gcc
LD = i386-elf-ld

# Compile c source to object file
%.o: %.c ${HEADERS}
	${CC} -ffreestanding -c $< -o $@ ${CFLAGS}

%.o.debug: %.c ${HEADERS}
	${CC} -ffreestanding -c $< -o $@ ${CFLAGS} -g

drivers.o: ${OBJ}
	${LD} -o $@ $^ -r

drivers.o.debug: ${OBJ_DEBUG}
	${LD} -o $@ $^ -r

all: drivers.o

debug: drivers.o.debug

clean:
	rm -f *.o

clean_debug:
	rm -f *.o.debug

debug_makefile:
	$(info ${OBJ})

clean_all: clean clean_debug

