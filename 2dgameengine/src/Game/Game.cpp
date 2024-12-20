#include "Game.h"
#include "../Logger/Logger.h"
#include "../ECS/ECS.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>


Game::Game() {
	isRunning = false;
	Logger::Log("Game constructor called.");
}

Game::~Game() {
	Logger::Log("Game destructor called.");
}

void Game::Initialize() {
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		Logger::Err("Error initializing SDL");
		return;
	}
	// Window params
	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);
	windowWidth = displayMode.w;
	windowHeight = displayMode.h;
	window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		windowWidth,
		windowHeight,
		SDL_WINDOW_BORDERLESS
	);
	if (!window) {
		Logger::Err("Error creating SDL Window");
		return;
	}

	// Renderer params
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
	 SDL_RENDERER_PRESENTVSYNC);

	if (!renderer){
		Logger::Err("Error creating SDL renderer");
		return;
	}

	//Real Full Screen
	//SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	isRunning = true;
}

void Game::ProcessInput() {
	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent)){
		switch (sdlEvent.type) {
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_KEYDOWN:
			if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
				isRunning = false;
			}
			break;
		}
	}
}



void Game::Setup(){
	//TODO:
	//Entity tank = registry.CreateEntity();
	// tank.AddComponent<TransformComponent>();
	// ...
}


void Game::Update() {
	// If too fast, stall until desired frame time
	int timeToWait = MILLISECONDS_PER_FRAME - (SDL_GetTicks()- msPreviousFrame); 
	if (timeToWait > 0 && timeToWait <= MILLISECONDS_PER_FRAME){
		SDL_Delay(timeToWait);
	}
	
	// The difference in ticks since the last frame, converted into seconds
	double deltaTime = (SDL_GetTicks() - msPreviousFrame) / 1000.0;

	msPreviousFrame = SDL_GetTicks();

	// TODO:
	// MovementSystem.Update();
	//...

}

void Game::Render() {
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);

	// TODO: Render game objects
	
	SDL_RenderPresent(renderer);
}

void Game::Run() {
	Setup();
	while(isRunning) {
		ProcessInput();
		Update();
		Render();
	}
}

void Game::Destroy() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}