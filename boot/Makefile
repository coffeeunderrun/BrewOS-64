ASBOOT := -felf32
ASLOAD := -felf64 -mamd64

CCFLAG += -ffreestanding -nostdlib -mno-red-zone -zmax-page-size=0x1000

LDFLAG += -n
LDBOOT := -Tarch/$(ARCH)/stage1.ld
LDLOAD := -Tarch/$(ARCH)/stage2.ld

BOOT_SRCS := stage1.s bios.s ext2.s
BOOT_OBJS := $(addprefix $(OBJ_PATH)/, $(addsuffix .boot.o, $(BOOT_SRCS)))
BOOT_DEPS := $(BOOT_OBJS:.o=.d)

LOAD_SRCS := stage2.s bios.s ext2.s
LOAD_OBJS := $(addprefix $(OBJ_PATH)/, $(addsuffix .load.o, $(LOAD_SRCS)))
LOAD_DEPS := $(LOAD_OBJS:.o=.d)

boot: $(IMG_PATH)/brewdisk.img

clean:
	@rm -f $(BOOT_OBJS) $(BOOT_DEPS) $(LOAD_OBJS) $(LOAD_DEPS)
	@rm -f $(OBJ_PATH)/brewboot $(ROOT_PATH)/brewload $(IMG_PATH)/brewdisk.img

$(BOOT_OBJS) $(LOAD_OBJS): | $(OBJ_PATH) $(ROOT_PATH)

$(IMG_PATH)/brewdisk.img: | $(IMG_PATH)

$(IMG_PATH)/brewdisk.img: $(OBJ_PATH)/brewboot $(ROOT_PATH)/brewload
	@echo "Generating $(@F)..."
	@mke2fs -text2 -d$(ROOT_PATH) -F $@ 32M
	@dd if=$< of=$@ bs=512 count=2 conv=notrunc

$(OBJ_PATH)/brewboot: $(BOOT_OBJS)
	@echo "Linking $(@F)..."
	@$(LD) $(CCFLAG) $(LDFLAG) $(LDBOOT) -o$@ $^

$(ROOT_PATH)/brewload: $(LOAD_OBJS)
	@echo "Linking $(@F)..."
	@$(LD) $(CCFLAG) $(LDFLAG) $(LDLOAD) -o$@ $^

$(OBJ_PATH)/%.s.boot.o: arch/$(ARCH)/%.s
	@echo "Assembling $(@F)..."
	@$(AS) $(ASFLAG) $(ASBOOT) -M $< > $(@:.o=.d)
	@$(AS) $(ASFLAG) $(ASBOOT) -o$@ $<

$(OBJ_PATH)/%.s.load.o: arch/$(ARCH)/%.s
	@echo "Assembling $(@F)..."
	@$(AS) $(ASFLAG) $(ASLOAD) -M $< > $(@:.o=.d)
	@$(AS) $(ASFLAG) $(ASLOAD) -o$@ $<

$(OBJ_PATH) $(ROOT_PATH):
	@mkdir -p $@

-include $(BOOT_DEPS) $(LOAD_DEPS)

.PHONY: $(IMG_PATH)/brewdisk.img
