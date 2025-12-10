# Copyright (c) 2025 Enzo Belmonte
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

# Brief:  Makefile for ARM Cortex-A8
# Author: Enzo Belmonte <ebelmonte@frba.utn.edu.ar>
# Date:  Jun 05, 2025

# Toolchain
CHAIN = arm-none-eabi
QEMU = qemu-system-arm  

# Flags
CFLAGS = -std=gnu99 -Wall -mfpu=neon -mhard-float -mcpu=cortex-a8 -DCPU_A8 
AFLAGS = -mfpu=neon 
LDSCRIPT = memmap.ld
EXTRA_CFLAGS = 
EXTRA_AFLAGS =
LDEXTRAS =

# QEMU configuration
QEMU_MACHINE = -M realview-pb-a8 -m 32M
QEMU_FLAGS = -no-reboot -nographic
QEMU_MONITOR_PORT = -monitor telnet:127.0.0.1:1234,server,nowait
QEMU_GDB_PORT = -gdb tcp::2159
QEMU_KERNEL = -kernel $(BIN)$(BIOS).bin
EXTRA_QEMU_FLAGS =

ifdef DEBUG
  EXTRA_CFLAGS += -g -O0 -DDEBUG
  EXTRA_AFLAGS += -g 
endif

ifdef UART
  EXTRA_QEMU_FLAGS += -serial mon:stdio
endif

# Directorios
DIR = $(shell pwd)
SRC = src/
OBJ = obj/
BIN = bin/
LST = lst/
INC = inc/

# Nombre del BIOS
BIOS = bios

# Archivos fuente
SOURCES_C = $(shell find $(SRC) -name '*.c')
SOURCES_S = $(shell find $(SRC) -name '*.s')
HEADERS = $(shell find $(SRC) -name '*.h')
OBJS = $(patsubst $(SRC)%.c, $(OBJ)%.o, $(SOURCES_C))
OBJS += $(patsubst $(SRC)%.s, $(OBJ)%.o, $(SOURCES_S))

# Targets
.PHONY: all run debug clean dirs

all: dirs $(BIN)bios.bin $(OBJ)bios.elf

dirs:
	mkdir -p $(OBJ) $(BIN) $(LST)

$(BIN)bios.bin: $(OBJ)bios.elf
	@echo ""
	@echo "Generando archivo binario..."
	$(CHAIN)-objcopy -O binary $< $@
	@echo "Archivo binario generado: $@"

$(OBJ)bios.elf: $(OBJS)
	@echo ""
	@echo "Linkeando..."
	$(CHAIN)-ld -T $(LDSCRIPT) $(LDEXTRAS) $(OBJS) -o $@ -Map $(LST)bios_ld_map.map
	@echo "Linkeo finalizado!!"
	@echo "Archivo ELF generado: $@"
	@echo ""
	@echo "Generando archivos de información: mapa de memoria y símbolos"
	readelf -a $@ > $(LST)bios_readelf.txt
	$(CHAIN)-objdump -D $@ > $(LST)bios.lst

$(OBJ)%.o: $(SRC)%.c | dirs
	@mkdir -p $(dir $@)
	@echo "Compilando $< ..."
	$(CHAIN)-gcc $(CFLAGS) $(EXTRA_CFLAGS) -I $(INC) -c $< -o $@

$(OBJ)%.o: $(SRC)%.s | dirs
	@mkdir -p $(dir $@)
	@mkdir -p $(dir $(LST)$*.lst)
	@echo "Ensamblando $< ..."
	$(CHAIN)-as $(AFLAGS) $(EXTRA_AFLAGS) $< -o $@ -a > $(LST)$*.lst

run: all
	@echo "Ejecutando BIOS en QEMU..."
	@echo "Para conectarse al monitor, use: telnet localhost 1234"
	$(QEMU) $(QEMU_MACHINE) $(QEMU_FLAGS) \
	$(QEMU_MONITOR_PORT) \
	$(QEMU_KERNEL) $(EXTRA_QEMU_FLAGS)

debug: 
	$(MAKE) all DEBUG=1
	@echo "Iniciando QEMU en modo depuración..."
	@echo "Para conectarse al monitor, use: telnet localhost 1234"
	@echo "Para conectarse al depurador GDB, use: ddd -debugger gdb-multiarch $(DIR)/$(OBJ)$(BIOS).elf"
	$(QEMU) $(QEMU_MACHINE) $(QEMU_FLAGS) \
	$(QEMU_MONITOR_PORT) \
	$(QEMU_KERNEL) -S $(QEMU_GDB_PORT) $(EXTRA_QEMU_FLAGS)

clean:
	@echo "Limpiando archivos generados..."
	rm -rf $(OBJ) $(BIN) $(LST)
