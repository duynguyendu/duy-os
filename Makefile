# Gcc flags
CFLAGS = -I./include -fno-pie 
CC = i386-elf-gcc
LD = i386-elf-ld

SUBDIRS = drivers lib kernel/arch/i386

OBJ = kernel/arch/i386/kernel_i386.o drivers/drivers.o lib/lib.o 
OBJ_DEBUG = ${OBJ:.o=.o.debug}

all: run

# Run make in subdirs
.PHONY: subdirs ${SUBDIRS}
subdirs: ${SUBDIRS}
${SUBDIRS}:
	${MAKE} -C $@ ${MAKECMDGOALS}

kernel.bin: ${OBJ}
	${LD} -o $@ $^ --oformat=binary -Ttext=0x1000

# Elf debug file for kernel
kernel.elf: ${OBJ_DEBUG}
	${LD} -o $@ $^ -Ttext=0x1000

os.bin: kernel/arch/i386/boot.bin kernel.bin 
	cat $^ > $@

run: subdirs os.bin
	qemu-system-i386 -fda os.bin

debug: subdirs os.bin kernel.elf
	qemu-system-i386 -s -S -fda os.bin &
	gdb -ex "target remote localhost:1234" -ex "symbol-file kernel.elf" -ex "set architecture i386" -ex "target record-full"

clean: subdirs
	rm -f *.bin

clean_debug: subdirs
	rm -f *.elf

clean_all: clean clean_debug
