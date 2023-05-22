//
// Created by Александр Бернатович on 18.05.2023.
//

#ifndef DECIMAL_S21_DECIMAL_H
#define DECIMAL_S21_DECIMAL_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OK 0
#define FAIL_CONV 1
typedef struct {
    unsigned int bits[4];
} s21_decimal;

void set_minus_sign(s21_decimal *value);
unsigned int s21_from_int_to_decimal(int src, s21_decimal *dst);
void fill_decimals_with_zeros(s21_decimal *val);

unsigned int get_a_degree_of_decimal(const s21_decimal *value);

int s21_truncate(s21_decimal *decimal);

int get_sign(const s21_decimal *value);

#endif //DECIMAL_S21_DECIMAL_H
