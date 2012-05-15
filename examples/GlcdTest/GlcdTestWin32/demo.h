#pragma once

#include "glcd.h"
#include <drivers/KS0108.h>
#include <ColorDisplay.h>

void Update();
void Setup();

using namespace Mcucpp;
typedef KS0108<Glcd::Cs1, Glcd::Cs2, Glcd::Rst, Glcd::Rw, Glcd::Di, Glcd::E, Glcd::LcdDataPort> Lcd;
//typedef ColorDisplay Lcd;