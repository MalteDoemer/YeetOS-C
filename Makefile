
export MAKE=make --no-print-directory

IMAGE=$(abspath disk.img)
HDD=/dev/loop0
PART=/dev/loop1
MNT=/mnt/vdisk
KERNEL=kernel/YeetOS


all:
	$(MAKE) -C kernel

clean:
	$(MAKE) -C kernel clean

format:
	dd if=/dev/zero of=$(IMAGE) bs=512 count=131072
	printf "o\nn\np\n1\n\n\na\np\nw\n" | fdisk $(IMAGE) 
	sudo losetup -o 1048576 /dev/loop3 $(IMAGE)
	sudo mkfs.fat -h 2048 -R 8 /dev/loop3
	sudo losetup -d /dev/loop3

	$(MAKE) attach
	sudo grub-install --root-directory=$(MNT) --no-floppy --modules="normal part_msdos fat multiboot" $(HDD)
	$(MAKE) detach

attach:
	sudo losetup $(HDD) $(IMAGE)
	sudo losetup -o 1048576 $(PART) $(IMAGE)
	sudo mount $(PART) $(MNT)

detach:
	sudo umount $(PART)
	sudo losetup -d $(HDD)
	sudo losetup -d $(PART)

install: all
	$(MAKE) attach
	sudo mkdir -p $(MNT)/boot
	sudo mkdir -p $(MNT)/boot/grub
	sudo cp grub.cfg $(MNT)/boot/grub/grub.cfg
	sudo cp $(KERNEL) $(MNT)/boot/YeetOS
	$(MAKE) detach

run: install
	qemu-system-i386.exe \
	-drive format=raw,file='\\wsl$$\Ubuntu$(IMAGE)',if=ide \
	-m 512 \
	-name "YeetOS" \
	-d cpu_reset
	
debug: install
	qemu-system-i386.exe \
	-drive format=raw,file='\\wsl$$\Ubuntu$(IMAGE)',if=ide \
	-m 512 \
	-name "YeetOS" \
	-S -gdb tcp::9000