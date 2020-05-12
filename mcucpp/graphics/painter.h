
#ifndef __MCUCPP_PAINTER_H__
#define __MCUCPP_PAINTER_H__

#include <template_utils.h>
#include <static_assert.h>

namespace Mcucpp
{
	namespace Graphics
	{
		using namespace Util;
	
		template<class Display>
		class Painter
		{
			// verify that Display::Coord is signed
			STATIC_ASSERT(IsSigned<typename Display::Coord>::value);
			// and at least 16 bit
			STATIC_ASSERT(sizeof(typename Display::Coord) >= 2);
			Display &_display;
		public:
			typedef typename Display::Coord Coord;
			typedef typename Display::Color Color;
			
			enum OutputMode
			{
				WriteMode = Display::WriteMode,
				XorMode = Display::XorMode,
				AndMode = Display::AndMode,
				AndNotMode = Display::AndNotMode,
				InvertMode = Display::InvertMode
			};

			typedef int_fast8_t PenWidthType;

			
			Painter(Display &display)
				:_display(display),
				_color(Display::DefaultColor), 
				_backColor(Display::DefaultBackground), 
				_penWidth(1),
				_charInterval(1)
			{
				
			}
			void SetCharInterval(PenWidthType i);
			void SetPenWidth(PenWidthType w);
			void SetColor(Color c);
			void SetBackColor(Color c);
			void DrawLine(Coord x1, Coord y1, Coord x2, Coord y2);
			void DrawCircle(Coord x, Coord y, Coord r);
			void DrawRect(Coord left, Coord top, Coord width, Coord height, bool filled=false);
			void FillRect(Coord left, Coord top, Coord width, Coord height);
			void Clear();
			void PutPixel(Coord x, Coord y, Color color);
			Coord Width(){return Display::Width();}
			Coord Height(){return Display::Height();}
			void SetOutputMode(OutputMode mode);

			template<class Bitmap>
			void DrawBitmap(const Bitmap &pic, Coord x, Coord y);

			template<class CharPtr, class Font>
			void DrawText(CharPtr str, Coord x, Coord y, const Font &font);

		protected:
			void CircleHelper(Coord x, Coord y, Coord dx, Coord dy);
			Color _color, _backColor;
			PenWidthType _penWidth;
			PenWidthType _charInterval;
			static const PenWidthType MaxPenWidth = 10;
		public:
			class Checker
			{
			public:
				Checker(unsigned width, unsigned height, Color c1 = Display::DefaultColor, Color c2 = Display::DefaultBackground)
						:_width(width), _height(height), _c1(c1), _c2(c2)
				{}
				Color operator()(unsigned x, unsigned y)const
				{
					return (x & 1) == (y & 1) ? _c1 : _c2;
				}
				unsigned Width()const{return _width;}
				unsigned Height()const{return _height;}
			protected:
				unsigned _width, _height;
				Color _c1, _c2;
			};

			class Fill
			{
			public:
				Fill(unsigned width, unsigned height, Color c = Display::DefaultColor)
						:_width(width), _height(height), _c(c)
				{}
				Color operator()(unsigned, unsigned)const
				{
					return _c;
				}
				unsigned Width()const{return _width;}
				unsigned Height()const{return _height;}
			protected:
				unsigned _width, _height;
				Color _c;
			};
		};

		template<class Display>
		template<class CharPtr, class Font>
		void Painter<Display>::DrawText(CharPtr str, Coord x, Coord y, const Font &font)
		{
			while(*str)
			{
				const typename Font::CharType pic = font.GetChar(*str);
				DrawBitmap(pic, x, y);
				x += pic.Width();
				if(_charInterval)
				{
					_display.DrawBitmap(Fill(_charInterval, pic.Height()), x, y, _backColor, _color);
					x += _charInterval;
				}
				++str;
			}
		}

		template<class Display>
		void Painter<Display>::SetOutputMode(OutputMode mode)
		{
			_display.SetOutputMode((typename Display::OutputMode)mode);
		}

		template<class Display>
		void Painter<Display>::PutPixel(Coord x, Coord y, Color color)
		{
			_display.PutPixel(x, y, color);
		}

		template<class Display>
		void Painter<Display>::Clear()
		{
			_display.Fill(Display::DefaultBackground);
		}

