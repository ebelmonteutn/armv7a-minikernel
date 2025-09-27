# API Reference

## Visión General

Esta referencia documenta todas las APIs públicas del Mini-Kernel ARMv7-A, organizadas por categorías funcionales. Las APIs están diseñadas para ser simples, eficientes y educativas.

## System Calls API

### my_printf()

Implementa printf básico para salida de texto vía UART.

```c
int my_printf(const char *format, ...);
```

**Parámetros:**
- `format`: String de formato estilo printf
- `...`: Argumentos variables según especificadores

**Retorno:**
- Número de caracteres escritos, o -1 en error

**Especificadores soportados:**
- `%s` - String
- `%u` - Unsigned integer  
- `%d` - Signed integer
- `%x` - Hexadecimal lowercase
- `%X` - Hexadecimal uppercase
- `%%` - Literal %

**Ejemplo:**
```c
void tarea1(void) {
    uint32_t value = 42;
    my_printf("Value: %u (0x%X)\n", value, value);
}
```

### printf()

Alias para my_printf para compatibilidad.

```c
#define printf my_printf
```

## Task Functions API

### Fibonacci

Calcula el número de Fibonacci para un índice dado.

```c
uint32_t fibonacci(uint32_t n);
```

**Parámetros:**
- `n`: Índice del número Fibonacci (0 ≤ n ≤ 40 recomendado)

**Retorno:**
- Número Fibonacci correspondiente

**Complejidad:** O(2^n) - implementación recursiva

**Ejemplo:**
```c
for (uint32_t i = 0; i < 10; i++) {
    uint32_t fib = fibonacci(i);
    my_printf("Fibonacci(%u) = %u\n", i, fib);
}
```

### Conjetura de Collatz

Implementa un paso del algoritmo de Collatz.

```c
uint32_t conjetura_collatz(uint32_t n);
```

**Parámetros:**
- `n`: Número entero positivo

**Retorno:**
- Siguiente número en la secuencia de Collatz
- Si n es par: n/2
- Si n es impar: 3n+1

**Ejemplo:**
```c
uint32_t num = 7;
while (num != 1) {
    my_printf("%u ", num);
    num = conjetura_collatz(num);
}
my_printf("1\n");  // Output: 7 22 11 34 17 52 26 13 40 20 10 5 16 8 4 2 1
```

### Factorización Prima

Calcula los factores primos de un número.

```c
void factorizacion_primos(uint32_t n, uint32_t factores[]);
```

**Parámetros:**
- `n`: Número a factorizar
- `factores[]`: Array para almacenar factores (debe terminar con 0)

**Retorno:**
- Ninguno (resultado en array de factores)

**Ejemplo:**
```c
uint32_t factores[20];
uint32_t numero = 28;

factorizacion_primos(numero, factores);

my_printf("Factores primos de %u: ", numero);
for (int i = 0; factores[i] != 0; i++) {
    my_printf("%u ", factores[i]);
}
// Output: Factores primos de 28: 2 2 7
```

## Hardware Abstraction Layer API

### UART Functions

#### uart_init()

Inicializa un puerto UART con configuración específica.

```c
void uart_init(_uart_t *uart, uint32_t baudrate);
```

**Parámetros:**
- `uart`: Puntero a estructura UART
- `baudrate`: Velocidad de transmisión (ej: 115200)

**Ejemplo:**
```c
_uart_t *uart0 = (_uart_t*)UART0_ADDR;
uart_init(uart0, 115200);
```

#### uart_putc()

Envía un carácter por UART.

```c
void uart_putc(_uart_t *uart, unsigned int c);
```

**Parámetros:**
- `uart`: Puntero a estructura UART
- `c`: Carácter a enviar

**Comportamiento:**
- Bloquea hasta que FIFO TX tenga espacio
- Maneja automáticamente caracteres especiales

#### uart_getc()

Recibe un carácter desde UART.

```c
unsigned int uart_getc(_uart_t *uart);
```

