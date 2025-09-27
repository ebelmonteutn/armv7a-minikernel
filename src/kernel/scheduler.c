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
 * @file    scheduler.c
 * @brief   Implementación de funciones para el scheduler
 * @author  Enzo Belmonte <ebelmonte@frba.utn.edu.ar>
 * @date    2025-07-18
 */
#include "defines.h"

__attribute__((section(".tcb_data"))) tcb_context_t tcb_tareas;

__attribute__((section(".text"))) void scheduler_init(void)
{
    uint32_t i = 0;
    uint32_t *ptr;
    xPSR_t spsr = {.bits = {.M = MODE_SYS,
                            .T = 0,
                            .F = 0,
                            .I = 0,
                            .A = 0,
                            .E = 0,
                            .IT_2_7 = 0,
                            .GE = 0,
                            .reserved1 = 0,
                            .J = 0,
                            .IT_0_1 = 0,
                            .Q = 0,
                            .V = 0,
                            .C = 0,
                            .Z = 1,
                            .N = 0}};
    tcb_tareas.task_id_actual = TASK_IDLE;
    tcb_tareas.run = 0;

    ptr = &_tareaidle_irq_stack_top_ - 16;  // 16 palabras para contexto (full descending)
    tcb_tareas.tareas[TASK_IDLE].ticks = 5; // Ticks para la tarea idle
    tcb_tareas.tareas[TASK_IDLE].ticks_actuales = 0;
    tcb_tareas.tareas[TASK_IDLE].ptr_tarea = tarea_idle;
    tcb_tareas.tareas[TASK_IDLE].task_id = TASK_IDLE;
    tcb_tareas.tareas[TASK_IDLE].spsr = spsr;
    tcb_tareas.tareas[TASK_IDLE].sp_irq = ptr;
    tcb_tareas.tareas[TASK_IDLE].sp_svc = &_tareaidle_svc_stack_top_;
    tcb_tareas.tareas[TASK_IDLE].sp_sys = &_tareaidle_sys_stack_top_; // Asignar el mismo stack para SVC y SYS
    tcb_tareas.tareas[TASK_IDLE].lr_svc = NULL;
    tcb_tareas.tareas[TASK_IDLE].lr_sys = (uint32_t *)tarea_idle;
    ptr[0] = (uint32_t)(ptr + 2);
    ptr[1] = tcb_tareas.tareas[TASK_IDLE].spsr.xPSR; // Guardar el xPSR en el stack
    for (i = 2; i < 15; i++)
    {
        ptr[i] = 0; // Inicializar los registros R0-R12
    }
    ptr[15] = (uint32_t)tcb_tareas.tareas[TASK_IDLE].ptr_tarea; // LR

    // Tarea 1
    ptr = &_tarea1_irq_stack_top_ - 16;
    tcb_tareas.tareas[TASK_1].ticks = 8;
    tcb_tareas.tareas[TASK_1].ticks_actuales = 0;
    tcb_tareas.tareas[TASK_1].ptr_tarea = tarea1;
    tcb_tareas.tareas[TASK_1].task_id = TASK_1;
    tcb_tareas.tareas[TASK_1].spsr = spsr;
    tcb_tareas.tareas[TASK_1].sp_irq = ptr;
    tcb_tareas.tareas[TASK_1].sp_svc = &_tarea1_svc_stack_top_;
    tcb_tareas.tareas[TASK_1].sp_sys = &_tarea1_sys_stack_top_; // Asignar el mismo stack para SVC y SYS
    tcb_tareas.tareas[TASK_1].lr_svc = NULL;
    tcb_tareas.tareas[TASK_1].lr_sys = (uint32_t *)tarea1;
    ptr[0] = (uint32_t)(ptr + 2);
    ptr[1] = tcb_tareas.tareas[TASK_1].spsr.xPSR;
    for (i = 2; i < 15; i++)
    {
        ptr[i] = 0;
    }
    ptr[15] = (uint32_t)tcb_tareas.tareas[TASK_1].ptr_tarea;

    // Tarea 2
    ptr = &_tarea2_irq_stack_top_ - 16;
    tcb_tareas.tareas[TASK_2].ticks = 12;
    tcb_tareas.tareas[TASK_2].ticks_actuales = 0;
    tcb_tareas.tareas[TASK_2].ptr_tarea = tarea2;
    tcb_tareas.tareas[TASK_2].task_id = TASK_2;
    tcb_tareas.tareas[TASK_2].spsr = spsr;
    tcb_tareas.tareas[TASK_2].sp_irq = ptr;
    tcb_tareas.tareas[TASK_2].sp_svc = &_tarea2_svc_stack_top_;
    tcb_tareas.tareas[TASK_2].sp_sys = &_tarea2_sys_stack_top_; // Asignar el mismo stack para SVC y SYS
    tcb_tareas.tareas[TASK_2].lr_svc = NULL;
    tcb_tareas.tareas[TASK_2].lr_sys = (uint32_t *)tarea2;
    ptr[0] = (uint32_t)(ptr + 2);
    ptr[1] = tcb_tareas.tareas[TASK_2].spsr.xPSR;
    for (i = 2; i < 15; i++)
    {
        ptr[i] = 0;
    }
    ptr[15] = (uint32_t)tcb_tareas.tareas[TASK_2].ptr_tarea;

    // Tarea 3
    ptr = &_tarea3_irq_stack_top_ - 16;
    tcb_tareas.tareas[TASK_3].ticks = 5;
    tcb_tareas.tareas[TASK_3].ticks_actuales = 0;
    tcb_tareas.tareas[TASK_3].ptr_tarea = tarea3;
    tcb_tareas.tareas[TASK_3].task_id = TASK_3;
    tcb_tareas.tareas[TASK_3].spsr = spsr;
    tcb_tareas.tareas[TASK_3].sp_irq = ptr;
    tcb_tareas.tareas[TASK_3].sp_svc = &_tarea3_svc_stack_top_;
    tcb_tareas.tareas[TASK_3].sp_sys = &_tarea3_sys_stack_top_; // Asignar el mismo stack para SVC y SYS
    tcb_tareas.tareas[TASK_3].lr_svc = NULL;
    tcb_tareas.tareas[TASK_3].lr_sys = (uint32_t *)tarea3;
    ptr[0] = (uint32_t)(ptr + 2);
    ptr[1] = tcb_tareas.tareas[TASK_3].spsr.xPSR;
    for (i = 2; i < 15; i++)
    {
        ptr[i] = 0;
    }
    ptr[15] = (uint32_t)tcb_tareas.tareas[TASK_3].ptr_tarea;
}

