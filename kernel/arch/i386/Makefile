# Generate list of sources
C_SOURCES = $(wildcard *.c interrupt/*.c)
HEADERS = $(wildcard ../../../include/*.h include/*.h)

# List of object files
OBJ = ${C_SOURCES:.c=.o}
OBJ_DEBUG = ${C_SOURCES:.c=.o.debug}

# Gcc flags
CFLAGS = -I${PROJECT_DIR}/include -I${PROJECT_DIR}/kernel/arch/i386/include -fno-pie -ffreestanding -Wall -Wno-int-conversion
CC = i386-elf-gcc
LD = i386-elf-ld

%.o: %.c ${HEADERS}
	${CC} -ffreestanding -c $< -o $@ ${CFLAGS}

# Debug file
%.o.debug: %.c ${HEADERS}
	${CC} -ffreestanding -c $< -o $@ ${CFLAGS} -g

%.o: %.asm
	nasm $< -f elf32 -o $@

all: kernel_i386.o boot.o

boot.o: boot/bootstrap.asm
	nasm $< -o $@ -f elf32

kernel_i386.o: interrupt/interrupt.o load_gdt.o ${OBJ} 
	${CC} -T linker.ld -o $@ $^ -r

kernel_i386.o.debug: interrupt/interrupt.o load_gdt.o ${OBJ_DEBUG} 
	${CC} -T linker.ld -o $@ $^ -r

clean:
	rm -f *.o interrupt/*.o *.bin

clean_debug:
	rm -f *.o.debug interrupt/*.o.debug

clean_all: clean clean_debug

debug_makefile:
	$(info $(C_SOURCES))
	$(info $(OBJ))

debug: kernel_i386.o.debug boot.o
