# Capa de Abstracción de Hardware (HAL)

## Visión General

La Capa de Abstracción de Hardware (HAL) del Mini-Kernel proporciona una interfaz uniforme para acceder a los componentes de hardware de la plataforma RealView PB-A8. Esta capa permite que el kernel sea portable entre diferentes plataformas ARM manteniendo una API consistente.

## Arquitectura de la HAL

### Estructura del BSP

```
src/bsp/
├── board_init.c         # Inicialización general de la placa
├── uart/                # Driver UART PL011  
├── timer/               # Driver Timer SP804
├── gic/                 # Generic Interrupt Controller
└── memory/              # Configuración de memoria

inc/bsp/
├── board_init.h
├── uart.h
├── timer.h
└── gic.h
```

### Abstracción por Capas

```
┌─────────────────────┐
│   Kernel Services   │ <- Scheduler, Syscalls
├─────────────────────┤
│       HAL API       │ <- uart_putc(), timer_init()
├─────────────────────┤  
│   Hardware Drivers  │ <- PL011, SP804, GIC
├─────────────────────┤
│   Hardware Regs     │ <- Memory-mapped I/O
└─────────────────────┘
```

## UART Driver (PL011)

### Registros del Hardware

```c
typedef struct {
    volatile uint32_t DR;        // 0x000 - Data Register
    volatile uint32_t RSR_ECR;   // 0x004 - Receive Status/Error Clear
    volatile uint32_t reserved1[4];
    volatile uint32_t FR;        // 0x018 - Flag Register
    volatile uint32_t reserved2;
    volatile uint32_t ILPR;      // 0x020 - Low-power Counter
    volatile uint32_t IBRD;      // 0x024 - Integer Baud Rate
    volatile uint32_t FBRD;      // 0x028 - Fractional Baud Rate  
    volatile uint32_t LCRH;      // 0x02C - Line Control
    volatile uint32_t CR;        // 0x030 - Control Register
    volatile uint32_t IFLS;      // 0x034 - Interrupt FIFO Level
    volatile uint32_t IMSC;      // 0x038 - Interrupt Mask
    volatile uint32_t RIS;       // 0x03C - Raw Interrupt Status
    volatile uint32_t MIS;       // 0x040 - Masked Interrupt Status
    volatile uint32_t ICR;       // 0x044 - Interrupt Clear
    volatile uint32_t DMACR;     // 0x048 - DMA Control
} _uart_t;
```

### Definiciones de Hardware

```c
// UART Base addresses en RealView PB-A8
#define UART0_ADDR    0x10009000
#define UART1_ADDR    0x1000A000
#define UART2_ADDR    0x1000B000
#define UART3_ADDR    0x1000C000

// Flag Register bits
#define UART_FR_TXFE  (1 << 7)  // Transmit FIFO Empty
#define UART_FR_RXFF  (1 << 6)  // Receive FIFO Full
#define UART_FR_TXFF  (1 << 5)  // Transmit FIFO Full
#define UART_FR_RXFE  (1 << 4)  // Receive FIFO Empty
#define UART_FR_BUSY  (1 << 3)  // UART Busy

// Control Register bits
#define UART_CR_CTSEN (1 << 15) // CTS Enable
#define UART_CR_RTSEN (1 << 14) // RTS Enable
#define UART_CR_RTS   (1 << 11) // Request to Send
#define UART_CR_RXE   (1 << 9)  // Receive Enable
#define UART_CR_TXE   (1 << 8)  // Transmit Enable
#define UART_CR_LBE   (1 << 7)  // Loopback Enable
#define UART_CR_UARTEN (1 << 0) // UART Enable
```

### API del Driver

```c
// Inicialización
void uart_init(_uart_t *uart, uint32_t baudrate);

// I/O básico
void uart_putc(_uart_t *uart, unsigned int c);
unsigned int uart_getc(_uart_t *uart);

// I/O con verificación
int uart_putc_nonblocking(_uart_t *uart, unsigned int c);
int uart_getc_nonblocking(_uart_t *uart, unsigned int *c);

// String I/O
void uart_puts(_uart_t *uart, const char *str);
void uart_gets(_uart_t *uart, char *buffer, int max_len);

// Status
int uart_tx_ready(_uart_t *uart);
int uart_rx_ready(_uart_t *uart);
```

### Implementación del Driver