__attribute__((section(".text"))) void scheduler(void)
{
    tcb_t *actual = &tcb_tareas.tareas[tcb_tareas.task_id_actual];
    actual->ticks_actuales++;
    if (tcb_tareas.run != 1)
    {
        tcb_tareas.run = 1; // Ver si hace falta
    }
    if (actual->ticks_actuales >= actual->ticks)
    {
        tcb_tareas.task_id_actual++;
        actual->ticks_actuales = 0; // Reiniciar los ticks actuales
        if (tcb_tareas.task_id_actual >= CANT_TASKS)
        {
            tcb_tareas.task_id_actual = TASK_IDLE; // Volver a la tarea idle si se supera el límite
        }
    }
}

__attribute__((section(".text"))) void save_context(tcb_t *tcb, uint32_t *sp_irq)
{
    uint32_t *temp_sp_svc;
    uint32_t *temp_sp_sys;
    uint32_t *temp_lr_svc;
    uint32_t *temp_lr_sys;
    tcb->sp_irq = sp_irq;
    __asm__("CPS %0" : : "i"(MODE_SYS));        // Cambio al modo SYS
    __asm__("MOV %0, SP" : "=r"(temp_sp_sys));  // Guardo el stack pointer en el contexto
    __asm__("MOV %0, LR" : "=r"(temp_lr_sys));  // Guardo el link register del modo SYS
    __asm__("CPS %0" : : "i"(MODE_SVC));        // Cambio al modo SVC
    __asm__("MOV %0, SP" : "=r"(temp_sp_svc));  // Guardo el stack pointer en el contexto
    __asm__("MOV %0, LR" : "=r"(temp_lr_svc));  // Guardo el link register del modo SVC
    __asm__("CPS %0" : : "i"(MODE_IRQ));        // Vuelvo al modo IRQ
    tcb->sp_svc = temp_sp_svc;                   // Guardo el stack pointer de SVC
    tcb->sp_sys = temp_sp_sys;                   // Guardo el stack pointer de SYS
    tcb->lr_svc = temp_lr_svc;       // Guardo el link register de SVC
    tcb->lr_sys = temp_lr_sys;       // Guardo el link register de SYS
}

__attribute__((section(".text"))) void context_switch(tcb_t *tcb, uint32_t **sp_irq)
{
    uint32_t *temp_sp_svc;
    uint32_t *temp_sp_sys;
    uint32_t *temp_lr_svc;
    uint32_t *temp_lr_sys;
    *sp_irq = tcb->sp_irq;
    temp_sp_svc = tcb->sp_svc;
    temp_sp_sys = tcb->sp_sys;
    temp_lr_svc = tcb->lr_svc;
    temp_lr_sys = tcb->lr_sys;
    __asm__("CPS %0" : : "i"(MODE_SYS));        // Cambio al modo SYS
    __asm__("MOV SP, %0" : : "r"(temp_sp_sys)); // Restauro el stack pointer en el contexto
    __asm__("MOV LR, %0" : : "r"(temp_lr_sys)); // Restauro el link register en el contexto
    __asm__("CPS %0" : : "i"(MODE_SVC));        // Cambio al modo SVC
    __asm__("MOV SP, %0" : : "r"(temp_sp_svc)); // Restauro el stack pointer en el contexto
    __asm__("MOV LR, %0" : : "r"(temp_lr_svc)); // Restauro el link register en el contexto
    __asm__("CPS %0" : : "i"(MODE_IRQ));        // Vuelvo al modo IRQ
}
