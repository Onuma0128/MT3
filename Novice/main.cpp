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

	Vector3 a{0.2f, 1.0f, 0.0f};
	Vector3 b{2.4f, 3.1f, 1.2f};
	Vector3 c = a + b;
	Vector3 d = a - b;
	Vector3 e = a * 2.4f;

	Vector3 rotate{0.4f, 1.43f, -0.8f};
	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
	Matrix4x4 rotateMatrix = rotateXMatrix * rotateYMatrix * rotateZMatrix;

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
		Matrix4x4 worldMatrix = MakeAffineMatrix(traiangleScale, traiangleRotate, traiangleTranslate);
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		ImGui::Begin("Camera");
		ImGui::DragFloat("CameraDistance", &cameraDistance, 0.01f);
		ImGui::DragFloat("CameraHorizontalAngle", &cameraHorizontalAngle, 0.01f);
		ImGui::DragFloat("CameraVerticalAngle", &cameraVerticalAngle, 0.01f);
		ImGui::End();

		ImGui::Begin("Window");
		ImGui::Text("c:%f,%f,%f", c.x, c.y, c.z);
		ImGui::Text("d:%f,%f,%f", d.x, d.y, d.z);
		ImGui::Text("e:%f,%f,%f", e.x, e.y, e.z);
		ImGui::Text(
			"matrix:\n%f,%f,%f,%f\n%f,%f%f,%f\n%f,%f,%f,%f\n%f,%f,%f,%f\n",
			rotateMatrix.m[0][0],rotateMatrix.m[0][1],rotateMatrix.m[0][2],
			rotateMatrix.m[0][3],rotateMatrix.m[1][0],rotateMatrix.m[1][1],
			rotateMatrix.m[1][2],rotateMatrix.m[1][3],rotateMatrix.m[2][0],
			rotateMatrix.m[2][1],rotateMatrix.m[2][2],rotateMatrix.m[2][3],
			rotateMatrix.m[3][0],rotateMatrix.m[3][1],rotateMatrix.m[3][2],
			rotateMatrix.m[3][3]);
		ImGui::End();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(worldViewProjectionMatrix, viewportMatrix);

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
