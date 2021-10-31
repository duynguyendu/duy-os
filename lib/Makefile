# Generate list of sources
C_SOURCES = $(wildcard ./*.c asm/*.c)
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

lib.o: ${OBJ}
	${CC} -T ${PROJECT_DIR}/linker.ld -o $@ $^ -r

lib.o.debug: ${OBJ_DEBUG}
	${CC} -T ${PROJECT_DIR}/linker.ld -o $@ $^ -r

all: lib.o

debug: lib.o.debug

clean:
	rm -f *.o asm/*.o

clean_debug:
	rm -f *.o.debug asm/*.o.debug

clean_all: clean clean_debug
