;;; Unknown OS GRUB Bootstrap ;;;

[BITS 32]

global _start
_start:
	mov esp, _system_stack
	jmp _entry_stub

align 4
mboot:
	MBOOT_PAGE_ALIGN equ 1<<0
	MBOOT_MEM_INFO equ 1<<1
	MBOOT_AOUT_KLUDGE equ 1<<16
	MBOOT_HEADER_MAGIC equ 0x1BADB002
	MBOOT_HEADER_FLAGS equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO | MBOOT_AOUT_KLUDGE
	MBOOT_CHECKSUM equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

	extern code, bss, end

	dd MBOOT_HEADER_MAGIC
	dd MBOOT_HEADER_FLAGS
	dd MBOOT_CHECKSUM

	dd mboot
	dd code
	dd bss
	dd end
	dd _start

_entry_stub:
	extern kernel_main
	call kernel_main
	jmp $

section .bss
	resb 8192
_system_stack:

