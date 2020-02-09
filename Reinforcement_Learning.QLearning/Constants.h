#pragma once
#include <iostream>

using namespace std;

#ifndef CONSTANTS_H
#define CONSTANTS_H
	enum class action {
		up = 0,
		right,
		down,
		left
	};

	enum class env_type {
		wall = 0,
		path,
		food, 
		head
	};

	struct env_sign {
		char wall = '*';
		char path = '.';
		char head = '^';
		char food = '%';
	};
	static env_sign e_sign;

	struct window_size {
		//coœ pomyœlec, by da³o siê ustawiaæ
		int width = 50;
		int height = 50;
		const int block_width = 10;
		const int block_height = 10;

		void set_width(const int & w) {
			width = w;
		}

		void set_height(const int & h) {
			height = h;
		}
	};
	static window_size win_size;

#endif
