
#ifndef __MCUCPP_PAINTER_H__
#define __MCUCPP_PAINTER_H__

#include <template_utils.h>


namespace Graphics
{
	using namespace Util;

	template<class Display>
	class Painter
	{
	public:
		typedef int Coord;
		typedef typename Display::Color Color;

		Painter()
			:_color(Display::DefaultColor), _penWidth(1)
		{
			
		}

		void DrawLine(Coord x1, Coord y1, Coord x2, Coord y2);
		void DrawCircle(Coord x, Coord y, Coord r);
		void DrawRect(Coord left, Coord top, Coord width, Coord height);
		template<class Bitmap>
		void DrawBitmap(Bitmap &pic, Coord x, Coord y);
		template<class Font>
		void DrawText(const char *str, Coord x, Coord y, Font &font);
	protected:
		Color _color;
		unsigned _penWidth;
	};

	template<class Display>
	void Painter<Display>::DrawLine(Coord x1, Coord y1, Coord x2, Coord y2)
	{
		Coord deltaX = x2 - x1;
	    Coord deltaY = y2 - y1;
	    Coord signX = 1;
	    Coord signY = 1;
		if(deltaX < 0)
		{
			signX = -1;
			deltaX = -deltaX;
		}

		if(deltaY < 0)
		{
			signY = -1;
			deltaY = -deltaY;
		}

	    Coord error = deltaX - deltaY;
	 	Coord length = max(deltaX, deltaY);

	    for (;length; length--)
	    {
	        Display::PutPixel(x1, y1, 1);
				 
	        Coord error2 = error * 2;
 
	        if(error2 > -deltaY)
	        {
	            error -= deltaY;
	            x1 += signX;
	        }
 
	        if(error2 < deltaX)
	        {
	            error += deltaX;
	            y1 += signY;
	        }
	    }
	}

	template<class Display>
	template<class Bitmap>
	void Painter<Display>::DrawBitmap(Bitmap &pic, Coord x, Coord y)
	{
		Display::DrawBitmap(pic, x, y);
	}

	template<class Display>
	template<class Font>
	void Painter<Display>::DrawText(const char *str, Coord x, Coord y, Font &font)
	{
		char c;
		while((c = *str++))
		{
			typename Font::CharType pic = font.GetChar(c);
			Display::DrawBitmap(pic, x, y);
			x += pic.Width() ;
		}
	}
	
	template<class Display>
	void Painter<Display>::DrawRect(Coord left, Coord top, Coord width, Coord height)
	{
		DrawLine(left, top, left + width, top);
		DrawLine(left, top, left, top + height);
		DrawLine(left + width, top, left + width, top + height);
		DrawLine(left + width, top + height, left, top + height);
	}
}
#endif
