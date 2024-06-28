#pragma once
#include <cmath>
#include <cassert>

// パイ
const float pi = 3.14f;

struct Vector3 {
	float x, y, z;
};
struct Matrix4x4 {
	float m[4][4];
};
struct Sphere {
	Vector3 center;
	float radius;
};
struct Line {
	Vector3 origin;
	Vector3 diff;
};
struct Ray {
	Vector3 origin;
	Vector3 diff;
};
struct Segment {
	Vector3 origin;
	Vector3 diff;
};
struct Plane {
	Vector3 normal;
	float distance;
};
struct Triangle {
	Vector3 vertices[3];
};
struct AABB {
	Vector3 min;
	Vector3 max;
};
struct OBB {
	Vector3 center;
	Vector3 orientations[3];
	Vector3 size;
};

// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m);
// 座標変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);
// 1.x軸の回転行列
Matrix4x4 MakeRotateXMatrix(float radian);
// 2.y軸の回転行列
Matrix4x4 MakeRotateYMatrix(float radian);
// 3.z軸の回転行列
Matrix4x4 MakeRotateZMatrix(float radian);

// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2);
// 減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2);
// 内積
float Dot(const Vector3& v1, const Vector3& v2);
// 長さ
float Length(const Vector3& v);
// 正規化
Vector3 Normalize(const Vector3& v);
// スカラー倍
Vector3 Multiply(float scalar, const Vector3& v);
// ラープ関数
Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);
// 行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
// 3次元アフィン変換
Matrix4x4 MakeScaleMatrix(const Vector3 scale);
Matrix4x4 MakeRotateMatrix(const Vector3 rotate);
Matrix4x4 MakeTranslateMatrix(const Vector3 translate);
Matrix4x4 MakeAfineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

// クロス積
Vector3 Cross(const Vector3& v1, const Vector3& v2);

// 1、投資投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspecRatio, float nearClip, float farClip);
// 2、正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);
// 3、ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);
//
Matrix4x4 MakeLookAtMatrix(const Vector3& eye, const Vector3& target, const Vector3& up);

// 正射影ベクトル
Vector3 Project(const Vector3& v1, const Vector3& v2);
// 最近接点
Vector3 ClosestPoint(const Vector3& point, const Segment& segment);
//
Vector3 Perpendicular(const Vector3& vector);

//球と球の衝突判定
bool IsCollision(const Sphere& s1, const Sphere& s2);
//球と平面の衝突判定
bool IsCollision(const Sphere& sphere, const Plane& plane);
//線と平面の衝突判定
bool IsCollision(const Segment& segment, const Plane& plane);
//三角形と線の衝突判定
bool LineIntersectsPlane(const Triangle& triangle, const Segment& segment, Vector3& intersection);
bool IsPointInTriangle(const Triangle& triangle, const Vector3& point);
bool IsCollision(const Triangle& triangle, const Segment& segment);
//AABBとAABBの衝突判定(矩形の衝突判定)
bool IsCollision(const AABB& a, const AABB& b);
//AABBと球の衝突判定
bool IsCollision(const AABB& aabb, const Sphere& sphere);

// Grid
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);
//Sphere
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4 viewportMatrix, uint32_t color);
//平面の描画
void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);
//三角形の描画
void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);
//AABBの描画(矩形)
void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);
//OBBの描画
void DrawOBB(const OBB& obb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);
//ベジエ曲線の描画
void DrawBezier(const Vector3& controlPoint0, const Vector3& controlPoint1, const Vector3& controlPoint2,
	 const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

// 数値表示
static const int kColumnWidth = 60;
void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label);
