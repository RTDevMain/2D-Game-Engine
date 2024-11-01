#include "Game.h"
#include "glm/ext/vector_float2.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <glm/glm.hpp>
#include <iostream>

Game::Game() {
	isRunning = false;
	std::cout << "Game constructor called." << std::endl;
}

Game::~Game() {
	std::cout << "Game destructor called." << std::endl;
}

void Game::Initialize() {
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cerr << "Error initializing SDL" << std::endl;
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
		std::cerr << "Error creating SDL window" << std::endl;
		return;
	}

	// Renderer params
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
	 SDL_RENDERER_PRESENTVSYNC);

	if (!renderer){
		std::cerr << "Error creating SDL renderer" << std::endl;
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

glm::vec2 playerPosition;
glm::vec2 playerVelocity;


void Game::Setup(){
	playerPosition = glm::vec2(10.0, 20.0);
	playerVelocity = glm::vec2(100.0, 0.0);
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

	playerPosition.x += playerVelocity.x * deltaTime;
	playerPosition.y += playerVelocity.y * deltaTime;
}

void Game::Render() {
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);

	SDL_Surface* surface = IMG_Load("./assets/images/tank-tiger-right.png");
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect dstRect = {
		static_cast<int>(playerPosition.x),
		static_cast<int>(playerPosition.y),
		32,
	    32
	};

	SDL_RenderCopy(renderer, texture, NULL, &dstRect);
	SDL_DestroyTexture(texture);
	
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