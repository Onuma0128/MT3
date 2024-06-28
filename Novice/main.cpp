#include <Novice.h>
#include "MT3.h"
#include <imgui.h>

const char kWindowTitle[] = "LE2A_04_オオヌマ_リオ";

const int kWindowWidth = 1280;
const int kWindowHeight = 720;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Vector3 traiangleScale{1, 1, 1};
	Vector3 traiangleRotate{};
	Vector3 traiangleTranslate{};

	Vector3 cameraScale{1, 1, 1};
	Vector3 cameraRotate{0.26f, 0, 0};
	Vector3 cameraPosition{0.0f,1.9f,-6.49f};

	float cameraDistance = 10.0f;
	float cameraHorizontalAngle = -1.575f;
	float cameraVerticalAngle = 0.26f;

	Vector3 rotate{0.0f, 0.0f, 0.0f};
	OBB obb{
		.center{-1.0f,0.0f,0.0f},
		.orientations = {
			{1.0f,0.0f,0.0f},
			{0.0f,1.0f,0.0f},
			{0.0f,0.0f,1.0f}},
		.size{0.5f,0.5f,0.5f}
	};
	uint32_t color = WHITE;

	Sphere sphere{
	    .center{0.0f, 0.0f, 0.0f},
        .radius{0.5f}
    };

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		// カメラ位置の更新（球面座標系から直交座標系への変換）
		cameraPosition.x = cameraDistance * cosf(cameraVerticalAngle) * cosf(cameraHorizontalAngle);
		cameraPosition.y = cameraDistance * sinf(cameraVerticalAngle);
		cameraPosition.z = cameraDistance * cosf(cameraVerticalAngle) * sinf(cameraHorizontalAngle);

		// ビュー行列の計算
		Vector3 cameraTarget{0.0f, 0.0f, 0.0f}; // カメラが見るターゲット（原点）
		Vector3 up{0.0f, 1.0f, 0.0f};           // 上方向

		Matrix4x4 viewMatrix = MakeLookAtMatrix(cameraPosition, cameraTarget, up);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 worldMatrix = MakeAfineMatrix(traiangleScale, traiangleRotate, traiangleTranslate);
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		Matrix4x4 rotateMatrix = Multiply(MakeRotateXMatrix(rotate.x), Multiply(MakeRotateYMatrix(rotate.y), MakeRotateZMatrix(rotate.z)));
		for (uint32_t i = 0; i < 3; ++i) {
			obb.orientations[i].x = rotateMatrix.m[i][0];
			obb.orientations[i].y = rotateMatrix.m[i][1];
			obb.orientations[i].z = rotateMatrix.m[i][2];
		}

		// obbWorldMatrixInverseの計算
		Matrix4x4 obbTranslateMatrix = MakeTranslateMatrix(obb.center);
		Matrix4x4 obbRotateMatrix = rotateMatrix;
		Matrix4x4 obbScaleMatrix = MakeScaleMatrix(obb.size);
		Matrix4x4 obbWorldMatrixInverse = Inverse(Multiply(Multiply(obbScaleMatrix, obbRotateMatrix), obbTranslateMatrix));

		Vector3 centerInOBBLocalSpace = Transform(sphere.center, obbWorldMatrixInverse);
		Vector3 halfSize = Multiply(3.0f, obb.size);
		AABB aabbOBBLocal{
		    .min{-halfSize.x, -halfSize.y, -halfSize.z},
            .max{halfSize.x,  halfSize.y,  halfSize.z }
        };

		Sphere sphereOBBLocal{
			.center = centerInOBBLocalSpace,
			.radius = sphere.radius
		};
		// ローカル空間での衝突判定
		if (IsCollision(aabbOBBLocal, sphereOBBLocal)) {
			color = RED;
		} else {
			color = WHITE;
		}

		ImGui::Begin("Camera");
		ImGui::DragFloat("CameraDistance", &cameraDistance, 0.01f);
		ImGui::DragFloat("CameraHorizontalAngle", &cameraHorizontalAngle, 0.01f);
		ImGui::DragFloat("CameraVerticalAngle", &cameraVerticalAngle, 0.01f);
		ImGui::End();

		ImGui::Begin("Obj");
		ImGui::DragFloat3("size", &obb.size.x, 0.01f);
		ImGui::DragFloat3("rotate", &rotate.x, 0.01f);
		ImGui::DragFloat3("obb.center", &obb.center.x, 0.01f);
		ImGui::DragFloat3("Sphere.center", &sphere.center.x, 0.01f);
		ImGui::DragFloat("Sphere.radius", &sphere.radius, 0.01f);
		ImGui::DragFloat3("SphereOBBLocal", &sphereOBBLocal.center.x, 0.01f);
		ImGui::End();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(worldViewProjectionMatrix, viewportMatrix);
		DrawOBB(obb, worldViewProjectionMatrix, viewportMatrix, color);
		DrawSphere(sphere, worldViewProjectionMatrix, viewportMatrix, color);

		DrawAABB(aabbOBBLocal, worldViewProjectionMatrix, viewportMatrix, 0x00FF00FF);
		DrawSphere(sphereOBBLocal, worldViewProjectionMatrix, viewportMatrix, 0x00FF00FF);

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
