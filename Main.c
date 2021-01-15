#include<Functions.h>

void Test() {
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	
	int isRunning = init_window(&window, &renderer);
	
	if (!isRunning) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create or initialize SDL: %s", SDL_GetError());
		SDL_Quit();
		return;
	}
	float bvx = 200.0f, bvy = 200.0f, pvx = 0.0f, pvy = 0.0f;
	ObjNode** table = setup();
	int lastFrameTime = SDL_GetTicks();
	while (isRunning) {
		processInput(&isRunning, &pvx, &pvy);
		update(table, &lastFrameTime, &bvx, &bvy, &pvx, &pvy, &isRunning);
		render(renderer, table);
	}
	destroyWindow(window, renderer, table);
}

int main(int argc, char* args[]) {
	Test();
	return 0;
}