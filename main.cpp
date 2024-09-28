#include "func.h"
#include "shop.h"

int main()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    IMG_Init(IMG_INIT_PNG);

    SDL_Window* window = SDL_CreateWindow("Clicker Clone", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font* font = TTF_OpenFont("DejaVuSans.ttf", 75);
    
    int frameTime, xPos = WIDTH/2, yPos = HEIGHT/2, amountToSee = 1;
    auto[score, upgradeString, scorePerClick, scorePerSecond, timeStamp] = loadSaveFile();
    bigInt elapsedTime = std::chrono::duration_cast<std::chrono::seconds>
        (stringToTs(tsToString()) - stringToTs(timeStamp)).count();
    score += scorePerSecond * elapsedTime;
    Uint64 frameStart;
    bool running = true;
   
    std::unordered_map<int,int> upgrades = parseUpgrades(upgradeString);
    std::vector<std::shared_ptr<shop>> shopStorage;

    std::thread saveThread(autoSave, std::ref(running), std::ref(score), std::ref(upgrades), std::ref(scorePerClick), std::ref(scorePerSecond));
    std::thread scoreThread(autoScore, std::ref(running), std::ref(score), std::ref(scorePerSecond));
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
                    switch(clickVal)
                    {
                        case 0:
                            score += scorePerClick; 
                            break;
                        case 1 ... 9:
                            upgradeShop(shopStorage[clickVal-1], score, amountToSee);
                            upgrades[clickVal] = shopStorage[clickVal-1]->shopLevel;
                            //! Figure out how much scorePerClick to get per upgrade or scorePerSecond
                            break;
                        case 10 ... 13:
                            amountToSee = checkAmountToSee(clickVal);
                            break;
                    }
                    break;
                }
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 0,0,0,0);
        SDL_RenderClear(renderer);
        drawButton(renderer);
        drawSideBays(renderer, font, xPos, yPos);
        drawShopLevels(renderer, font, shopStorage, xPos);
        drawShopCosts(renderer, font, shopStorage, score, xPos, amountToSee);
        drawScore(renderer, font, score);
        SDL_RenderPresent(renderer);
        frameTime = SDL_GetTicks64() - frameStart;
        if(frameDelay > frameTime)
            {SDL_Delay(frameDelay - frameTime);}
        {std::lock_guard<std::mutex> lock(saveLock);}
        {std::lock_guard<std::mutex> lock(scoreLock);}
    }

    drawSaveScreen(renderer, font);
    if(saveThread.joinable() and scoreThread.joinable())
        {saveThread.join(); scoreThread.join();}
    logSaveData(score.str(), bundleMap(upgrades), scorePerClick.str(), scorePerSecond.str(), tsToString());

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    return 0;
}