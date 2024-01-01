#pragma once
#include "dateTypes.h"
#include "worldToScreen.h"

enum ground_type {
	wall = '#',
	path = '-',
	start = '*',
	checkPoint = '+'
};
struct Polygon{struct Vector3** points; int num_of_points; enum ground_type type; };
struct Screen_Poly{struct Vector2Int** points; int num_of_points; enum ground_type type;}; 
struct Polygon*** get_all_Polygons();
struct Screen_Poly*** get_screen_poly_ptr();
void set_all_Polygons(int map_index);
int get_poly_num_y();
int get_poly_num_x();
struct Screen_Poly* malloc_polygon2d(int points);

