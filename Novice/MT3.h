#pragma once
#include <cmath>
#include <cassert>

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
//パイ
const float pi = 3.14f;

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

// スカラー倍
Vector3 Multiply(float scalar, const Vector3& v);
// 行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
// 3次元アフィン変換
Matrix4x4 MakeAfineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

// クロス積
Vector3 Cross(const Vector3& v1, const Vector3& v2);

// 1、投資投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspecRatio, float nearClip, float farClip);
// 2、正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);
// 3、ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

// Grid
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);
//Sphere
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4 viewportMatrix, uint32_t color);

// 数値表示
static const int kColumnWidth = 60;
void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label);
