# Glosario

## A

**AAPCS (ARM Architecture Procedure Call Standard)**
Standard que define cómo las funciones pasan parámetros y manejan la pila en arquitectura ARM.

**ARM (Advanced RISC Machines)**
Familia de arquitecturas de procesadores RISC de 32-bit diseñados por ARM Holdings.

**ARMv7-A (ARM Architecture version 7 - Application)**
Versión de la arquitectura ARM optimizada para aplicaciones de alto rendimiento, incluye soporte para virtualización y múltiples cores.

**ASM (Assembly Language)**
Lenguaje de programación de bajo nivel que corresponde directamente a instrucciones de máquina.

**ASSERT**
Macro de debugging que verifica una condición y detiene el programa si es falsa.

## B

**Bare-Metal**
Programación directa sobre hardware sin sistema operativo subyacente.

**Bootloader**
Código que se ejecuta al inicio para configurar el hardware básico y cargar el kernel principal.

**BSP (Board Support Package)**
Capa de software que abstrae el hardware específico de una placa.

**Branch**
Instrucción que cambia el flujo de ejecución del programa saltando a otra dirección.

## C

**Context Switch**
Proceso de guardar el estado de una tarea y cargar el estado de otra tarea.

**Cooperative Multitasking**
Sistema donde las tareas deben voluntariamente ceder el control del procesador.

**CPSR (Current Program Status Register)**
Registro que contiene flags de estado del procesador ARM, incluyendo modo actual e interrupciones.

**Cross-Compilation**
Proceso de compilar código en una plataforma (x86) para ejecutar en otra (ARM).

## D

**DMA (Direct Memory Access)**
Característica que permite a periféricos acceder directamente a memoria sin involucrar al CPU.

**DSB (Data Synchronization Barrier)**
Instrucción que asegura que todas las operaciones de memoria anteriores se completen antes de continuar.

## E

**ELF (Executable and Linkable Format)**
Formato estándar para archivos ejecutables, objetos y bibliotecas compartidas.

**Exception Handler**
Rutina de código que se ejecuta cuando ocurre una excepción (interrupción, fault, etc.).

## F

**FIQ (Fast Interrupt Request)**
Tipo de interrupción ARM de alta prioridad con respuesta más rápida que IRQ.

**FIFO (First In, First Out)**
Estructura de datos tipo cola donde el primer elemento en entrar es el primero en salir.

## G

**GCC (GNU Compiler Collection)**
Conjunto de compiladores de código abierto que incluye soporte para ARM.

**GDB (GNU Debugger)**
Debugger de código abierto que permite examinar programas durante ejecución.

**GIC (Generic Interrupt Controller)**
Controlador de interrupciones estándar ARM que gestiona y distribuye interrupciones.

## H

**HAL (Hardware Abstraction Layer)**
Capa que proporciona interfaz uniforme para acceder al hardware.

**Handler**
Función que maneja un evento específico como una interrupción o excepción.

## I

**IRQ (Interrupt Request)**
Señal que indica que un dispositivo necesita atención del procesador.

**ISB (Instruction Synchronization Barrier)**
Instrucción que vacía el pipeline del procesador asegurando que instrucciones posteriores vean efectos de las anteriores.

**ISR (Interrupt Service Routine)**
Función que se ejecuta en respuesta a una interrupción específica.

## K

**Kernel**
Núcleo del sistema operativo que gestiona recursos del sistema y proporciona servicios básicos.

## L

**Link Register (LR)**
Registro ARM (R14) que almacena la dirección de retorno cuando se llama una función.

**Linker Script**
Archivo que define cómo el enlazador organiza las secciones del programa en memoria.

## M

**Memory Mapped I/O**
Técnica donde registros de dispositivos aparecen como direcciones de memoria normal.

**MMU (Memory Management Unit)**
Unidad de hardware que traduce direcciones virtuales a físicas y proporciona protección de memoria.

**Mode (ARM)**
Estado operativo del procesador que determina qué registros y instrucciones están disponibles.

## N

**NEON**
Extensión SIMD de ARM para procesamiento paralelo de datos (Single Instruction, Multiple Data).

**NOP (No Operation)**
Instrucción que no realiza operación útil, usada para timing o alineación.

## P

**PC (Program Counter)**
Registro que apunta a la siguiente instrucción a ejecutar.

**Peripheral**
Dispositivo de hardware conectado al procesador (UART, timer, etc.).

**Preemptive Multitasking**
Sistema donde el kernel puede forzar cambios de tarea sin cooperación explícita.

**PL011**
Controlador UART estándar ARM utilizado en muchas plataformas.

## Q

**QEMU (Quick Emulator)**
Emulador de código abierto que puede simular diversos sistemas ARM.

**Quantum**
Período de tiempo asignado a una tarea en scheduling preemptive.

## R

**RealView**
Plataforma de desarrollo ARM emulada por QEMU.

**Register**
Ubicación de almacenamiento de alta velocidad dentro del procesador.

**RISC (Reduced Instruction Set Computer)**
Arquitectura de procesador con conjunto de instrucciones simples y regulares.

**Round-Robin**
Algoritmo de scheduling que asigna tiempo por igual a todas las tareas en rotación.

## S

**SP (Stack Pointer)**
Registro que apunta al tope de la pila actual.

**SP804**
Controlador de timer dual estándar ARM.

