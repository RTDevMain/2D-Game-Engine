#ifndef GAME_H
#define GAME_H

#include "../ECS/ECS.h"
#include <SDL2/SDL.h>

// Optional fps cap
const int FPS = 60;
const int MILLISECONDS_PER_FRAME = 1000 / FPS;

class Game {
private:
	bool isRunning;
	int msPreviousFrame;
	SDL_Window* window;
	SDL_Renderer* renderer;

	Registry* registry;

public:
	Game();
	~Game();
	void Initialize();
	void Run();
	void Setup();
	void ProcessInput();
	void Update();
	void Render();
	void Destroy();
	int windowWidth;
	int windowHeight;
};

#endif               