#include "MyGame.h"
#include "YKFramework.h"

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	std::unique_ptr<YKFramework> game = std::make_unique<MyGame>();

	game->Run();
	
	return 0;
}