**Parámetros:**
- `uart`: Puntero a estructura UART

**Retorno:**
- Carácter recibido (0-255)

**Comportamiento:**
- Bloquea hasta recibir un carácter

#### uart_puts()

Envía una cadena de caracteres.

```c
void uart_puts(_uart_t *uart, const char *str);
```

**Parámetros:**
- `uart`: Puntero a estructura UART
- `str`: Cadena terminada en NULL

### Timer Functions

#### timer_init()

Inicializa un timer SP804.

```c
void timer_init(sp804_timer_t *timer, uint32_t load_value, uint32_t control);
```

**Parámetros:**
- `timer`: Puntero a estructura timer
- `load_value`: Valor inicial del contador
- `control`: Registro de control (flags combinados con OR)

**Control flags:**
- `TIMER_CTRL_EN`: Habilitar timer
- `TIMER_CTRL_PERIODIC`: Modo periódico
- `TIMER_CTRL_INTEN`: Habilitar interrupción
- `TIMER_CTRL_32BIT`: Contador de 32 bits
- `TIMER_CTRL_ONESHOT`: Modo one-shot

**Ejemplo:**
```c
sp804_timer_t *timer0 = (sp804_timer_t*)(TIMER0_1_BASE);
timer_init(timer0, 0x10000, 
           TIMER_CTRL_EN | TIMER_CTRL_PERIODIC | TIMER_CTRL_INTEN);
```

#### timer_start() / timer_stop()

Control de ejecución del timer.

```c
void timer_start(sp804_timer_t *timer);
void timer_stop(sp804_timer_t *timer);
```

#### timer_get_value()

Obtiene el valor actual del contador.

```c
uint32_t timer_get_value(sp804_timer_t *timer);
```

**Retorno:**
- Valor actual del contador (decremental)

#### timer_clear_interrupt()

Limpia la interrupción pendiente del timer.

```c
void timer_clear_interrupt(sp804_timer_t *timer);
```

### GIC Functions

#### gic_init()

Inicializa el Generic Interrupt Controller.

```c
void gic_init(void);
```

**Comportamiento:**
- Configura distribuidor e interfaz CPU
- Establece prioridades por defecto
- Habilita el controlador

#### gic_enable_interrupt() / gic_disable_interrupt()

Control de interrupciones individuales.

```c
void gic_enable_interrupt(uint32_t irq_id);
void gic_disable_interrupt(uint32_t irq_id);
```

**Parámetros:**
- `irq_id`: ID de la interrupción (ej: IRQ_TIMER0)

**IRQ IDs comunes:**
- `IRQ_TIMER0`: 36
- `IRQ_TIMER1`: 37  
- `IRQ_UART0`: 44

#### gic_get_interrupt_id()

Obtiene el ID de la interrupción actual.

```c
uint32_t gic_get_interrupt_id(void);
```

**Retorno:**
- ID de la interrupción activa (0-1023)
- 1023 si no hay interrupciones

#### gic_end_interrupt()

Señaliza el fin del manejo de una interrupción.

```c
void gic_end_interrupt(uint32_t irq_id);
```

**Parámetros:**
- `irq_id`: ID de la interrupción terminada

## Board Support Package API

### board_init()

Inicialización general de la plataforma.

```c
void board_init(void);
```

**Comportamiento:**
1. Inicializa GIC
2. Configura UART0 para debug
3. Configura timer del sistema
4. Habilita interrupciones necesarias

### get_system_ticks()

Obtiene el número de ticks del sistema desde el boot.

```c
uint32_t get_system_ticks(void);
```

**Retorno:**
- Número de ticks transcurridos

**Uso:**
```c
uint32_t start = get_system_ticks();
// ... operación ...
uint32_t elapsed = get_system_ticks() - start;
my_printf("Operation took %u ticks\n", elapsed);
```

## Kernel Services API

### Sistema de Tareas

#### get_current_task()

