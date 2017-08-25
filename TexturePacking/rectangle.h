#pragma once
#include <vector>	// for create a vector 
					//container to store the structure

// for rectangle structrue
// this structure will be useful when executing all the algorithms

struct rectangle {
	rectangle(int w, int h);		// constructor
	~rectangle();					// destructor
	int width;						// the width of the rectangle
	int height;						// the height of the rectangle
	int x;	// the x coordinate of the starting point of the rectangle
	int y;	// the y coordinate of teh starting point of the rectangles
};

// overloading comparion functions for necessary ordering 
// ordering according to decreasing height
bool greater_h(const rectangle & r1, const rectangle & r2);
// ordering according to decreasing width
bool greater_w(const rectangle & r1, const rectangle & r2);
// ordering according to decreasing width and decreasing height
bool greater_w_greater_h(const rectangle & r1, const rectangle & r2);
// ordering according to decreasing height and decreasing width
bool greater_h_greater_w(const rectangle & r1, const rectangle & r2);