#pragma once
//#define KS0108_USEBITMAP_CONTEXT
#include <drivers/KS0108.h>

void Update();
void Setup();

using namespace Mcucpp;
using namespace IO;

typedef Pc0Inv Cs1;
typedef Pc1Inv Cs2;
typedef Pc2 Rst;
typedef Pc3 Rw;
typedef Pc4 Di;
typedef Pc5 E;
typedef PinList<Pa0, Pa1, Pa2, Pa3, Pa4, Pa5, Pa6, Pa7> LcdDataBus;
typedef KS0108<Cs1, Cs2, Rst, Rw, Di, E, LcdDataBus> Lcd;
