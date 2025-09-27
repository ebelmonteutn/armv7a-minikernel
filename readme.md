---

# Mini-Kernel para ARMv7-A

## Descripción

Este proyecto es un mini-kernel o sistema operativo "bare-metal" diseñado desde cero para la arquitectura de procesadores ARMv7-A. Fue desarrollado como parte de la materia **Sistemas Digitales II** de la Universidad Tecnológica Nacional, Facultad Regional Buenos Aires (UTN.BA).

El kernel se ejecuta en el emulador **QEMU** sobre una máquina virtual `realview-pb-a8` (ARM Cortex-A8) y demuestra los conceptos fundamentales de la programación de sistemas a bajo nivel.

## Características Principales

*   **Arquitectura ARMv7-A:** Diseñado para correr en cores como el ARM Cortex-A8 (emulado) y fácilmente portable al Cortex-A9 (hardware Zynq).
*   **Manejo de Modos del Procesador:** Inicialización y gestión de pilas separadas para los modos `IRQ`, `FIQ`, `SVC`, `System`, `Abort` y `Undefined`.
*   **Manejadores de Excepciones:** Implementación robusta de manejadores para interrupciones (`IRQ`) y llamadas al sistema (`SVC`) en lenguaje ensamblador con llamadas a rutinas de servicio en C.
*   **Scheduler Cooperativo:** Un planificador de tareas simple, no apropiativo, basado en ticks de un temporizador emulado.
*   **API de Llamadas al Sistema:** Abstracción para que las tareas interactúen con el kernel a través de la instrucción `SVC`. Se incluye una implementación de `my_printf` para escribir en la UART emulada.

## Requisitos de Software

*   **Toolchain de Cross-Compilación:** [ARM GNU Toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-a/downloads) (`arm-none-eabi-gcc`).
*   **Herramienta de Build:** `make`.
*   **Emulador:** **QEMU** (`qemu-system-arm`).
*   **Cliente Telnet:** Para interactuar con el monitor de QEMU.

## Estructura del Proyecto

El repositorio está organizado en directorios funcionales para separar el código fuente, los archivos de cabecera y los productos de la compilación.

```
.
├── Makefile             # Script principal de compilación y ejecución
├── memmap.ld            # Linker script para el mapa de memoria en QEMU
├── inc/                 # Directorio para todos los archivos de cabecera (.h)
├── src/                 # Directorio para todo el código fuente (.c, .s)
├── obj/                 # Directorio para archivos objeto (.o) - generado por make
├── bin/                 # Directorio para ejecutables (.elf, .bin) - generado por make
└── lst/                 # Directorio para listados de ensamblador - generado por make
```

## Compilación y Ejecución

Asegúrate de que `arm-none-eabi-gcc` y `qemu-system-arm` estén instalados y accesibles en el PATH de tu sistema.

### 1. Compilar el Proyecto

Para compilar el kernel, simplemente ejecuta `make` en el directorio raíz:
```bash
make
```
Esto creará los directorios `obj`, `bin` y `lst`, compilará el código fuente y generará el ejecutable final `bin/bios.elf` y el binario `bin/bios.bin`.

### 2. Ejecutar en QEMU

Para iniciar la emulación, usa el target `run`:
```bash
make run
```
QEMU se iniciará con el kernel cargado. Se abrirá un monitor de QEMU en `telnet localhost 1234` al cual te puedes conectar desde otra terminal.

### 3. Depurar con GDB

El Makefile está preparado para iniciar una sesión de depuración.

1.  **Inicia QEMU en modo depuración:**
    ```bash
    make debug
    ```
    QEMU se iniciará y se detendrá inmediatamente, esperando una conexión de GDB en el puerto `tcp::2159`.

2.  **Conecta GDB:**
    En otra terminal, inicia el depurador GDB apuntando al archivo ELF que contiene los símbolos de depuración.
    ```bash
    gdb-multiarch build/bios.elf  # O el GDB de tu toolchain: arm-none-eabi-gdb
    ```

3.  **Dentro de GDB:**
    Conéctate al target y comienza a depurar.
    ```gdb
    (gdb) target remote localhost:2159
    (gdb) # Ahora puedes poner breakpoints (b), continuar (c), etc.
    ```

### 4. Limpiar el Proyecto

Para eliminar todos los archivos y directorios generados por la compilación:
```bash
make clean
```

## Autor

*   **Enzo Belmonte** - *Estudiante de Ing. Electrónica, UTN.BA* - `ebelmonte@frba.utn.edu.ar`

---