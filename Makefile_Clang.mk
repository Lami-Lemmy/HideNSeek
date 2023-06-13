# Project directory
BUILD := build
TARGET := HideNSeek
SYMBOLS_SRC := ../symbols.txt
SYMBOLS_OUT := $(TARGET)_P.rel.lst $(TARGET)_E.rel.lst $(TARGET)_J.rel.lst $(TARGET)_K.rel.lst

# Compiler definitions
CLANG := ../clang/clang
CC := $(CLANG)
LD := $(DEVKITPPC)/bin/powerpc-eabi-ld
PORT := ../port.py
ELF2REL := ../elf2rel/elf2rel

CFILES := $(wildcard *.c)
CPPFILES := $(wildcard *.cpp)
INCLUDE := ../include

OFILES		:=	$(CPPFILES:.cpp=_cpp.o) $(CFILES:.c=_c.o)
OFILES		:= $(addprefix $(BUILD)/, $(OFILES))

DUMMY != mkdir -p $(BUILD)

CFLAGS := --target=powerpc-gekko-ibm-kuribo-eabi -Os -fno-rtti -fno-short-enums -fshort-wchar -fdeclspec -fno-exceptions -nodefaultlibs -ffreestanding


default: $(BUILD)/$(TARGET)_P.rel $(BUILD)/$(TARGET)_E.rel \
         $(BUILD)/$(TARGET)_J.rel $(BUILD)/$(TARGET)_K.rel

clean:
	@echo cleaning...
	@rm -rf $(BUILD)
	
$(BUILD)/%_c.o: %.c
	@echo $<
	@$(CC) $(CFLAGS) -I../include -c -o $@ $<

$(BUILD)/%_cpp.o: %.cpp
	@echo $<
	@$(CC) $(CFLAGS) -I../include -c -o $@ $<

$(BUILD)/$(TARGET).elf: $(OFILES)
	@echo linking ... $(TARGET).elf
	@$(LD) -Tlink.ld -r $(OFILES) -o $@

$(BUILD)/$(TARGET)_P.rel.lst: $(SYMBOLS_SRC)
	@echo porting ... $@
	@python $(PORT) P $(SYMBOLS_SRC) $@ 
$(BUILD)/$(TARGET)_E.rel.lst: $(SYMBOLS_SRC)
	@echo porting ... $@
	@python $(PORT) E $(SYMBOLS_SRC) $@ 
$(BUILD)/$(TARGET)_J.rel.lst: $(SYMBOLS_SRC)
	@echo porting ... $@
	@python $(PORT) J $(SYMBOLS_SRC) $@ 
$(BUILD)/$(TARGET)_K.rel.lst: $(SYMBOLS_SRC)
	@echo porting ... $@
	@python $(PORT) K $(SYMBOLS_SRC) $@ 

$(BUILD)/$(TARGET)_%.rel: $(BUILD)/$(TARGET).elf $(BUILD)/$(TARGET)_%.rel.lst
	@echo output ... $@
	$(ELF2REL) -i $(BUILD)/$(TARGET).elf -s $@.lst -o $@
