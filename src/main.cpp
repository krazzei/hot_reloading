// Not sure if I want this here or in the Preprocessor definitions
//#define GLFW_INCLUDE_GLCOREARB
//#define GL_GLEXT_PROTOTYPES
#define WIN32_LEAN_AND_MEAN 1

#include <iostream>
#include <Windows.h>
#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "template.h"

#define Kilobytes(value) value * 1024
#define Megabytes(value) Kilobytes(value) * 1024
#define Gigabytes(value) Megabytes(value) * 1024

FILETIME lastLoadTime;

game_init* funcInitGame;
game_render_and_update* funcRenderAndUpdate;
game_teardown* funcTeardown;

game_memory GameMemory;

game_state GameState;

int RemoveFileFromPath(char *in_str)
{
	int index = -1;
	for (int i = 0; i < MAX_PATH; ++i)
	{
		if (in_str[i] == '\\')
		{
			index = i;
		}

		if (in_str[i] == '\0')
		{
			break;
		}
	}

	in_str[index+1] = '\0';

	return index + 1;
}

void MakePath(char *in_str, char* fileName)
{
	GetModuleFileName(NULL, in_str, sizeof(char) * MAX_PATH);

	int index = RemoveFileFromPath(in_str);

	while (true)
	{
		in_str[index] = *fileName;
		if (*fileName == '\0')
		{
			break;
		}
		fileName++;
		index++;
	}
}

HINSTANCE LoadGameLibrary()
{
	char *libraryPath = new char[MAX_PATH];
	char *libraryCopyPath = new char[MAX_PATH];

	MakePath(libraryPath, "template_lib.dll");
	MakePath(libraryCopyPath, "template_lib_temp.dll");

	CopyFile(libraryPath, libraryCopyPath, false);

	HINSTANCE lib = LoadLibrary(libraryCopyPath);

	delete[] libraryPath;
	delete[] libraryCopyPath;

	if (!lib)
	{
		printf("Can't Load game library");
		funcInitGame = GameInitStub;
		funcRenderAndUpdate = GameRenderAndUpdateStub;
		return NULL;
	}

	funcInitGame = (game_init*)GetProcAddress(lib, "InitGame");
	funcRenderAndUpdate = (game_render_and_update*)GetProcAddress(lib, "RenderAndUpdate");
	funcTeardown = (game_teardown*)GetProcAddress(lib, "Teardown");

	return lib;
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#ifdef DEBUG
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
#endif

	// TODO: Look into WinMain parameters to see if anything will need to be used.
	GLFWwindow* window;

	if (!glfwInit())
	{
		return -1;
	}

	glfwWindowHint(GLFW_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_VERSION_MINOR, 0);
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}

	GameMemory = {};

	GameMemory.MemorySize = Megabytes(100);
	GameMemory.Memory = VirtualAlloc(0, GameMemory.MemorySize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	GameState = {};

	GameState.triangle = (render_stuff*)GameMemory.Memory;

	HINSTANCE GameLib = LoadGameLibrary();

	funcInitGame(&GameMemory, &GameState);
	
	glfwSwapInterval(1);

	int frameCount = 0;

	while (!glfwWindowShouldClose(window))
	{
		if (frameCount++ > 240)
		{
			FreeLibrary(GameLib);
			GameLib = LoadGameLibrary();

			frameCount = 0;
		}

		funcRenderAndUpdate(&GameState);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	funcTeardown(&GameState);
	FreeLibrary(GameLib);
	glfwTerminate();
	return 0;
}