		template<class Display>
		void Painter<Display>::SetCharInterval(PenWidthType i)
		{
			_charInterval = i;
		}

		template<class Display>
		void Painter<Display>::SetColor(Color c)
		{
			_color = c;
		}

		template<class Display>
		void Painter<Display>::SetBackColor(Color c)
		{
			_backColor = c;
		}

		template<class Display>
		void Painter<Display>::SetPenWidth(PenWidthType w)
		{
			if(w <= 0) 
				w = 1;
			if(w > MaxPenWidth) w = MaxPenWidth;
			_penWidth = w;
		}

		template<class Display>
		void Painter<Display>::FillRect(Coord left, Coord top, Coord width, Coord height)
		{
			Fill fill(width, height, _color);
			DrawBitmap(fill, left, top);
		}

		template<class Display>
		void Painter<Display>::DrawLine(Coord x1, Coord y1, Coord x2, Coord y2)
		{
			Coord deltaX = x2 - x1;
			Coord deltaY = y2 - y1;
			int_fast8_t signX = 1;
			int_fast8_t signY = 1;

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

			PenWidthType pw2m = -(_penWidth >> 1);
			PenWidthType pw2p = _penWidth + pw2m;

			for (;length; length--)
			{
				if(deltaX < deltaY)
					for(PenWidthType i = pw2m; i < pw2p; i++)
						_display.PutPixel(x1+i, y1, _color);
				else
					for(PenWidthType i = pw2m; i < pw2p; i++)
						_display.PutPixel(x1, y1+i, _color);
					 
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
		void Painter<Display>::DrawBitmap(const Bitmap &pic, Coord x, Coord y)
		{
			_display.DrawBitmap(pic, x, y, _color, _backColor);
		}

			
		template<class Display>
		void Painter<Display>::DrawRect(Coord left, Coord top, Coord width, Coord height, bool filled)
		{
			PenWidthType pw2m = _penWidth >> 1;
			PenWidthType pw2p = _penWidth - pw2m;

			Fill horizEdge(width - _penWidth, _penWidth, _color);
			DrawBitmap(horizEdge, left + pw2p, top - pw2m);
			DrawBitmap(horizEdge, left + pw2p, top + height - pw2m);
			Fill vertEdge(_penWidth, height + _penWidth, _color);
			DrawBitmap(vertEdge, left - pw2m, top - pw2m);
			DrawBitmap(vertEdge, left + width - pw2m, top - pw2m);

			if(filled)
			{
				Fill region(width - _penWidth, height - _penWidth, _backColor);
				DrawBitmap(region, left + pw2p, top + pw2m);
			}
		}

		template<class Display>
		void Painter<Display>::CircleHelper(Coord x, Coord y, Coord dx, Coord dy)
		{
			_display.PutPixel(x+dx, y+dy, _color);
			if(dx)
				_display.PutPixel(x-dx, y+dy, _color);
			if(dy)
			{
				_display.PutPixel(x+dx, y-dy, _color);
				if(dx)
					_display.PutPixel(x-dx, y-dy, _color);
			}
		}

		template<class Display>
		void Painter<Display>::DrawCircle(Coord x, Coord y, Coord r)
		{  
			r += _penWidth >> 1;
			Coord sqrRInt = sqr(max(r - _penWidth, 0));
			Coord dx = 0, dy = r, old_dy = dy, d = 3 - 2*r;
			while (dx <= dy)
			{	
				if(_penWidth == 1)
				{
					CircleHelper(x, y, dx, dy);
					if(dx != dy)
						CircleHelper(x, y, dy, dx);
				}else
				{
					if(old_dy != dy && dx != dy)
					{
						Coord dxLim = sqrRInt - sqr(dy);
						for(Coord i=0; sqr(dx - i) > dxLim && dx >= i; i++)
						{
							CircleHelper(x, y, dx-i, dy);
						}
					}
					Coord dyLim = sqrRInt - sqr(dx);
					for(Coord i=0; sqr(dy - i) > dyLim && dy >= i; i++)
					{
						CircleHelper(x, y, dy-i, dx);
					}
				}
				old_dy = dy;
				if (d < 0)
					d += 4*dx + 6;
				else 
					d += 4*(dx-(dy--)) + 10;
				++dx;
			}
		}
	}
}
#endif
