#ifndef __BCD_HPP__
#define __BCD_HPP__

#define LSB 0x0F
#define MSB 0x70

static uint8_t BCDtoBIN(uint8_t u)
{
    return (u & LSB) + (((u & MSB) >> 4) * 10);
}

static uint8_t BINtoBCD(uint8_t u)
{
    return (u % 10) + ((u / 10) << 4);
}

#endif