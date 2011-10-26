
#include <avr/io.h>
#include <iopins.h>
#include <pinlist.h>


template<class Rows, class Cols, bool decoder = false>
class MatrixKeyboard
{
	typedef typename Cols::DataType DataType;
public:
	static unsigned GetKeyCode()
	{
		Cols::template SetConfiguration<Cols::PullUpOrDownIn>();
		Cols::template Write(0);
		Rows::template SetConfiguration<Rows::Out>();
		unsigned result = 0;
		
		const DataType rowsCount = decoder ?  
				(1 << Rows::Length) - 1: 
				Rows::Length;

		for(DataType i=0; i < rowsCount; i++)
		{
			if(decoder)
				Rows::Write(i);
			else
				Rows::Write(1 << i);

			asm("nop\nnop\nnop\n");
			DataType cols = Cols::PinRead();
		
			if((DataType)(cols & (cols-1)) == 0) // only one key pressed in one row
			{
				for(DataType j = 1; j < Cols::Length + 1; j++, cols >>= 1)
				if(cols & 1)
				{
					result = i * Cols::Length + j;
					break;
				}
			}
		}
		Cols::template SetConfiguration<Cols::PullUpOrDownIn>();
		Cols::template Write<0>();
		Rows::template SetConfiguration<Rows::PullUpOrDownIn>();
		Cols::template Write<0>();
		return result;
	}
};



using namespace IO;

typedef PinList<Pb0Inv, Pb1Inv, Pb2Inv, Pb3Inv> KeybRows;
typedef PinList<Pa4Inv, Pa5Inv, Pa6Inv, Pa7Inv> KeybCols;

typedef MatrixKeyboard<KeybRows, KeybCols, false> Keyb;

int main()
{

	PORTC = Keyb::GetKeyCode();

	while(1)
	{
	
	}
}
