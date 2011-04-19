#pragma once

class DisableInterrupts
{
public:
	DisableInterrupts()
	{
		_sreg = SREG;
		asm("cli");
	}
	~DisableInterrupts()
	{
		SREG = _sreg;
	}
	operator bool()
	{return false;}
private:
	uint8_t _sreg;
};

#define ATOMIC if(DisableInterrupts di = DisableInterrupts()){}else
