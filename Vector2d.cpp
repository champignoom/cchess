#include "Vector2d.hpp"
#include <ostream>

using std::ostream;

Vector2d Vector2d::quandrant() const {
	return {quandrantInt(x), quandrantInt(y)};
}

int Vector2d::quandrantInt(int k) {
	return k<0 ?-1 :k>0 ?1 :0;
}

ostream &operator <<(ostream &o, Vector2d v) {
	o <<"(" <<v.x <<"," <<v.y <<")";
	return o;
}
