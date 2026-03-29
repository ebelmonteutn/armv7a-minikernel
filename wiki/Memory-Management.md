# Gestión de Memoria

## Visión General

El Mini-Kernel implementa un sistema de gestión de memoria estático y determinístico, diseñado para un entorno bare-metal sin MMU habilitada. La gestión se basa en un layout de memoria fijo definido en tiempo de compilación.

## Mapa de Memoria del Sistema

### Layout General (QEMU RealView PB-A8)

```
0x00000000 ┌─────────────────┐
           │ Exception Vector│ 32 bytes
           │ Table           │
0x00000020 ├─────────────────┤
           │ Low Memory      │
           │ (Reserved)      │
           ...
0x70000000 ├─────────────────┤
           │ RAM Region      │
           │ (128MB total)   │
0x70010000 ├─────────────────┤ <- _PUBLIC_RAM_INIT
           │                 │
           │  KERNEL SPACE   │
           │                 │
0x70020000 ├─────────────────┤ <- _PUBLIC_STACK_INIT  
           │                 │
           │  STACK SPACE    │
           │  (1MB total)    │
           │                 │
0x70120000 ├─────────────────┤
           │ Free RAM        │
           │                 │
0x78000000 └─────────────────┘
```

### Kernel Memory Layout

El kernel organiza su espacio de memoria de la siguiente manera:

```
0x70010000 ┌─────────────────┐ <- Kernel Start
           │  .text          │
           │ ┌─────────────┐ │
           │ │reset_vector │ │ <- Boot code
           │ │start        │ │
           │ │kernel_text  │ │ <- Kernel code
           │ └─────────────┘ │
           │ ┌─────────────┐ │
           │ │tareaidle_txt│ │ <- Task code sections
           │ │tarea1_text  │ │
           │ │tarea2_text  │ │
           │ │tarea3_text  │ │
           │ └─────────────┘ │
           ├─────────────────┤
           │  .rodata        │ <- Read-only data
           ├─────────────────┤
           │  .data          │ <- Initialized data
           │ ┌─────────────┐ │
           │ │tcb_data     │ │ <- Task control blocks
           │ └─────────────┘ │
           ├─────────────────┤
           │  .bss           │ <- Uninitialized data
           └─────────────────┘
```

## Stack Organization

### Stack Layout por Modo

```
0x70020000 ┌─────────────────┐ <- Stack Top
           │                 │
           │  C Stack        │ 4KB
           │  (System Mode)  │
           │                 │
0x70021000 ├─────────────────┤
           │  System Stack   │ 4KB  
           │                 │
0x70022000 ├─────────────────┤
           │  IRQ Stack      │ 512B
0x70022200 ├─────────────────┤
           │  FIQ Stack      │ 512B
0x70022400 ├─────────────────┤
           │  SVC Stack      │ 512B
0x70022600 ├─────────────────┤
           │  ABT Stack      │ 512B
0x70022800 ├─────────────────┤
           │  UND Stack      │ 512B
0x70022A00 ├─────────────────┤
           │  Task Stacks    │ 512B each
           │  (per task)     │
           └─────────────────┘
```

### Inicialización de Stacks

El archivo `startup.s` configura cada stack:

```assembly
; Configuración de stacks para cada modo
_start:
    ; Modo IRQ
    CPSID if, #MODE_IRQ
    LDR SP, =_irq_stack_top_
    
    ; Modo FIQ  
    CPSID if, #MODE_FIQ
    LDR SP, =_fiq_stack_top_
    
    ; Modo SVC
    CPSID if, #MODE_SVC
    LDR SP, =_svc_stack_top_
    
    ; ... otros modos
```

## Secciones Específicas

### 1. Vector Table Section

```c
.section .text
tabla:
    LDR PC, lit_reset_vector      // 0x00000000
    LDR PC, lit_undef_handler     // 0x00000004
    LDR PC, lit_svc_handler       // 0x00000008
    LDR PC, lit_pabt_handler      // 0x0000000C
    LDR PC, lit_dabt_handler      // 0x00000010
    LDR PC, lit_reserved_handler  // 0x00000014
    LDR PC, lit_irq_handler       // 0x00000018
    LDR PC, lit_fiq_handler       // 0x0000001C
```

### 2. Task Code Sections

Cada tarea tiene su propia sección para permitir control granular:

```c
__attribute__((section(".tareaidle_text"))) void tarea_idle(void);
__attribute__((section(".tarea1_text"))) void tarea1(void);  
__attribute__((section(".tarea2_text"))) void tarea2(void);
__attribute__((section(".tarea3_text"))) void tarea3(void);
```

### 3. Task Control Block Data

```c
__attribute__((section(".tcb_data"))) uint32_t global_tarea1 = 0;
__attribute__((section(".tcb_data"))) uint32_t global_tarea2 = 0;
```

## Linker Script (memmap.ld)

### Configuración de Memoria

