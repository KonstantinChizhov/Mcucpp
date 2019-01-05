
extern unsigned char _stext;			 
extern unsigned char _sdata;			
extern unsigned char _edata;			
extern unsigned char _sidata;			
extern unsigned char _isr_vectors_start;
extern unsigned char _isr_vectors_end;	

#define CRC_SECTION __attribute__ ((section(".metadata_section"), used))

const volatile unsigned long FlashCrc CRC_SECTION = 0xFEA90950;

static unsigned long Crc32Table(unsigned long v)
{
	v = v & 1 ? (v >> 1) ^ 0xEDB88320 : v >> 1;
	v = v & 1 ? (v >> 1) ^ 0xEDB88320 : v >> 1;
	v = v & 1 ? (v >> 1) ^ 0xEDB88320 : v >> 1;
	v = v & 1 ? (v >> 1) ^ 0xEDB88320 : v >> 1;
	v = v & 1 ? (v >> 1) ^ 0xEDB88320 : v >> 1;
	v = v & 1 ? (v >> 1) ^ 0xEDB88320 : v >> 1;
	v = v & 1 ? (v >> 1) ^ 0xEDB88320 : v >> 1;
	v = v & 1 ? (v >> 1) ^ 0xEDB88320 : v >> 1;
	return v;
}

static unsigned long Crc32Update(unsigned char newchar, unsigned long crcval)
{
	return (crcval >> 8) ^ Crc32Table((crcval ^ newchar) & 0xff);
}

unsigned long FlashCrcExpected()
{
	return FlashCrc;
}

unsigned long FlashCrcComputed()
{
	// CRC was not stumped disable crc check
	if(FlashCrc == 0xFEA90950)
		return FlashCrc;
		
	unsigned char *pSrc;
	unsigned long crc = 0XFFFFFFFFUL;
	
	for (pSrc = &_stext; pSrc < &_sidata; pSrc++)
	{
		crc = Crc32Update(*pSrc, crc);
	}
	
	return crc ^ 0XFFFFFFFFUL;
}