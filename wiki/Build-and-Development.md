# Build y Desarrollo

## Configuración del Entorno

### Requisitos del Sistema

- **Sistema Operativo**: Linux (Ubuntu/Debian recomendado), macOS, o WSL2 en Windows
- **Memoria**: Mínimo 2GB RAM disponible
- **Espacio en disco**: Al menos 1GB libre
- **Conexión a internet**: Para descargar herramientas

### Herramientas Necesarias

#### 1. ARM GNU Toolchain

**Instalación en Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install gcc-arm-none-eabi binutils-arm-none-eabi
```

**Instalación en macOS:**
```bash
# Con Homebrew
brew install arm-none-eabi-gcc

# Con MacPorts  
sudo port install arm-none-eabi-gcc
```

**Descarga directa:**
- [ARM Developer Website](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-a/downloads)
- Extraer en `/opt/gcc-arm/` o directorio similar
- Añadir al PATH: `export PATH=/opt/gcc-arm/bin:$PATH`

#### 2. QEMU System ARM

**Ubuntu/Debian:**
```bash
sudo apt install qemu-system-arm
```

**macOS:**
```bash
brew install qemu
```

**Desde fuente:**
```bash
git clone https://github.com/qemu/qemu.git
cd qemu
./configure --target-list=arm-softmmu
make -j$(nproc)
sudo make install
```

#### 3. Herramientas de Desarrollo

```bash
# Build tools
sudo apt install build-essential make

# Debugging tools  
sudo apt install gdb-multiarch ddd

# Telnet client para QEMU monitor
sudo apt install telnet

# Optional: Análisis de código
sudo apt install cppcheck clang-tidy
```

### Verificación de la Instalación

```bash
# Verificar ARM toolchain
arm-none-eabi-gcc --version
arm-none-eabi-objdump --version

# Verificar QEMU
qemu-system-arm --version

# Verificar make
make --version
```

## Compilación

### Estructura del Build System

El proyecto utiliza GNU Make con un Makefile principal:

```
Makefile              # Build script principal
├── Toolchain setup   # Configuración de herramientas  
├── Directory setup   # Creación de directorios
├── File discovery    # Búsqueda de archivos fuente
├── Compilation rules # Reglas de compilación
└── Utility targets   # Targets de utilidad
```

### Variables del Makefile

```makefile
# Toolchain
CHAIN = arm-none-eabi
QEMU = qemu-system-arm

# Compilation flags
CFLAGS = -std=gnu99 -Wall -mfpu=neon -mhard-float -mcpu=cortex-a8 -DCPU_A8
AFLAGS = -mfpu=neon
LDSCRIPT = memmap.ld

# QEMU configuration  
QEMU_MACHINE = -M realview-pb-a8 -m 32M
QEMU_FLAGS = -no-reboot -nographic
```

### Targets Disponibles

#### Compilación Básica

```bash
# Compilar todo el proyecto
make

# Equivalente a:
make all
```

**Proceso de compilación:**
1. Crear directorios (`obj/`, `bin/`, `lst/`)
2. Compilar archivos `.c` a `.o`
3. Ensamblar archivos `.s` a `.o`  
4. Enlazar todos los objetos
5. Generar archivos ELF y BIN

#### Compilación Debug

```bash
# Compilar con símbolos de debug
make DEBUG=1

# O usar target específico
make debug
```

**Flags adicionales en modo debug:**
- `-g`: Información de debug
- `-O0`: Sin optimización
- `-DDEBUG`: Define macro DEBUG

#### Ejecución

```bash
# Ejecutar en QEMU
make run

# Con output de UART en consola
make run UART=1
```

#### Depuración

```bash
# Ejecutar en modo debug (QEMU pausa esperando GDB)
make debug

