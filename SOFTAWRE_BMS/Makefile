#***********************************************************************
#* @file        Makefile
#* @author      Luc Pouyadou
#* @copyright   
#* @brief       Main project builder with GCC
#***********************************************************************

PROGNAME  = BMS_V02

#***********************************************************************

LDSCRIPT    = STM32L152RETX_FLASH.ld
MCU_MODEL   = STM32L152xx
MCU_FAMILY  = STM32L1xx
MCU_SYSTEM  = system_stm32l1xx.c
MCU_STARTUP = startup_stm32l152retx.s

#***********************************************************************

OBJDIR   = build

PREFIX ?= arm-none-eabi-
TCHAIN   = $(PREFIX)
CC=$(TCHAIN)gcc
OC=$(TCHAIN)objcopy
OD=$(TCHAIN)objdump
SZ=$(TCHAIN)size
DB=$(TCHAIN)gdb

BASE_DIR = .
TOPDIR   = $(BASE_DIR)
CMSDIR   = $(TOPDIR)/LIBRARY_EXT
DEVDIR   = $(CMSDIR)/Device/Include
SRCDIR   = $(TOPDIR)/Device/
LDSDIR  ?= $(CMSDIR)/Device/gcc

MAKEFLAGS += -j$(nproc)

CPUFLAGS = -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -mfpu=auto

CFLAGS  = $(CPUFLAGS)
#CFLAGS += -I $(TOPDIR)/littlefs
CFLAGS += -I $(CMSDIR)/Include
CFLAGS += -I $(DEVDIR)/Include

CFLAGS += -D$(MCU_MODEL)
CFLAGS += -Wall -Wextra -Werror -Wfatal-errors
CFLAGS += -Wno-implicit-fallthrough
CFLAGS += -O2 -g3 -fdata-sections -ffunction-sections -nostartfiles
CFLAGS += -MMD -MP

LDFLAGS  = $(CPUFLAGS) -g3 -T $(LDSDIR)/$(LDSCRIPT)
LDFLAGS += -specs=nano.specs -u _printf_float
LDFLAGS += -Wl,--check-sections -Wl,--gc-sections -Wl,--warn-common -Wl,--warn-section-align

LDLIBS   = -lc -lm -lnosys

VPATH = $(TOPDIR)

# Simplified declaration of source files
A_SOURCES  = ./LIBRARY_EXT/startup/startup_stm32l152retx.s
C_SOURCES += $(wildcard ./LIBRARY_EXT/Device/Sources/*.c)
C_SOURCES += $(wildcard MAIN/*.c)
#C_SOURCES += $(wildcard DOMAIN/*.c)
C_SOURCES += $(wildcard PERIPH/*/*.c)
#C_SOURCES += $(wildcard LIBRARY_EXT/*.c)
#C_SOURCES += $(wildcard DATAS/*.c)

OBJS  = $(patsubst %.s, %.o, $(A_SOURCES))
OBJS += $(patsubst %.c, %.o, $(C_SOURCES))
OBJS := $(addprefix $(OBJDIR)/, $(OBJS))

ifeq ($V,)
Q=@
endif

# Just in case we need to introduce Windows customization
Remove    = rm -Rf $1
CheckDir_ = if [ ! -d "$1" ]; then echo "	MKDIR	$1"; mkdir -p "$1"; fi
CheckDir  = $(call CheckDir_,$(abspath $1))

# Main rules
all: $(OBJDIR)/$(PROGNAME).elf $(OBJDIR)/$(PROGNAME).bin $(OBJDIR)/$(PROGNAME).dis
	$(Q)$(SZ) $<

doc:
	$(Q)doxygen
	$(MAKE) -C build/latex

flash: $(OBJDIR)/$(PROGNAME).bin
	$(Q) STM32_Programmer.sh -c port=SWD mode=UR -d $< 0x8000000

read:
	$(Q) STM32_Programmer.sh -q -c port=SWD mode=HOTPLUG -r32 $(ADDR) $(SIZE)

write:
	$(Q) STM32_Programmer.sh -q -c port=SWD mode=HOTPLUG -w32 $(ADDR) $(VALUE)

regs:
	$(Q) STM32_Programmer.sh -q -c port=SWD mode=HOTPLUG -coreReg

reset:
	$(Q) STM32_Programmer.sh -q -c port=SWD mode=UR -hardRst

gdb: all
	$(Q)$(DB) -iex "set auto-load safe-path /" $(OBJDIR)/$(PROGNAME).elf

# Special rule to create graphics source files
Src/%.bin: Src/%.png
	@echo "	PNG	$@"
	$(Q)convert $< MONO:$@


# Force recompilation of main to update build date
$(OBJDIR)/MAIN/main.o: .FORCE

# Automatic rules
$(OBJDIR)/$(PROGNAME).bin: $(OBJDIR)/$(PROGNAME).elf
	@echo "	OBJCOPY	$@"
	$(Q)$(OC) -O binary $< $@

$(OBJDIR)/$(PROGNAME).dis: $(OBJDIR)/$(PROGNAME).elf
	@echo "	OBJDUMP	$@"
	$(Q)$(OD) -d $< > $@

$(OBJDIR)/%.o: %.s
	@$(call CheckDir,$(dir $@))
	@echo "	AS	$@"
	$(Q)$(CC) $(CFLAGS) -x assembler-with-cpp -c -o $@ $<

$(OBJDIR)/%.o: %.c
	@$(call CheckDir, $(dir $@))
	@echo "	CC	$@"
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

$(OBJS): Makefile

$(OBJDIR)/$(PROGNAME).elf: $(OBJS) $(LDSDIR)/$(LDSCRIPT)
	@echo "	LD	$@"
	$(Q)$(CC) $(LDFLAGS) -Wl,-Map,$(patsubst %.elf,%.map,$@) $(OBJS) $(LDLIBS) -o $@

clean:
	@$(call Remove, $(OBJDIR))

cleanall:
	$(MAKE) clean
	$(MAKE) all

.FORCE:
.PHONY: all run clean cleanall

-include $(wildcard $(OBJDIR)/*.d)
-include $(wildcard $(OBJDIR)/*/*.d)