```c
void uart_init(_uart_t *uart, uint32_t baudrate) {
    // Disable UART
    uart->CR = 0;
    
    // Set baud rate (assuming 24MHz clock)
    uint32_t divisor = (24000000 * 4) / baudrate;
    uart->IBRD = divisor >> 6;
    uart->FBRD = divisor & 0x3F;
    
    // Set format: 8N1 (8-bit, no parity, 1 stop bit)
    uart->LCRH = (0x3 << 5);  // 8-bit word length
    
    // Enable UART, TX, RX
    uart->CR = UART_CR_UARTEN | UART_CR_TXE | UART_CR_RXE;
}

void uart_putc(_uart_t *uart, unsigned int c) {
    // Wait until TX FIFO not full
    while (uart->FR & UART_FR_TXFF);
    
    // Write character
    uart->DR = c;
}

unsigned int uart_getc(_uart_t *uart) {
    // Wait until RX FIFO not empty
    while (uart->FR & UART_FR_RXFE);
    
    // Read character
    return uart->DR & 0xFF;
}
```

## Timer Driver (SP804)

### Registros del Hardware

```c
typedef struct {
    volatile uint32_t LOAD;      // 0x000 - Load Register
    volatile uint32_t VALUE;     // 0x004 - Current Value
    volatile uint32_t CONTROL;   // 0x008 - Control Register
    volatile uint32_t INTCLR;    // 0x00C - Interrupt Clear
    volatile uint32_t RIS;       // 0x010 - Raw Interrupt Status
    volatile uint32_t MIS;       // 0x014 - Masked Interrupt Status
    volatile uint32_t BGLOAD;    // 0x018 - Background Load
} sp804_timer_t;
```

### Definiciones de Hardware

```c
// Timer base addresses
#define TIMER0_1_BASE 0x10011000  // Timer 0 y 1
#define TIMER2_3_BASE 0x10012000  // Timer 2 y 3

// Offsets para dual timer
#define TIMER0_OFFSET 0x00
#define TIMER1_OFFSET 0x20

// Control Register bits
#define TIMER_CTRL_EN       (1 << 7)  // Timer Enable  
#define TIMER_CTRL_PERIODIC (1 << 6)  // Periodic Mode
#define TIMER_CTRL_INTEN    (1 << 5)  // Interrupt Enable
#define TIMER_CTRL_32BIT    (1 << 1)  // 32-bit Counter
#define TIMER_CTRL_ONESHOT  (1 << 0)  // One-shot Mode

// Prescaler values
#define TIMER_CTRL_PREDIV_1   (0 << 2)  // No prescaler
#define TIMER_CTRL_PREDIV_16  (1 << 2)  // /16 prescaler  
#define TIMER_CTRL_PREDIV_256 (2 << 2)  // /256 prescaler
```

### API del Driver

```c
// Inicialización
void timer_init(sp804_timer_t *timer, uint32_t load_value, uint32_t control);

// Control
void timer_start(sp804_timer_t *timer);
void timer_stop(sp804_timer_t *timer);
void timer_reset(sp804_timer_t *timer);

// Configuración
void timer_set_load(sp804_timer_t *timer, uint32_t load_value);
void timer_set_control(sp804_timer_t *timer, uint32_t control);

// Estado
uint32_t timer_get_value(sp804_timer_t *timer);
int timer_interrupt_pending(sp804_timer_t *timer);
void timer_clear_interrupt(sp804_timer_t *timer);
```

### Implementación del Driver

```c
void timer_init(sp804_timer_t *timer, uint32_t load_value, uint32_t control) {
    // Disable timer
    timer->CONTROL = 0;
    
    // Set load value
    timer->LOAD = load_value;
    
    // Clear any pending interrupt
    timer->INTCLR = 1;
    
    // Set control register
    timer->CONTROL = control;
}

void timer_start(sp804_timer_t *timer) {
    timer->CONTROL |= TIMER_CTRL_EN;
}

void timer_stop(sp804_timer_t *timer) {
    timer->CONTROL &= ~TIMER_CTRL_EN;
}

void timer_clear_interrupt(sp804_timer_t *timer) {
    timer->INTCLR = 1;  // Any write clears interrupt
}
```

## Generic Interrupt Controller (GIC PL390)

### Registros del Hardware

#### GIC Distributor

