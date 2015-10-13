all:

# $(eval $(call dep-on-c, dep, c-source))
define dep-on-c
 $(2:%.c=%.c.d) $(2:%.c=%.c.o): $1
endef

# $(eval $(call qasm-dep-on-c, qasm-source, c-source))
define qasm-dep-on-c
 $(call dep-on-c, $(1:%.qasm=%.qasm.bin), $2)
 $(call dep-on-c, $(1:%.qasm=%.qasm.bin.hex), $2)
endef

NNAMES := nnames.txt
QNAMES := qnames.txt
NNAMES_MK := $(NNAMES).mk
QNAMES_MK := $(QNAMES).mk
XNNAMES := x$(NNAMES)
XQNAMES := x$(QNAMES)

CC := gcc
QTC := qtc
QBIN2HEX := qbin2hex
AR := ar
RANLIB := ranlib
RM := rm -f
SED := sed

sinclude $(NNAMES_MK)
sinclude $(QNAMES_MK)

TARGET := libvc4vec.so libvc4vec.a
SRCS := vc4vec.c $(SRCS_NNAMES) $(SRCS_QNAMES)
DEPS := $(SRCS:%.c=%.c.d)
OBJS := $(SRCS:%.c=%.c.o)
QASMS := $(QASMS_QNAMES)
QBINS := $(QASMS:%.qasm=%.qasm.bin)
QHEXS := $(QASMS:%.qasm=%.qasm.bin.hex)
ALLDEPS = $(MAKEFILE_LIST_SANS_DEPS)
CFLAGS_LOCAL := -Wall -Wextra -O2 -g -pipe
LDLIBS_LOCAL := -lmailbox -lvc4v3d
ARFLAGS := cr

$(NNAMES_MK): $(NNAMES)
	$(SED) 's/^\(.*\)$$/SRCS_NNAMES += \1.c/' <$< >$@

$(QNAMES_MK): $(QNAMES)
	$(SED) 's/^\(.*\)$$/SRCS_QNAMES += \1.c/' <$< >$@
	$(SED) 's/^\(.*\)$$/QASMS_QNAMES += \1.qasm/' <$< >>$@
	$(SED) 's/^\(.*\)$$/$$(eval $$(call qasm-dep-on-c, \1.qasm, \1.c))/' <$< >>$@

$(XNNAMES): $(NNAMES)
	$(SED) 's/^\(.*\)$$/X(\1)/' <$< >$@

$(XQNAMES): $(QNAMES)
	$(SED) 's/^\(.*\)$$/X(\1)/' <$< >$@

VALID_MAKECMDGOALS := all $(TARGET) $(SRCS) $(DEPS) $(OBJS) $(QASMS) $(QBINS) $(QHEXS) $(NNAMES_MK) $(QNAMES_MK) $(XNNAMES) $(XQNAMES) clean
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
	$(RM) $(XQNAMES)
	$(RM) $(XNNAMES)
	$(RM) $(QNAMES_MK)
	$(RM) $(NNAMES_MK)
