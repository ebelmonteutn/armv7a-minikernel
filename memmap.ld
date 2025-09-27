/*
 * Copyright (c) 2025 Enzo Belmonte
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/* 
    Definiciones necesarias: formato de salida, arquitectura y punto de entrada
*/
OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)

/* 
    Definiciones de constantes y macros
*/
MAX_TASKS = 4;

/* 
    Definiciones de simbolos necesarios
*/
_PUBLIC_RAM_INIT = 0x70010000;
_PUBLIC_STACK_INIT = 0x70020000;

/* 
    Definiciones de tamaños de pila para diferentes modos
*/
C_STACK_SIZE = 4K;
SYS_STACK_SIZE = 4K;
IRQ_STACK_SIZE = 512;
FIQ_STACK_SIZE = 512;
SVC_STACK_SIZE = 512;
ABT_STACK_SIZE = 512;
UND_STACK_SIZE = 512;
TAREAS_STACK_SIZE = 512;

/* 
    Definición del mapa de memoria
*/
MEMORY
{
    public_ram	: org = _PUBLIC_RAM_INIT, len = 128M
    public_stack : org = _PUBLIC_STACK_INIT, len = 1M
}

/* 
    Definición de las secciones
*/
SECTIONS
{
    . = _PUBLIC_RAM_INIT;
    .text : { 
        *(.reset_vector*) 
        *(.start*) 
        *(.kernel_text*)
        *(.tareaidle_text*)
        *(.tarea1_text*)
        *(.tarea2_text*)
        *(.tarea3_text*)
        *(.tarea4_text*)
        *(.text*)
        } > public_ram
    
    .data : { *(.data*) } > public_ram

    .tcb_data : { *(.tcb_data*) } > public_ram

    .bss : {
        . = ALIGN(4);
        __bss_start__ = .;
        *(.bss*)
        __bss_end__ = .;
        } > public_ram
    
    . = _PUBLIC_STACK_INIT;
    .stack :
    {
        _stack_start_ = .;

        . = . + IRQ_STACK_SIZE;
        . = ALIGN(4);
        _irq_stack_top_ = .;

        . = . + FIQ_STACK_SIZE;
        . = ALIGN(4);
        _fiq_stack_top_ = .;

        . = . + SVC_STACK_SIZE;
        . = ALIGN(4);
        _svc_stack_top_ = .;

        . = . + UND_STACK_SIZE;
        . = ALIGN(4);
        _und_stack_top_ = .;

        . = . + ABT_STACK_SIZE;
        . = ALIGN(4);
        _abt_stack_top_ = .;

        . = . + C_STACK_SIZE;
        . = ALIGN(4);
        _c_stack_top_ = .;
    } > public_stack
    .tareaidle_stack :
    {
        _tareaidle_stack_start_ = .;

        . = . + TAREAS_STACK_SIZE;
        . = ALIGN(4);
        _tareaidle_irq_stack_top_ = .;

        . = . + TAREAS_STACK_SIZE;
        . = ALIGN(4);
        _tareaidle_svc_stack_top_ = .;

        . = . + TAREAS_STACK_SIZE;
        . = ALIGN(4);
        _tareaidle_sys_stack_top_ = .; 
    } > public_stack
    .tarea1_stack :
    {
        _tarea1_stack_start_ = .;

        . = . + TAREAS_STACK_SIZE;
        . = ALIGN(4);
        _tarea1_irq_stack_top_ = .;

        . = . + TAREAS_STACK_SIZE;
        . = ALIGN(4);
        _tarea1_svc_stack_top_ = .;

        . = . + TAREAS_STACK_SIZE;
        . = ALIGN(4);
        _tarea1_sys_stack_top_ = .;
    } > public_stack
    .tarea2_stack :
    {
        _tarea2_stack_start_ = .;

        . = . + TAREAS_STACK_SIZE;
        . = ALIGN(4);
        _tarea2_irq_stack_top_ = .;

        . = . + TAREAS_STACK_SIZE;
        . = ALIGN(4);
        _tarea2_svc_stack_top_ = .;

        . = . + TAREAS_STACK_SIZE;
        . = ALIGN(4);
        _tarea2_sys_stack_top_ = .;
    } > public_stack
    .tarea3_stack :
    {
        _tarea3_stack_start_ = .;

        . = . + TAREAS_STACK_SIZE;
        . = ALIGN(4);
        _tarea3_irq_stack_top_ = .;

        . = . + TAREAS_STACK_SIZE;
        . = ALIGN(4);
        _tarea3_svc_stack_top_ = .;

        . = . + TAREAS_STACK_SIZE;
        . = ALIGN(4);
        _tarea3_sys_stack_top_ = .;
    }  > public_stack
}