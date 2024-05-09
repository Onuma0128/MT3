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

	Vector3 cross{};

	Vector3 traiangleScale{1, 1, 1};
	Vector3 traiangleRotate{};
	Vector3 traiangleTranslate{};

	Vector3 cameraScale{1, 1, 1};
	Vector3 cameraRotate{0, 0, 0};
	Vector3 cameraPosition{0.0f,0.0f,-5.0f};
	Vector3 kLocalVertices[3];
	kLocalVertices[0] = {0.0f, 0.5f, 0.0f};
	kLocalVertices[1] = {0.5f, -0.5f, 0.0f};
	kLocalVertices[2] = {-0.5f, -0.5f, 0.0f};


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
		if (keys[DIK_W]) {
			traiangleTranslate.z += 0.01f;
		}
		if (keys[DIK_A]) {
			traiangleTranslate.x -= 0.01f;
		}
		if (keys[DIK_S]) {
			traiangleTranslate.z -= 0.01f;
		}
		if (keys[DIK_D]) {
			traiangleTranslate.x += 0.01f;
		}

		traiangleRotate.y += 0.1f;

		Matrix4x4 worldMatrix = MakeAfineMatrix(traiangleScale, traiangleRotate, traiangleTranslate);
		Matrix4x4 cametaMatrix = MakeAfineMatrix(cameraScale, cameraRotate, cameraPosition);
		Matrix4x4 viewMatrix = Inverse(cametaMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);
		Vector3 screenVertices[3]{};
		for (uint32_t i = 0; i < 3; ++i) {
			Vector3 ndcVertex = Transform(kLocalVertices[i], worldViewProjectionMatrix);
			screenVertices[i] = Transform(ndcVertex, viewportMatrix);
		}
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		cross = Cross(screenVertices[1], screenVertices[2]);
		float dot = Dot(cameraPosition, cross);
		if (dot <= 0) {
			Novice::DrawTriangle(
			    int(screenVertices[0].x), int(screenVertices[0].y), 
				int(screenVertices[1].x), int(screenVertices[1].y), 
				int(screenVertices[2].x), int(screenVertices[2].y), RED, kFillModeSolid);
		}

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
