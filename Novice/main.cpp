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

	Vector3 translates[3] = {
	    {0.2f, 1.0f, 0.0f},
        {0.4f, 0.0f, 0.0f},
        {0.3f, 0.0f, 0.0f}
    };
	Vector3 rotates[3] = {
	    {0.0f, 0.0f, -6.8f},
        {0.0f, 0.0f, -1.4f},
        {0.0f, 0.0f, 0.0f }
    };
	Vector3 scales[3] = {
	    {1.0f, 1.0f, 1.0f},
	    {1.0f, 1.0f, 1.0f},
	    {1.0f, 1.0f, 1.0f}
	};

	Sphere sphere[3]{};
	uint32_t color[3]{RED, GREEN, BLUE};

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

		Matrix4x4 sphereWorldMatrix[3]{};
		for (int i = 0; i < 3; ++i) {
			sphereWorldMatrix[i] = MakeAffineMatrix(scales[i], rotates[i], translates[i]);
			sphere[i].center = Transform(sphere[i].center, sphereWorldMatrix[i]);
			sphere[i].radius = 0.1f;
		}

		ImGui::Begin("Spheres");
		//0
		ImGui::DragFloat3("scale0", &scales[0].x, 0.01f);
		ImGui::DragFloat3("rotate0", &rotates[0].x, 0.01f);
		ImGui::DragFloat3("translate0", &translates[0].x, 0.01f);
		//1
		ImGui::DragFloat3("scale1", &scales[1].x, 0.01f);
		ImGui::DragFloat3("rotate1", &rotates[1].x, 0.01f);
		ImGui::DragFloat3("translate1", &translates[1].x, 0.01f);
		//2
		ImGui::DragFloat3("scale2", &scales[2].x, 0.01f);
		ImGui::DragFloat3("rotate2", &rotates[2].x, 0.01f);
		ImGui::DragFloat3("translate2", &translates[2].x, 0.01f);
		ImGui::End();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(worldViewProjectionMatrix, viewportMatrix);
		for (int i = 0; i < 3; ++i) {
			DrawSphere(sphere[i], worldViewProjectionMatrix, viewportMatrix, color[i]);
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
