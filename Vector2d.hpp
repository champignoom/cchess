#ifndef VECTOR2D_HPP
#define VECTOR2D_HPP

#include <ostream>

struct Vector2d {
	int x, y;
	Vector2d(int x_, int y_): x(x_), y(y_) { }
	Vector2d operator +(Vector2d other) const { return {x+other.x, y+other.y}; }
	Vector2d operator -(Vector2d other) const { return {x-other.x, y-other.y}; }
	Vector2d &operator +=(Vector2d other) { x += other.x; y += other.y; return *this; }
	Vector2d &operator -=(Vector2d other) { x -= other.x; y -= other.y; return *this; }
	bool operator ==(Vector2d other) const { return x==other.x && y==other.y; }
	bool operator !=(Vector2d other) const { return x!=other.x || y!=other.y; }
	bool isOnAxis() const { return x==0 || y==0; }
	bool isZero() const { return x==0 && y==0; }

	Vector2d quandrant() const;
	static int quandrantInt(int k);

	friend std::ostream &operator <<(std::ostream &, Vector2d);
};

#endif
