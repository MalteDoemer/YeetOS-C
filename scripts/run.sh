#/bin/bash

IMAGE=$(realpath disk.img)
HDD=/dev/loop0
PART=/dev/loop1
MNT=/mnt/vdisk
KERNEL=YeetOS

qemu-system-i386.exe -drive format=raw,file=\\\\wsl\$\\Ubuntu$IMAGE,if=ide -m 512 -name "YeetOS" -monitor stdio -k ch
