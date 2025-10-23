# SPDX-Licence-Identifier: X11
# Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

ifeq ($(TARGET),amd64)
	TARGET_EFI = BOOTX64.EFI
	TARGET_QEMU = qemu-system-x86_64
endif

image/root/keirmael.elf: kernel/kernel.target.out
	cp $< $@

image/root/efi/boot/$(TARGET_EFI): $(HYPER_OUT) | image/root/efi/boot
	cp $< $@

image/root/efi/boot:
	mkdir -p $@

.PHONY: run
run: image/root/keirmael.elf image/root/efi/boot/$(TARGET_EFI)
	$(TARGET_QEMU) -drive file=fat:rw:image/root,media=disk,format=raw -drive if=pflash,format=raw,readonly=on,file=$(OVMF) -debugcon stdio -display none -no-reboot $(QEMU_FLAGS)

.PHONY: debug
debug: QEMU_FLAGS = -s -S -d int
debug: run
