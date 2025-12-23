# Debugging y Troubleshooting

## Visión General

Esta guía cubre técnicas avanzadas de depuración y resolución de problemas específicas para el Mini-Kernel ARMv7-A, incluyendo herramientas, metodologías y soluciones a problemas comunes.

## Herramientas de Depuración

### GDB Multiarch

#### Configuración Inicial

**Archivo `.gdbinit` en el directorio del proyecto:**
```gdb
# Configuración básica
set architecture arm
set endian little
set confirm off

# Conexión automática
define connect
    target remote localhost:2159
    file obj/bios.elf
end

# Reset del sistema
define reset
    monitor system_reset
    continue
end

# Recarga de símbolos
define reload
    file obj/bios.elf
    load
    symbol-file obj/bios.elf
end

# Mostrar información del contexto actual
define ctx
    info registers
    x/10i $pc
    bt
end

# Auto-cargar símbolos al inicio
file obj/bios.elf
```

#### Sesión de Depuración Típica

```bash
# Terminal 1: Iniciar QEMU en modo debug
make debug

# Terminal 2: Conectar GDB
gdb-multiarch
(gdb) connect
(gdb) break _start
(gdb) continue
```

### QEMU Monitor

#### Comandos de Sistema

```bash
# Conectar al monitor
telnet localhost 1234

# Información del sistema
(qemu) info version
(qemu) info registers  
(qemu) info cpus
(qemu) info memory
(qemu) info mtree

# Control de ejecución
(qemu) stop
(qemu) cont
(qemu) system_reset
(qemu) quit
```

#### Análisis de Memoria

```bash
# Examinar memoria
(qemu) x/16x 0x70010000          # 16 words en hex
(qemu) x/32b 0x70010000          # 32 bytes
(qemu) x/10i 0x70010000          # 10 instrucciones

# Dump de memoria
(qemu) dump-guest-memory memory.dump
(qemu) dump-guest-memory -p memory.dump  # Con info física

# Modificar memoria
(qemu) pmemsave 0x70010000 0x1000 bootloader.bin
```

## Técnicas de Depuración

### 1. Análisis de Boot Sequence

#### Breakpoints Críticos

```gdb
# Punto de entrada
(gdb) break _start
(gdb) break reset_vector

# Inicialización de stacks
(gdb) break init_stacks

# Board initialization  
(gdb) break board_init

# Primera tarea
(gdb) break tarea_idle
```

#### Verificar Inicialización de Stacks

```gdb
(gdb) break _start
(gdb) continue
(gdb) info registers

# Verificar stack pointers después de init
(gdb) break init_board
(gdb) continue
(gdb) print/x $sp
(gdb) x/16x $sp
```

### 2. Debug de Exception Handlers

#### Interceptar Excepciones

```gdb
# Breakpoints en handlers
(gdb) break svc_handler
(gdb) break irq_handler
(gdb) break reset_vector
(gdb) break undef_handler
(gdb) break pabt_handler
(gdb) break dabt_handler
```

#### Analizar SVC Calls

```gdb
(gdb) break C_SVC_handler
(gdb) continue
# Cuando se detenga:
(gdb) print svc_num
(gdb) print/x sp_irq
(gdb) x/8x sp_irq
```

### 3. Task Debugging

#### Monitorear Context Switches

```c
// Agregar al código para debug
void debug_task_switch(uint32_t from_task, uint32_t to_task) {
    my_printf("Context switch: Task %u -> Task %u\n", from_task, to_task);
    
    // Debug stack pointers
    my_printf("Task %u SP: 0x%08x\n", from_task, get_task_sp(from_task));
    my_printf("Task %u SP: 0x%08x\n", to_task, get_task_sp(to_task));
}
```

#### Debug Stack Overflow

```c
#define STACK_CANARY 0xDEADBEEF

void check_stack_integrity(uint32_t task_id) {
    uint32_t *stack_base = get_task_stack_base(task_id);
    
    if (stack_base[0] != STACK_CANARY) {
        my_printf("STACK OVERFLOW detected in task %u!\n", task_id);
        my_printf("Expected: 0x%08x, Got: 0x%08x\n", 
                  STACK_CANARY, stack_base[0]);
        
        // Dump stack info
        dump_stack_info(task_id);
        
        // Halt system
        while(1) { HALT_CPU; }
    }
}
```

### 4. Interrupt Debugging

