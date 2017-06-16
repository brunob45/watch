#ifndef _GPIOR_H_
#define _GPIOR_H_

namespace GPIOReg
{
static __inline__ void set(const uint8_t bit)
{
    GPIOR0 |= _BV(bit);
}

static __inline__ void clear(const uint8_t bit)
{
    GPIOR0 &= ~_BV(bit);
}

static __inline__ uint8_t get(const uint8_t bit)
{
    return (GPIOR0 & _BV(bit));
}
}

#endif // _GPIOR_H_