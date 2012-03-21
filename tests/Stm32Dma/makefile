#
#       !!!! Do NOT edit this makefile with an editor which replace tabs by spaces !!!!    
#
##############################################################################################
# 
# On command line:
#
# make all = Create project
#
# make clean = Clean project files.
#
# To rebuild project do "make clean" and "make all".
#

##############################################################################################
# Start of default section
#
PathToRoot = ../../

TRGT = arm-none-eabi-
CC   = $(TRGT)gcc
CPP  = $(TRGT)g++
CP   = $(TRGT)objcopy
AS   = $(TRGT)gcc -x assembler-with-cpp

BIN  = $(CP) -O ihex  --only-section .text --only-section .rodata --only-section .ctors --only-section .dtors
OBJDUMP = $(TRGT)objdump
SIZE = $(TRGT)size

MCU  = cortex-m3

# List all default C defines here, like -D_DEBUG=1
DDEFS = -DSTM32F10X_MD_VL -DF_CPU=8000000

# List all default ASM defines here, like -D_DEBUG=1
DADEFS = 

# List all default directories to look for include files here
DINCDIR = $(PathToRoot)mcucpp/ARM/Stm32 $(PathToRoot)mcucpp

# List the default directory to look for the libraries here
DLIBDIR =

# List all default libraries here
DLIBS = 

#
# End of default section
##############################################################################################

##############################################################################################
# Start of user section
#

# Define project name here
PROJECT = main

# Define linker script file here
LDSCRIPT_RAM = ./prj/stm32_100xB.ld

# List all user C define here, like -D_DEBUG=1
UDEFS = 

# Define ASM defines here
UADEFS = 

# List C source files here
SRC  = ./src/startup.c $(PathToRoot)mcucpp/ARM/Stm32/src/core_cm3.c
CPPSRC = ./src/main.cpp

# List ASM source files here
ASRC =

# List all user directories here
UINCDIR = ./inc

# List the user directory to look for the libraries here
ULIBDIR =

# List all user libraries here
ULIBS = 

# Define optimisation level here
OPT = -Os

#
# End of user defines
##############################################################################################


INCDIR  = $(patsubst %,-I%,$(DINCDIR) $(UINCDIR))
LIBDIR  = $(patsubst %,-L%,$(DLIBDIR) $(ULIBDIR))
DEFS    = $(DDEFS) $(UDEFS)
ADEFS   = $(DADEFS) $(UADEFS)
OBJS    = $(ASRC:.s=.o) $(SRC:.c=.o) $(CPPSRC:.cpp=.o)
LIBS    = $(DLIBS) $(ULIBS)
MCFLAGS = -mcpu=$(MCU)

ASFLAGS = $(MCFLAGS) -g -gdwarf-2 -Wa,-amhls=$(<:.s=.lst) $(ADEFS)
CPFLAGS = $(MCFLAGS) $(OPT) -gdwarf-2 -mthumb -fomit-frame-pointer -Wall -Wstrict-prototypes -fverbose-asm -Wa,-ahlms=$(<:.c=.lst) $(DEFS)
CPPFLAGS = $(MCFLAGS) $(OPT) -gdwarf-2 -mthumb -fomit-frame-pointer -Wall -fverbose-asm -Wa,-ahlms=$(<:.cpp=.lst) $(DEFS) -fno-exceptions
LDFLAGS_RAM = $(MCFLAGS) -mthumb -nostartfiles -T$(LDSCRIPT_RAM) -Wl,-Map=$(PROJECT).map,--cref $(LIBDIR)

# Generate dependency information
CPFLAGS += -MD -MP -MF .dep/$(@F).d

#
# makefile rules
#

all: $(OBJS) $(PROJECT).elf $(PROJECT).hex $(PROJECT).lss

%o : %c
	$(CC) -c $(CPFLAGS) -I . $(INCDIR) $< -o $@

%o : %cpp
	$(CPP) -c $(CPPFLAGS) -I . $(INCDIR) $< -o $@

%o : %s
	$(AS) -c $(ASFLAGS) $< -o $@

%.elf: $(OBJS)
	$(CPP) $(OBJS) $(LDFLAGS_RAM) $(LIBS) -o $@
	$(SIZE) $@

%hex: %elf
	$(BIN) $< $@

%lss: %elf
	$(OBJDUMP) -h -S $(PROJECT).elf > $(PROJECT).lss

clean:
	-rm -f -v $(OBJS)
	-rm -f -v $(PROJECT).elf
	-rm -f -v $(PROJECT).map
	-rm -f -v $(PROJECT).hex
	-rm -f -v $(PROJECT).lss
	-rm -f -v -R ./src/*.bak
	-rm -f -v -R ./src/*.lst
	-rm -f -v -R ./src/*.lss
	-rm -f -v -R .dep

# 
# Include the dependency files, should be the last of the makefile
#
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

# *** EOF ***