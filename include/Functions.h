#pragma once
#include <ObjList.h>

int init_window(SDL_Window** windowRef, SDL_Renderer** rendererRef) {

	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to initialize SDL: %s", SDL_GetError());
		return FALSE;
	}

	SDL_Window* window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
	);

	if (!window) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create window: %s", SDL_GetError());
		return FALSE;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create renderer: %s", SDL_GetError());
		return FALSE;
	}

	*windowRef = window;
	*rendererRef = renderer;

	return TRUE;
}

ObjNode** setup() {
	ObjNode** table = initHashTable(30);
	insertObject(table, 30, 20, 20, 15, 15, 230, 141, 57, 255, "ball");
	insertObject(table, 30, WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT - 40, 100, 20, 64, 133, 207, 255, "player");
	return table;
}

void processInput(int* isRunning, float* pvx, float* pvy) {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
		case SDL_QUIT:
			*isRunning = FALSE;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) *isRunning = FALSE;
			if (event.key.keysym.sym == SDLK_LEFT) *pvx = -400.0f;
			if (event.key.keysym.sym == SDLK_RIGHT) *pvx = 400.0f;
			break;
		default:
			*pvx = 0.0f;
			*pvy = 0.0f;
			break;
	}
}

void update(ObjNode** table, int* lastFrameTime, float* bvx, float* bvy, float* pvx, float* pvy, int* isRunning) {
	int timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - *lastFrameTime);
	if (timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME) SDL_Delay(timeToWait);
	float deltaTime = (SDL_GetTicks() - *lastFrameTime) / 1000.0f;
	*lastFrameTime = SDL_GetTicks();
	ObjNode* player = getObject(table, 30, "player");
	ObjNode* ball = getObject(table, 30, "ball");

	if (ball->entity->x >= WINDOW_WIDTH - ball->entity->width || ball->entity->x <= 0) *bvx = -*bvx;
	if (ball->entity->y <= 0) *bvy = -*bvy;
	if (ball->entity->y >= WINDOW_HEIGHT - ball->entity->height) {
		SDL_Log("YOU LOST!\n");
		*isRunning = FALSE;
	}
	float offset = 0.0f;
	if (player->entity->x >= WINDOW_WIDTH - player->entity->width) {
		*pvx = 0;
		offset = -1.0f;
	}

	if (player->entity->x <= 0) {
		*pvx = 0;
		offset = 1.0f;
	}

	float ax = ball->entity->x;	
	float bx = ball->entity->x + ball->entity->width;
	float y = ball->entity->y + ball->entity->height;

	if ((y <= player->entity->y + player->entity->height && y >= player->entity->y)
		&& ((ax <= player->entity->x + player->entity->width && ax >= player->entity->x)
		|| (bx <= player->entity->x + player->entity->width && bx >= player->entity->x))) {
		*bvy = -*bvy;
	}
	updateObj(table, 30, *bvx * deltaTime, *bvy * deltaTime, 0, 0, 0, 0, 0, 0, "ball");
	updateObj(table, 30, *pvx * deltaTime + offset, *pvy * deltaTime, 0, 0, 0, 0, 0, 0, "player");
	
}

void render(SDL_Renderer* renderer, ObjNode** table) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	renderTable(table, 30, renderer);
	SDL_RenderPresent(renderer);
}

void destroyWindow(SDL_Window* window, SDL_Renderer* renderer, ObjNode** hashTable) {
	deleteTable(hashTable, 30);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}