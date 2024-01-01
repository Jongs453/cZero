#include <math.h>
#include <stdlib.h>


int* plot_line_low(int x0, int y0, int x1, int y1, int* points_dest)
{
	int dx = x1 - x0;
	int dy = y1 - y0;
	int yi = 1;
	if (dy < 0)
	{
		yi = -1;
		dy = -dy;
	}
	int D = (2 * dy) - dx;
	int y = y0;

	int x;
	int index = 1;
	for(x = x0; x < x1; x++)
	{
		//plot x y
		points_dest[index++] = y;
		points_dest[index++] = x;
		//increment
		if (D > 0)
		{
			y = y + yi;
			D = D + (2 * (dy - dx));
		}
		else
		{
			D = D + 2*dy;
		}
	}
	return points_dest;
}

int* plot_line_high(int x0, int y0, int x1, int y1, int* points_dest)
{
	int dx = x1 - x0;
	int dy = y1 - y0;
	int xi = 1;
	if (dx < 0)
	{
		xi = -1;
		dx = -dx;
	}
	int D = (2 * dx) - dy;
	int x = x0;
	
	int y;
	int index = 1;
	for(y = y0; y < y1; y++)
	{

		//plot
		points_dest[index++] = y;
		points_dest[index++] = x;
		//increment
		if(D > 0)
		{
			x = x + xi;
			D = D + (2 * (dx - dy));
		}
		else 
		{
			D = D + 2*dx;
		}
	}
		
	return points_dest;
}
/*
int* plot_line(int x0, int y0, int x1, int y1)
{
	int* points;
	int delta_y = abs(y1 - y0);
	int delta_x = abs(x1 - x0);/*
	if(x0 < 0 || y0 < 0 || x1 < 0 || y1 < 0)
	{
		int* bail = malloc(sizeof(int));
		bail[0] = 0;
		return bail;
	}
	if (delta_y < delta_x)
	{
		points = malloc(sizeof(int) * 2 * delta_x + sizeof(int));
		points[0] = delta_x * 2 + 1;
		if (x0 > x1)
			return plot_line_low(x1, y1, x0, y0, points);
		else
			return plot_line_low(x0, y0, x1, y1, points);
	}
	else
	{
		points = malloc(sizeof(int) * 2 * delta_y + sizeof(int));
		points[0] = delta_y + 1;
		if (y0 > y1)
			return plot_line_high(x1, y1, x0, y0, points);
		else
			return plot_line_high(x0, y0, x1, y1, points);
	}
        
}
*/
int* plot_line(int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int error = dx + dy;
	int length = dx >= (-dy) ? dx : -dy;
	if(length > 100000)
	{
		int* bail = malloc(sizeof(int));
		bail[0] = 0;
		return bail;
	}
	int* output = malloc(sizeof(int)*2* length + sizeof(int) );
	output[0] = length * 2 + 1;
	int i = 1;
    while (1) {
        if (x0 == x1 && y0 == y1)
            break;

	//plot
	output[i++] = y0;
	output[i++] = x0;

        int e2 = 2 * error;

        if (e2 >= dy) {
            if (x0 == x1)
                break;
            error += dy;
            x0 += sx;
        }

        if (e2 <= dx) {
            if (y0 == y1)
                break;
            error += dx;
            y0 += sy;
        }
    }
	return output;
}