```c
typedef struct {
    volatile uint32_t ICDDCR;        // 0x000 - Distributor Control
    volatile uint32_t ICDICTR;       // 0x004 - Interrupt Controller Type
    volatile uint32_t ICDIIDR;       // 0x008 - Distributor Implementer ID
    uint32_t reserved1[29];
    volatile uint32_t ICDISER[32];   // 0x100 - Interrupt Set-Enable
    volatile uint32_t ICDICER[32];   // 0x180 - Interrupt Clear-Enable
    volatile uint32_t ICDISPR[32];   // 0x200 - Interrupt Set-Pending
    volatile uint32_t ICDICPR[32];   // 0x280 - Interrupt Clear-Pending
    volatile uint32_t ICDABR[32];    // 0x300 - Active Bit Register
    uint32_t reserved2[32];
    volatile uint32_t ICDIPR[255];   // 0x400 - Interrupt Priority
    uint32_t reserved3;
    volatile uint32_t ICDIPTR[255];  // 0x800 - Interrupt Processor Targets
    uint32_t reserved4;
    volatile uint32_t ICDICFR[64];   // 0xC00 - Interrupt Configuration
} gic_distributor_t;
```

#### GIC CPU Interface

```c
typedef struct {
    volatile uint32_t ICCICR;        // 0x000 - CPU Interface Control
    volatile uint32_t ICCPMR;        // 0x004 - Interrupt Priority Mask
    volatile uint32_t ICCBPR;        // 0x008 - Binary Point Register
    volatile uint32_t ICCIAR;        // 0x00C - Interrupt Acknowledge
    volatile uint32_t ICCEOIR;       // 0x010 - End of Interrupt
    volatile uint32_t ICCRPR;        // 0x014 - Running Priority
    volatile uint32_t ICCHPIR;       // 0x018 - Highest Priority Pending
    volatile uint32_t ICCABPR;       // 0x01C - Aliased Binary Point
} gic_cpu_interface_t;
```

### Definiciones de Hardware

```c
// GIC base addresses en RealView PB-A8
#define GIC_DIST_BASE 0x1E001000
#define GIC_CPU_BASE  0x1E000100

// Interrupt IDs
#define IRQ_TIMER0        36
#define IRQ_TIMER1        37
#define IRQ_UART0         44
#define IRQ_UART1         45
#define IRQ_UART2         46
#define IRQ_UART3         47

// GIC constants
#define GIC_CPU_CTRL_ENABLE     (1 << 0)
#define GIC_DIST_CTRL_ENABLE    (1 << 0)
#define GIC_MAX_INTERRUPTS      1024
```

### API del Driver

```c
// Inicialización
void gic_init(void);

// Control de interrupciones
void gic_enable_interrupt(uint32_t irq_id);
void gic_disable_interrupt(uint32_t irq_id);

// Manejo de interrupciones
uint32_t gic_get_interrupt_id(void);
void gic_end_interrupt(uint32_t irq_id);

// Configuración
void gic_set_priority(uint32_t irq_id, uint32_t priority);
void gic_set_target(uint32_t irq_id, uint32_t cpu_mask);
```

### Implementación del Driver

```c
static gic_distributor_t *gic_dist = (gic_distributor_t*)GIC_DIST_BASE;
static gic_cpu_interface_t *gic_cpu = (gic_cpu_interface_t*)GIC_CPU_BASE;

void gic_init(void) {
    // Disable GIC
    gic_dist->ICDDCR = 0;
    gic_cpu->ICCICR = 0;
    
    // Initialize all interrupts as disabled, edge-triggered, priority 0xA0
    for (int i = 0; i < 32; i++) {
        gic_dist->ICDICER[i] = 0xFFFFFFFF;  // Disable all
        gic_dist->ICDICPR[i] = 0xFFFFFFFF;  // Clear pending
    }
    
    // Set all priorities to default
    for (int i = 0; i < 255; i++) {
        gic_dist->ICDIPR[i] = 0xA0A0A0A0;
    }
    
    // Set all interrupts to target CPU 0
    for (int i = 0; i < 255; i++) {
        gic_dist->ICDIPTR[i] = 0x01010101;
    }
    
    // Enable GIC distributor
    gic_dist->ICDDCR = GIC_DIST_CTRL_ENABLE;
    
    // Enable GIC CPU interface  
    gic_cpu->ICCPMR = 0xF0;  // Priority mask
    gic_cpu->ICCICR = GIC_CPU_CTRL_ENABLE;
}

void gic_enable_interrupt(uint32_t irq_id) {
    uint32_t reg = irq_id / 32;
    uint32_t bit = irq_id % 32;
    
    gic_dist->ICDISER[reg] = (1 << bit);
}

uint32_t gic_get_interrupt_id(void) {
    uint32_t irq = gic_cpu->ICCIAR;
    return irq & 0x3FF;  // Interrupt ID is in bits [9:0]
}

void gic_end_interrupt(uint32_t irq_id) {
    gic_cpu->ICCEOIR = irq_id;
}
```

