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
 * @file    scheduler.h
 * @brief   Declaración de funciones y estructuras para el planificador de tareas
 * @author  Enzo Belmonte <ebelmonte@frba.utn.edu.ar>
 * @date    2025-07-18
 */
#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "defines.h"

#define MODE_USER 0x10 // Modo de usuario
#define MODE_FIQ 0x11  // Modo FIQ
#define MODE_IRQ 0x12  // Modo IRQ
#define MODE_SVC 0x13  // Modo SVC
#define MODE_ABT 0x17  // Modo Abort
#define MODE_UND 0x1B  // Modo Undefined
#define MODE_SYS 0x1F  // Modo System

extern uint32_t MAX_TASKS;
extern uint32_t _tareaidle_irq_stack_top_;
extern uint32_t _tareaidle_svc_stack_top_;
extern uint32_t _tareaidle_sys_stack_top_;
extern uint32_t _tarea1_irq_stack_top_;
extern uint32_t _tarea1_svc_stack_top_;
extern uint32_t _tarea1_sys_stack_top_;
extern uint32_t _tarea2_irq_stack_top_;
extern uint32_t _tarea2_svc_stack_top_;
extern uint32_t _tarea2_sys_stack_top_;
extern uint32_t _tarea3_irq_stack_top_;
extern uint32_t _tarea3_svc_stack_top_;
extern uint32_t _tarea3_sys_stack_top_;

extern void tarea_idle(void);
extern void tarea1(void);
extern void tarea2(void);
extern void tarea3(void);

#define CANT_TASKS 4

typedef enum
{
    TASK_IDLE = 0,
    TASK_1,
    TASK_2,
    TASK_3,
    TASK_INIT,
} task_id_t;

typedef union
{
    uint32_t xPSR;
    struct
    {
        uint32_t M : 5;         // Bits [4:0] - Mode bits
        uint32_t T : 1;         // Bit 5 - Thumb execution state
        uint32_t F : 1;         // Bit 6 - FIQ disable
        uint32_t I : 1;         // Bit 7 - IRQ disable
        uint32_t A : 1;         // Bit 8 - Asynchronous abort disable
        uint32_t E : 1;         // Bit 9 - Endianness execution state
        uint32_t IT_2_7 : 6;    // Bits [15:10] - IT[7:2] for Thumb-2
        uint32_t GE : 4;        // Bits [19:16] - Greater than or Equal flags
        uint32_t reserved1 : 4; // Bits [23:20] - Reserved
        uint32_t J : 1;         // Bit 24 - Jazelle execution state
        uint32_t IT_0_1 : 2;    // Bits [26:25] - IT[1:0] for Thumb-2
        uint32_t Q : 1;         // Bit 27 - Saturation condition flag
        uint32_t V : 1;         // Bit 28 - Overflow condition flag
        uint32_t C : 1;         // Bit 29 - Carry condition flag
        uint32_t Z : 1;         // Bit 30 - Zero condition flag
        uint32_t N : 1;         // Bit 31 - Negative condition flag
    } bits;
} xPSR_t;

typedef struct
{
    uint32_t ticks;
    uint32_t ticks_actuales;
    void (*ptr_tarea)(void);
    task_id_t task_id;
    uint32_t *sp_irq;
    uint32_t *sp_svc;
    uint32_t *sp_sys;
    uint32_t *lr_svc;
    uint32_t *lr_sys;
    xPSR_t spsr;
} tcb_t;

typedef struct
{
    tcb_t tareas[CANT_TASKS];
    task_id_t task_id_actual;
    uint8_t run;
} tcb_context_t;

void scheduler_init(void);
void scheduler(void);
void save_context(tcb_t *tcb, uint32_t *sp_irq);
void context_switch(tcb_t *tcb, uint32_t **sp_irq);

#endif // SCHEDULER_H_