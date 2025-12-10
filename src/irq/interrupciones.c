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
 * @file    interrupciones.c
 * @brief   Implementación de funciones para el manejo de interrupciones
 * @author  Enzo Belmonte <ebelmonte@frba.utn.edu.ar>
 * @date    2025-06-17
 */
#include "defines.h"

extern tcb_context_t tcb_tareas;

__attribute__((section(".text"))) unsigned int identify_IRQ(void)
{
    unsigned int irq_num;
    _gicc_t *const GICC0 = (_gicc_t *)GICC0_ADDR;
    irq_num = GICC0->IAR;

    return irq_num;
}

__attribute__((section(".text"))) uint32_t *C_IRQ_handler(uint32_t *sp_irq)
{
    unsigned int irq_ack;
    unsigned int irq_id;
    uint32_t *ret_sp_irq = sp_irq;
    _gicc_t *const GICC0 = (_gicc_t *)GICC0_ADDR;
    irq_ack = GICC0->IAR;
    irq_id = irq_ack & 0x3FFU;
    switch (irq_id)
    {
    case GIC_SOURCE_TIMER0:
        ret_sp_irq = TIMER0_IRQHandler(sp_irq);
        break;
    case GIC_SOURCE_TIMER1:
        // Manejar la interrupción del temporizador 1
        NOP;
        break;
    case GIC_SOURCE_TIMER2:
        // Manejar la interrupción del temporizador 2
        NOP;
        break;
    case GIC_SOURCE_TIMER3:
        // Manejar la interrupción del temporizador 3
        NOP;
        break;
    case GIC_SOURCE_UART0:
        // Manejar la interrupción del UART 0
        NOP;
        break;
    case GIC_SOURCE_UART1:
        // Manejar la interrupción del UART 1
        NOP;
        break;
    case GIC_SOURCE_UART2:
        // Manejar la interrupción del UART 2
        NOP;
        break;
    case GIC_SOURCE_UART3:
        // Manejar la interrupción del UART 3
        NOP;
        break;
    default:
        NOP; // Placeholder para manejar otras interrupciones
        break;
    }

    GICC0->EOIR = irq_ack;

    return ret_sp_irq;
}

__attribute__((section(".text"))) uint32_t *TIMER0_IRQHandler(uint32_t *sp_irq)
{
    uint32_t *ret_sp_irq = sp_irq;
    tcb_t *actual;
    tcb_t *next;
    _timer_t *const TIMER0 = (_timer_t *)TIMER0_ADDR;

    actual = &tcb_tareas.tareas[tcb_tareas.task_id_actual];
    if (tcb_tareas.run == 1)
    {
        // Guardar contexto de la tarea actual
        save_context(actual, sp_irq);
    }

    // Lógica de cambio de tarea
    scheduler();

    // Limpiar la interrupción del timer para evitar reentradas
    TIMER0->Timer1IntClr = 1U;

    // Cargar contexto de la nueva tarea
    next = &tcb_tareas.tareas[tcb_tareas.task_id_actual];
    context_switch(next, &ret_sp_irq);
    return ret_sp_irq;
}
