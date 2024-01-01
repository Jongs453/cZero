// using bresenhams algo, see https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm#All_cases
//for optimisatioin, we return a int array, where int[1 + 2n] is the y coord of a point
//and int[1 + 2n+1] the x coord of a point on the line. to loop over the points later we put 
//the length of the array (number of points + 1) to int[0]

int* plot_line(int x0, int y0, int x1, int y1);
