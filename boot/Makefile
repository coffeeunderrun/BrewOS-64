AS_FLAGS_BOOT := -felf32
AS_FLAGS_LOAD := -felf64 -mamd64

LD_FLAGS      += -n -nostdlib
LD_FLAGS_BOOT := -Tarch/$(MACHINE)/stage1.ld
LD_FLAGS_LOAD := -Tarch/$(MACHINE)/stage2.ld

SHARED_SRCS := arch/$(MACHINE)/bios.s arch/$(MACHINE)/ext2.s

BOOT_SRCS := arch/$(MACHINE)/stage1.s $(SHARED_SRCS)
BOOT_OBJS := $(addsuffix .boot.o, $(BOOT_SRCS))

LOAD_SRCS := arch/$(MACHINE)/stage2.s $(SHARED_SRCS)
LOAD_OBJS := $(addsuffix .load.o, $(LOAD_SRCS))

ALL_OBJS := $(BOOT_OBJS) $(LOAD_OBJS)
ALL_DEPS := $(ALL_OBJS:.o=.d)

clean:
	@rm -f $(ALL_OBJS) $(ALL_DEPS) boot loader

boot: $(BOOT_OBJS)
	$(LD) $(LD_FLAGS) $(LD_FLAGS_BOOT) -o$@ $^

loader: $(LOAD_OBJS)
	$(LD) $(LD_FLAGS) $(LD_FLAGS_LOAD) -o$@ $^

%.s.boot.o: %.s
	$(AS) $(AS_FLAGS) $(AS_FLAGS_BOOT) -M $< > $(@:.o=.d)
	$(AS) $(AS_FLAGS) $(AS_FLAGS_BOOT) -o$@ $<

%.s.load.o: %.s
	$(AS) $(AS_FLAGS) $(AS_FLAGS_LOAD) -M $< > $(@:.o=.d)
	$(AS) $(AS_FLAGS) $(AS_FLAGS_LOAD) -o$@ $<

-include $(ALL_DEPS)
