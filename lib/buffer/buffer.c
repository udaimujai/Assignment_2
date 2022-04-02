#include "buffer.h"
#include <stdbool.h>
#include <math.h>
#define BUF_END 64
#define BUF_START 0
#define FULL_MASK ((uint64_t)0x0001)
#define FULL_MASK_8 ((uint8_t)1U)
#define BUFFER_CAPACITY 8
#define BUFFER_MODULO 8
#define NEXT_ARRAY_BEGINING 8

void buffer_insert(uint8_t *buf, uint8_t start, uint8_t length, uint64_t value)
{
    uint8_t array_start = floor(start / BUFFER_CAPACITY); //to find which byte the writing starts
    uint8_t arrar_bit_num = start % BUFFER_MODULO;
    bool lsw;
    for (uint8_t i = 0; i < length; i++)
    {
        lsw = !!((FULL_MASK << i) & value);
        if (array_start < BUFFER_CAPACITY)
        {
            if (lsw)
            {
                buf[array_start] |= (FULL_MASK_8 << arrar_bit_num);
            }
            else
            {
                buf[array_start] &= ~(FULL_MASK_8 << arrar_bit_num);
            }
            arrar_bit_num++;
            if (arrar_bit_num == NEXT_ARRAY_BEGINING)
            {
                array_start++;
                arrar_bit_num = 0;
            }
        }
    }
}

uint64_t buffer_extract(uint8_t *buf, uint8_t start, uint8_t length)
{
    uint8_t array_start = floor(start / BUFFER_MODULO);
    uint8_t arrar_bit_num = start % BUFFER_MODULO;
    uint64_t data_string = 0x0000U;

    bool lsw;
    for (uint8_t i = 0; i < length; i++)
    {
        lsw = !!((FULL_MASK << arrar_bit_num) & buf[array_start]);
        if (array_start < NEXT_ARRAY_BEGINING)
        {
            if (lsw)
            {
                data_string |= (FULL_MASK << i);
            }
            else
            {
                data_string &= ~(FULL_MASK << i);
            }
            arrar_bit_num++;
            if (arrar_bit_num == NEXT_ARRAY_BEGINING)
            {
                array_start++;
                arrar_bit_num = 0;
            }
        }
    }
    return data_string;
}
