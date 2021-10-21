# Generate list of sources
C_SOURCES = $(wildcard *.c boot/interrupt/*.c)
HEADERS = $(wildcard ../../../include/*.h include/*.h)

# List of object files
OBJ = ${C_SOURCES:.c=.o}
OBJ_DEBUG = ${C_SOURCES:.c=.o.debug}

# Gcc flags
CFLAGS = -I${HOME}/duy-os/include -I${HOME}/duy-os/kernel/arch/i386/include -fno-pie 
CC = i386-elf-gcc
LD = i386-elf-ld

%.o: %.c ${HEADERS}
	${CC} -ffreestanding -c $< -o $@ ${CFLAGS}

# Debug file
%.o.debug: %.c ${HEADERS}
	${CC} -ffreestanding -c $< -o $@ ${CFLAGS} -g

%.o: %.asm
	nasm $< -f elf -o $@

boot.bin: boot/bootstrap.asm
	nasm $< -f bin -o $@

kernel_i386.o: boot/kernel_entry.o boot/interrupt/interrupt.o ${OBJ} 
	${LD} -T linker.ld -o $@ $^ -r

kernel_i386.o.debug: boot/kernel_entry.o boot/interrupt/interrupt.o ${OBJ_DEBUG} 
	${LD} -T linker.ld -o $@ $^ -r

clean:
	rm -f *.o boot/interrupt/*.o

clean_debug:
	rm -f *.o.debug boot/interrupt/*.o.debug

clean_all: clean clean_debug

debug_makefile:
	$(info $(C_SOURCES))
	$(info $(OBJ))

debug: kernel_i386.o.debug boot.bin

all: kernel.o boot.bin
