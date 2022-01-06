OS_NAME = duyos

# Tools
CC = i386-elf-gcc
LD = i386-elf-ld
ASM = nasm
CFLAGS = -I./include -I./kernel/arch/i386/include -fno-pie -ffreestanding -Wall -Wno-int-conversion -nostdlib -lgcc
ASMFLAGS = -f elf32

# Compile file destination
BUILD_DIR = build

# Source directories
KERNEL_DIR = kernel/arch/i386
DRIVERS_DIR = drivers
LIB_DIR = lib

KERNEL_SOURCE = ${wildcard ${KERNEL_DIR}/*.c ${KERNEL_DIR}/interrupt/*.c ${KERNEL_DIR}/kernel/*.c}
KERNEL_ASM = ${wildcard ${KERNEL_DIR}/*.asm ${KERNEL_DIR}/interrupt/*.asm}
DRIVERS_SOURCE = ${wildcard ${DRIVERS_DIR}/*.c}
LIB_SOURCE = ${wildcard ${LIB_DIR}/*.c}
BOOT_ASM = ${KERNEL_DIR}/boot/bootstrap.asm

HEADERS = ${wildcard include/*.h ${KERNEL_DIR}/include/*.h ${KERNEL_DIR}/include/interrupt/*.h ${KERNEL_DIR}/include/kernel/*.h}

KERNEL_OBJ = ${patsubst %.c, ${BUILD_DIR}/%.o, ${KERNEL_SOURCE}}
KERNEL_ASM_OBJ = ${patsubst %.asm, ${BUILD_DIR}/%.o, ${KERNEL_ASM}}
DRIVERS_OBJ = ${patsubst ${DRIVERS_DIR}/%.c, ${BUILD_DIR}/${DRIVERS_DIR}/%.o, ${DRIVERS_SOURCE}}
LIB_OBJ = ${patsubst ${LIB_DIR}/%.c, ${BUILD_DIR}/${LIB_DIR}/%.o, ${LIB_SOURCE}}
BOOT_OBJ = ${KERNEL_DIR}/boot/bootstrap.o

KERNEL = ${BUILD_DIR}/kernel.bin
KERNEL_ELF = ${BUILD_DIR}/kernel.elf

ISO_DIR = ${BUILD_DIR}/iso
ISO_BOOTDIR = ${BUILD_DIR}/iso/boot
GRUB_DIR = ${BUILD_DIR}/iso/boot/grub
ISO = ${BUILD_DIR}/${OS_NAME}.iso

.PHONY: ${KERNEL_SOURCE} ${KERNEL_ASM} ${DRIVERS_SOURCE} ${LIB_SOURCE} ${BOOT_ASM}

all: ${BUILD_DIR} kernel

run: iso
	# qemu-system-i386 -drive id=disk,file=${ISO},if=none -device ahci,id=ahci -device ide-hd,drive=disk,bus=ahci.0
	# qemu-system-i386 -cdrom ${ISO}
	qemu-system-i386 ${ISO}

debug: CFLAGS += -g
debug: ASMFLAGS += -g
debug: ${BUILD_DIR} ${KERNEL_ELF}
	qemu-system-i386 -kernel ${KERNEL} -s -S &
	gdb -ex "target remote localhost:1234" -ex "symbol-file ${KERNEL_ELF}" -ex "set architecture i386" -ex "target record-full"

iso: ${BUILD_DIR} ${ISO}
${ISO}: ${GRUB_DIR} ${KERNEL}
	@mkdir -p ${ISO_BOOTDIR}
	cp ${KERNEL} ${ISO_BOOTDIR}/duyos.bin
	cp grub.cfg ${GRUB_DIR}/grub.cfg
	grub-mkrescue -o $@ ${ISO_DIR}
	
kernel: ${BUILD_DIR} ${KERNEL}

${KERNEL}: ${BOOT_OBJ} ${KERNEL_OBJ} ${KERNEL_ASM_OBJ} ${DRIVERS_OBJ} ${LIB_OBJ}
	${CC} -T linker.ld -o $@ $^ ${CFLAGS}

${KERNEL_ELF}: ${KERNEL}
	objcopy --only-keep-debug ${KERNEL} $@
	objcopy --strip-debug ${KERNEL}

${BOOT_OBJ}: ${BOOT_ASM}
	${ASM} ${ASMFLAGS} -o $@ $^

${KERNEL_OBJ}: ${BUILD_DIR}/${KERNEL_DIR}/%.o : ${KERNEL_DIR}/%.c	
	${CC} ${CFLAGS} -c $< -o $@

${KERNEL_ASM_OBJ}: ${BUILD_DIR}/${KERNEL_DIR}/%.o : ${KERNEL_DIR}/%.asm
	${ASM} ${ASMFLAGS} -o $@ $<

${DRIVERS_OBJ}: ${BUILD_DIR}/${DRIVERS_DIR}/%.o : ${DRIVERS_DIR}/%.c	
	${CC} ${CFLAGS} -c $< -o $@

${LIB_OBJ}: ${BUILD_DIR}/${LIB_DIR}/%.o : ${LIB_DIR}/%.c	
	${CC} ${CFLAGS} -c $< -o $@

${GRUB_DIR}:
	@mkdir -p ${BUILD_DIR}/iso/boot/grub

${BUILD_DIR}:
	@mkdir -p ${BUILD_DIR}/${KERNEL_DIR}/interrupt
	@mkdir -p ${BUILD_DIR}/${KERNEL_DIR}/kernel
	@mkdir -p ${BUILD_DIR}/${DRIVERS_DIR}
	@mkdir -p ${BUILD_DIR}/${LIB_DIR}

clean:
	rm -rf ${BUILD_DIR}

