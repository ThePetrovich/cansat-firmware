# Project settings
TARG = cansat
ARCH_DEFAULT = avr
MCU_DEFAULT = atmega128

# Libraries & includes
INCLUDES = -I"./include"

# Project directories
SRCDIR = src
BUILDDIR = build
TARGDIR = bin

# Compiler & linker settings
CC = "C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe"
ASM = "C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe"
AR = "C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-ar.exe"

# AVR-specific tool settings
OBJCOPY_CMD = "C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe"
OBJDUMP_CMD = "C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objdump.exe"
AVR_SIZE_CMD = "C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-size.exe"

# Host platform settings
RM_CMD = rm -rf
MKDIR_CMD = mkdir -p
COPY_CMD = cp
 
# Compiler & linker flags
CFLAGS = -x c -DDEBUG $(INCLUDES) -Os -g2 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -mrelax -c -std=gnu99 -Wall -Wextra
ASMFLAGS = -Wa,-gdwarf2 -x assembler-with-cpp -c -B -DDEBUG $(INCLUDES) -Os -g2 -mrelax -Wall -Wextra
LDFLAGS = -Wl,-static -Wl,-Map="$(TARGDIR)/$(TARG).map" -Wl,--gc-sections -mrelax -lm -Wall -Wextra

# AVR-specific tool flags
OBJCOPY_HEX_FLAGS = -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures
OBJCOPY_EEP_FLAGS = -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex
OBJCOPY_SIGN_FLAGS = -j .user_signatures --set-section-flags=.user_signatures=alloc,load --change-section-lma .user_signatures=0 --no-change-warnings -O ihex
OBJCOPY_SREC_FLAGS = -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures
OBJDUMP_LSS_FLAGS = -h -S

#-------------------------------------------------------------------------------
# DO NOT EDIT BELOW THIS LINE
#-------------------------------------------------------------------------------

# Recursive makefile wildcard
# https://stackoverflow.com/a/12959764
rwildcard=$(wildcard $(addsuffix $2, $1)) $(foreach d,$(wildcard $(addsuffix *, $1)),$(call rwildcard,$d/,$2))

# Automatically find all source files
SRCS = $(call rwildcard, $(SRCDIR)/,*.c) main.c
ASM_SRCS = $(call rwildcard, $(SRCDIR)/,*.S)

# Generate build directory structure
DIRS = $(addprefix $(BUILDDIR)/,$(dir $(SRCS)))

# Generate list of objects
OBJS = $(addprefix $(BUILDDIR)/,$(SRCS:.c=.o)) $(addprefix $(BUILDDIR)/,$(ASM_SRCS:.S=.o))

ifndef $(mcu)
mcu = $(MCU_DEFAULT)
else
ifeq ($(strip $(mcu)),)
	$(mcu) := $(MCU_DEFAULT)
endif
endif
 
all: dirs $(TARG)

# Create build directories
dirs:
	$(MKDIR_CMD) $(BUILDDIR)
	$(MKDIR_CMD) $(TARGDIR)
ifneq ($(strip $(DIRS)),)
	$(MKDIR_CMD) $(DIRS)
endif

# Compile main target
$(TARG): $(OBJS)
	$(CC) -o $(TARGDIR)/$@.elf $(OBJS) $(LDFLAGS) -mmcu=$(mcu)
	$(OBJCOPY_CMD)  $(OBJCOPY_HEX_FLAGS) "$(TARGDIR)/$(TARG).elf" "$(TARGDIR)/$(TARG).hex"
	$(OBJCOPY_CMD)  $(OBJCOPY_EEP_FLAGS) "$(TARGDIR)/$(TARG).elf" "$(TARGDIR)/$(TARG).eep" || exit 0
	$(OBJDUMP_CMD)  $(OBJDUMP_LSS_FLAGS) "$(TARGDIR)/$(TARG).elf" > "$(TARGDIR)/$(TARG).lss"
	$(OBJCOPY_CMD)  $(OBJCOPY_SREC_FLAGS) "$(TARGDIR)/$(TARG).elf" "$(TARGDIR)/$(TARG).srec"
	$(OBJCOPY_CMD)  $(OBJCOPY_SIGN_FLAGS) "$(TARGDIR)/$(TARG).elf" "$(TARGDIR)/$(TARG).usersignatures" || exit 0
	$(AVR_SIZE_CMD) "$(TARGDIR)/$(TARG).elf"
	$(COPY_CMD) "$(TARGDIR)/$(TARG).elf" "./$(TARG).elf"

# Compile objects
$(BUILDDIR)/%.o: %.c
	$(CC) $(CFLAGS) -mmcu=$(mcu) -c -o $@ $<

$(BUILDDIR)/%.o: %.S
	$(ASM) $(ASMFLAGS) -mmcu=$(mcu) -c -o $@ $<

# Clean
cleandirs:
	$(RM_CMD) $(TARGDIR) $(BUILDDIR)

cleanobjs:
	$(RM_CMD) $(OBJS)

cleanmisc:
	$(RM_CMD) $(TARGDIR)/$(TARG).map $(TARGDIR)/$(TARG).usersignatures $(TARGDIR)/$(TARG).srec $(TARGDIR)/$(TARG).lss $(TARGDIR)/$(TARG).eep

clean: cleanobjs cleanmisc cleandirs
	$(RM_CMD) $(TARGDIR)/$(TARG).elf $(TARG).elf $(TARGDIR)/$(TARG).bin $(TARGDIR)/$(TARG).hex lib$(TARG).a $(TARGDIR)/lib$(TARG).a