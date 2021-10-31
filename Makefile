# Gcc flags
CFLAGS = -I${PROJECT_DIR}/include -I${PROJECT_DIR}/kernel/arch/i386/include -fno-pie -ffreestanding -Wall -Wno-int-conversion
CC = i386-elf-gcc
LD = i386-elf-ld

SUBDIRS = drivers lib kernel/arch/i386

ASM_SOURCE = kernel/arch/i386/bootstrap.asm
C_SOURCE = $(wildcard drivers/*.c lib/*.c kernel/arch/i386/*.c)
HEADERS = $(wildcard include/*.h kernel/arch/i386/include/*.h)

OBJ = kernel/arch/i386/kernel_i386.o drivers/drivers.o lib/lib.o 
OBJ_DEBUG = ${OBJ:.o=.o.debug}

all: run

# Run make in subdirs
.PHONY: subdirs ${SUBDIRS}
subdirs: ${SUBDIRS}
${SUBDIRS}:
	${MAKE} -C $@ ${MAKECMDGOALS}

os.bin: kernel/arch/i386/boot.o ${OBJ} 
	${CC} -T linker.ld -o $@ -ffreestanding -O2 -nostdlib $^ -Ttext=0x10000

# Elf debug file for kernel
kernel.elf: ${OBJ_DEBUG}
	${LD} -T linker.ld -o $@ $^ -Ttext=0x10000

os.iso: os.bin
	mkdir -p iso/boot/grub
	cp os.bin iso/boot/os.bin
	cp grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o os.iso iso

run: subdirs os.iso
	qemu-system-i386 -cdrom os.iso

debug: subdirs os.iso kernel.elf
	qemu-system-i386 -s -S -cdrom os.iso &
	gdb -ex "target remote localhost:1234" -ex "symbol-file kernel.elf" -ex "set architecture i386" -ex "target record-full"

clean: subdirs
	rm -f *.bin

clean_debug: subdirs
	rm -f *.elf

clean_all: clean clean_debug