```ld
/* Definiciones de memoria */
_PUBLIC_RAM_INIT = 0x70010000;
_PUBLIC_STACK_INIT = 0x70020000;

/* Tamaños de stack */
C_STACK_SIZE = 4K;
SYS_STACK_SIZE = 4K; 
IRQ_STACK_SIZE = 512;
FIQ_STACK_SIZE = 512;
SVC_STACK_SIZE = 512;
ABT_STACK_SIZE = 512;
UND_STACK_SIZE = 512;
TAREAS_STACK_SIZE = 512;

MEMORY {
    public_ram   : org = _PUBLIC_RAM_INIT, len = 128M
    public_stack : org = _PUBLIC_STACK_INIT, len = 1M  
}
```

### Definición de Secciones

```ld
SECTIONS {
    . = _PUBLIC_RAM_INIT;
    
    .text : {
        *(.reset_vector*)  /* Boot code primero */
        *(.start*)
        *(.kernel_text*)   /* Código del kernel */
        *(.tareaidle_text*)
        *(.tarea1_text*)   /* Código de tareas */
        *(.tarea2_text*)
        *(.tarea3_text*)
        *(.text*)          /* Resto del código */
    } > public_ram
    
    .rodata : {
        *(.rodata*)
    } > public_ram
    
    .data : {
        *(.tcb_data*)      /* TCBs primero */
        *(.data*)
    } > public_ram
    
    .bss : {
        *(.bss*)
        *(COMMON)
    } > public_ram
}
```

## Gestión de Stack por Tarea

### Task Control Block (Conceptual)

```c
typedef struct {
    uint32_t *sp;          // Stack pointer actual
    uint32_t state;        // Estado de la tarea
    uint32_t *stack_base;  // Base del stack
    uint32_t stack_size;   // Tamaño del stack
} task_tcb_t;
```

### Stack Frame durante Context Switch

```
Stack Top    ┌─────────────┐
             │ SPSR        │ <- Status register
             ├─────────────┤
             │ R0          │ <- Argumentos/retorno
             │ R1          │
             │ R2          │  
             │ R3          │
             ├─────────────┤
             │ R4-R11      │ <- Registros preservados
             │ (callee-    │
             │  saved)     │
             ├─────────────┤
             │ LR          │ <- Return address
             └─────────────┘
```

## Memory Safety y Debugging

### 1. Stack Overflow Detection

El kernel no implementa detección automática, pero se pueden usar técnicas:

```c
// Ejemplo de check manual
#define STACK_MAGIC 0xDEADBEEF

void check_stack_overflow(uint32_t *stack_base) {
    if (stack_base[0] != STACK_MAGIC) {
        // Stack overflow detected!
        panic("Stack overflow");
    }
}
```

### 2. Memory Layout Verification

Durante el boot se pueden verificar los símbolos:

```c
extern uint32_t __text_start__, __text_end__;
extern uint32_t __data_start__, __data_end__;
extern uint32_t __bss_start__, __bss_end__;

void verify_memory_layout(void) {
    // Verificar que las secciones no se solapen
    assert(__text_end__ <= __data_start__);
    assert(__data_end__ <= __bss_start__);
}
```

## Peripheral Memory Map

### Dispositivos Mapeados en Memoria

```
0x10000000 ┌─────────────────┐
           │ System Control  │
0x10001000 ├─────────────────┤
           │ UART0 (PL011)   │
0x10002000 ├─────────────────┤ 
           │ UART1 (PL011)   │
0x10003000 ├─────────────────┤
           │ UART2 (PL011)   │
           ...
0x10011000 ├─────────────────┤
           │ Timer 0/1       │
           │ (SP804)         │
0x10012000 ├─────────────────┤
           │ Timer 2/3       │ 
           │ (SP804)         │
           ...
0x1E000000 ├─────────────────┤
           │ GIC Distributor │
0x1E001000 ├─────────────────┤
           │ GIC CPU Interface│
           └─────────────────┘
```

## Optimizaciones Futuras

### 1. Dynamic Memory Allocation

```c
// Heap manager simple
typedef struct heap_block {
    size_t size;
    int free;
    struct heap_block *next;
} heap_block_t;

void* kmalloc(size_t size);
void kfree(void* ptr);
```

### 2. Memory Protection

- Habilitar MMU para protección de memoria
- Implementar regiones separadas user/kernel
- Stack guards automáticos

### 3. Virtual Memory

- Page tables básicas
- Memory mapping para dispositivos
- Shared memory entre tareas

## Herramientas de Análisis

### 1. Memory Map Analysis

```bash
# Ver el mapa de memoria generado
arm-none-eabi-objdump -h obj/bios.elf

# Ver símbolos y direcciones  
arm-none-eabi-nm obj/bios.elf | sort

# Tamaño de secciones
arm-none-eabi-size obj/bios.elf
```

### 2. Stack Usage Analysis

```bash
# Analizar uso de stack (con -fstack-usage)
arm-none-eabi-gcc -fstack-usage ...

# Ver archivos .su generados
cat *.su
```

## Referencias

- [ARM Memory Model](https://developer.arm.com/documentation/den0013/latest/)
- [GNU Linker Manual](https://sourceware.org/binutils/docs/ld/)
- [ARM AAPCS - Stack Usage](https://developer.arm.com/documentation/ihi0042/)