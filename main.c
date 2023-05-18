#include <stdio.h>
#include "s21_decimal.h"


int main() {
    s21_decimal value = {0};
    value.bits[0]=16;
    unsigned int mask = get_bit(&value,4);
    printf("%d",mask);
    return 0;
}

unsigned int get_bit(s21_decimal *value, unsigned int bit_number) {
    unsigned int bit_on = 0;
    if(bit_number/32<4) { //3 bytes and less
        unsigned int mask = 1u << bit_number%32;
        bit_on = value->bits[bit_number/32] & mask;
    }
    return bit_on ? 1 : 0;
}

void set_bit_up(s21_decimal *decimal_bit, unsigned int bit){
    unsigned int mask = 1u<<bit%32; // which bit want to up
    if(bit / 32 <4) {
        decimal_bit->bits[bit / 32] |= mask; // up bit(1)
    }
}

void set_bit_down(s21_decimal *decimal_bit, unsigned int bit){
    unsigned int mask = 1u<<bit%32; // which bit want to up
    if (bit / 32 <4){
        decimal_bit->bits[bit/32] &= ~mask; // down bit (0)
    }
}

void clear_bits(s21_decimal *decimal) {
    memset(decimal->bits, 0, sizeof(decimal->bits));
}

unsigned int bit_addition(s21_decimal *value_one, s21_decimal *value_two, s21_decimal *result) {
    clear_bits(result);
    unsigned int status = OK;
    int magnifier_next_digit_of_number = 0;

    for (unsigned int i = 0; i < 96; i++) {
        unsigned int current_bit_one = get_bit(value_one, i);
        unsigned int current_bit_two = get_bit(value_two, i);

        //if both bits equal zero and magnifier == 0 then we set 0 in result decimal
        //if magnifier == 1 then we set 1 in result decimal and reset to zero magnifier
        if (!current_bit_one && !current_bit_two) {
            if (magnifier_next_digit_of_number) {
                set_bit_up(result, i);
                magnifier_next_digit_of_number = 0;
            } else {
                set_bit_down(result, i);
            }
            //if bits not equal (1 - 0 or 0 - 1) and magnifier == 0
            //we set 1 in result decimal (use | for compare value)
            //if magnifier == 1 we set 0 in result decimal and saving the value of magnifier
        } else if (current_bit_one != current_bit_two) {
            if (magnifier_next_digit_of_number) {
                set_bit_down(result, i);
            } else {
                set_bit_up(result, i);
            }
            //if current bits are equal and not zero (1 == 1) ... I'm tired...
        } else if(current_bit_one==current_bit_two) {
            if (magnifier_next_digit_of_number) {
                set_bit_up(result, i);
            } else {
                set_bit_down(result, i);
                magnifier_next_digit_of_number = 1;
            }
        }

        if (i == 95 && magnifier_next_digit_of_number == 1) {
            status = INF;
        }
    }
    return status;
}

unsigned int get_sign(const s21_decimal* bit){
    unsigned int mask = 1u << 31;
    return (bit->bits[3] && mask) != 0;
}


//wtf?
int get_scale(const s21_decimal *value){
    return (char)(value->bits[3]>>16);
}

void scale_equalize(s21_decimal *val1, s21_decimal *val2) {
    s21_decimal ten = {{10u, 0, 0, 0}};
    if (get_scale(val1) < get_scale(val2)) {
        int diff = get_scale(val2) - get_scale(val1);
        if (get_bit(val2, 95) == 0 && get_bit(val2, 94) == 0 &&
            get_bit(val2, 93) == 0) {
            for (int i = 0; i < diff; i++) {
                bit_multiplication(*val1, ten, val1);
            }
            set_scale(val1, get_scale(val2));
        } else {
            for (int i = 0; i < diff; i++) {
                scale_decrease(val2);
            }
            set_scale(val2, get_scale(val1));
        }
    } else {
        int diff = get_scale(val1) - get_scale(val2);
        if (get_bit(*val1, 95) == 0 && get_bit(*val1, 94) == 0 &&
            get_bit(*val1, 93) == 0) {
            for (int i = 0; i < diff; i++) {
                bit_multiplication(*val2, ten, val2);
            }
            set_scale(val2, get_scale(val1));
        } else {
            for (int i = 0; i < diff; i++) {
                scale_decrease(val1);
            }
            set_scale(val1, get_scale(val2));
        }
    }
}

void scale_decrease(s21_decimal *a) {
    s21_decimal ten = {{10u, 0, 0, 0}};
    if (last_bit(*a) < 32 && a->bits[0] < 10) a->bits[0] = 0;
    s21_decimal remainder;
    bit_division(*a, ten, &remainder, a);
}



