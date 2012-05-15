

#include "../common/resources.h"
#include <graphics/painter.h>
#include <demo.h>
#include <stdlib.h>

using namespace Mcucpp;
typedef Painter<Lcd> MyPainter;

void Setup()
{
	Lcd::Init();
}

// name conflict with Win32 macros
#ifdef DrawText
#undef DrawText
#endif

struct Demo
{
	void (*DemoFunc)(unsigned);
	unsigned frames;
};

void PixelDemo(unsigned frame)
{
	MyPainter p;
	
	for(int i = 0; i < 50; i++)
		p.PutPixel(rand() & 127, rand() & 63, 1);
}

void LineDemo(unsigned frame)
{
	MyPainter p;
	p.Clear();
	for(int i = 1; i < 8; i++)
	{
		p.SetPenWidth(i);
		p.DrawLine(5, i*8, 60, i*8);
	}
	p.SetPenWidth(2);
	p.DrawLine(64+frame, 5, 125 - frame, 60);
	p.DrawLine(125, 5+frame, 64, 60-frame);
}

void RectDemo(unsigned frame)
{
	MyPainter p;
	p.Clear();
	p.SetPenWidth(1);
	p.DrawRect(1, 1, 127 - 2, 63 - 2);
	p.SetPenWidth(3);
	p.DrawRect(8, 8, 127 - 17, 63 - 17);
	p.SetPenWidth(5);
	unsigned delta = frame & 15;
	p.DrawRect(15 + delta, 15 + delta, 127 - 30 - 2*delta, 63 - 30 - 2*delta);
}

void CircleDemo(unsigned frame)
{
	MyPainter p;
	p.Clear();
	p.SetPenWidth(1);
	p.DrawCircle(64, 32, 50 + ((frame >> 1) & 15));
	p.SetPenWidth(1);
	p.DrawCircle(64, 32, 32);
	p.SetPenWidth(2);
	p.DrawCircle(64, 32, 28);
	p.SetPenWidth(3);
	p.DrawCircle(64, 32, 22);
	p.SetPenWidth(4);
	p.DrawCircle(64, 32, 15);
}


void BitmapDemo(unsigned frame)
{
	MyPainter p;
	p.Clear();
	
	p.DrawBitmap(MyPainter::Checker(20, 20), 10, 10);
	p.DrawBitmap(MyPainter::Fill(30, 20), 10, 40);
	p.DrawBitmap(MyPainter::Checker(30, 15), 70, 40);

	p.SetOutputMode(MyPainter::AndNotMode);
	p.DrawBitmap(bitmap, 20, -30+(frame&63));
	p.SetOutputMode(MyPainter::XorMode);
	p.DrawBitmap(bitmap, 20, -30+(frame&63));
	p.SetOutputMode(MyPainter::WriteMode);
}

void TextDemo(unsigned frame)
{
	MyPainter p;
	p.Clear();
	p.DrawText("Hello,", 30, 10, bigFont);
	p.DrawText("world!!!", 20, 30, smallFont);
	
	p.SetColor(0);
	p.SetBackColor(1);
	p.DrawText("Mcucpp graphics demo", 0, 56, smallFont);
}

Demo demonstrations[]=
{
	{TextDemo, 50},
	{PixelDemo, 50},
	{LineDemo, 60},
	{RectDemo, 50},
	{CircleDemo, 100},
	{BitmapDemo, 200},
};

static unsigned frame=0;
static unsigned nextDemoframe=demonstrations[0].frames;
static unsigned currentDemo = 0;

void Update()
{
	demonstrations[currentDemo].DemoFunc(demonstrations[currentDemo].frames + frame - nextDemoframe);
	frame++;
	if(frame == nextDemoframe)
	{
		currentDemo++;
		if(currentDemo >= sizeof(demonstrations)/sizeof(demonstrations[0]))
		{
			currentDemo = 0;
			frame = 0;
		}
		nextDemoframe = frame + demonstrations[currentDemo].frames;
	}
}

