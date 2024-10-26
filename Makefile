AS = nasm
ASFLAGS = -f elf

CC = i686-elf-gcc
CCFLAGS = -Wall -m32 -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -ffreestanding -fno-stack-protector -I./include -Wno-incompatible-pointer-types -Wno-pointer-sign

LD = i686-elf-ld
LDFLAGS = -m elf_i386 -T kernel/link.ld

VM = qemu-system-i386
VMFLAGS = -cdrom UnknownOS.iso

all: install run

install: kernel/kernel.bin
	mkdir -p iso/boot/grub
	cp kernel/kernel.bin iso/boot/kernel.bin
	cp grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o UnknownOS.iso iso

kernel/kernel.bin: boot/bootstrap.o kernel/link.ld kernel/kernel.o drivers/sys.o drivers/io.o drivers/vga.o drivers/serial.o
	$(LD) $(LDFLAGS) -o kernel/kernel.bin boot/bootstrap.o kernel/kernel.o drivers/sys.o drivers/io.o drivers/vga.o drivers/serial.o

kernel/%.o: kernel/%.c
	$(CC) $(CCFLAGS) -c -o $@ $<

drivers/%.o: drivers/%.c
	$(CC) $(CCFLAGS) -c -o $@ $<

boot/bootstrap.o: boot/bootstrap.asm
	$(AS) $(ASFLAGS) -o $@ $<

drivers/io.o: drivers/io.asm
	$(AS) $(ASFLAGS) -o $@ $<

run: UnknownOS.iso
	$(VM) $(VMFLAGS)

clean:
	rm -rf iso *.iso boot/*.o kernel/*.o kernel/*.bin drivers/*.o