Obtiene el ID de la tarea actualmente ejecutándose.

```c
uint32_t get_current_task(void);
```

**Retorno:**
- ID de la tarea actual (0-3)

#### yield_cpu()

Cede voluntariamente el control del CPU (cooperativo).

```c
void yield_cpu(void);
```

**Comportamiento:**
- Fuerza un context switch al scheduler
- Permite que otras tareas se ejecuten

### Memory Utilities

#### memset()

Implementación básica de memset.

```c
void *memset(void *s, int c, size_t n);
```

**Parámetros:**
- `s`: Puntero al área de memoria
- `c`: Valor a establecer (como unsigned char)
- `n`: Número de bytes

**Retorno:**
- Puntero al área de memoria `s`

#### memcpy()

Implementación básica de memcpy.

```c
void *memcpy(void *dest, const void *src, size_t n);
```

**Parámetros:**
- `dest`: Destino de la copia
- `src`: Origen de los datos
- `n`: Número de bytes a copiar

**Retorno:**
- Puntero a `dest`

## Utility Macros

### Debug Macros

```c
#ifdef DEBUG
    #define DEBUG_PRINT(fmt, ...) my_printf("[DEBUG] " fmt, ##__VA_ARGS__)
    #define DEBUG_FUNC() my_printf("[DEBUG] %s()\n", __FUNCTION__)
    #define DEBUG_VAR(var) my_printf("[DEBUG] %s = %u\n", #var, (uint32_t)var)
#else
    #define DEBUG_PRINT(fmt, ...)
    #define DEBUG_FUNC()
    #define DEBUG_VAR(var)
#endif
```

### CPU Control Macros

```c
#define HALT_CPU    __asm__("WFI")      // Wait For Interrupt
#define NOP         __asm__("NOP")       // No Operation  
#define DSB()       __asm__("DSB")       // Data Synchronization Barrier
#define ISB()       __asm__("ISB")       // Instruction Synchronization Barrier
#define DMB()       __asm__("DMB")       // Data Memory Barrier
```

### Interrupt Control

```c
// Disable/Enable interrupts
static inline void disable_interrupts(void) {
    __asm__ volatile ("CPSID if");
}

static inline void enable_interrupts(void) {
    __asm__ volatile ("CPSIE if");
}

// Critical section macros
#define ENTER_CRITICAL_SECTION() disable_interrupts()
#define EXIT_CRITICAL_SECTION()  enable_interrupts()
```

## Constants and Addresses

### Memory Map Constants

```c
// RAM layout
#define RAM_BASE            0x70000000
#define RAM_SIZE            0x08000000    // 128MB
#define KERNEL_BASE         0x70010000
#define STACK_BASE          0x70020000

// Peripheral base addresses  
#define UART0_ADDR          0x10009000
#define UART1_ADDR          0x1000A000
#define TIMER0_1_BASE       0x10011000
#define TIMER2_3_BASE       0x10012000
#define GIC_DIST_BASE       0x1E001000
#define GIC_CPU_BASE        0x1E000100
```

### Stack Sizes

```c
#define C_STACK_SIZE        4096    // 4KB
#define SYS_STACK_SIZE      4096    // 4KB  
#define IRQ_STACK_SIZE      512     // 512B
#define FIQ_STACK_SIZE      512     // 512B
#define SVC_STACK_SIZE      512     // 512B
#define ABT_STACK_SIZE      512     // 512B
#define UND_STACK_SIZE      512     // 512B
#define TASK_STACK_SIZE     512     // 512B per task
```

### System Call Numbers

```c
#define SYS_WRITE           1       // my_printf syscall
#define SYS_READ            2       // Reserved
#define SYS_EXIT            3       // Reserved  
#define SYS_YIELD           4       // Reserved
```

## Error Codes

### Return Values

