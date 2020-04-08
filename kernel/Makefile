AS_FLAGS += -felf64 -mamd64

# TODO: Include -fbuiltin in C_FLAGS when C library supports all necessary functions
C_FLAGS += -Iinclude -ffreestanding -mno-red-zone -mcmodel=kernel

CXX_FLAGS += -fno-exceptions -fno-rtti

LD_FLAGS += -Tarch/$(MACHINE)/kernel.ld

ifeq ($(BUILD), debug)
AS_FLAGS += -gdwarf2
C_FLAGS  += -gdwarf-2 -O0
else
C_FLAGS  += -O3
endif

LIBS := -lk -lgcc -nostdlib

CRTB_OBJ := $(shell $(CXX) $(CXX_FLAGS) $(C_FLAGS) -print-file-name=crtbegin.o)
CRTE_OBJ := $(shell $(CXX) $(CXX_FLAGS) $(C_FLAGS) -print-file-name=crtend.o)

KERN_SRCS := \
	arch/$(MACHINE)/arch.s arch/$(MACHINE)/entry.s \
	arch/$(MACHINE)/interrupts.s arch/$(MACHINE)/interrupts.cpp \
	arch/$(MACHINE)/memory.cpp \
	arch/$(MACHINE)/syscalls.s arch/$(MACHINE)/syscalls.cpp \
	syscalls/mmap.cpp syscalls/write.cpp \
	main.cpp
KERN_OBJS := $(addsuffix .o, $(KERN_SRCS))

ALL_OBJS := $(KERN_OBJS)
ALL_DEPS := $(ALL_OBJS:.o=.d)

clean:
	@rm -f $(ALL_OBJS) $(ALL_DEPS) kernel

kernel: $(LIB_PATH)/crti.o $(CRTB_OBJ) $(KERN_OBJS) $(CRTE_OBJ) $(LIB_PATH)/crtn.o
	$(CXX) $(CXX_FLAGS) $(C_FLAGS) $(LD_FLAGS) -o$@ $^ $(LIBS)

%.s.o: %.s
	$(AS) $(AS_FLAGS) -M $< > $(@:.o=.d)
	$(AS) $(AS_FLAGS) -o$@ $<

%.c.o: %.c
	$(CC) $(CC_FLAGS) $(C_FLAGS) -MMD -c -o$@ $<

%.cpp.o: %.cpp
	$(CXX) $(CXX_FLAGS) $(C_FLAGS) -MMD -c -o$@ $<

-include $(ALL_DEPS)
