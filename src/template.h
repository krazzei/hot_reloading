#pragma once

#define GAME_RENDER_AND_UPDATE(name) void name()
typedef GAME_RENDER_AND_UPDATE(game_render_and_update);
GAME_RENDER_AND_UPDATE(GameRenderAndUpdateStub)
{
}

#define GAME_INIT(name) bool name()
typedef GAME_INIT(game_init);
GAME_INIT(GameInitStub)
{
	return false;
}
