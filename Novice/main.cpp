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

	Segment segment{
	    {-2.0f, -1.0f, 0.0f},
        {3.0f,  2.0f,  2.0f}
    };
	Vector3 point{-1.5f, 0.6f, 0.6f};
	
	Vector3 project = Project(Subtract(point, segment.origin), segment.diff);

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
		
		Vector3 closestPoint = ClosestPoint(point, segment);

		Sphere pointSphere{point, 0.01f};
		Sphere closestPointSphere{closestPoint, 0.01f};

		Matrix4x4 worldMatrix = MakeAfineMatrix(traiangleScale, traiangleRotate, traiangleTranslate);
		Matrix4x4 cametaMatrix = MakeAfineMatrix(cameraScale, cameraRotate, cameraPosition);
		Matrix4x4 viewMatrix = Inverse(cametaMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		Vector3 start = Transform(Transform(segment.origin, worldViewProjectionMatrix), viewportMatrix);
		Vector3 end = Transform(Transform(Add(segment.origin,segment.diff), worldViewProjectionMatrix), viewportMatrix);

		ImGui::Begin("Window");
		ImGui::DragFloat3("point", &point.x, 0.01f);
		ImGui::DragFloat3("segment_origin", &segment.origin.x, 0.01f);
		ImGui::DragFloat3("segment_diff", &segment.diff.x, 0.01f);
		ImGui::DragFloat3("project", &project.x, 0.01f);
		ImGui::End();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(worldViewProjectionMatrix, viewportMatrix);
		Novice::DrawLine(int(start.x), (int)start.y, int(end.x), (int)end.y, WHITE);
		DrawSphere(pointSphere, worldViewProjectionMatrix, viewportMatrix, RED);
		DrawSphere(closestPointSphere, worldViewProjectionMatrix, viewportMatrix, BLACK);

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
