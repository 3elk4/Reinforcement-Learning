#pragma once
#include "Pair.h"
using namespace std;

#ifndef CONSTANTS_H
	enum class action {
		up = 0,
		right,
		down,
		left,
		none
	};

	struct window_size {
		const int width = 50;
		const int height = 50;
		const int block_width = 10;
		const int block_height = 10;
	} ;
	static window_size win_size;
	
#endif


