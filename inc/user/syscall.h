/*!
 * @file    syscall.h
 * @brief   Declaración de funciones y estructuras para las llamadas al sistema
 * @author  Enzo Belmonte <ebelmonte@frba.utn.edu.ar>
 * @date    2025-07-18
 */
#ifndef USER_SYSCALL_H_
#define USER_SYSCALL_H_

#include <stddef.h>

/*!
 * @brief Funcion que imprime un mensaje en la salida estándar.
 *
 * @param[in] buf Buffer de datos a escribir.
 * 
 * @return	  Devuelve la cantidad de bytes escritos o -1 en error.
 */
int my_printf(const char *buf);

int my_printf_len(const char *buf, size_t len);

#endif /* USER_SYSCALL_H_ */