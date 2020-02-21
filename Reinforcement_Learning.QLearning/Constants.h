//#pragma once
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
		double wall = -1.0;
		double path = 0.0;
		double food = 5.0;
	};
	static env_type_reward env_reward;

	struct env_sign {
		char wall = '*';
		char path = '.';
		char head = '^';
		char food = '%';
	};
	static env_sign e_sign;

	class window_size {
	public:
		int get_block_width() { return block_width; }
		int get_block_height() { return block_height; }
		int get_width() { return width; }
		int get_height() { return height; }
		void set_width(int w) { width = w; }
		void set_height(int h) { height = h; }

		static window_size & get_instance() {
			static window_size singleton;
			return singleton;
		}
	private:
		const int block_width = 10;
		const int block_height = 10;
		int width;
		int height;

		/*window_size();
		window_size(const window_size &);*/
	};
	/*struct window_size {
		//coœ pomyœlec, by da³o siê ustawiaæ
		int width = 50;
		int height = 50;
		const int block_width = 10;
		const int block_height = 10;

		void set_width(const int & w) {
			this->width = w;
		}

		void set_height(const int & h) {
			this->height = h;
		}
	};
	window_size win_size;*/

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