# En otra terminal:
gdb-multiarch obj/bios.elf
(gdb) target remote localhost:2159
(gdb) continue
```

#### Limpieza

```bash
# Eliminar archivos generados
make clean
```

### Compilación Paso a Paso

#### 1. Descubrir Archivos

```makefile
SOURCES_C = $(shell find $(SRC) -name '*.c')
SOURCES_S = $(shell find $(SRC) -name '*.s')
OBJS = $(patsubst $(SRC)%.c, $(OBJ)%.o, $(SOURCES_C))
OBJS += $(patsubst $(SRC)%.s, $(OBJ)%.o, $(SOURCES_S))
```

#### 2. Compilar C a Objeto

```makefile
$(OBJ)%.o: $(SRC)%.c | dirs
	@mkdir -p $(dir $@)
	@echo "Compilando $< ..."
	$(CHAIN)-gcc $(CFLAGS) $(EXTRA_CFLAGS) -I $(INC) -c $< -o $@
```

#### 3. Ensamblar S a Objeto

```makefile  
$(OBJ)%.o: $(SRC)%.s | dirs
	@mkdir -p $(dir $@)
	@mkdir -p $(dir $(LST)$*.lst)
	@echo "Ensamblando $< ..."
	$(CHAIN)-as $(AFLAGS) $(EXTRA_AFLAGS) $< -o $@ -a > $(LST)$*.lst
```

#### 4. Enlazar Ejecutable

```makefile
$(OBJ)$(BIOS).elf: $(OBJS)
	@echo "Enlazando $@ ..."
	$(CHAIN)-ld -T $(LDSCRIPT) $(OBJS) -o $@
	$(CHAIN)-objdump -D $@ > $(LST)$(BIOS).list
```

#### 5. Generar Binario

```makefile
$(BIN)$(BIOS).bin: $(OBJ)$(BIOS).elf
	@echo "Generando binario $@ ..."
	$(CHAIN)-objcopy -O binary $< $@
```

## Configuración de Desarrollo

### Editor/IDE Recomendados

#### Visual Studio Code

**Extensiones recomendadas:**
```json
{
    "recommendations": [
        "ms-vscode.cpptools",
        "webfreak.debug", 
        "ms-vscode.makefile-tools",
        "dan-c-underwood.arm",
        "ms-vscode.hexeditor"
    ]
}
```

**Configuración del workspace (.vscode/settings.json):**
```json
{
    "C_Cpp.default.compilerPath": "/usr/bin/arm-none-eabi-gcc",
    "C_Cpp.default.includePath": [
        "${workspaceFolder}/inc",
        "${workspaceFolder}/src"
    ],
    "C_Cpp.default.defines": [
        "CPU_A8",
        "DEBUG"
    ],
    "files.associations": {
        "*.s": "arm",
        "*.ld": "linker-script"
    }
}
```

#### Vim/Neovim

**Plugins útiles:**
- `ctrlp.vim`: Navegación de archivos
- `ale`: Linting en tiempo real
- `vim-commentary`: Comentarios rápidos
- `tagbar`: Vista de funciones/símbolos

**Configuración .vimrc:**
```vim
" ARM assembly syntax
au BufNewFile,BufRead *.s,*.S set filetype=arm

" C settings
set tabstop=4
set shiftwidth=4
set expandtab

" Make integration
set makeprg=make\ -C\ %:p:h
```

### Análisis Estático de Código

#### Cppcheck

```bash
# Instalar
sudo apt install cppcheck

# Ejecutar análisis
cppcheck --enable=all --inconclusive --std=c99 src/
```

#### Clang Static Analyzer

```bash
# Instalar
sudo apt install clang-tools

# Análisis
scan-build make clean
scan-build make
```

### Herramientas de Profiling

#### Análisis de Tamaño

```bash
# Tamaño de secciones
arm-none-eabi-size obj/bios.elf

# Información detallada
arm-none-eabi-objdump -h obj/bios.elf

