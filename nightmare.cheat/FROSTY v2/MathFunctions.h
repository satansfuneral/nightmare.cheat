#pragma once
#include "Vector.h"
#include "MiscDefinitions.h"
#include <algorithm>
#include <stdint.h>
#define PI 3.14159265358979323846f
#define M_PI_F		((float)(M_PI))	// Shouldn't collide with anything.
#define DEG2RAD( x ) ( ( float )( x ) * ( float )( ( float )( PI ) / 180.0f ) )

#ifndef DEG2RAD2
#define DEG2RAD2( x  )  ( (float)(x) * (float)(M_PI_F / 180.f) )
#endif

#define RAD2DEG( x ) ( ( float )( x ) * ( float )( 180.0f / ( float )( PI ) ) )
#define RADPI 57.295779513082f

void AngleVectors(const Vector &angles, Vector *forward);
int flGetDistance(Vector from, Vector to);
void VectorTransform(const Vector in1, float in2[3][4], Vector &out);
void VectorTransform2(const Vector & in1, const matrix3x4_t & in2, Vector & out);
void SinCos(float a, float* s, float*c);
void VectorAngles(Vector forward, Vector &angles);
void AngleVectors(const Vector &angles, Vector *forward, Vector *right, Vector *up);
void FindPoint(Vector &point, int screenwidth, int screenheight, int degrees);
void Normalize(Vector &vIn, Vector &vOut);
float NormalizeYaw(float yaw);
void NormalizeNum(Vector & vIn, Vector & vOut);
void CalcAngle(Vector src, Vector dst, Vector &angles);
void AngleVectors(const Vector& qAngles, Vector& vecForward);
void AngleVectors(const QAngle &angles, Vector& forward, Vector& right, Vector& up);
void AverageDifference(const Vector& a, const Vector& b, float& result);
Vector CalcAngle(Vector& src, Vector& dst);
#define M_RADPI 57.295779513082f
inline void compute_angle(const Vector &source, const Vector &destination, QAngle& angles)
{
	Vector delta = source - destination;
	angles.x = static_cast< float >(asin(delta.z / delta.Length()) * M_RADPI);
	angles.y = static_cast< float >(atan(delta.y / delta.x) * M_RADPI);
	angles.z = 0.0f;

	if (delta.x >= 0.0f)
		angles.y += 180.0f;
}
inline QAngle compute_angle(const Vector &source, const Vector &destination)
{
	QAngle angles;

	Vector delta = source - destination;
	angles.x = static_cast< float >(asin(delta.z / delta.Length()) * M_RADPI);
	angles.y = static_cast< float >(atan(delta.y / delta.x) * M_RADPI);
	angles.z = 0.0f;

	if (delta.x >= 0.0f)
		angles.y += 180.0f;

	return angles;
}
inline float get_distance(const Vector &start, const Vector &end)
{
	float distance = sqrt((start - end).Length());

	if (distance < 1.0f)
		distance = 1.0f;

	return distance;
}
inline void clamp_angles(QAngle& angles)
{
	if (angles.x > 89.0f) angles.x = 89.0f;
	else if (angles.x < -89.0f) angles.x = -89.0f;

	if (angles.y > 180.0f) angles.y = 180.0f;
	else if (angles.y < -180.0f) angles.y = -180.0f;

	angles.z = 0;
}
inline float ClampYaw(float yaw)
{
	while (yaw > 180.f)
		yaw -= 360.f;
	while (yaw < -180.f)
		yaw += 360.f;
	return yaw;
}
inline void normalize_angles(QAngle& angles)
{
	for (auto i = 0; i < 3; i++) {
		while (angles[i] < -180.0f) angles[i] += 360.0f;
		while (angles[i] >  180.0f) angles[i] -= 360.0f;
	}
}
inline bool sanitize_angles(QAngle &angles)
{
	QAngle temp = angles;
	normalize_angles(temp);
	clamp_angles(temp);

	if (!isfinite(temp.x) ||
		!isfinite(temp.y) ||
		!isfinite(temp.z))
		return false;

	angles = temp;

	return true;
}

inline Vector angle_vector(Vector meme)
{
	auto sy = sin(meme.y / 180.f * static_cast<float>(PI));
	auto cy = cos(meme.y / 180.f * static_cast<float>(PI));

	auto sp = sin(meme.x / 180.f * static_cast<float>(PI));
	auto cp = cos(meme.x / 180.f* static_cast<float>(PI));

	return Vector(cp*cy, cp*sy, -sp);
}
inline float distance_point_to_line(Vector Point, Vector LineOrigin, Vector Dir)
{
	auto PointDir = Point - LineOrigin;

	auto TempOffset = PointDir.Dot(Dir) / (Dir.x*Dir.x + Dir.y*Dir.y + Dir.z*Dir.z);
	if (TempOffset < 0.000001f)
		return FLT_MAX;

	auto PerpendicularPoint = LineOrigin + (Dir * TempOffset);

	return (Point - PerpendicularPoint).Length();
}
