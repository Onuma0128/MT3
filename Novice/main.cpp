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

	Spring spring{};
	spring.anchor = {0.0f, 0.0f, 0.0f};
	spring.naturalLength = 1.0f;
	spring.stiffness = 100.0f;
	spring.dampingCoefficient = 2.0f;

	Ball ball{};
	ball.position = {1.2f, 0.0f, 0.0f};
	ball.mass = 2.0f;
	ball.radius = 0.05f;
	ball.color = BLUE;

	Sphere sphere{};

	float deltaTime = 1.0f / 60.0f;

	Vector3 position[2]{};

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

		Vector3 diff = ball.position - spring.anchor;
		float length = Length(diff);
		if (length != 0.0f) {
			Vector3 direction = Normalize(diff);
			Vector3 restPosition = spring.anchor + direction * spring.naturalLength;
			Vector3 disolacement = length * (ball.position - restPosition);
			Vector3 restoringForce = -spring.stiffness * disolacement;
			Vector3 dampingForce = -spring.dampingCoefficient * ball.velocity;
			Vector3 force = restoringForce + dampingForce;
			ball.acceleration = force / ball.mass;
		}
		ball.velocity = ball.acceleration * deltaTime + ball.velocity;
		ball.position = ball.velocity * deltaTime + ball.position;

		sphere.center = ball.position;
		sphere.radius = ball.radius;

		position[0] = Transform(Transform(spring.anchor, worldViewProjectionMatrix), viewportMatrix);
		position[1] = Transform(Transform(sphere.center, worldViewProjectionMatrix), viewportMatrix);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(worldViewProjectionMatrix, viewportMatrix);
		Novice::DrawLine((int)position[0].x, (int)position[0].y, (int)position[1].x, (int)position[1].y, WHITE);
		DrawSphere(sphere, worldViewProjectionMatrix, viewportMatrix, ball.color);

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
