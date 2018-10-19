#include <text/symbol_range.h>

namespace Mcucpp
{
	SymbolRange	BaseLatinUpper = 
	{
		0x0041, 26, 32, UpperCase
	};
	
	SymbolRange	BaseLatinLower = 
	{
		0x0061, 26, -32, LowerCase
	};

	SymbolRange	RussianUpper = 
	{
		0x0410, 32, 32, UpperCase
	};
	
	SymbolRange	RussianLower = 
	{
		0x0430, 32, -32, LowerCase
	};
	
	
}