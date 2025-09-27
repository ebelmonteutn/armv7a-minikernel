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