## Board Initialization

### Secuencia de Inicialización

```c
void board_init(void) {
    // 1. Initialize GIC first (needed for other peripherals)
    gic_init();
    
    // 2. Initialize UART for early debug output
    uart_init((_uart_t*)UART0_ADDR, 115200);
    
    // 3. Initialize system timer
    timer_init((sp804_timer_t*)(TIMER0_1_BASE + TIMER0_OFFSET),
               0x10000,  // Load value for ~1ms at 24MHz
               TIMER_CTRL_EN | TIMER_CTRL_PERIODIC | 
               TIMER_CTRL_INTEN | TIMER_CTRL_32BIT);
    
    // 4. Enable timer interrupt in GIC
    gic_enable_interrupt(IRQ_TIMER0);
    
    // 5. Initialize other peripherals as needed
    // ...
    
    // 6. Final system configuration
    my_printf("Board initialization complete\n");
}
```

## Memory Map Abstraction

### Physical Memory Layout

```c
// RealView PB-A8 Memory Map
#define RAM_BASE        0x70000000
#define RAM_SIZE        0x08000000  // 128MB

#define ROM_BASE        0x00000000  
#define ROM_SIZE        0x04000000  // 64MB

#define PERIPH_BASE     0x10000000
#define PERIPH_SIZE     0x10000000  // 256MB

// Peripheral base addresses
#define UART0_BASE      0x10009000
#define UART1_BASE      0x1000A000
#define TIMER01_BASE    0x10011000
#define TIMER23_BASE    0x10012000
#define GIC_DIST_BASE   0x1E001000
#define GIC_CPU_BASE    0x1E000100
```

### Memory Access Macros

```c
// Safe memory access macros
#define READ_REG32(addr)    (*(volatile uint32_t*)(addr))
#define WRITE_REG32(addr, val) (*(volatile uint32_t*)(addr) = (val))

#define SET_BIT(reg, bit)   ((reg) |= (1 << (bit)))
#define CLEAR_BIT(reg, bit) ((reg) &= ~(1 << (bit)))
#define TEST_BIT(reg, bit)  (((reg) >> (bit)) & 1)

// Peripheral register access
#define UART_REG(base, offset) (*(volatile uint32_t*)((base) + (offset)))
#define TIMER_REG(base, offset) (*(volatile uint32_t*)((base) + (offset)))
```

## Portabilidad

### Abstracción de Plataforma

```c
// Platform-specific configuration
typedef struct {
    uint32_t ram_base;
    uint32_t ram_size;
    uint32_t uart_base[4];
    uint32_t timer_base[2];
    uint32_t gic_dist_base;
    uint32_t gic_cpu_base;
    uint32_t system_clock;
} platform_config_t;

// RealView PB-A8 configuration
const platform_config_t realview_pba8_config = {
    .ram_base = 0x70000000,
    .ram_size = 0x08000000,
    .uart_base = {0x10009000, 0x1000A000, 0x1000B000, 0x1000C000},
    .timer_base = {0x10011000, 0x10012000},
    .gic_dist_base = 0x1E001000,
    .gic_cpu_base = 0x1E000100,
    .system_clock = 24000000
};
```

### Conditional Compilation

```c
// Platform-specific code
#ifdef PLATFORM_REALVIEW_PBA8
    #include "bsp/realview_pba8.h"
#elif defined(PLATFORM_VEXPRESS_A9)
    #include "bsp/vexpress_a9.h"
#elif defined(PLATFORM_ZYNQ)
    #include "bsp/zynq.h"
#else
    #error "No platform specified"
#endif

// Board-specific initialization
void platform_init(void) {
#ifdef PLATFORM_REALVIEW_PBA8
    realview_board_init();
#elif defined(PLATFORM_VEXPRESS_A9)  
    vexpress_board_init();
#endif
}
```

