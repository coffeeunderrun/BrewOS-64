OS_NAME  ?= "Brew OS x64"
ARCH     ?= x86_64
HOST     ?= elf
ISO      ?= brewos.iso
TOOL_DIR ?= tools/bin

# Assembler
AS        = nasm
AS_FLAGS ?=
AS_FLAGS += -f elf64

# C compiler
CC        = $(TOOL_DIR)/$(ARCH)-$(HOST)-gcc
CC_FLAGS ?=
CC_FLAGS += -std=gnu11 -masm=intel -fno-builtin -nostdlib -lgcc

# C compiler (kernel)
KERN_CC_FLAGS ?=
KERN_CC_FLAGS += $(CC_FLAGS) -Ikernel/inc -ffreestanding -mcmodel=kernel 
KERN_CC_FLAGS += -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -mno-sse3 -mno-3dnow

# C compiler (libk)
LIBK_CC_FLAGS ?=
LIBK_CC_FLAGS += $(KERN_CC_FLAGS) -Ilibc/inc -D__libk

# C compiler (libc)
LIBC_CC_FLAGS ?=
LIBC_CC_FLAGS += $(CC_FLAGS) -Ilibc/inc -D__libc

# Linker
LD        = $(TOOL_DIR)/$(ARCH)-$(HOST)-gcc
LD_FLAGS ?= 

# Linker (kernel)
KERN_LD_FLAGS ?=
KERN_LD_FLAGS += $(KERN_CC_FLAGS) -T kernel/arch/$(ARCH)/link.ld -z max-page-size=0x1000 

# Linker (libc)
LIBC_LD_FLAGS ?=
LIBC_LD_FLAGS += $(LIBC_CC_FLAGS) -r

# ISO builder
IM        = grub-mkrescue
IM_FLAGS ?=

# Virtual machine
VM        = qemu-system-$(ARCH)
VM_FLAGS ?= -localtime

ARCH_SRCS = multiboot.s bootstrap.s irq/irq.s irq/irq.c
ARCH_OBJS = $(addprefix o/$(ARCH)/, $(addsuffix .o, $(ARCH_SRCS)))
ARCH_DEPS = $(ARCH_OBJS:.o=.d)
ARCH_DIRS = $(sort $(dir $(ARCH_OBJS)))

KERN_SRCS = kernel.c
KERN_OBJS = $(addprefix o/, $(addsuffix .o, $(KERN_SRCS)))
KERN_DEPS = $(KERN_OBJS:.o=.d)
KERN_DIRS = $(sort $(dir $(KERN_OBJS)))

LIBK_SRCS =
LIBK_OBJS = $(addprefix o/libk/, $(addsuffix .o, $(LIBK_SRCS)))
LIBK_DEPS = $(LIBK_OBJS:.o=.d)
LIBK_DIRS = $(sort $(dir $(LIBK_OBJS)))

LIBC_SRCS =
LIBC_OBJS = $(addprefix o/libc/, $(addsuffix .o, $(LIBC_SRCS)))
LIBC_DEPS = $(LIBC_OBJS:.o=.d)
LIBC_DIRS = $(sort $(dir $(LIBC_OBJS)))

ISO_DIRS = iso/boot/grub

run: ; @$(VM) $(VM_FLAGS) -drive media=cdrom,format=raw,file=$(ISO)

debug: AS_FLAGS += -F dwarf
debug: CS_FLAGS += -g
debug: VM_FLAGS += -s
debug: all

all: iso

iso: $(ISO_DIRS) $(ISO)

kernel: $(ARCH_DIRS) $(LIBK_DIRS) $(KERN_DIRS) kernel.bin

libc: $(LIBC_DIRS) o/libc.o

clean: ; @rm -rf *.iso *.bin *.a iso o

$(ISO): kernel
	@cp kernel.bin iso/boot
	@echo "menuentry \"$(value OS_NAME)\" { multiboot2 /boot/kernel.bin }" > iso/boot/grub/grub.cfg
	@$(IM) $(IM_FLAGS) -o $(ISO) iso
 
kernel.bin: $(ARCH_OBJS) $(KERN_OBJS) $(LIBK_OBJS)
	@echo "Linking $@\n"
	@$(LD) -MD $^ -o $@ $(KERN_LD_FLAGS)

o/libc.o: $(LIBC_OBJS)
	@echo "Linking $@\n"
	@$(LD) -MD $^ -o $@ $(LIBC_LD_FLAGS)

# Assembler source (arch)
o/%.s.o: kernel/arch/%.s
	@echo "Assembling $@"
	@$(AS) -MD $(@:.o=.d) $< -o $@ $(AS_FLAGS)

# C compiler source (arch)
o/%.c.o: kernel/arch/%.c
	@echo "Compiling $@"
	@$(CC) -MD -c $< -o $@ $(KERN_CC_FLAGS)

# C compiler source (kernel)
o/%.c.o: kernel/src/%.c
	@echo "Compiling $@"
	@$(CC) -MD -c $< -o $@ $(KERN_CC_FLAGS)

# C compiler source (libk)
o/libk/%.c.o: libc/src/%.c
	@echo "Compiling $@"
	@$(CC) -MD -c $< -o $@ $(LIBK_CC_FLAGS)

# C compiler source (libc)
o/libc/%.c.o: libc/src/%.c
	@echo "Compiling $@"
	@$(CC) -MD -c $< -o $@ $(LIBC_CC_FLAGS)

$(ARCH_DIRS): ; @mkdir -p $@
$(KERN_DIRS): ; @mkdir -p $@
$(LIBK_DIRS): ; @mkdir -p $@
$(LIBC_DIRS): ; @mkdir -p $@
$(ISO_DIRS): ; @mkdir -p $@

-include $(ARCH_DEPS) $(KERN_DEPS) $(LIBK_DEPS) $(LIBC_DEPS)
