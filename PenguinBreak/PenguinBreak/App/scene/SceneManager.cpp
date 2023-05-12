#include "SceneManager.h"
#include<sstream>
#include<iomanip>
#include "Input.h"
#include"Shape.h"
#include"PostEffect.h"
#include"GameScene.h"
#include"TitleScene.h"
#include"ResultScene.h"
SceneManager::SceneManager()
{}
SceneManager::~SceneManager()
{
	scene_->Finalize();
	delete scene_;
}
void SceneManager::Initialize()
{
	//汎用機能
	//ゲームウィンドウの作成
	Window::Get()->CreateGameWindow();
	// DirectX初期化処理
	DirectXBase::Get()->Initilize();
	//入力の初期化
	Input::Get()->Initialize();
	//シェーダーの読み込み
	ShaderManager::LoadShaders();
	//ライト静的初期化
	LightGroup::StaticInitialize(DirectXBase::Get()->GetDevice());
	//スプライト静的初期化
	Sprite::StaticInit(DirectXBase::Get()->GetDevice(), DirectXBase::Get()->GetCmandList());
	//テキストクラス初期化
	Texture::Get()->Init(DirectXBase::Get()->GetDevice());
	Pipeline::CreatePipeline(DirectXBase::Get()->GetDevice());
	//デバックテキスト初期化
	DebugText::Get()->Initialize();
	//スプライトクラス作成
	Sprite::Get()->Init();
	//FBX初期化
	FbxLoader::GetInstance()->Initialize(DirectXBase::Get()->GetDevice());
	FBXObject3d::SetDevice(DirectXBase::Get()->GetDevice());
	FBXObject3d::SetCmdList(DirectXBase::Get()->GetCmandList());
	FBXObject3d::CreateGraphicsPipeline();
	FBXObject3d::CreateShadowPipeline();
	//図形モデル初期化
	Shape::Init(DirectXBase::Get()->GetDevice());
	//パーティクル初期化
	ParticleManager::StaticInitialize(DirectXBase::Get()->GetDevice(), DirectXBase::Get()->GetCmandList(), window_width, window_height);
	//3Dオブジェクト初期化
	Object::Init(DirectXBase::Get()->GetDevice(), DirectXBase::Get()->GetCmandList());
	PostEffect::Get()->Initialize(DirectXBase::Get()->GetDevice());
	//音作成
	Audio::Get()->Init();
	//シーンをタイトルに設定
	BaseScene* scene = new TitleScene();
	SetNextScene(scene);
}

void SceneManager::Update()
{
	Input::Get()->Update();
	time += 0.01f;
	//次のシーンの予約があるなら
	if (nextScene_)
	{
		if (scene_)
		{
			scene_->Finalize();
			delete scene_;
		}
		//シーン切り替え
		scene_ = nextScene_;
		nextScene_ = nullptr;

		scene_->SetSceneManager(this);
		//次のシーンを初期化する
		scene_->Init();
	}

	//更新
	scene_->Update();
}

void SceneManager::Draw()
{
	PostEffect::Get()->PreDrawScene(DirectXBase::Get()->GetCmandList());
	Object::InitDraw(), Sprite::Get()->PreDraw();
	//カメラ目線の描画
	scene_->Draw();
	DebugText::Get()->DrawAll();

	PostEffect::Get()->PostDrawScene(DirectXBase::Get()->GetCmandList());
	DirectXBase::Get()->PreDraw();
	////ポストエフェクトの描画
	PostEffect::Get()->Draw(DirectXBase::Get()->GetCmandList(), Vec4{ 1.0f,1.0f,1.0f,1.0f }, time);
	//UI描画
	DirectXBase::Get()->ResourceBarrier();
}

void SceneManager::Delete()
{
	Object::Delete();
	FbxLoader::GetInstance()->Finalize();
}