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

/*!
 * @file    startup.s
 * @brief   Codigo de Assembler para la inicializacion de la pila.
 * @author  Enzo Belmonte <ebelmonte@frba.utn.edu.ar>
 * @date    2025-06-10
 */
.equ CANT_TABLE, 16

// Definiciones de modos de CPU
.equ MODE_USER, 16
.equ MODE_FIQ, 17
.equ MODE_IRQ, 18
.equ MODE_SVC, 19
.equ MODE_ABT, 23
.equ MODE_UND, 27
.equ MODE_SYS, 31

// Externs para los manejadores de excepciones
.extern reset_vector
.extern undef_handler
.extern svc_handler
.extern pabt_handler
.extern dabt_handler
.extern reserved_handler
.extern irq_handler
.extern fiq_handler

//Externs para la inicializacion de la pila
.extern _irq_stack_top_
.extern _fiq_stack_top_
.extern _svc_stack_top_
.extern _und_stack_top_
.extern _abt_stack_top_
.extern _c_stack_top_

//Externs de funciones
.extern board_init
.extern halt_cpu

.global _start

.section .data
string: .asciz "Prueba de svcall\n"

.code 32
.section .text

tabla:
    LDR PC, lit_reset_vector
    LDR PC, lit_undef_handler
    LDR PC, lit_svc_handler
    LDR PC, lit_pabt_handler
    LDR PC, lit_dabt_handler
    LDR PC, lit_reserved_handler
    LDR PC, lit_irq_handler
    LDR PC, lit_fiq_handler

lit_reset_vector:    .word reset_vector
lit_undef_handler:   .word undef_handler
lit_svc_handler:     .word svc_handler
lit_pabt_handler:    .word pabt_handler
lit_dabt_handler:    .word dabt_handler
lit_reserved_handler:.word reserved_handler
lit_irq_handler:     .word irq_handler
lit_fiq_handler:     .word fiq_handler

_start:
    CPSID if, #MODE_IRQ // Modo IRQ con IRQ y FIQ deshabilitados
    LDR SP, =_irq_stack_top_
    CPSID if, #MODE_FIQ //Modo FIQ con IRQ y FIQ deshabilitados
    LDR SP, =_fiq_stack_top_
    CPSID if, #MODE_SVC // Modo SVC con IRQ y FIQ deshabilitados
    LDR SP, =_svc_stack_top_
    CPSID if, #MODE_UND // Modo UND con IRQ y FIQ deshabilitados
    LDR SP, =_und_stack_top_
    CPSID if, #MODE_ABT // Modo ABT con IRQ y FIQ deshabilitados
    LDR SP, =_abt_stack_top_
    CPSID if, #MODE_SYS // Modo System con IRQ y FIQ deshabilitados
    LDR SP, =_c_stack_top_
    CPS #MODE_SVC // Cambia a modo SVC

    LDR R0, =tabla         
    LDR R1, =0x00000000    
    MOV R2, #CANT_TABLE           

// Copia la tabla de vectores de los handlers de interrupciones a la dirección 0x00000000
copy_tabla:
    LDR R3, [R0], #4
    STR R3, [R1], #4
    SUBS R2, R2, #1
    BNE copy_tabla

init_board:
    BLX board_init // Llama a la función de inicialización de la placa
    CPSIE if // Habilita interrupciones 

_halt:
    B halt_cpu 
//    WFI
//    B _halt
.end
