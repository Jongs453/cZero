#include <stdlib.h>
#include <math.h>
#include "dateTypes.h"
#include "worldToScreen.h"
void world_to_screen_point(struct Vector3* point, struct Camera* camera, int y_res, int x_res, struct Vector2Int* dest)
{
	
	float dis = sqrtf((point->y - camera->pos.y) * (point->y- camera->pos.y) + (point->x- camera->pos.x ) * (point->x- camera->pos.x));
	float angleToCenter = -(atanf((point->x- camera->pos.x)/(point->y- camera->pos.y)) - (camera->rotation)); 
	angleToCenter = atanf((-(point->y - camera->pos.y)) / (point->x - camera->pos.x))  - camera->rotation;

	//point is in negative x in relatioon to cam
	if(point->x - camera->pos.x < 0)
	{
		angleToCenter += 1.5f * M_PI;
	}
	else {
		angleToCenter += 0.5f * M_PI;
	}
	
	//handle large angles and make them negative
	if(fabsf(angleToCenter) > M_PI)
	{

		float mult = angleToCenter < 0 ? 1 : -1;
		angleToCenter = (2 * M_PI - fabsf(angleToCenter)) * mult;

	}

     
	float offseted_dis = cosf(fabsf(angleToCenter))*dis;				
	
	//we cant render point that are "behind" the camere, e.g looking straight down is not possible 
	float height_screen_angle = - camera->down_angle + atanf((camera->pos.z - point->z) / offseted_dis);

	//add on 
	float xpos = (tanf(angleToCenter) / tanf(camera->fov)) * x_res + (x_res * 0.5f);
	float ypos = (tanf(height_screen_angle) / tanf(camera->h_fov)) * y_res + (y_res * 0.5f);

	//add on 
	dest->y = roundf(ypos);
	dest->x = roundf(xpos);


	//janky of offscreen detect
	if(fabsf(angleToCenter) > M_PI * 0.5f) dest->x = -1;
	if(fabsf(height_screen_angle) > M_PI * 0.5f) dest->y = -1;
}
