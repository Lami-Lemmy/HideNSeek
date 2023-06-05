# Building on Linux through wine:
## wine ../cw/mwcceppc.exe -O4,p -gccext on -enum int -Cpp_exceptions off -RTTI off -fp hard -use_lmw_stmw on -sdata 0 -sdata2 0 -ipa file -func_align 4 -c -gccinc -I../include loader.cpp -o build\\loader.plf
## wine ../cw/mwldeppc.exe -codeaddr 0x80004100 -fp hard build\\loader.plf -o build\\loader.elf
## $DEVKITPPC/bin/powerpc-eabi-objcopy build/loader.elf build/loader.bin -O binary


# Project directory
BUILD := build
TARGET := loader

# Compiler definitions
CC := ../cw/mwcceppc.exe
LD := ../cw/mwldeppc.exe
OBJCOPY := $(DEVKITPPC)/bin/powerpc-eabi-objcopy

SOURCES := loader.cpp
INCLUDE := ../include

DUMMY != mkdir -p $(BUILD)

CFLAGS := -O4,p -gccext on -enum int -Cpp_exceptions off -RTTI off -fp hard \
          -use_lmw_stmw on -sdata 0 -sdata2 0 -ipa file -func_align 4 -c -gccinc \
		  -I$(INCLUDE) $(SOURCES) -o $(BUILD)/$(TARGET).plf
LDFLAGS := -codeaddr 0x80004100 -fp hard $(BUILD)/$(TARGET).plf -o $(BUILD)/$(TARGET).elf

default: $(BUILD)/$(TARGET).bin

clean:
	@echo cleaning...
	@rm -rf $(BUILD)

$(BUILD)/$(TARGET).plf: $(SOURCES)
	@echo building ... $(TARGET).plf
	$(CC) $(CFLAGS)

$(BUILD)/$(TARGET).elf: $(BUILD)/$(TARGET).plf
	@echo linking ... $(TARGET).elf
	$(LD) $(LDFLAGS)
	
$(BUILD)/$(TARGET).bin: $(BUILD)/$(TARGET).elf
	@echo stripping ... $(TARGET).bin
	$(OBJCOPY) $(BUILD)/$(TARGET).elf $(BUILD)/$(TARGET).bin -O binary