# Símbolos ordenados por tamaño  
arm-none-eabi-nm --print-size --size-sort obj/bios.elf
```

#### Análisis de Stack

```bash
# Compilar con información de stack usage
make EXTRA_CFLAGS="-fstack-usage"

# Ver archivos .su generados
find . -name "*.su" -exec cat {} \;
```

#### Map File Analysis

```bash  
# Generar map file
make LDEXTRAS="-Map=obj/bios.map"

# Analizar uso de memoria
grep -E "^\.text|^\.data|^\.bss" obj/bios.map
```

## Depuración Avanzada

### GDB Multiarch Setup

**Archivo .gdbinit:**
```gdb
# Configuración inicial
set architecture arm
set endian little
target remote localhost:2159

# Comandos útiles
define reset
    monitor system_reset
end

define reload  
    file obj/bios.elf
    load
end

# Auto-load symbols
file obj/bios.elf
```

### Debugging Techniques

#### 1. Hardware Breakpoints

```gdb
# Breakpoint en función
(gdb) break main
(gdb) break tarea1

# Breakpoint en dirección
(gdb) break *0x70010000

# Conditional breakpoints
(gdb) break tarea1 if i == 5
```

#### 2. Watchpoints

```gdb
# Watch variable changes
(gdb) watch global_tarea1

# Watch memory location
(gdb) watch *0x70020000
```

#### 3. Stack Trace

```gdb
# Ver call stack
(gdb) backtrace

# Ver registros
(gdb) info registers

# Ver memoria
(gdb) x/16x $sp
```

#### 4. Dissassembly

```gdb
# Disensamblar función
(gdb) disas tarea1

# Disensamblar desde PC
(gdb) x/10i $pc
```

### QEMU Monitor Commands

**Conectar al monitor:**
```bash
telnet localhost 1234
```

**Comandos útiles:**
```
# Información del sistema
(qemu) info registers
(qemu) info memory
(qemu) info mtree

# Control de ejecución  
(qemu) stop
(qemu) cont
(qemu) system_reset

# Memory dump
(qemu) x/16x 0x70010000
(qemu) dump-guest-memory mem.dump
```

## Testing

### Unit Testing Framework

**Estructura básica para tests:**
```c
// test_framework.h
#define ASSERT(condition) \
    do { \
        if (!(condition)) { \
            my_printf("ASSERT FAILED: %s:%d\n", __FILE__, __LINE__); \
            return -1; \
        } \
    } while(0)

#define TEST_PASS 0
#define TEST_FAIL -1

// test_fibonacci.c
int test_fibonacci(void) {
    ASSERT(fibonacci(0) == 0);
    ASSERT(fibonacci(1) == 1); 
    ASSERT(fibonacci(5) == 5);
    ASSERT(fibonacci(10) == 55);
    
    my_printf("fibonacci tests: PASSED\n");
    return TEST_PASS;
}
```

### Integration Testing

**Test de sistema completo:**
```bash
#!/bin/bash
# run_tests.sh

echo "Building kernel..."
make clean && make || exit 1

echo "Running system tests..."
timeout 30s make run UART=1 > test_output.txt 2>&1 &
QEMU_PID=$!

sleep 5
echo "system_reset" | telnet localhost 1234
sleep 10

kill $QEMU_PID
grep -q "Fibonacci" test_output.txt && echo "Test 1: PASS" || echo "Test 1: FAIL"
grep -q "Collatz" test_output.txt && echo "Test 2: PASS" || echo "Test 2: FAIL"
```

### Performance Testing

```c
// performance_tests.c
void test_task_switching_overhead(void) {
    uint32_t start_ticks = get_system_ticks();
    
    // Force multiple context switches
    for (int i = 0; i < 100; i++) {
        yield_cpu();
    }
    
    uint32_t end_ticks = get_system_ticks();
    my_printf("100 context switches took %u ticks\n", end_ticks - start_ticks);
}
```

## Continuous Integration

### GitHub Actions Workflow

```yaml
# .github/workflows/build.yml
name: Build and Test

