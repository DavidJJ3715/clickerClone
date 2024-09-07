#include "func.h"
#include "shop.h"

int main()
{
    //! https://integers.info/large-numbers/googol
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    IMG_Init(IMG_INIT_PNG);

    SDL_Window* window = SDL_CreateWindow("Clicker Clone", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font* font = TTF_OpenFont("DejaVuSans.ttf", 75);

    int frameTime, xPos = WIDTH/2, yPos = HEIGHT/2;
    auto[score, upgradeString, zeroCount] = loadHighScore();
    Uint64 frameStart;
    bool running = true;
    std::string label = labelMap[zeroCount];
   
    std::unordered_map<int,int> upgrades = parseUpgrades(upgradeString);
    std::vector<std::shared_ptr<shop>> shopStorage;

    std::thread saveThread(periodicSave, std::ref(score), std::ref(upgrades), std::ref(running), std::ref(zeroCount));
    for(int i=1;i<9;i++)
    {
        std::shared_ptr<shop> temp(new shop(upgrades[i],i));
        shopStorage.push_back(temp);
    }

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
                    int clickVal = checkClick(xPos, yPos);
                    if(clickVal == 0)
                        {score += 1;}
                    break;
                }
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 0,0,0,0);
        SDL_RenderClear(renderer);
        drawButton(renderer);
        drawSideBays(renderer, font, xPos, yPos);
        drawShopLevels(renderer, font, shopStorage, xPos);
        drawScore(renderer, font, score, label);
        SDL_RenderPresent(renderer);
        frameTime = SDL_GetTicks64() - frameStart;
        if(frameDelay > frameTime)
            {SDL_Delay(frameDelay - frameTime);}
        {std::lock_guard<std::mutex> lock(scoreLock);}
    }

    drawSaveScreen(renderer, font);
    if(saveThread.joinable())
        {saveThread.join();}
    saveHighScore(score, bundleMap(upgrades), zeroCount);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    return 0;
}