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

	Sphere sphere[2]{};
	sphere[0].radius = 0.6f;
	sphere[1] = {0.8f, 0, 1.0f, 0.4f};
	uint32_t sphereColor = BLACK;


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

		Matrix4x4 worldMatrix = MakeAfineMatrix(traiangleScale, traiangleRotate, traiangleTranslate);
		Matrix4x4 cametaMatrix = MakeAfineMatrix(cameraScale, cameraRotate, cameraPosition);
		Matrix4x4 viewMatrix = Inverse(cametaMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		ImGui::Begin("Window");
		ImGui::DragFloat3("sphere[0].center", &sphere[0].center.x, 0.01f);
		ImGui::DragFloat("sphere[0].radius", &sphere[0].radius, 0.01f);
		ImGui::DragFloat3("sphere[1].center", &sphere[1].center.x, 0.01f);
		ImGui::DragFloat("sphere[1].radius", &sphere[1].radius, 0.01f);
		ImGui::DragFloat3("CameraTranslate", &cameraPosition.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);
		ImGui::End();

		if (IsCollision(sphere[0], sphere[1])) {
			sphereColor = RED;
		} else {
			sphereColor = BLACK;
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(worldViewProjectionMatrix, viewportMatrix);

		DrawSphere(sphere[0], worldViewProjectionMatrix, viewportMatrix, sphereColor);
		DrawSphere(sphere[1], worldViewProjectionMatrix, viewportMatrix, sphereColor);

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
