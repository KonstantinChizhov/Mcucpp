#pragma once

class ColorDisplay
{
public:
	static const int width = 128;
	static const int height = 64;

	enum OutputMode
	{
		WriteMode = 0,
		XorMode = 1,
		AndMode = 2,
		InvertMode = 3
	};
	typedef int_fast16_t Coord;
	typedef uint32_t Color;
	static const Color DefaultColor = 0x00ffffff;
	static const Color DefaultBackground = 0;

	static Coord Width() {return 128;}
	static Coord Height() {return 64;}

	static void Init(){}
	static void Fill(Color color)
	{
		for(Coord dx = 0; dx<width; dx++)
			for(Coord dy = 0; dy<height; dy++)
				PutPixel(dx, dy, color);
	}

	static void PutPixel(Coord x, Coord y, Color color)
	{
		_buffer[y*width + x] = color;
	}

	template<class BitmapT>
	static void DrawBitmap(const BitmapT &bitmap, Coord x, Coord y, Color foreground = DefaultColor, Color background = DefaultBackground)
	{
		for(Coord dx = 0; dx<bitmap.Width(); dx++)
			for(Coord dy = 0; dy<bitmap.Height(); dy++)
				PutPixel(x+dx, y+dy, bitmap(dx, dy) ? foreground : DefaultBackground);
	}

	template<class PixmapT>
	static void DrawPixmap(const PixmapT &pixmap, Coord x, Coord y)
	{
		for(Coord dx = 0; dx<pixmap.Width(); dx++)
			for(Coord dy = 0; dy<pixmap.Height(); dy++)
				PutPixel(x+dx, y+dy, bitmap(dx, dy));
	}

	static void Flush(){}
	inline static void SetOutputMode(OutputMode mode)
	{
		_mode = mode;
	}

	static void Draw(HDC dc)
	{
		RECT rect;
		GetClipBox(dc, &rect);
		
		BITMAPINFO info;
		memset(&info, 0, sizeof(info));
		info.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
		info.bmiHeader.biBitCount	 = 32;
		info.bmiHeader.biWidth       = width;
		info.bmiHeader.biHeight      = -height; 
		info.bmiHeader.biPlanes      = 1;
		info.bmiHeader.biCompression = BI_RGB;
		info.bmiHeader.biSizeImage   = width*height*sizeof(_buffer[0]);

		int res = StretchDIBits(dc, 
					rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
					 0, 0, width, height,
					_buffer, &info, DIB_RGB_COLORS, SRCCOPY);
	}
protected:
	static OutputMode _mode;
	static uint32_t _buffer[width*height];
};

