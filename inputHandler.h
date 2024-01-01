#pragma once

enum gbutton{
	left1 = 'f',
	left2 = 'd',
	alt_left1 = 'F',
	alt_left2 = 'D',

	right1 = 'j',
	right2 = 'k',
	alt_right1 = 'J',
	alt_right2 = 'K',

	empty = '\0'
};
	
enum gbutton get_next_input();
void start_input_reader();