#### Analizar IRQ Flow

```gdb
# Breakpoint en IRQ entry
(gdb) break irq_handler

# Examinar estado cuando se dispara
(gdb) continue
# En IRQ handler:
(gdb) info registers
(gdb) print/x *((uint32_t*)0x1E000000)  # GIC status
```

#### Monitor Timer Interrupts

```c
static uint32_t irq_count = 0;
static uint32_t last_irq_time = 0;

void debug_timer_irq(void) {
    uint32_t current_time = get_system_ticks();
    uint32_t interval = current_time - last_irq_time;
    
    irq_count++;
    
    if (irq_count % 100 == 0) {  // Log cada 100 IRQs
        my_printf("IRQ #%u, interval: %u ticks\n", irq_count, interval);
    }
    
    last_irq_time = current_time;
}
```

## Debug con Prints

### Macro System para Debug

```c
// defines.h
#ifdef DEBUG
    #define DEBUG_PRINT(fmt, ...) my_printf("[DEBUG] " fmt, ##__VA_ARGS__)
    #define DEBUG_FUNC() my_printf("[DEBUG] %s()\n", __FUNCTION__)
    #define DEBUG_LINE() my_printf("[DEBUG] %s:%d\n", __FILE__, __LINE__)
    #define DEBUG_VAR(var) my_printf("[DEBUG] %s = %u\n", #var, (uint32_t)var)
    #define DEBUG_HEX(var) my_printf("[DEBUG] %s = 0x%08x\n", #var, (uint32_t)var)
#else
    #define DEBUG_PRINT(fmt, ...)
    #define DEBUG_FUNC()
    #define DEBUG_LINE()  
    #define DEBUG_VAR(var)
    #define DEBUG_HEX(var)
#endif

// Assertions
#define ASSERT(condition) \
    do { \
        if (!(condition)) { \
            my_printf("ASSERT FAILED: %s:%d - %s\n", \
                     __FILE__, __LINE__, #condition); \
            while(1) { HALT_CPU; } \
        } \
    } while(0)
```

### Debug Prints Estratégicos

```c
void tarea1(void) {
    DEBUG_FUNC();
    uint32_t i = 0;
    
    while (1) {
        DEBUG_PRINT("Starting fibonacci calculation loop\n");
        
        for (i = 0; i < 10; i++) {
            DEBUG_VAR(i);
            uint32_t result = fibonacci(i);
            DEBUG_PRINT("fibonacci(%u) = %u\n", i, result);
            printf("Fibonacci(%u) = %u\n", i, result);
        }
        
        DEBUG_PRINT("Fibonacci loop completed\n");
    }
}
```

## Análisis de Performance

### Profiling Manual

```c
// Performance counters
static struct {
    uint32_t task_switches;
    uint32_t irq_count;
    uint32_t syscall_count;
    uint32_t boot_time;
} performance_stats = {0};

void profile_task_switch(void) {
    performance_stats.task_switches++;
    
    if (performance_stats.task_switches % 1000 == 0) {
        my_printf("Task switches: %u\n", performance_stats.task_switches);
    }
}

void profile_syscall(uint32_t syscall_num) {
    performance_stats.syscall_count++;
    
    static uint32_t syscall_histogram[256] = {0};
    syscall_histogram[syscall_num]++;
}

void print_performance_stats(void) {
    my_printf("=== Performance Statistics ===\n");
    my_printf("Task switches: %u\n", performance_stats.task_switches);
    my_printf("IRQ count: %u\n", performance_stats.irq_count);
    my_printf("Syscall count: %u\n", performance_stats.syscall_count);
    my_printf("Boot time: %u ticks\n", performance_stats.boot_time);
}
```

### Memory Usage Analysis

```bash
# Análisis de tamaño de secciones
arm-none-eabi-size -A obj/bios.elf

# Análisis de símbolos por tamaño
arm-none-eabi-nm --print-size --size-sort obj/bios.elf > symbols_by_size.txt

# Stack usage analysis (compilar con -fstack-usage)
make EXTRA_CFLAGS="-fstack-usage"
find . -name "*.su" -exec cat {} \; | sort -k2 -n
```

## Problemas Comunes y Soluciones

### 1. Sistema No Bootea

#### Síntomas
- QEMU se inicia pero no hay salida
- Sistema se cuelga después del reset

