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
 * @file    utils.h
 * @brief   Funciones utilitarias para cálculos matemáticos.
 * @author  Enzo Belmonte <ebelmonte@frba.ut.edu.ar>
 * @date    Fecha: 11/07/2025
 */
#ifndef UTILS_H_
#define UTILS_H_
#define ERROR_SQRT 0.001 // Error tolerable para la raíz cuadrada

/*!
 * @brief Función que calcula la raíz cuadrada de un número entero.
 *
 * @param[in] x Número del cual se desea calcular la raíz cuadrada.
 * @return	  La raíz cuadrada de x y -1 en caso de error.
 */
int raiz_cuadrada_int(unsigned int x);

#endif /* UTILS_H_ */