unsigned int s21_add(s21_decimal *decimal_one, s21_decimal *decimal_two, s21_decimal *decimal_result) {
    clear_bits(decimal_result);
    int status = OK;

    if (!get_sign(decimal_one) && !get_sign(decimal_two)) {
        if (get_scale(decimal_one) != get_scale(decimal_two)) {
            scale_equalize(decimal_one, decimal_two);
        }

        int bit_additioin_result = OK;
        s21_decimal tmp_res;
        bit_additioin_result = bit_addition(number_1, number_2, &tmp_res);

        if (bit_additioin_result == INF) {
            return_value = INF;
        } else {
            *result = tmp_res;
            result->bits[3] = number_1.bits[3];
        }

    } else if (get_sign(&number_1) && !get_sign(&number_2)) {
        set_sign(&number_1, 0);
        return_value = s21_sub(number_2, number_1, result);

    } else if (!get_sign(&number_1) && get_sign(&number_2)) {
        set_sign(&number_2, 0);
        return_value = s21_sub(number_1, number_2, result);

    } else {
        set_sign(&number_1, 0);
        set_sign(&number_2, 0);
        return_value = s21_add(number_1, number_2, result);
        if (return_value == INF)
            return_value = NEGATIVE_INF;
        else
            set_sign(result, 1);
    }

    return return_value;
}

/*


void set_sign(s21_decimal *value,int sign){
    unsigned int mask = 1u << 31;
    if(sign != 0) {
        value->bits[3] |= mask;
    } else {
        value->bits[3] &= ~mask;
    }
}



void set_sclae(s21_decimal *value, int scale) {
    int clearMask = ~(0xFF <<16);
    value->bits[3] &=clearMask;
    int mask = scale<<16;
    value->bits[3] |= mask;
}

int last_bit(s21_decimal number) {
    int last_bit = 95;
    while(last_bit>=0 && get_bit(number, last_bit)==0) {
        last_bit--;
    }
    return last_bit;
}

int shift_left(s21_decimal *value, int offset) {
    int res = OK; // OK == 0
    int lastbit = last_bit(*value);
    if(lastbit +offset >95) {
        res = INF;
    } else {
        for (int i = 0; i<offset; i++) {
            int bit31 = get_bit(*value,31);
            int bit63= get_bit(*value, 63);
            value->bits[0] <<=1;
            value->bits[1] <<=1;
            value->bits[2] <<=1;
            if(bit31) set_bit(value,32,1);
            if(bit63) set_bit(value,64,1);
        }
    }
    return res;
}

int is_zero(s21_decimal val1, s21_decimal val2) {
    int is_zero = FALSE;
    s21_decimal *ptr1 = &val1;
    s21_decimal *ptr2 = &val2;
    if (ptr1 && ptr2) {
        if (!val1.bits[0] && !val2.bits[0] && !val1.bits[1] && !val2.bits[1] &&
            !val1.bits[2] && !val2.bits[2]) {
            is_zero = TRUE;
        }
    }
    return is_zero;
}


void bits_copy(s21_decimal src, s21_decimal *dest) {
    dest->bits[0] = src.bits[0];
    dest->bits[1] = src.bits[1];
    dest->bits[2] = src.bits[2];
    dest->bits[3] = src.bits[3];
}



void bit_multiplication(s21_decimal val1, s21_decimal ten, s21_decimal *res) {
    clear_bits(res);
    s21_decimal tmp;
    int lastbit = last_bit(val1);
    for (int i = 0; i <= lastbit; i++) {
        clear_bits(&tmp);
        int val_bit = get_bit(val1, i);
        if (val_bit) {
            tmp = ten;
            shift_left(&tmp, i);
            bit_addition(*res, tmp, res);
        }
    }
}

void scale_decrease(s21_decimal *a) {
    s21_decimal ten = {{10u, 0, 0, 0}};
    if (last_bit(*a) < 32 && a->bits[0] < 10) a->bits[0] = 0;
    s21_decimal remainder;
    bit_division(*a, ten, &remainder, a);
}

void scale_equalize(s21_decimal *val1, s21_decimal *val2) {
    s21_decimal ten = {{10u, 0, 0, 0}};
    if (get_scale(val1) < get_scale(val2)) {
        int diff = get_scale(val2) - get_scale(val1);
        if (get_bit(*val2, 95) == 0 && get_bit(*val2, 94) == 0 &&
            get_bit(*val2, 93) == 0) {
            for (int i = 0; i < diff; i++) {
                bit_multiplication(*val1, ten, val1);
            }
            set_scale(val1, get_scale(val2));
        } else {
            for (int i = 0; i < diff; i++) {
                scale_decrease(val2);
            }
            set_scale(val2, get_scale(val1));
        }
    } else {
        int diff = get_scale(val1) - get_scale(val2);
        if (get_bit(*val1, 95) == 0 && get_bit(*val1, 94) == 0 &&
            get_bit(*val1, 93) == 0) {
            for (int i = 0; i < diff; i++) {
                bit_multiplication(*val2, ten, val2);
            }
            set_scale(val2, get_scale(val1));
        } else {
            for (int i = 0; i < diff; i++) {
                scale_decrease(val1);
            }
            set_scale(val1, get_scale(val2));
        }
    }
}


 */