#### Diagnóstico
```gdb
# Conectar GDB y verificar PC
(gdb) connect
(gdb) info registers
(gdb) x/10i $pc

# ¿Está en el punto de entrada correcto?
(gdb) break _start
(gdb) continue
```

#### Soluciones Comunes
1. **Vector table mal configurada**:
   ```c
   // Verificar en startup.s
   copy_tabla:
       LDR R3, [R0], #4
       STR R3, [R1], #4    // ¿Dirección correcta?
   ```

2. **Stack no inicializado**:
   ```c
   // Verificar inicialización de SP
   LDR SP, =_c_stack_top_
   ```

3. **Linker script incorrecto**:
   ```ld
   // Verificar direcciones en memmap.ld
   _PUBLIC_RAM_INIT = 0x70010000;  // ¿Correcta para QEMU?
   ```

### 2. Stack Overflow

#### Síntomas
- Sistema se reinicia inesperadamente
- Comportamiento errático
- Corrupción de variables globales

#### Diagnóstico
```c
// Agregar stack canaries
void init_stack_canaries(void) {
    uint32_t *stack_base;
    
    // Para cada stack de modo
    stack_base = (uint32_t*)(_c_stack_top_ - C_STACK_SIZE);
    stack_base[0] = 0xDEADBEEF;
    
    stack_base = (uint32_t*)(_irq_stack_top_ - IRQ_STACK_SIZE);  
    stack_base[0] = 0xDEADBEEF;
    // ... etc
}

void check_all_stack_canaries(void) {
    // Verificar todos los canaries
    if (get_stack_canary(STACK_C) != 0xDEADBEEF) {
        my_printf("C Stack overflow!\n");
    }
    // ... etc
}
```

#### Soluciones
1. **Aumentar tamaño de stack**:
   ```ld
   // En memmap.ld
   IRQ_STACK_SIZE = 1K;  // Era 512
   ```

2. **Reducir uso de stack**:
   ```c
   // Evitar arrays grandes en stack
   // uint32_t big_array[1000];  // ¡NO!
   static uint32_t big_array[1000];  // SÍ
   ```

### 3. Interrupts No Funcionan

#### Síntomas
- Timer no genera ticks
- No hay context switching
- UART interrupts no se procesan

#### Diagnóstico
```gdb
# Verificar GIC está habilitado
(qemu) x/w 0x1E000000
# Debe ser != 0

# Verificar interrupts habilitadas en CPU
(gdb) info registers
# CPSR no debe tener I bit (bit 7) set
```

#### Soluciones
1. **GIC no inicializado**:
   ```c
   void gic_init(void) {
       gic_dist->ICDDCR = 1;           // Enable distributor
       gic_cpu->ICCICR = 1;            // Enable CPU interface
       gic_cpu->ICCPMR = 0xF0;         // Priority mask
   }
   ```

2. **Interrupts deshabilitadas en CPU**:
   ```assembly
   ; Habilitar interrupts después de init
   CPSIE if
   ```

3. **Vector table incorrecta**:
   ```c
   // Verificar que apunta a handlers correctos
   lit_irq_handler: .word irq_handler
   ```

### 4. Context Switch Falla

#### Síntomas
- Solo una tarea ejecuta
- Sistema se cuelga en context switch
- Pérdida de datos de tareas

#### Diagnóstico
```c
// Debug context save/restore
void debug_context_switch(uint32_t from_task, uint32_t to_task) {
    my_printf("Switch %u->%u\n", from_task, to_task);
    
    // Dump registers antes del switch
    uint32_t *sp = get_current_stack_pointer();
    for (int i = 0; i < 16; i++) {
        my_printf("R%d: 0x%08x\n", i, sp[i]);
    }
}
```

#### Soluciones
1. **Stack corruption durante save**:
   ```assembly
   ; Verificar orden correcto de PUSH/POP
   svc_handler:
       STMFD SP!, {R0-R3, R12, LR}  ; Save
       ; ...
       LDMFD SP!, {R0-R3, R12, PC}^  ; Restore
   ```

2. **SP no guardado correctamente**:
   ```c
   // Guardar SP correctamente en TCB
   current_task_tcb->stack_pointer = current_sp;
   ```

### 5. UART No Funciona

#### Síntomas
- No hay salida de printf
- my_printf no imprime nada
- Caracteres corruptos

