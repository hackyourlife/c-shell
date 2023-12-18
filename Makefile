#!/bin/make
export	PREFIX	:=
export	CC	:=	$(PREFIX)gcc
export	LD	:=	$(PREFIX)ld -v
export	OBJCOPY	:=	$(PREFIX)objcopy
export	NM	:=	$(PREFIX)nm
export	SIZE	:=	$(PREFIX)size

#-------------------------------------------------------------------------------
.SUFFIXES:
#-------------------------------------------------------------------------------
TARGET		:=	shellcode
INCLUDES	:=	include
SOURCES		:=	src
BUILD		:=	build
LD_DIR		:=	ld

# device config
DEVICE		:=	UNIX

OPTFLAGS	:=	-Os -g -Wall -Wextra -std=c99 \
			-nostdlib -fPIE -static-pie \
			-static-libgcc \
			-ffreestanding -nodefaultlibs \
			-fno-stack-protector \
			-ffunction-sections -fdata-sections \
			-flto #-fuse-linker-plugin

CFLAGS		:=	$(OPTFLAGS) \
			-D $(DEVICE) -D _ISOC99_SOURCE \
			-DREVISION=\"$(REVISION)\" \
			-DCOMMIT=\"$(COMMIT)\" \
			-DBUILDDATE=\"$(BUILDDATE)\" \
			-DBUILDTIME=\"$(BUILDTIME)\" \
			$(INCLUDE)

LDFLAGS		:=	$(OPTFLAGS) \
			-Wl,-x -Wl,--gc-sections \
			-Wl,-T$(LD_PATH)/x86.ld \
			-Wl,-zexecstack,--no-warn-rwx-segments

CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))

ifneq ($(BUILD),$(notdir $(CURDIR)))
#-------------------------------------------------------------------------------
export	DEPSDIR	:=	$(CURDIR)/$(BUILD)
export	OFILES	:=	$(CFILES:.c=.o) $(SFILES:.s=.o)
export	VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) $(CURDIR)
export	INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
				-I$(CURDIR)/$(BUILD)
export	LD_PATH :=	$(CURDIR)/$(LD_DIR)
export	TFILES	:=	$(TARGET).bin $(TARGET).lst $(TARGET).sz

export REVISION	:=	$(shell git rev-list --count HEAD)
export COMMIT	:=	$(shell git rev-parse --short HEAD)
export BUILDDATE:=	$(shell date -u +"%Y-%m-%d")
export BUILDTIME:=	$(shell date -u +"%H:%M:%S")


.PHONY: $(BUILD) clean all

$(BUILD):
	@echo compiling...
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/$(firstword $(MAKEFILE_LIST))

clean:
	@echo "[CLEAN]"
	@rm -rf $(BUILD) $(OFILES)

$(TARGET): $(TFILES)

else

#-------------------------------------------------------------------------------
# main target
#-------------------------------------------------------------------------------
.PHONY: all

all: $(TFILES) loader.elf

loader.elf:
	@echo "[CCLD]  $(notdir $@)"
	@$(CC) -MMD -MP -MF $(DEPSDIR)/$*.d -O3 -Wall ../loader/loader.c -o $@

%.o: %.c
	@echo "[CC]    $(notdir $@)"
	@$(CC) -MMD -MP -MF $(DEPSDIR)/$*.d $(CFLAGS) -c $< -o $@

%.o: %.s
	@echo "[AS]    $(notdir $@)"
	@$(CC) -MMD -MP -MF $(DEPSDIR)/$*.d $(CFLAGS) -c $< -o $@

$(TARGET).elf: $(OFILES)
	@echo "[LD]    $(notdir $@)"
	@$(CC) $(LDFLAGS) $(OFILES) -o $@ -Wl,-Map=$(@:.elf=.map)
	@#$(LD) -m elf_x86_64 -static -pie --no-dynamic-linker --no-eh-frame-hdr -z text -nostdlib -x --gc-sections -T$(LD_PATH)/x86.ld $(OFILES) -o $@ -Map=$(@:.elf=.map)

%.lst: %.elf
	@echo "[NM]    $(notdir $@)"
	@$(NM) --size-sort -S $< > $@

%.sz: %.elf
	@echo "[SIZE]  $(notdir $@)"
	@$(SIZE) $< > $@

%.bin: %.elf
	@echo "[BIN]   $(notdir $@)"
	@$(OBJCOPY) -O binary $^ $@

-include $(DEPSDIR)/*.d

#-------------------------------------------------------------------------------
endif
#-------------------------------------------------------------------------------
