ASM=nasm

hellomake: boot.asm
	$(ASM) boot.asm -f bin -o boot.bin

test:
	echo "HELLO" $1
