#include <rectangle.h>

// the constructor, nothing special
rectangle::rectangle(int w, int h) : width(w), height(h) {
	x = 0;
	y = 0;
}

// the destructor and nothing special, too
rectangle::~rectangle() {

}

// ordering according to decreasing width
bool greater_w(const rectangle & r1, const rectangle & r2) {
	return r1.width > r2.width;
}

// ordering according to decreasing height
bool greater_h(const rectangle & r1, const rectangle & r2) {
	return r1.height > r2.height;
}

// ordering according to decreasing height and decreasing width
bool greater_h_greater_w(const rectangle & r1, const rectangle & r2) {
	if (r1.height != r2.height)
		return r1.height > r2.height;
	else
		return r1.width < r2.width;
}

// ordering according to decreasing width and decreasing height
bool greater_w_greater_h(const rectangle & r1, const rectangle & r2) {
	if (r1.width != r2.width)
		return r1.width > r2.width;
	else
		return r1.height < r2.height;
}