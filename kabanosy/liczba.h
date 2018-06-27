/**
 * Interfejs modułu implementującego reprezentację liczb całkowitych.
 */

#ifndef LICZBA_H
#define LICZBA_H

/**
 * Reprezentacja liczby całkowitej.
 */
typedef struct liczba *Liczba;

/**
 * Daje reprezentację @n.
 */
Liczba reprezentacja(int n);

/**
 * Daje wartość @a lub 0, jeśli nie mieści się ona w zakresie typu int.
 */
int wartosc(Liczba a);

/**
 * Usuwa z pamięci liczbę @a.
 */
void usun(Liczba a);

/**
 * Czyta liczbę z wejścia.
 */
Liczba czytaj(void);

/**
 * Pisze @a na wyjście.
 */
void pisz(Liczba a);

/**
 * Daje sumę @a + @b.
 */
Liczba suma(Liczba a, Liczba b);

/**
 * Daje różnicę @a - @b.
 */
Liczba roznica(Liczba a, Liczba b);

/**
 * Daje iloczyn @a * @b.
 */
Liczba iloczyn(Liczba a, Liczba b);

/**
 * Daje -1 dla ujemnego @a, 0 jeśli @a jest zerem, 1 jeśli @a jest dodatnie.
 */
int znak(Liczba a);

#endif
