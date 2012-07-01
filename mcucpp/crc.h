#ifndef CRC16_H
#define CRC16_H
#include <inttypes.h>

template<uint16_t polynom>
inline uint16_t Crc16Table(uint16_t v)
{
    uint8_t i = 8; 
    do v = v & 1 ? (v >> 1) ^ polynom : v >> 1; while(i--);
    return v;
}

template<uint16_t polynom>
inline uint16_t Crc16Update(uint8_t newchar, uint16_t crcval)
{
    return (crcval >> 8) ^ Crc16Table<polynom>((crcval ^ newchar) & 0x00ff);
}

template<uint16_t polynom>
inline uint16_t Crc16(const uint8_t* message, uint16_t length, uint16_t crc)
{
    for(uint16_t i = 0; i < length; i++)
    {
       crc = Crc16Update<polynom>(message[i], crc);
    }
    return crc;
}

#endif
