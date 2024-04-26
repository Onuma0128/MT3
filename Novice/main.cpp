#include <Novice.h>
#include "MT3.h"

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
	//クロス積
	Vector3 v1{1.2f, -3.9f, 2.5f};
	Vector3 v2{2.8f, 0.4f, -1.3f};
	Vector3 cross = Cross(v1, v2);

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
		Novice::DrawTriangle(
		    int(screenVertices[0].x), int(screenVertices[0].y), int(screenVertices[1].x), int(screenVertices[1].y),
			int(screenVertices[2].x), int(screenVertices[2].y), RED, kFillModeSolid
		);
		/*Novice::ScreenPrintf(0, 0, "%f", screenVertices[0].x);
		Novice::ScreenPrintf(0, 20, "%f", screenVertices[0].y);
		Novice::ScreenPrintf(0, 40, "%f", screenVertices[1].x);
		Novice::ScreenPrintf(0, 60, "%f", screenVertices[1].y);
		Novice::ScreenPrintf(0, 80, "%f", screenVertices[2].x);
		Novice::ScreenPrintf(0, 100, "%f", screenVertices[2].y);*/

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
