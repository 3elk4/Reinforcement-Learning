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

	enum class rl_mode {
		qlearning = 0,
		sarsa,
		approx
	};

	enum class env_type {
		wall = 0,
		path,
		food, 
		head
	};

	struct env_type_reward {
		double wall = -100.0;
		double path = -1.0;
		double food = 0.0;
	};
	static env_type_reward env_reward;

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

	enum class program_state {
		none = 0,
		train_and_play,
		qlearning,
		sarsa,
		approx,
		init_env,
		mode_choosing,
		setting_parameters,
		episode
	};

#endif
