AS_FLAGS += -felf64 -mamd64

CC_FLAGS += -Iinclude -ffreestanding -fbuiltin -mno-red-zone -mcmodel=kernel

LD_FLAGS += -Tarch/$(MACHINE)/kernel.ld

ifeq ($(BUILD), debug)
AS_FLAGS += -gdwarf2
CC_FLAGS += -gdwarf-2 -O0
LD_FLAGS += -Xlinker -Map=.
else
CC_FLAGS += -O3
endif

LIBS := -lgcc -nostdlib

KERN_SRCS := arch/$(MACHINE)/entry.s main.c
KERN_OBJS := $(addsuffix .o, $(KERN_SRCS))

ALL_OBJS := $(KERN_OBJS)
ALL_DEPS := $(ALL_OBJS:.o=.d)

clean:
	@rm -f $(ALL_OBJS) $(ALL_DEPS) kernel

kernel: $(KERN_OBJS)
	$(CC) $(CC_FLAGS) $(LD_FLAGS) -o$@ $^ $(LIBS)

%.s.o: %.s
	$(AS) $(AS_FLAGS) -M $< > $(@:.o=.d)
	$(AS) $(AS_FLAGS) -o$@ $<

%.c.o: %.c
	$(CC) $(CC_FLAGS) $(C_FLAGS) -MMD -c -o$@ $<

-include $(ALL_DEPS)
