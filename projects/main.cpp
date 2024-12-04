#include "MyGame.h"
#include "YKFramework.h"
#include "D3DResourceLeakChecker.h"

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	static D3DResourceLeakChecker leakCheck_;

	std::unique_ptr<YKFramework> game = std::make_unique<MyGame>();

	game->Run();
	
	return 0;
}