```c
#define SUCCESS             0
#define ERROR_INVALID_PARAM -1
#define ERROR_TIMEOUT       -2
#define ERROR_BUSY          -3
#define ERROR_NOT_READY     -4
#define ERROR_HARDWARE      -5
```

### Usage Example

```c
int result = uart_init_with_timeout(uart0, 115200, 1000);
if (result != SUCCESS) {
    switch (result) {
        case ERROR_TIMEOUT:
            my_printf("UART init timeout\n");
            break;
        case ERROR_HARDWARE:
            my_printf("UART hardware error\n");
            break;
        default:
            my_printf("Unknown error: %d\n", result);
    }
}
```

## Usage Examples

### Complete Task Example

```c
__attribute__((section(".custom_task_text"))) 
void custom_task(void) {
    uint32_t counter = 0;
    
    // Initialize task-specific resources
    DEBUG_FUNC();
    my_printf("Custom task started\n");
    
    while (1) {
        // Main task loop
        counter++;
        
        if (counter % 100 == 0) {
            my_printf("Task alive: %u iterations\n", counter);
            
            // Performance measurement
            uint32_t start_time = get_system_ticks();
            
            // Do some work
            uint32_t result = fibonacci(10);
            
            uint32_t elapsed = get_system_ticks() - start_time;
            my_printf("Fibonacci(10) = %u (took %u ticks)\n", 
                     result, elapsed);
        }
        
        // Cooperative yield
        if (counter % 1000 == 0) {
            yield_cpu();
        }
    }
}
```

### Interrupt Handler Example  

```c
void custom_irq_handler(void) {
    uint32_t irq_id = gic_get_interrupt_id();
    
    switch (irq_id) {
        case IRQ_TIMER0:
            // Handle timer interrupt
            timer_clear_interrupt((sp804_timer_t*)TIMER0_1_BASE);
            
            // Update system state
            system_tick_handler();
            break;
            
        case IRQ_UART0:
            // Handle UART interrupt  
            handle_uart_data();
            break;
            
        default:
            my_printf("Unhandled IRQ: %u\n", irq_id);
            break;
    }
    
    // Always acknowledge interrupt
    gic_end_interrupt(irq_id);
}
```

### Hardware Abstraction Example

```c
void setup_custom_peripheral(void) {
    // Initialize UART for communication
    _uart_t *uart = (_uart_t*)UART1_ADDR;
    uart_init(uart, 9600);
    
    // Setup timer for periodic tasks
    sp804_timer_t *timer = (sp804_timer_t*)(TIMER2_3_BASE + TIMER1_OFFSET);
    timer_init(timer, 0x100000, 
               TIMER_CTRL_EN | TIMER_CTRL_PERIODIC | TIMER_CTRL_INTEN);
    
    // Enable interrupts
    gic_enable_interrupt(IRQ_UART1);
    gic_enable_interrupt(IRQ_TIMER1);
    
    my_printf("Custom peripheral setup complete\n");
}
```

## Deprecated/Future APIs

### Planned Additions

```c
// Memory management (planned)
void* kmalloc(size_t size);
void kfree(void *ptr);

// Inter-task communication (planned)  
int send_message(uint32_t dest_task, const void *data, size_t len);
int receive_message(void *buffer, size_t max_len);

// File system (planned)
int open(const char *path, int flags);
int read(int fd, void *buffer, size_t count);
int write(int fd, const void *buffer, size_t count);
int close(int fd);
```

### Deprecated (None currently)

No hay APIs deprecated en la versión actual.

## Version History

- **v1.0.0**: API inicial con soporte básico para syscalls, HAL y tareas
- **v1.1.0**: (Planned) Adición de memory management
- **v2.0.0**: (Planned) Preemptive scheduling y IPC

## References

- [ARM AAPCS - Procedure Call Standard](https://developer.arm.com/documentation/ihi0042/)
- [ARM Cortex-A8 Technical Reference Manual](https://developer.arm.com/documentation/ddi0344/)
- [C Standard Library Reference](https://en.cppreference.com/w/c)