#### Diagnóstico
```gdb
# Verificar inicialización UART
(gdb) x/w 0x10009000  # UART0 base
(gdb) x/w 0x10009030  # Control register

# Test manual
(qemu) x/w 0x10009000
(qemu) pmemsave 0x10009000 0x100 uart_regs.bin
```

#### Soluciones
1. **UART no inicializada**:
   ```c
   void uart_init(_uart_t *uart) {
       uart->CR = 0x301;     // Enable UART, TX, RX
       uart->LCRH = 0x60;    // 8-bit, no parity
       uart->IBRD = 16;      // Baud rate
       uart->FBRD = 0;
   }
   ```

2. **Dirección base incorrecta**:
   ```c
   #define UART0_ADDR 0x10009000  // Verificar contra QEMU docs
   ```

## Advanced Debugging Techniques

### 1. Trace Execution

```c
// Execution tracer
#ifdef TRACE_EXECUTION
static uint32_t trace_buffer[1000];
static uint32_t trace_index = 0;

void trace_function_entry(const char *func_name) {
    uint32_t func_hash = simple_hash(func_name);
    trace_buffer[trace_index++] = func_hash;
    
    if (trace_index >= 1000) {
        trace_index = 0;  // Circular buffer
    }
}

void dump_trace_buffer(void) {
    my_printf("=== Execution Trace ===\n");
    for (int i = 0; i < trace_index; i++) {
        my_printf("0x%08x\n", trace_buffer[i]);
    }
}
#endif
```

### 2. Memory Corruption Detection

```c
// Memory corruption detector
typedef struct {
    uint32_t magic_start;
    uint8_t data[];
    uint32_t magic_end;
} guarded_memory_t;

#define MEMORY_MAGIC 0xCAFEBABE

void* debug_malloc(size_t size) {
    guarded_memory_t *mem = malloc(sizeof(guarded_memory_t) + size + 4);
    mem->magic_start = MEMORY_MAGIC;
    *(uint32_t*)(mem->data + size) = MEMORY_MAGIC;
    return mem->data;
}

void debug_free(void *ptr) {
    guarded_memory_t *mem = (guarded_memory_t*)((uint8_t*)ptr - sizeof(uint32_t));
    
    ASSERT(mem->magic_start == MEMORY_MAGIC);
    // Check end magic too...
    
    free(mem);
}
```

### 3. Real-time Debug Dashboard

```c
// Simple text-based dashboard
void debug_dashboard(void) {
    static uint32_t last_update = 0;
    uint32_t current_time = get_system_ticks();
    
    if (current_time - last_update > 1000) {  // Update every 1000 ticks
        my_printf("\033[2J\033[H");  // Clear screen
        my_printf("=== Mini-Kernel Debug Dashboard ===\n");
        my_printf("Uptime: %u ticks\n", current_time);
        my_printf("Current task: %u\n", get_current_task());
        my_printf("Task switches: %u\n", get_task_switches());
        my_printf("IRQ count: %u\n", get_irq_count());
        my_printf("Free memory: %u bytes\n", get_free_memory());
        
        print_task_states();
        
        last_update = current_time;
    }
}
```

## Herramientas Externas

### 1. Objdump Analysis

```bash
# Disassembly completo
arm-none-eabi-objdump -D obj/bios.elf > disassembly.txt

# Solo secciones específicas
arm-none-eabi-objdump -d -j .text obj/bios.elf

# Con código fuente intercalado
arm-none-eabi-objdump -S obj/bios.elf
```

### 2. Readelf Analysis  

```bash
# Headers de secciones
arm-none-eabi-readelf -S obj/bios.elf

# Symbol table
arm-none-eabi-readelf -s obj/bios.elf

# Program headers
arm-none-eabi-readelf -l obj/bios.elf
```

### 3. Memory Analysis Tools

```bash
# Hexdump de binario
hexdump -C bin/bios.bin | head -50

# Strings en el binario
arm-none-eabi-strings obj/bios.elf

# Dependencies
arm-none-eabi-ldd obj/bios.elf  # Si aplicable
```

## Referencias

- [ARM Debugging Guide](https://developer.arm.com/documentation/101471/latest/)
- [GDB Manual](https://sourceware.org/gdb/current/onlinedocs/gdb/)
- [QEMU Monitor Commands](https://qemu.readthedocs.io/en/latest/system/monitor.html)
- [ARM Exception Handling](https://developer.arm.com/documentation/dui0471/latest/)