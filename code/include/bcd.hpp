/*
 * Nom: bcd.hpp
 * Copyright (C) 2016 Bruno Bousquet
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description:
 * Version: 1.0
 * Date : 14 juin 2017
 */

#ifndef __BCD_HPP__
#define __BCD_HPP__

#define LSB 0x0F
#define MSB 0x70

static __inline__ uint8_t BCDtoBIN(uint8_t u)
{
    return (u & LSB) + (((u & MSB) >> 4) * 10);
}

static __inline__ uint8_t BINtoBCD(uint8_t u)
{
    return (u % 10) + ((u / 10) << 4);
}

#endif