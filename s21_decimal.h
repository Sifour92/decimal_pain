//
// Created by Александр Бернатович on 18.05.2023.
//

#ifndef DECIMAL_S21_DECIMAL_H
#define DECIMAL_S21_DECIMAL_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

typedef struct {
    unsigned int bits[4];
} s21_decimal;

enum returns { OK, INF, NEGATIVE_INF, DIVISION_BY_ZERO };
enum converts { SUCCESS, CONVERTING_ERROR };

unsigned int s21_add(s21_decimal *decimal_one, s21_decimal *decimal_two, s21_decimal *decimal_result);

unsigned int get_bit(s21_decimal *value, unsigned int bit_number);
void set_bit_up(s21_decimal *decimal_bit, unsigned int bit);
void set_bit_down(s21_decimal *decimal_bit, unsigned int bit);


unsigned int bit_addition(s21_decimal *value_one, s21_decimal *value_two, s21_decimal *result);
unsigned int get_sign(const s21_decimal* bit);
void scale_decrease(s21_decimal *a);

int get_scale(const s21_decimal *varPtr);
void scale_equalize(s21_decimal *number_1, s21_decimal *number_2);
int last_bit(s21_decimal number);
void bits_copy(s21_decimal src, s21_decimal *dest);
void set_scale(s21_decimal *varPtr, int scale);
int shift_left(s21_decimal *value, int offset);
void set_bit(s21_decimal *varPtr, int bit, int value);
void set_sign(s21_decimal *varPtr, int sign);
void clear_bits(s21_decimal *varPtr);
void bit_multiplication(s21_decimal val1, s21_decimal ten, s21_decimal *res);
void bit_division(s21_decimal number1, s21_decimal number2,
                  s21_decimal *remainder, s21_decimal *res);
void bit_subtraction(s21_decimal number_1, s21_decimal number_2,
                     s21_decimal *result);


#endif //DECIMAL_S21_DECIMAL_H
