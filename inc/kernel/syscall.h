/*!
 * @file    syscall.h
 * @brief   Declaración de funciones y estructuras para las llamadas al sistema
 * @author  Enzo Belmonte <ebelmonte@frba.utn.edu.ar>
 * @date    2025-07-18
 */
#ifndef SYSCALL_H_
#define SYSCALL_H_

#include "defines.h"
#include <stdint.h>

// Forward declaration para evitar dependencia circular
struct _uart_t;

typedef unsigned short umode_t;

typedef long int blksize_t;

typedef enum
{
    SYS_WRITE = 4, // Escribe datos en un descriptor de archivo
} svc_call_t;

/*!
 * @brief Funcion que escribe en pantalla.
 *
 * @param[in] buf Buffer de datos a escribir.
 * 
 * @return	  Devuelve la cantidad de bytes escritos o -1 en error.
 */
int sys_my_printf(const char *buf);

/*!
 * @brief Funcion para manejar la interrupción del temporizador 0.
 *
 * @param[in] num Número de la llamada al sistema.
 * @param[in] arg1 Primer argumento de la llamada al sistema.
 * @param[in] sp_irq Variable de entrada que recibe el stack pointer.
 * @return	  Devuelve el stack pointer original.
 */
uint32_t *C_SVC_handler(uint32_t svc_num, uint32_t *regs);

#endif /* SYSCALL_H_ */