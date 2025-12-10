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
 * @file    interrupciones.h
 * @brief   Declaración de funciones de manejo de interrupciones
 * @author  Enzo Belmonte <ebelmonte@frba.utn.edu.ar>
 * @date    2025-06-17
 */
#ifndef INTERRUPCIONES_H_
#define INTERRUPCIONES_H_

/*!
 * @brief Funcion para identificar la interrupción.
 *
 * @return	  Devuelve el número de interrupción identificada.
 */
unsigned int identify_IRQ(void);

/*!
 * @brief Funcion para manejar las interrupciones.
 *
 * @param[in] sp Variable de entrada que recibe el stack pointer.
 * @return	  Devuelve el stack pointer original.
 */
uint32_t *C_IRQ_handler(uint32_t *sp_irq);

/*!
 * @brief Funcion para inicializar la placa.
 *
 * @return	  None
 */
uint32_t *TIMER0_IRQHandler(uint32_t *sp_irq);

#endif /* INTERRUPCIONES_H_ */