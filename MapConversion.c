#include "dateTypes.h"
#include "maps.h"
#include "MapConversion.h"
#include <stdlib.h>
#include "worldToScreen.h"

struct Polygon*** all_polys = NULL;
struct Screen_Poly*** screen_polys = NULL;
int poly_num_x = 0;
int poly_num_y = 0;

const float tile_size = 0.2f;
struct Polygon* poly_from_tile(float y, float x, char c)
{
	struct Polygon* p = malloc(sizeof(struct Polygon));
	p->num_of_points = 4;
	p->points = malloc(sizeof(struct Vector3*) * p->num_of_points);
	p->type = c;
	int i;
	for(i = 0; i < p->num_of_points; i++)
	{
		p->points[i] = malloc(sizeof(struct Vector3));
		p->points[i]->z = 0;
	}

	p->points[0]->x = x; p->points[0]->y = y;
	p->points[1]->x = x + tile_size; p->points[1]->y = y;
	p->points[2]->x = x + tile_size; p->points[2]->y = y + tile_size;
	p->points[3]->x = x; p->points[3]->y = y + tile_size;

	return p;
	
}
void set_all_Polygons(int map_index)
{
	if(all_polys != NULL)
	{
		int i;
		for(i = 0; i < poly_num_y; i++)
		{
			int j;
			for(j = 0; i < poly_num_x; i++)
			{
				//free points
				int k;
				for(k = 0; k < all_polys[i][j]->num_of_points; k++)
				{
					free(all_polys[i][j]->points[k]);
					free(screen_polys[i][j]->points[k]);
				}
				free(all_polys[i][j]);
				free(screen_polys[i][j]);
			}
			free(all_polys[i]);
			free(screen_polys[i]);
		}
		free(all_polys);
		free(screen_polys);
	}


	//calc lengths
	int i = 0;
	int j = 0;
	while(course[map_index][i][0] != '\0')
	{
		while(course[map_index][i][j] != '\0')
		{
			j++;
		}
		i++;

	}

	//create Polys
	poly_num_x = j;
	poly_num_y = i;
	int s = sizeof(struct Polygon**) * poly_num_y;
	all_polys = malloc(s);
	screen_polys = malloc(s);
	i = 0;
	j = 0;
	while(course[map_index][i][0] != '\0')
	{
		all_polys[i] = malloc(sizeof(struct Polygon*) * poly_num_x);
		screen_polys[i] = malloc(sizeof(struct Polygon*) * poly_num_x);
		j = 0;
		while(course[map_index][i][j] != '\0')
		{
			//add poly
			all_polys[i][j] = poly_from_tile((float)i * tile_size, (float)j * tile_size, course[map_index][i][j]);

			//add screen polys
			screen_polys[i][j] = malloc_polygon2d(all_polys[i][j]->num_of_points);
			screen_polys[i][j]->num_of_points = all_polys[i][j]->num_of_points;
			screen_polys[i][j]->type = all_polys[i][j]->type;


			j++;
		}
		
		i++;
	}
}

struct Polygon*** get_all_Polygons()
{
	return all_polys;
}
int get_poly_num_y()
{
	return poly_num_y;
}
int get_poly_num_x()
{
	return poly_num_x;
}

struct Screen_Poly* malloc_polygon2d(int points)
{
	struct Screen_Poly* p = malloc(sizeof(struct Polygon));
	p->points = malloc(sizeof(struct Vector2Int*) * points);
	int i;
	for(i = 0; i < points; i++)
	{
		p->points[i] = malloc(sizeof(struct Vector2Int));
	}

	return p;
}
struct Screen_Poly*** get_screen_poly_ptr()
{
	return screen_polys;
}
