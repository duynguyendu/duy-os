# Generate list of sources
C_SOURCES = $(wildcard kernel/*.c interrupt/*.c drivers/*.c lib/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h lib/*.h interrupt/*.h)

# TODO dir path

# List of object files
OBJ = ${C_SOURCES:.c=.o}
OBJ_DEBUG = ${C_SOURCES:.c=.o.debug}

# Gcc flags
CFLAGS = -I./ -fno-pie 
# CC = gcc -mtune=i386 -m32
# LD = ld -m elf_i386
CC = i386-elf-gcc
LD = i386-elf-ld

# Compile c source to object file
%.o: %.c ${HEADERS}
	${CC} -ffreestanding -c $< -o $@ ${CFLAGS}

%.o.debug: %.c ${HEADERS}
	${CC} -ffreestanding -c $< -o $@ ${CFLAGS} -g

%.o: %.asm
	nasm $< -f elf -o $@

kernel.bin: boot/kernel_entry.o interrupt/interrupt.o ${OBJ} 
	${LD} -T linker.ld -o $@ $^ --oformat=binary -Ttext=0x1000

boot.bin: boot/boot.asm
	nasm $< -f bin -o $@

os.bin: boot.bin kernel.bin 
	cat $^ > $@

run: os.bin
	qemu-system-i386 -fda os.bin

kernel.elf: boot/kernel_entry.o interrupt/interrupt.o ${OBJ_DEBUG}
	${LD} -T linker.ld -o $@ $^ -Ttext=0x1000

debug: os.bin kernel.elf
	qemu-system-i386 -s -S -fda os.bin &
	gdb -ex "target remote localhost:1234" -ex "symbol-file kernel.elf" -ex "set architecture i386" -ex "target record-full"

clean:
	rm -f kernel/*.o drivers/*.o *.bin interrupt/*.o lib/*.o

clean_debug:
	rm -f kernel/*o.debug drivers/*.o.debug *.elf interrupt/*.o.debug lib/*.o.debug

clean_all: clean clean_debug

debug_makefile:
	$(info $(C_SOURCES))
	$(info $(OBJ))

all: run
