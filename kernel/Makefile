ASFLAG += -felf64 -mamd64

ifeq ($(DEBUG), 1)
ASFLAG += -gdwarf2
CCFLAG += -gdwarf-2 -O0
else
CCFLAG += -O3
endif

CCFLAG += -ffreestanding -nostdlib -mno-red-zone -zmax-page-size=0x1000 -mcmodel=kernel \
	-fno-exceptions -fno-rtti -I$(INC_PATH)/kernel

LDFLAG += -Tarch/$(ARCH)/kernel.ld

KERN_SRCS := arch.s arch.c entry.s interrupts.s interrupts.c main.cpp memory.c
KERN_OBJS := $(addprefix $(OBJ_PATH)/, $(addsuffix .kern.o, $(KERN_SRCS)))
KERN_DEPS := $(KERN_OBJS:.o=.d)

CRTI_OBJS := $(OBJ_PATH)/crti.s.kern.o
CRTN_OBJS := $(OBJ_PATH)/crtn.s.kern.o
CRTX_DEPS := $(CRTI_OBJS:.o=.d) $(CRTN_OBJS:.o=.d)

CRTB_OBJS := $(shell $(CC) $(CCFLAG) -print-file-name=crtbegin.o)
CRTE_OBJS := $(shell $(CC) $(CCFLAG) -print-file-name=crtend.o)

VPATH = arch/$(ARCH)

kernel: $(ROOT_PATH)/brewkern

clean:
	@rm -f $(KERN_OBJS) $(KERN_DEPS)
	@rm -f $(ROOT_PATH)/brewkern

$(KERN_OBJS): | $(OBJ_PATH)

$(ROOT_PATH)/brewkern: | $(ROOT_PATH)

$(ROOT_PATH)/brewkern: $(CRTI_OBJS) $(CRTB_OBJS) $(KERN_OBJS) $(CRTE_OBJS) $(CRTN_OBJS)
	@echo "Linking $(@F)..."
	@$(LD) $(CCFLAG) $(LDFLAG) -o$@ $^ -lk

$(OBJ_PATH)/%.s.kern.o: %.s
	@echo "Assembling $(@F)..."
	@$(AS) $(ASFLAG) -M $< > $(@:.o=.d)
	@$(AS) $(ASFLAG) -o$@ $<

$(OBJ_PATH)/%.c.kern.o: %.c
	@echo "Compiling $(@F)..."
	@$(CC) $(CCFLAG) -MMD -c -o$@ $<

$(OBJ_PATH)/%.cpp.kern.o: %.cpp
	@echo "Compiling $(@F)..."
	@$(CC) $(CCFLAG) -MMD -c -o$@ $<

$(OBJ_PATH) $(ROOT_PATH):
	@mkdir -p $@

-include $(KERN_DEPS) $(CRTX_DEPS)
