.PHONY: all
all: os.iso

loader.o: loader.s
	nasm -f elf32 loader.s

os.iso: iso/boot/grub/stage2_eltorito iso/boot/grub/menu.lst iso/boot/kernel.elf
	genisoimage -R                  \
	-b boot/grub/stage2_eltorito    \
	-no-emul-boot                   \
	-boot-load-size 4               \
	-A os                           \
	-input-charset utf8             \
	-quiet                          \
	-boot-info-table                \
	-o os.iso                       \
	iso

iso/boot/kernel.elf: loader.o link.ld
	ld -T link.ld -melf_i386 loader.o -o iso/boot/kernel.elf

run: bochsrc.txt os.iso
	bochs -f bochsrc.txt -q

clean:
	rm -rf kernel.elf
	rm -rf os.iso
	rm -rf loader.o
	rm -rf iso/boot/kernel.elf
