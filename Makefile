# Generate list of sources
C_SOURCES = $(wildcard kernel/*.c drivers/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h)

# List of object files
OBJ = ${C_SOURCES:.c=.o}
OBJ_DEBUG = ${C_SOURCES:.c=.o.debug}

# Gcc flags
INCLUDE_PATH = -I./

# Compile c source to object file
%.o: %.c ${HEADERS}
	gcc ${INCLUDE_PATH} -ffreestanding -c $< -o $@ -mtune=i386 -fno-pie -m32

%.o.debug: %.c ${HEADERS}
	gcc ${INCLUDE_PATH} -ffreestanding -c $< -o $@ -mtune=i386 -fno-pie -m32 -g

%.o: %.asm
	nasm $< -f elf -o $@

%.bin : %.asm
	nasm $< -f bin -I ’../../16 bit/’ -o $@

kernel.bin: boot/kernel_entry.o ${OBJ}
	ld -T linker.ld -o $@ $^ --oformat=binary -m elf_i386 -Ttext=0x1000

boot.bin: boot/boot.asm
	nasm $< -f bin -o $@

os.bin: boot.bin kernel.bin 
	cat $^ > $@

run: os.bin
	qemu-system-i386 -fda os.bin

kernel.elf: boot/kernel_entry.o ${OBJ_DEBUG}
	ld -T linker.ld -o $@ $^ -m elf_i386 -Ttext=0x1000

debug: os.bin kernel.elf
	qemu-system-i386 -s -S -fda os.bin &
	gdb -ex "target remote localhost:1234" -ex "symbol-file kernel.elf" -ex "set architecture i386"

clean:
	rm -f kernel/*.o drivers/*.o 
	rm -f *.bin

clean_debug:
	rm -f kernel/*o.debug drivers/*.o.debug
	rm -f *.elf

clean_all: clean clean_debug

debug_makefile:
	$(info $(C_SOURCES))
	$(info $(OBJ))

all: run
