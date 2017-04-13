#pragma once

// Now the platform layer has this included... not sure how I feel about that.
#include <glm\glm.hpp>

struct game_memory
{
	unsigned long long MemorySize;
	void *Memory;
};

struct render_stuff
{
	GLuint program;
	GLuint attribPosition;
	GLuint vertexArray;
	GLuint transformPosition;
	glm::mat4 transform;
};

struct game_state
{
	render_stuff *triangle;
};

#define GAME_RENDER_AND_UPDATE(name) void name(game_state *State)
typedef GAME_RENDER_AND_UPDATE(game_render_and_update);
GAME_RENDER_AND_UPDATE(GameRenderAndUpdateStub)
{
}

#define GAME_INIT(name) bool name(game_memory *GameMemory, game_state *State)
typedef GAME_INIT(game_init);
GAME_INIT(GameInitStub)
{
	return false;
}

#define GAME_TEARDOWN(name) void name(game_state *State)
typedef GAME_TEARDOWN(game_teardown);
GAME_TEARDOWN(GameTeardownStub)
{
}
