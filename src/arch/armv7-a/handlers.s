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
 * @file    handlers.s
 * @brief   Codigo de Assembler para los manejadores de excepciones
 * @author  Enzo Belmonte <ebelmonte@frba.utn.edu.ar>
 * @date    2025-06-17
 */
.extern _start

//Externs para los handlers de irq
.extern C_IRQ_handler
.extern C_SVC_handler
.extern identify_IRQ

.global undef_handler
.global svc_handler
.global pabt_handler
.global dabt_handler
.global reserved_handler
.global irq_handler
.global fiq_handler

.code 32
.section .text
undef_handler:
    B .
svc_handler:
    PUSH {R0-R12, LR}
    MOV R8, SP
    MRS R9, SPSR
    PUSH {R8, R9}
    MOV R1, SP
    LDR R0,[LR,#-4]
    BIC R0,R0,#0xFF000000
    BLX C_SVC_handler
    MOV SP, R0
    POP {R0, R1}
    MOV SP, R0
    MSR SPSR, R1
    POP {R0-R12, LR}
    MOVS PC, LR
pabt_handler:
    B .
dabt_handler:
    B .
reserved_handler:
    B .
irq_handler:
    SUB LR, LR, #4
    PUSH {R0-R12, LR}
    MOV R0, SP
    MRS R1, SPSR
    PUSH {R0, R1}
    MOV R0, SP
    BLX C_IRQ_handler
    MOV SP, R0
    POP {R0, R1}
    MOV SP, R0
    MSR SPSR, R1
    POP {R0-R12, LR}
    MOVS PC, LR
fiq_handler:
    B .
.end
