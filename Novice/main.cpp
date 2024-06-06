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

	AABB aabb1{
	    .min{-0.5f, -0.5f, -0.5f},
        .max{0.0f,  0.0f,  0.0f }
    };
	uint32_t color = WHITE;
	AABB aabb2{
	    .min{0.2f, 0.2f, 0.2f},
        .max{1.0f, 1.0f, 1.0f}
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

		ImGui::Begin("Camera");
		ImGui::DragFloat("CameraDistance", &cameraDistance, 0.01f);
		ImGui::DragFloat("CameraHorizontalAngle", &cameraHorizontalAngle, 0.01f);
		ImGui::DragFloat("CameraVerticalAngle", &cameraVerticalAngle, 0.01f);
		ImGui::End();

		ImGui::Begin("Obj");
		ImGui::DragFloat3("aabb1.min", &aabb1.min.x, 0.01f);
		ImGui::DragFloat3("aabb1.max", &aabb1.max.x, 0.01f);
		ImGui::DragFloat3("aabb2.min", &aabb2.min.x, 0.01f);
		ImGui::DragFloat3("aabb2.max", &aabb2.max.x, 0.01f);
		ImGui::End();

		aabb1.min.x = (std::min)(aabb1.min.x, aabb1.max.x);
		aabb1.max.x = (std::max)(aabb1.min.x, aabb1.max.x);
		aabb1.min.y = (std::min)(aabb1.min.y, aabb1.max.y);
		aabb1.max.y = (std::max)(aabb1.min.y, aabb1.max.y);
		aabb1.min.z = (std::min)(aabb1.min.z, aabb1.max.z);
		aabb1.max.z = (std::max)(aabb1.min.z, aabb1.max.z);

		aabb2.min.x = (std::min)(aabb2.min.x, aabb2.max.x);
		aabb2.max.x = (std::max)(aabb2.min.x, aabb2.max.x);
		aabb2.min.y = (std::min)(aabb2.min.y, aabb2.max.y);
		aabb2.max.y = (std::max)(aabb2.min.y, aabb2.max.y);
		aabb2.min.z = (std::min)(aabb2.min.z, aabb2.max.z);
		aabb2.max.z = (std::max)(aabb2.min.z, aabb2.max.z);

		if (IsCollision(aabb1, aabb2)) {
			color = RED;
		} else {
			color = WHITE;
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(worldViewProjectionMatrix, viewportMatrix);
		DrawAABB(aabb1, worldViewProjectionMatrix, viewportMatrix, color);
		DrawAABB(aabb2, worldViewProjectionMatrix, viewportMatrix, color);

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
