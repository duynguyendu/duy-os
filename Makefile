# Generate list of sources
C_SOURCES = $(wildcard kernel/*.c drivers/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h)

# List of object files
OBJ = ${C_SOURCES:.c=.o}

# Compile c source to object file
%.o: %.c ${HEADERS}
	gcc -ffreestanding -c $< -o $@ -mtune=i386 -fno-pie -m32

%.o: %.asm
	nasm $< -f elf -o $@

%.bin : %.asm
	nasm $< -f bin -I ’../../16 bit/’ -o $@

kernel.bin: kernel/kernel_entry.o ${OBJ}
	ld -T linker.ld -o $@ $^ --oformat=binary -m elf_i386 -n -Ttext=0x1000

boot.bin: boot/boot.asm
	nasm $< -f bin -o $@

os.bin: boot.bin kernel.bin 
	cat $^ > $@

run:
	qemu-system-i386 os.bin

clean:
	rm kernel/*.o drivers/*.o boot/*.bin
	rm *.bin *.o *.dis

debug_makefile:
	$(info $(C_SOURCES))
	$(info $(OBJ))

all: run
