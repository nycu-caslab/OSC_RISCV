DISKNAME 	= NO NAME
DISKPATH 	= /Volumes/$(DISKNAME)

BASEURL 	= https://github.com/starfive-tech/VisionFive2/releases/download
PACKAGE 	= JH7110_VF2_6.6_v5.12.0.tar.bz2
DEVICETREE 	= jh7110-starfive-visionfive-2-v1.3b.dtb
UBOOT_CONF 	= vf2_uEnv.txt
KERNEL_IMG 	= kernel.img
BOOTLOADER 	= bootloader.img
ITS_FILE 	= visionfive2-fit-image.its
FIT_FILE 	= kernel.fit

.PHONY: all files pack dtb fit upload

all: files

files: pack dtb fit

pack: $(PACKAGE)
$(PACKAGE):
	wget "$(BASEURL)/$(PACKAGE)"

dtb: $(DEVICETREE)
%.dtb: $(PACKAGE)
	tar --strip-components 1 -xjvf $< '*/$@'

fit: $(FIT_FILE)
$(FIT_FILE): $(ITS_FILE) $(BOOTLOADER) $(DTB_FILE)
	mkimage -f $< -A riscv -O linux -T flat_dt $@


upload: $(UBOOT_CONF) $(FIT_FILE)
	cp $? "$(DISKPATH)"
