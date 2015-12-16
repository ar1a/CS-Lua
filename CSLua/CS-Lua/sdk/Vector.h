#pragma once
#include <math.h>
class matrix3x4_t;
struct Vec2
{
	Vec2() : x(0), y(0) {}
	Vec2(float _x, float _y) { x = _x; y = _y; }
	float x;
	float y;
};

#define M_PI 3.14159265358979323846
typedef float vec_t;
// 3D Vector
#define M_PI_F		((float)(M_PI))
#define DEG2RAD( x  )  ( (float)(x) * (float)(M_PI_F / 180.f) )
class Vector {
public:
	// Members
	vec_t x, y, z;

	// Construction/destruction:
	Vector(void);
	Vector(vec_t X, vec_t Y, vec_t Z);
	Vector(vec_t* clr);

	// Initialization
	void Init(vec_t ix = 0.0f, vec_t iy = 0.0f, vec_t iz = 0.0f);
	// TODO (Ilya): Should there be an init that takes a single float for consistency?

	// Got any nasty NAN's?
	bool IsValid() const;
	void Invalidate();

	// array access...
	vec_t operator[](int i) const;
	vec_t& operator[](int i);

	// Base address...
	vec_t* Base();
	vec_t const* Base() const;

	// Initialization methods
	void Random(vec_t minVal, vec_t maxVal);
	void Zero(); ///< zero out a vector

				 // equality
	bool operator==(const Vector& v) const;
	bool operator!=(const Vector& v) const;

	// arithmetic operations
	Vector& operator+=(const Vector& v) {
		x += v.x; y += v.y; z += v.z;
		return *this;
	}

	Vector& operator-=(const Vector& v) {
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}

	Vector& operator*=(float fl) {
		x *= fl;
		y *= fl;
		z *= fl;
		return *this;
	}

	Vector& operator*=(const Vector& v) {
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}

	Vector& operator/=(const Vector& v) {
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}

	// this ought to be an opcode.
	Vector&	operator+=(float fl) {
		x += fl;
		y += fl;
		z += fl;
		return *this;
	}

	// this ought to be an opcode.
	Vector&	operator/=(float fl) {
		x /= fl;
		y /= fl;
		z /= fl;
		return *this;
	}
	Vector&	operator-=(float fl) {
		x -= fl;
		y -= fl;
		z -= fl;
		return *this;
	}

	// negate the vector components
	void	Negate();

	// Get the vector's magnitude.
	vec_t	Length() const;

	// Get the vector's magnitude squared.
	vec_t LengthSqr(void) const {
		return (x*x + y*y + z*z);
	}

	// return true if this vector is (0,0,0) within tolerance
	bool IsZero(float tolerance = 0.01f) const {
		return (x > -tolerance && x < tolerance &&
			y > -tolerance && y < tolerance &&
			z > -tolerance && z < tolerance);
	}

	vec_t	NormalizeInPlace();
	Vector	Normalized() const;
	bool	IsLengthGreaterThan(float val) const;
	bool	IsLengthLessThan(float val) const;

	// check if a vector is within the box defined by two other vectors
	bool WithinAABox(Vector const &boxmin, Vector const &boxmax);

	// Get the distance from this vector to the other one.
	vec_t	DistTo(const Vector &vOther) const;

	// Get the distance from this vector to the other one squared.
	// NJS: note, VC wasn't inlining it correctly in several deeply nested inlines due to being an 'out of line' .  
	// may be able to tidy this up after switching to VC7
	vec_t DistToSqr(const Vector &vOther) const {
		Vector delta;

		delta.x = x - vOther.x;
		delta.y = y - vOther.y;
		delta.z = z - vOther.z;

		return delta.LengthSqr();
	}

	// Copy
	void	CopyToArray(float* rgfl) const;

	// Multiply, add, and assign to this (ie: *this = a + b * scalar). This
	// is about 12% faster than the actual vector equation (because it's done per-component
	// rather than per-vector).
	void	MulAdd(const Vector& a, const Vector& b, float scalar);

	// Dot product.
	vec_t	Dot(const Vector& vOther) const;

	// assignment
	Vector& operator=(const Vector &vOther);

	// 2d
	vec_t	Length2D(void) const {
		return sqrt(x * x + y * y);
	}
	vec_t	Length2DSqr(void) const;

	/// get the component of this vector parallel to some other given vector
	Vector  ProjectOnto(const Vector& onto);

	// copy constructors
	//	Vector(const Vector &vOther);

	// arithmetic operations
	Vector	operator-(void) const;

	Vector	operator+(const Vector& v) const;
	Vector	operator-(const Vector& v) const;
	Vector	operator*(const Vector& v) const;
	Vector	operator/(const Vector& v) const;
	Vector	operator*(float fl) const;
	Vector	operator/(float fl) const;

	// Cross product between two vectors.
	Vector	Cross(const Vector &vOther) const;

	// Returns a vector with the min or max in X, Y, and Z.
	Vector	Min(const Vector &vOther) const;
	Vector	Max(const Vector &vOther) const;

	Vector Angle()
	{
		Vector forward = *this;
		Vector angles;
		//This is valve's vectorangles
		float	tmp, yaw, pitch;

		if (forward[1] == 0 && forward[0] == 0)
		{
			yaw = 0;
			if (forward[2] > 0)
				pitch = 270;
			else
				pitch = 90;
		}
		else
		{
			yaw = (atan2(forward[1], forward[0]) * 180 / M_PI);
			if (yaw < 0)
				yaw += 360;

			tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
			pitch = (atan2(-forward[2], tmp) * 180 / M_PI);
			if (pitch < 0)
				pitch += 360;
		}

		angles[0] = pitch;
		angles[1] = yaw;
		angles[2] = 0;

		return angles;
	}
	void inline SinCos(float radians, float *sine, float *cosine)
	{
		*sine = sin(radians);
		*cosine = cos(radians);


	}
	Vector Forward(Vector angles)
	{
		Vector forward;
		float	sp, sy, cp, cy;

		SinCos(DEG2RAD(angles[1]), &sy, &cy);
		SinCos(DEG2RAD(angles[0]), &sp, &cp);

		forward.x = cp*cy;
		forward.y = cp*sy;
		forward.z = -sp;

		angles = angles * 8192.f + *this;

		return angles;
	}
	Vector sub(Vector& const v)
	{
		return (*this - v);
	}
	Vector mul(Vector& const v)
	{
		return (*this * v);
	}
	Vector div(Vector& const v)
	{
		return (*this / v);
	}

	Vector ToScreen(const matrix3x4_t& w2sMatrix);
	bool ScreenTransform(Vector& point, Vector& screen, matrix3x4_t& w2sMatrix);
};

void VectorCopy(const Vector& src, Vector& dst);
float VectorLength(const Vector& v);
void VectorLerp(const Vector& src1, const Vector& src2, vec_t t, Vector& dest);
void VectorCrossProduct(const Vector& a, const Vector& b, Vector& result);
vec_t NormalizeVector(Vector& v);


class __declspec(align(16)) VectorAligned : public Vector {
public:
	inline VectorAligned(void) {};
	inline VectorAligned(vec_t X, vec_t Y, vec_t Z) {
		Init(X, Y, Z);
	}

public:
	explicit VectorAligned(const Vector &vOther) {
		Init(vOther.x, vOther.y, vOther.z);
	}

	VectorAligned& operator=(const Vector &vOther) {
		Init(vOther.x, vOther.y, vOther.z);
		return *this;
	}



	float w;
};
struct Vec4
{
	float x;
	float y;
	float z;
	float w;
};