on: [push, pull_request]

jobs:
  build:
    runs-on: ubuntu-latest
    
    steps:
    - uses: actions/checkout@v2
    
    - name: Install ARM toolchain
      run: |
        sudo apt update
        sudo apt install gcc-arm-none-eabi qemu-system-arm
        
    - name: Build kernel
      run: make
      
    - name: Run tests  
      run: |
        timeout 30s make run UART=1 > test_output.txt 2>&1 &
        sleep 10
        pkill qemu
        grep -q "Fibonacci" test_output.txt
```

## Portabilidad

### Portar a Otro Hardware

#### 1. Modificar memmap.ld

```ld
/* Para hardware diferente */
_PUBLIC_RAM_INIT = 0x40000000;  /* Nueva dirección de RAM */
_PUBLIC_STACK_INIT = 0x40010000;

MEMORY {
    ram : org = _PUBLIC_RAM_INIT, len = 64M  /* Tamaño diferente */
}
```

#### 2. Actualizar BSP

```c
// board/new_board.c
#define NEW_UART_BASE 0x40001000
#define NEW_TIMER_BASE 0x40002000

void board_init(void) {
    // Configuración específica del nuevo hardware
    uart_init((uart_t*)NEW_UART_BASE);
    timer_init((timer_t*)NEW_TIMER_BASE);
}
```

#### 3. Modificar Makefile

```makefile
ifdef NEW_BOARD
    CFLAGS += -DNEW_BOARD
    QEMU_MACHINE = -M new-machine
endif
```

### Cross-compilation

```makefile
# Para diferentes architecturas ARM
ifdef CORTEX_A9
    CFLAGS += -mcpu=cortex-a9 -DCPU_A9
    QEMU_MACHINE = -M realview-pbx-a9
endif

ifdef CORTEX_A15  
    CFLAGS += -mcpu=cortex-a15 -DCPU_A15
    QEMU_MACHINE = -M vexpress-a15
endif
```

## Troubleshooting

### Problemas Comunes

#### 1. Toolchain no encontrado

```bash
# Error: arm-none-eabi-gcc: command not found
# Solución:
export PATH=/path/to/arm-toolchain/bin:$PATH

# O instalar:
sudo apt install gcc-arm-none-eabi
```

#### 2. QEMU no inicia

```bash
# Error: Could not access KVM kernel module
# Solución: Usar emulación de software
make run EXTRA_QEMU_FLAGS="-accel tcg"
```

#### 3. GDB no conecta

```bash
# Error: Remote connection closed
# Verificar que QEMU está en modo debug
make debug

# En otra terminal verificar puerto
netstat -an | grep 2159
```

#### 4. Símbolos de debug no cargan

```bash
# Recompilar con debug
make clean
make DEBUG=1

# Verificar símbolos
arm-none-eabi-objdump -t obj/bios.elf | head
```

### Logs y Debugging

#### Habilitar logs detallados

```bash
# QEMU verbose
make run EXTRA_QEMU_FLAGS="-d in_asm,cpu"

# Compilación verbose  
make V=1
```

#### Análisis de crashes

```bash
# Core dump en QEMU
make run EXTRA_QEMU_FLAGS="-s -S"

# GDB post-mortem
(gdb) file obj/bios.elf  
(gdb) target remote localhost:1234
(gdb) info registers
(gdb) backtrace
```

## Referencias

- [GNU Make Manual](https://www.gnu.org/software/make/manual/)
- [ARM GCC Toolchain Documentation](https://gcc.gnu.org/onlinedocs/gcc-4.7.3/gcc/ARM-Options.html)
- [QEMU System Emulation User Guide](https://qemu.readthedocs.io/en/latest/system/)
- [GDB User Manual](https://sourceware.org/gdb/current/onlinedocs/gdb/)