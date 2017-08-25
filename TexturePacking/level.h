#pragma once

// for level structure
// this structure will be useful when FFDH are executing 
struct level {
	level(int w, int h, int s);		// constructor
	~level();						// destructor
	int levelW;						// the width of the level
	int levelH;			// the height of the ceiling of the level
	int start;			// the height of the floor of the level
};