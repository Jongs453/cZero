#pragma once
struct Vector3{float x; float y; float z;}; //z is height
struct Vector2Int{int x; int y;};
struct Camera{struct Vector3 pos; float rotation; float down_angle; float fov; float h_fov;}; 
