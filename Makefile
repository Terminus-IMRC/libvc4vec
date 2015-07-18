all:

TARGET := libvc4vec.so libvc4vec.a
SRCS := vc4vec.c vc4vec_local.c vc4vec_mem.c mem_node.c qpu_job_launcher.c vi_add_vi_256.c vi_add_ci_256.c
DEPS := $(SRCS:%.c=%.c.d)
OBJS := $(SRCS:%.c=%.c.o)
QASMS := vi_add_vi_256.qasm vi_add_ci_256.qasm
QBINS := $(QASMS:%.qasm=%.qasm.bin)
QHEXS := $(QASMS:%.qasm=%.qasm.bin.hex)
ALLDEPS = $(MAKEFILE_LIST_SANS_DEPS)
CFLAGS_LOCAL := -Wall -Wextra -O2 -g -pipe
LDLIBS_LOCAL := -lmailbox -lvc4v3d
ARFLAGS := cr

# $(eval $(call dep-on-c, dep, c-source))
define dep-on-c
 $(2:%.c=%.c.d) $(2:%.c=%.c.o): $1
endef

# $(eval $(call qasm-dep-on-c, qasm-source, c-source))
define qasm-dep-on-c
 $(call dep-on-c, $(1:%.qasm=%.qasm.bin), $2)
 $(call dep-on-c, $(1:%.qasm=%.qasm.bin.hex), $2)
endef

$(eval $(call qasm-dep-on-c, vi_add_vi_256.qasm, vi_add_vi_256.c))
$(eval $(call qasm-dep-on-c, vi_add_ci_256.qasm, vi_add_ci_256.c))

CC := gcc
QTC := qtc
QBIN2HEX := qbin2hex
AR := ar
RANLIB := ranlib
RM := rm -f

VALID_MAKECMDGOALS := all $(TARGET) %.c.d %.c.o clean
NONEED_DEP_MAKECMDGOALS := clean

EXTRA_MAKECMDGOALS := $(filter-out $(VALID_MAKECMDGOALS), $(MAKECMDGOALS))
ifneq '$(EXTRA_MAKECMDGOALS)' ''
  $(error No rule to make target `$(firstword $(EXTRA_MAKECMDGOALS))')
else
  ifeq '$(filter-out $(NONEED_DEP_MAKECMDGOALS), $(MAKECMDGOALS))' '$(MAKECMDGOALS)'
    sinclude $(DEPS)
	else
    ifneq '$(words $(MAKECMDGOALS))' '1'
      $(error Specify only one target if you want to make target which needs no source code dependency)
    endif
  endif
endif

MAKEFILE_LIST_SANS_DEPS := $(filter-out %.c.d, $(MAKEFILE_LIST))

COMPILE.o = $(CC) $(CFLAGS) $(CFLAGS_LOCAL) $(EXTRACFLAGS) $(CPPFLAGS) $(CPPFLAGS_LOCAL) $(EXTRACPPFLAGS) $(TARGET_ARCH) -shared
COMPILE.c = $(CC) $(CFLAGS) $(CFLAGS_LOCAL) $(EXTRACFLAGS) $(CPPFLAGS) $(CPPFLAGS_LOCAL) $(EXTRACPPFLAGS) $(TARGET_ARCH) -c
COMPILE.d = $(CC) $(CFLAGS) $(CFLAGS_LOCAL) $(EXTRACFLAGS) $(CPPFLAGS) $(CPPFLAGS_LOCAL) $(EXTRACPPFLAGS) $(TARGET_ARCH) -M -MP -MT $<.o -MF $@

all: $(TARGET)

%.a: $(OBJS) $(ALLDEPS)
	$(AR) $(ARFLAGS) $@ $(OBJS)
	$(RANLIB) $@

%.so: $(OBJS) $(ALLDEPS)
	$(COMPILE.o) $(OUTPUT_OPTION) $(OBJS) $(LOADLIBES) $(LDLIBS) $(LDLIBS_LOCAL)

%.c.o: %.c $(ALLDEPS)
	$(COMPILE.c) $(OUTPUT_OPTION) $<

%.c.d: %.c $(ALLDEPS)
	$(COMPILE.d) $<

%.qasm.bin.hex: %.qasm.bin $(ALLDEPS)
	$(QBIN2HEX) <$< >$@

%.qasm.bin: %.qasm $(ALLDEPS)
	$(QTC) <$< >$@

.PHONY: clean
clean:
	$(RM) $(TARGET)
	$(RM) $(OBJS)
	$(RM) $(DEPS)
	$(RM) $(QBINS)
	$(RM) $(QHEXS)
