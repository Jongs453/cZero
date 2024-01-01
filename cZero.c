#include <curses.h>
#include <ncurses.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <wchar.h>

#include "dateTypes.h"
#include "worldToScreen.h"
#include "MapConversion.h"
#include "inputHandler.h"
#include "linedrawing.h"

WINDOW * win;

float y_cam_pos = 0.0f; //y is down, x is right
float x_cam_pos = 0.0f;
//angle goes counterclockwise lol 0 means looks to positive y
/*float cam_angle = M_PI * 0.0f;
float fov = (2 * M_PI)/3;
float cam_height = 0.3;
float cam_downangle = 0.07f * M_PI;
*/
struct Camera cam = {
	.pos.x = 2.5f,
	.pos.y = 0.0f,
	.pos.z = 0.3f,
	
	.rotation = 0.0f,

	.down_angle = 0.07f * M_PI,
	.fov = (2 * M_PI) / 3,
	//h_fov is set in setGlobals 
	
};
float math_mod(float a, float N) {return a - N*floor(a/N);}
void rotate_cam(float r) {cam.rotation = math_mod(cam.rotation + r, (2 * M_PI));}

struct config{
	float turn_fx;
};
struct config conf = {.turn_fx = 0.1};

//float tile_size = 0.2f;

int field_width = 20;
int field_height = 20;
//first num is y coord, second x
char field[20][21] = {
	"####################",
	"####################",
	"####################",
	"####################",
	"####################",
	"####################",
	"######----------####",
	"######----------####",
	"#############---####",
	"#############---####",
	"#############---####",
	"#############---####",
	"####################",
	"####################",
	"####################",
	"####################",
	"####################",
	"####################",
	"####################",
	"####################"};
#define res_x 240
#define res_y 60
char screen[res_y][res_x + 1];

void draw_poly_lines(struct Screen_Poly* poly)
{
	int vertex;
	for(vertex = 0; vertex < poly->num_of_points; vertex++)
	{
		int next_vertex_index = (vertex + 1) % poly->num_of_points;
		int x0 = poly->points[vertex]->x;
		int y0 = poly->points[vertex]->y;
		int x1 = poly->points[next_vertex_index]->x;
		int y1 = poly->points[next_vertex_index]->y;
		int* line_points = plot_line(x0, y0, x1, y1);
		
		int i;
		for(i = 1; i < line_points[0]; i += 2)
		{
			if(line_points[i] < 0 || line_points[i] >= res_y) continue;
			if(line_points[i+1] < 0 || line_points[i+1] >= res_x) continue;

			screen[line_points[i]][line_points[i+1]] = poly->type;
		}
		free(line_points);

	}
}

struct Vector2Int*** points;
void create_screen()
{
	//calculate points
	int y_len = field_height;
	int x_len = field_width;

	int i;
	/*
	for(int i = 0; i < y_len; i++)
	{
		float tile_ypos = (float)i * 0.2f;
		int j;
		for(int j = 0; j <x_len; j++)
		{
			float tile_xpos = (float)j * 0.2f;

			struct Vector3 p = {.y = tile_ypos, .x = tile_xpos, .z = 0};
			world_to_screen_point(&p, &cam, res_y, res_x, points[i][j]);

		}
	}*/

	struct Screen_Poly*** polys = get_screen_poly_ptr();
	for(i = 0; i < get_poly_num_y(); i++)
	{
		int j;
		for(j = 0; j < get_poly_num_x(); j++)
		{
			int p;
			for(p = 0; p < polys[i][j]->num_of_points; p++)
			{
				world_to_screen_point(get_all_Polygons()[i][j]->points[p], &cam , res_y, res_x, polys[i][j]->points[p]);
			}
		}
	}
	
	//reset screen
	for(int i = 0; i < res_y; i ++){
		int j;
		for(int j = 0; j < res_x; j++)
		{
			screen[i][j] = '-';
		}
		screen[i][res_x] = '\0';
	}
	
	//build screen
	/*
	for(int i = 0; i < y_len; i++)
	{
		int j;
		for (j = 0; j < x_len; j++) {
			if(points[i][j]->y >= res_y || points[i][j]->x >= res_x) continue;
			if(points[i][j]->y <0 || points[i][j]->x < 0) continue;
			out[points[i][j]->y][points[i][j]->x] = field[i][j];
		}
	}*/

	//new build screen
	for(int i = 0; i < get_poly_num_y(); i++)
	{
		int j;
		for (j = 0; j < get_poly_num_x(); j++) {
			//ignore normal ground
			if(polys[i][j]->type == path) continue;

			int points_on_screen = 0;
			int p;
			for(p = 0; p < polys[i][j]->num_of_points; p++)
			{
				struct Vector2Int* this_point = polys[i][j]->points[p];

				//check if point is on screen
				if(this_point->y >= res_y || this_point->x >= res_x) continue;
				if(this_point->y <0 || this_point->x < 0) continue;

				screen[this_point->y][this_point->x] = polys[i][j]->type;
				points_on_screen++;

			}

			//draw lines if at least 1 point is on screen
			if(points_on_screen == 0) continue;
			draw_poly_lines(polys[i][j]);
			
		}
	}
	
	//Print screen
	for(i = 0; i < res_y; i++)
	{
		wprintw(win,"%s", screen[i]);
		wprintw(win, "|\n");
	}


}



void set_globals()
{

	cam.h_fov = cam.fov * ((float)res_y/(float)res_x) * 1.8f;
	//printw("hFOV%.3f",height_fov);
	//init ScreenPoints
	points = malloc(sizeof(struct Vector2Int**) * field_height);
	int i;
	for(i = 0; i < field_height; i++)
	{
		points[i] = malloc(sizeof(struct Vector2Int*) * field_width);
		int j;
		for(j = 0; j < field_width; j++)
		{
			points[i][j] = malloc(sizeof(struct Vector2Int));
		}
	}


}

void apply_inputs()
{
	enum gbutton b = get_next_input();
	while (b != empty) {
	
		switch (b) {
			case (left1):
			case (left2):
				rotate_cam(conf.turn_fx);
				break;
			case (right1):
			case (right2):
				rotate_cam(-conf.turn_fx);
				break;
			default:
				break;
		}
		b = get_next_input();
	}
	
}
void update_screen()
{
	while (true) {
		usleep(10000);
		apply_inputs();
		wrefresh(win);
		wmove(win, 0, 0);
		//cam.rotation = fmodf(cam.rotation + 0.01f, 2* M_PI);
		cam.pos.y += 0.005f * cosf(cam.rotation);
		cam.pos.x += 0.005f * (sinf(cam.rotation));
		//printw("cam angle %.3f \n", cam_angle);
		create_screen();
	}
}

int main(){
	initscr();cbreak();noecho();

	//make curser invisible
	curs_set(0);
	win = newwin(res_y+ 2, res_x+ 2, 0, 0);
	set_all_Polygons(0);
	set_globals();
	start_input_reader();


	create_screen();
	update_screen();
	getch();
	endwin();
	return 0;
}

