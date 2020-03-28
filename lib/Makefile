ASFLAG += -felf64 -mamd64

ifeq ($(DEBUG), 1)
ASFLAG += -gdwarf2
CCFLAG += -gdwarf-2 -O0
else
CCFLAG += -O3
endif

CCFLAG += -I$(INC_PATH)/lib
CCLIBC :=
CCLIBK := -ffreestanding -nostdlib -mno-red-zone -zmax-page-size=0x1000 -mcmodel=kernel

LIBC_SRCS :=
LIBC_OBJS := $(addprefix $(OBJ_PATH)/, $(addsuffix .libc.o, $(LIBC_SRCS)))
LIBC_DEPS := $(LIBC_OBJS:.o=.d)

LIBK_SRCS := heap.c memcmp.c memcpy.c memmove.c memset.c kmmap.c vector.c
LIBK_OBJS := $(addprefix $(OBJ_PATH)/, $(addsuffix .libk.o, $(LIBK_SRCS)))
LIBK_DEPS := $(LIBK_OBJS:.o=.d)

libc: $(OBJ_PATH)/libc.a

libk: $(OBJ_PATH)/libk.a

clean:
	@rm -f $(LIBC_OBJS) $(LIBC_DEPS) $(LIBK_OBJS) $(LIBK_DEPS)
	@rm -f $(OBJ_PATH)/libc.a $(OBJ_PATH)/libk.a

$(LIBC_OBJS) $(LIBK_OBJS): | $(OBJ_PATH)

$(OBJ_PATH)/libc.a $(OBJ_PATH)/libk.a: | $(OBJ_PATH)

$(OBJ_PATH)/libc.a: $(LIBC_OBJS)
	@echo "Archiving $(@F)..."
	@$(AR) -rcs $@ $^

$(OBJ_PATH)/libk.a: $(LIBK_OBJS)
	@echo "Archiving $(@F)..."
	@$(AR) -rcs $@ $^

$(OBJ_PATH)/%.s.libc.o: %.s
	@echo "Assembling $(@F)..."
	@$(AS) $(ASFLAG) -M $< > $(@:.o=.d)
	@$(AS) $(ASFLAG) -o$@ $<

$(OBJ_PATH)/%.s.libk.o: %.s
	@echo "Assembling $(@F)..."
	@$(AS) $(ASFLAG) -M $< > $(@:.o=.d)
	@$(AS) $(ASFLAG) -o$@ $<

$(OBJ_PATH)/%.c.libc.o: %.c
	@echo "Compiling $(@F)..."
	@$(CC) $(CCFLAG) $(CCLIBC) -MMD -c -o$@ $<

$(OBJ_PATH)/%.c.libk.o: %.c
	@echo "Compiling $(@F)..."
	@$(CC) $(CCFLAG) $(CCLIBK) -MMD -c -o$@ $<

$(OBJ_PATH):
	@echo "Making $@..."
	@mkdir -p $@

-include $(LIBC_DEPS) $(LIBK_DEPS)
