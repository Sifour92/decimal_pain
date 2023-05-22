#include "s21_decimal.h"


int main() {
    s21_decimal decimal;
    int test = 123456;
    s21_from_int_to_decimal(test, &decimal);
    int mask = 1;
    mask = mask << 31;
    if(decimal.bits[3] & mask){
        printf("-%u",decimal.bits[0]);
    } else {
        printf("%u", decimal.bits[0]);
    }
    return 0;
}

int s21_from_decimal_to_int(s21_decimal *decimal, int *integer) {
    *integer = 0;
    int sign = get_sign(decimal);
    int error_code = s21_truncate(decimal);
    *integer = decimal->bits[0];
        if (sign)
            *integer *= -1;
    return error_code;
}

int s21_truncate(s21_decimal *decimal) {

    unsigned int power = get_a_degree_of_decimal(decimal);

    if (power) {

        uint64_t temp1, temp2, temp3;
        uint32_t mask = 0xFFFFFFFF;

        temp1 = (uint64_t) decimal->bits[0] * (uint64_t) pow(10, power);
        decimal->bits[0] = temp1 & mask;

        temp2 = (uint64_t) decimal->bits[1] * (uint64_t) pow(10, power) + (temp1 >> 32);
        decimal->bits[1] = temp2 & mask;

        temp3 = (uint64_t) decimal->bits[2] * (uint64_t) pow(10, power) + (temp2 >> 32);
        decimal->bits[2] = temp3 & mask;
    }


    //if (sign) set_sign(result, 1);
    return OK;
}

int get_sign(const s21_decimal *value) {
    unsigned int mask = 1u << 31;
    return (value->bits[3] & mask) != 0;
}

unsigned int get_a_degree_of_decimal(const s21_decimal *value) {
    return (unsigned char)(value->bits[3] >> 16);
}


unsigned int s21_from_int_to_decimal(int integer, s21_decimal *decimal) {
    unsigned int flag = OK;
    if (decimal != NULL) {
        fill_decimals_with_zeros(decimal);
        if (integer < 0) {
            set_minus_sign(decimal);
            integer = -integer;
        }
        decimal->bits[0] = integer;
    } else {
        flag = FAIL_CONV;
    }
    return flag;
}

void fill_decimals_with_zeros(s21_decimal *val) {
    memset(val->bits, 0, sizeof(val->bits)); }

void set_minus_sign(s21_decimal *value) {
    unsigned int mask = 1u << 31;
    value->bits[3] |= mask;
}

void set_plus_sign(s21_decimal *value) {
    unsigned int mask = 1u << 31;
    value->bits[3] &= ~mask;
}