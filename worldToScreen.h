#pragma once
#include "dateTypes.h"
void world_to_screen_point(struct Vector3* point, struct Camera* camera, int y_res, int x_res, struct Vector2Int* dest);
