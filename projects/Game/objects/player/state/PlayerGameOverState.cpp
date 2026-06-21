#include "PlayerGameOverState.h"
#include "PlayerStateContext.h"
#include "manager/AudioManager.h"

void PlayerGameOverState::OnEnter(PlayerStateContext* player)
{
	//ゲームオーバーSEの再生
	AudioManager::GetInstance()->PlaySE(SEType::kDeath02);
	//ゲームオーバーになった瞬間の回転
	player->GameOverRotate();
}

void PlayerGameOverState::OnUpdate(PlayerStateContext* player)
{
	player->UpdateGameOver();
}
