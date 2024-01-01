#include "inputHandler.h"
#include <curses.h>
#include <pthread.h>
#include <ncurses.h>
#include <stdlib.h>

struct InputData{char* buffer; int first_unproccesed;};
struct InputData input = {.buffer = NULL, .first_unproccesed = 0};
const int buffer_size = 16; //if 
int next_write_index = 0;

void* input_reader(void* vargp)
{
	while(1)
	{
		char c = getch();
		input.buffer[next_write_index] = c;
		next_write_index = (next_write_index + 1) % buffer_size;
	}
}

enum gbutton get_next_input()
{
	char c = input.buffer[input.first_unproccesed];
	input.buffer[input.first_unproccesed] = '\0';
	input.first_unproccesed = (input.first_unproccesed + 1) % buffer_size;
	return c;
}

void start_input_reader()
{
	input.buffer = malloc(sizeof(char) * buffer_size);
	int i;
	for(i = 0; i < buffer_size; i++)
	{
		input.buffer[i] = '\0';
	}
	pthread_t thread_id; 
	pthread_create(&thread_id, NULL, input_reader, NULL); 
}

