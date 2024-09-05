#include "func.h"

int main()
{
    int frameTime, xPos = WIDTH/2, yPos = HEIGHT/2;
    std::tuple<long long, std::string> loadTuple = loadHighScore();
    long long score = std::get<0>(loadTuple);
    std::string upgradeString = std::get<1>(loadTuple);
    Uint64 frameStart;
    bool running = true;
    std::unordered_map<int,int> upgrades = parseUpgrades(upgradeString);
    for (int i=1; i<9; i++) 
        {std::cout << i << ": " << upgrades[i] << "\n";}

    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    IMG_Init(IMG_INIT_PNG);

    SDL_Window* window = SDL_CreateWindow("Clicker Clone", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font* font = TTF_OpenFont("DejaVuSans.ttf", 75);

    std::thread saveThread(periodicSave, std::ref(score), std::ref(upgrades), std::ref(running));

    while(running)
    {
        
        frameStart = SDL_GetTicks64();
        
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            xPos = event.motion.x;
            yPos = event.motion.y;
            switch(event.type)
            {
                case SDL_QUIT:
                    {running = false; break;}
                case SDL_KEYDOWN:
                {
                    if(event.key.keysym.sym == SDLK_ESCAPE)
                        {running = selectionMenu(renderer, font, drawPause);}
                    break;
                }
                case SDL_MOUSEBUTTONDOWN:
                {
                    if(checkClick(xPos, yPos))
                        {score += 1;}
                    break;
                }
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 0,0,0,0);
        SDL_RenderClear(renderer);
        drawButton(renderer);
        drawSideBays(renderer, font, xPos, yPos);
        drawScore(renderer, font, score);
        SDL_RenderPresent(renderer);
        frameTime = SDL_GetTicks64() - frameStart;
        if(frameDelay > frameTime)
            {SDL_Delay(frameDelay - frameTime);}
        {std::lock_guard<std::mutex> lock(scoreLock);}
    }

    drawSaveScreen(renderer, font);
    if(saveThread.joinable())
        {saveThread.join();}
    saveHighScore(score, bundleMap(upgrades));

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    return 0;
}