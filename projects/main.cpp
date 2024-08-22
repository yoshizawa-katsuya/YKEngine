#include "MyGame.h"
#include "YKFramework.h"


//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	YKFramework* game = new MyGame();

	game->Run();

	delete game;
	

	//出力ウィンドウへの文字出力
	//OutputDebugStringA("Hello,DirectX!\n");

	


#ifdef DEBUG
	//debugController->Relese();
#endif // DEBUG
	
	
	//リソースリークチェック
	//IDXGIDebug1* debug;
	

	return 0;
}