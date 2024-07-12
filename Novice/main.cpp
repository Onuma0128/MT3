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

	Pendulum pendulum;
	pendulum.anchor = {0.0f, 1.0f, 0.0f};
	pendulum.length = 1.25f;
	pendulum.angle = 0.7f;
	pendulum.angularVelocity = 0.0f;
	pendulum.angularAcceleration = 0.0f;

	Sphere sphere{
	    .center = {0.8f, 0.0f, 0.0f},
        .radius = 0.08f
    };
	uint32_t color = WHITE;

	float deltaTime = 1.0f / 60.0f;

	bool start = false;

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

		ImGui::Begin("Sphere");
		ImGui::Checkbox("Start", &start);
		ImGui::End();

		if (start) {
			pendulum.angularAcceleration = 
				-(9.8f / pendulum.length) * std::sin(pendulum.angle);
			pendulum.angularVelocity += pendulum.angularAcceleration * deltaTime;
			pendulum.angle += pendulum.angularVelocity * deltaTime;

			sphere.center.x = pendulum.anchor.x + std::sin(pendulum.angle) * pendulum.length;
			sphere.center.y = pendulum.anchor.y - std::cos(pendulum.angle) * pendulum.length;
			sphere.center.z = pendulum.anchor.z;
		}

		Vector3 p0, p1;
		p0 = Transform(Transform(pendulum.anchor, worldViewProjectionMatrix), viewportMatrix);
		p1 = Transform(Transform(sphere.center, worldViewProjectionMatrix), viewportMatrix);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(worldViewProjectionMatrix, viewportMatrix);
		Novice::DrawLine((int)p0.x, (int)p0.y, (int)p1.x, (int)p1.y, color);
		DrawSphere(sphere, worldViewProjectionMatrix, viewportMatrix, color);

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
