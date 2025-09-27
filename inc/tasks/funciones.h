/*!
 * @file    funciones.h
 * @brief   Funciones para el cálculo del número de Fibonacci, conjetura de Collatz y factorización de numeros primos.
 * @author  Enzo Belmonte <ebelmonte@frba.ut.edu.ar>
 * @date    Fecha: 11/07/2025
 */
#ifndef FUNCIONES_H_
#define FUNCIONES_H_

/*!
 * @brief Función que calcula el enésimo número de Fibonacci.
 *
 * @param[in] n Número de Fibonacci a calcular.
 * @return	  El enésimo número de Fibonacci.
 */
unsigned int fibonacci(unsigned int n);

/*!
 * @brief Función que calcula la conjetura de Collatz para un número dado.
 *
 * @param[in] n Número para el cual se calculará la conjetura de Collatz.
 * @return	  El número de pasos necesarios para llegar a 1.
 */
unsigned int conjetura_collatz(unsigned int n);

/*!
 * @brief Función que factoriza un número en sus factores primos.
 *
 * @param[in] n Número a factorizar.
 * @param[out] factores Array donde se almacenarán los factores primos.
 * @return None
 */
void factorizacion_primos(unsigned int n, unsigned int *factores);

#endif // FUNCIONES_H_
