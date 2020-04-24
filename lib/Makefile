AS_FLAGS += -felf64 -mamd64

C_FLAGS      += -Iinclude
C_FLAGS_LIBC := -D__LIBC
C_FLAGS_LIBK := -D__LIBK -ffreestanding -mno-red-zone -mcmodel=kernel

CXX_FLAGS += -fno-exceptions -fno-rtti

ifeq ($(BUILD), debug)
AS_FLAGS += -gdwarf2
C_FLAGS  += -gdwarf-2 -O0
else
C_FLAGS  += -O3
endif

SHARED_SRCS := \
	cpp/libcpp.c cpp/new.cpp \
	stdio/fprintf.c stdio/printf.c stdio/sprintf.c \
	stdlib/heap.c \
	string/memchr.c string/memcmp.c string/memcpy.c string/memmove.c string/memset.c \
	string/strlen.c string/strncpy.c \
	mman/mmap.c \
	unistd/write.c

LIBC_SRCS := $(SHARED_SRCS)
LIBC_OBJS := $(addsuffix .libc.o, $(LIBC_SRCS))

LIBK_SRCS := $(SHARED_SRCS)
LIBK_OBJS := $(addsuffix .libk.o, $(LIBK_SRCS))

ALL_OBJS := $(CRT_OBJS) $(LIBC_OBJS) $(LIBK_OBJS) crt0.o crt1.o crti.o crtn.o
ALL_DEPS := $(ALL_OBJS:.o=.d)

LIBC_OUT := libc.a crt0.o crt1.o crti.o crtn.o
LIBC_SYS := $(addprefix $(LIB_PATH)/, $(LIBC_OUT))

LIBK_OUT := libk.a crti.o crtn.o
LIBK_SYS := $(addprefix $(LIB_PATH)/, $(LIBK_OUT))

vpath %.s arch/$(MACHINE)

clean:
	@rm -rf $(ALL_OBJS) $(ALL_DEPS) libc.a libk.a

install-headers: | $(INC_PATH)
	@cp -rfPT include $(INC_PATH)

install-libc: $(LIBC_SYS) install-headers

install-libk: $(LIBK_SYS) install-headers

libc: $(LIBC_OUT)

libk: $(LIBK_OUT)

libc.a: $(LIBC_OBJS)
	$(AR) -rcs $@ $^

libk.a: $(LIBK_OBJS)
	$(AR) -rcs $@ $^

%.o: %.s
	$(AS) $(AS_FLAGS) -M $< > $(@:.o=.d)
	$(AS) $(AS_FLAGS) -o$@ $<

%.s.libc.o: %.s
	$(AS) $(AS_FLAGS) -M $< > $(@:.o=.d)
	$(AS) $(AS_FLAGS) -o$@ $<

%.c.libc.o: %.c
	$(CC) $(CC_FLAGS) $(C_FLAGS) $(C_FLAGS_LIBC) -MMD -c -o$@ $<

%.cpp.libc.o: %.cpp
	$(CXX) $(CXX_FLAGS) $(C_FLAGS) $(C_FLAGS_LIBC) -MMD -c -o$@ $<

%.s.libk.o: %.s
	$(AS) $(AS_FLAGS) -M $< > $(@:.o=.d)
	$(AS) $(AS_FLAGS) -o$@ $<

%.c.libk.o: %.c
	$(CC) $(CC_FLAGS) $(C_FLAGS) $(C_FLAGS_LIBK) -MMD -c -o$@ $<

%.cpp.libk.o: %.cpp
	$(CXX) $(CXX_FLAGS) $(C_FLAGS) $(C_FLAGS_LIBK) -MMD -c -o$@ $<

$(LIB_PATH)/%: % | $(LIB_PATH)
	cp -Pf $< $@

$(INC_PATH) $(LIB_PATH):
	@mkdir -p $@

-include $(ALL_DEPS)