## Debug y Testing del HAL

### Hardware Testing Framework

```c
// HAL test suite
typedef struct {
    const char *name;
    int (*test_func)(void);
} hal_test_t;

int test_uart_loopback(void) {
    _uart_t *uart = (_uart_t*)UART0_ADDR;
    
    // Enable loopback mode
    uart->CR |= UART_CR_LBE;
    
    // Test character transmission
    char test_char = 'A';
    uart_putc(uart, test_char);
    char received = uart_getc(uart);
    
    // Disable loopback
    uart->CR &= ~UART_CR_LBE;
    
    return (received == test_char) ? 0 : -1;
}

int test_timer_basic(void) {
    sp804_timer_t *timer = (sp804_timer_t*)(TIMER0_1_BASE + TIMER0_OFFSET);
    
    timer_init(timer, 1000, TIMER_CTRL_32BIT | TIMER_CTRL_ONESHOT);
    timer_start(timer);
    
    // Timer should count down
    uint32_t initial = timer_get_value(timer);
    // Small delay
    for (volatile int i = 0; i < 1000; i++);
    uint32_t final = timer_get_value(timer);
    
    return (final < initial) ? 0 : -1;
}

const hal_test_t hal_tests[] = {
    {"UART Loopback", test_uart_loopback},
    {"Timer Basic", test_timer_basic},
    {NULL, NULL}
};

void run_hal_tests(void) {
    my_printf("=== HAL Test Suite ===\n");
    
    for (int i = 0; hal_tests[i].name; i++) {
        my_printf("Testing %s... ", hal_tests[i].name);
        
        if (hal_tests[i].test_func() == 0) {
            my_printf("PASS\n");
        } else {
            my_printf("FAIL\n");
        }
    }
}
```

### Performance Monitoring

```c
// HAL performance counters
typedef struct {
    uint32_t uart_tx_count;
    uint32_t uart_rx_count;
    uint32_t timer_irq_count;
    uint32_t gic_irq_count;
} hal_stats_t;

static hal_stats_t hal_stats = {0};

void hal_uart_tx_hook(void) {
    hal_stats.uart_tx_count++;
}

void hal_timer_irq_hook(void) {
    hal_stats.timer_irq_count++;
}

void print_hal_stats(void) {
    my_printf("=== HAL Statistics ===\n");
    my_printf("UART TX: %u\n", hal_stats.uart_tx_count);
    my_printf("UART RX: %u\n", hal_stats.uart_rx_count);
    my_printf("Timer IRQs: %u\n", hal_stats.timer_irq_count);
    my_printf("GIC IRQs: %u\n", hal_stats.gic_irq_count);
}
```

## Extensiones Futuras

### 1. DMA Support

```c
// DMA controller abstraction
typedef struct {
    void *src_addr;
    void *dest_addr; 
    uint32_t size;
    uint32_t config;
} dma_transfer_t;

void dma_init(void);
int dma_transfer(dma_transfer_t *transfer);
void dma_wait_complete(int channel);
```

### 2. GPIO Support

```c
// GPIO abstraction
typedef enum {
    GPIO_INPUT,
    GPIO_OUTPUT,
    GPIO_ALTERNATE
} gpio_mode_t;

void gpio_set_mode(int pin, gpio_mode_t mode);
void gpio_write(int pin, int value);
int gpio_read(int pin);
```

### 3. I2C/SPI Drivers

```c
// I2C driver
void i2c_init(uint32_t base_addr);
int i2c_write(uint8_t addr, const uint8_t *data, size_t len);
int i2c_read(uint8_t addr, uint8_t *data, size_t len);

// SPI driver  
void spi_init(uint32_t base_addr);
void spi_transfer(const uint8_t *tx_data, uint8_t *rx_data, size_t len);
```

## Referencias

- [ARM PrimeCell UART (PL011) Technical Reference Manual](https://developer.arm.com/documentation/ddi0183/)
- [ARM Dual-Timer Module (SP804) Technical Reference Manual](https://developer.arm.com/documentation/ddi0271/)
- [ARM Generic Interrupt Controller (GIC) Architecture Specification](https://developer.arm.com/documentation/ihi0048/)
- [RealView Platform Baseboard for ARM11 MPCore User Guide](https://developer.arm.com/documentation/dui0224/)