**SPSR (Saved Program Status Register)**
Registro que guarda CPSR cuando ocurre una excepción.

**Stack**
Estructura de datos LIFO usada para almacenar datos temporales y contexto de funciones.

**SVC (SuperVisor Call)**
Instrucción ARM que genera una interrupción de software para llamadas al sistema.

**Syscall (System Call)**
Mecanismo que permite a programas de usuario solicitar servicios del kernel.

## T

**Task**
Unidad básica de ejecución en el sistema operativo, similar a un proceso ligero.

**TCB (Task Control Block)**
Estructura que almacena información de estado de una tarea.

**Tick**
Unidad básica de tiempo del sistema, generalmente generada por un timer.

**Timer**
Dispositivo de hardware que genera interrupciones periódicas para timing del sistema.

**Toolchain**
Conjunto de herramientas para desarrollo (compilador, enlazador, debugger, etc.).

## U

**UART (Universal Asynchronous Receiver-Transmitter)**
Dispositivo de comunicación serie que convierte datos paralelos a serie.

**Undefined Instruction**
Excepción que ocurre cuando el procesador encuentra una instrucción no válida.

## V

**Vector Table**
Tabla que contiene direcciones de handlers para diferentes tipos de excepciones.

**VFP (Vector Floating Point)**
Unidad de punto flotante en procesadores ARM.

## W

**WFI (Wait For Interrupt)**
Instrucción ARM que pone el procesador en estado de bajo consumo hasta la próxima interrupción.

**Watchdog**
Timer de hardware que reinicia el sistema si no se refresca periódicamente.

## Términos Específicos del Proyecto

**Conjetura de Collatz**
Algoritmo matemático implementado en tarea2 que aplica reglas simples hasta llegar a 1.

**Factorización Prima**
Proceso de descomponer un número en sus factores primos, implementado en tarea3.

**Fibonacci**
Secuencia matemática donde cada número es suma de los dos anteriores, implementado en tarea1.

**Tarea Idle**
Tarea especial que se ejecuta cuando no hay otras tareas listas, típicamente ejecuta WFI.

## Acrónimos Comunes

| Acrónimo | Significado |
|----------|------------|
| API | Application Programming Interface |
| CPU | Central Processing Unit |
| DDR | Double Data Rate (memoria) |
| GPIO | General Purpose Input/Output |
| I2C | Inter-Integrated Circuit |
| OS | Operating System |
| RAM | Random Access Memory |
| ROM | Read-Only Memory |
| RTOS | Real-Time Operating System |
| SPI | Serial Peripheral Interface |
| USB | Universal Serial Bus |

## Terminología de Desarrollo

**Bug**
Error en el software que causa comportamiento incorrecto.

**Commit**
Snapshot de cambios en un sistema de control de versiones como git.

**Debug**
Proceso de encontrar y corregir errores en el software.

**Feature**
Nueva funcionalidad agregada al software.

**Fork**
Copia de un repositorio para desarrollo independiente.

**Issue**
Problema reportado o solicitud de característica en un proyecto.

**Merge**
Combinar cambios de diferentes branches de desarrollo.

**Pull Request (PR)**
Solicitud para incorporar cambios al repositorio principal.

**Repository**
Ubicación donde se almacena el código y su historial.

**Review**
Proceso de examinar código antes de incorporarlo al proyecto.

## Comandos y Herramientas

**arm-none-eabi-gcc**
Compilador GCC para arquitectura ARM bare-metal.

**arm-none-eabi-objdump**
Herramienta para examinar contenido de archivos objeto.

**gdb-multiarch**
Versión de GDB que soporta múltiples arquitecturas incluyendo ARM.

**make**
Herramienta de automatización de build que ejecuta Makefiles.

**telnet**
Cliente de terminal que permite conectar al monitor QEMU.

## Registros ARM Importantes

| Registro | Nombre | Propósito |
|----------|--------|-----------|
| R0-R3 | Argument/Result | Argumentos de función y valores de retorno |
| R4-R11 | Variable | Registros de variables locales |
| R12 | IP (Intra-Procedure) | Registro scratch |
| R13 | SP (Stack Pointer) | Puntero de pila |
| R14 | LR (Link Register) | Dirección de retorno |
| R15 | PC (Program Counter) | Contador de programa |

## Estados de Tarea

| Estado | Descripción |
|--------|-------------|
| Ready | Tarea lista para ejecutar |
| Running | Tarea ejecutándose actualmente |
| Blocked | Tarea esperando evento |
| Terminated | Tarea terminada |

## Modos del Procesador ARM

| Modo | Código | Descripción |
|------|--------|-------------|
| User | 10000 | Modo no privilegiado |
| FIQ | 10001 | Fast Interrupt |
| IRQ | 10010 | Interrupt |
| Supervisor | 10011 | Llamadas al sistema |
| Abort | 10111 | Errores de memoria |
| Undefined | 11011 | Instrucciones inválidas |
| System | 11111 | Privilegiado, registros user |

## Referencias

- [ARM Architecture Reference Manual](https://developer.arm.com/documentation/ddi0406/)
- [ARM Glossary](https://developer.arm.com/architectures/learn-the-architecture/armv8-a-instruction-set-architecture/glossary)
- [Embedded Systems Glossary](https://www.embedded.com/glossary/)
- [Linux Kernel Glossary](https://www.kernel.org/doc/html/latest/glossary.html)