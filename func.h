#ifndef _func_
#define _func_
#define SDL_MAIN_HANDLED
#include <iostream>
#include <string>
#include <random>
#include <tuple>
#include <memory>
#include <optional>
#include <chrono>
#include <map>
#include <vector>
#include <fstream>
#include <thread>
#include <functional>
#include <mutex>
#include "../SDL2/include/SDL2/SDL.h"
#include "../SDL2/include/SDL2/SDL_ttf.h"
#include "../SDL2/include/SDL2/SDL_image.h"

/********************************
*       Global Variables        *
*********************************/
const int WIDTH = 1200, HEIGHT = 800, frameDelay = 10;
std::mutex scoreLock;

/********************************
*       Save Functions          *
*********************************/
void saveHighScore(long long score)
{
    std::ofstream scoreFile("save.txt");
    scoreFile << score;
}

long long loadHighScore()
{
    long long score = 0;
    std::ifstream scoreFile("save.txt");
    scoreFile >> score;
    return score;
}

void periodicSave(long long& score, bool& running)
{
    while(running)
    {
        {
            std::lock_guard<std::mutex> lock(scoreLock);
            saveHighScore(score);
        }
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}

/********************************
*       Core Functionality      *
*********************************/
bool checkClick(int x, int y)
{
    if(((x > (WIDTH/2)-100) && (x < (WIDTH/2)+100)) && ((y < (HEIGHT/2)+100)) && (y > (HEIGHT/2)-100))
        {return true;}
    return false;
}

/********************************
*       Draw Functions          *
*********************************/
void drawButton(SDL_Renderer* renderer)
{
    SDL_Rect button = {((WIDTH/2)-100), ((HEIGHT/2)-100), 200, 200};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &button);
}

void drawScore(SDL_Renderer* renderer, TTF_Font* font, long long score)
{
    char scoreString[100];
    sprintf(scoreString, "Score: %lld", score);

    SDL_Rect textBox = {(WIDTH/2)-100, 50, 200, 50};
    SDL_Surface* surface = TTF_RenderText_Solid(font, scoreString, {255,255,255,0});
    SDL_Texture* scoreText = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_SetRenderDrawColor(renderer, 0,0,0,0);
    SDL_RenderFillRect(renderer, &textBox);
    SDL_SetRenderDrawColor(renderer, 255,255,255,255);
    SDL_RenderCopy(renderer, scoreText, nullptr, &textBox);
    
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(scoreText);
}

void drawSideBays(SDL_Renderer* renderer, TTF_Font* font)
{
    SDL_Rect achievementBox = {WIDTH/3, 0, WIDTH/3, HEIGHT};
    SDL_Rect upgradeBox = {WIDTH-(WIDTH/3), 0, WIDTH/3, HEIGHT};

    SDL_SetRenderDrawColor(renderer, 255,255,255,255);
    SDL_RenderDrawRect(renderer, &achievementBox);
    SDL_RenderDrawRect(renderer, &upgradeBox);

    for(int i=0; i < 8; i++)
    {
        SDL_Rect subBoxLeft = {0, i * (HEIGHT/8), WIDTH/3, HEIGHT};
        SDL_Rect subBoxRight = {WIDTH-(WIDTH/3), i * (HEIGHT/8), WIDTH/3, HEIGHT};

        SDL_RenderDrawRect(renderer, &subBoxLeft);
        SDL_RenderDrawRect(renderer, &subBoxRight);
    }
}

void drawSaveScreen(SDL_Renderer* renderer, TTF_Font* font)
{
    char message[10];
    sprintf(message, "Saving Please Wait");

    SDL_Rect textRect = {(WIDTH/2)-300,(HEIGHT/2)-150,600,300};
    SDL_Surface* surface = TTF_RenderText_Solid(font, message, {255,255,255,0});
    SDL_Texture* writing = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_SetRenderDrawColor(renderer,0,0,0,0);
    SDL_RenderClear(renderer);
    SDL_RenderFillRect(renderer, &textRect);
    SDL_RenderCopy(renderer, writing, nullptr, &textRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(writing);
    SDL_RenderPresent(renderer);
}

void drawPause(SDL_Renderer* renderer, TTF_Font* font, bool selection)
{
    SDL_SetRenderDrawColor(renderer,255,255,255,0); //Draw white background
    SDL_RenderClear(renderer);

    int menuWidth = 500, menuHeight = 500; //Bounds of the black box
    int menuX = (WIDTH - menuWidth)/2, menuY = (HEIGHT - menuHeight)/2; //Starting points of the black box
    SDL_Rect rect = {menuX, menuY, menuWidth, menuHeight}, border; //Black rectangle rect

    SDL_SetRenderDrawColor(renderer,0,0,0,0); //Choose black color
    SDL_RenderFillRect(renderer, &rect); //Fill in black rectangle

    SDL_Surface* surface = TTF_RenderText_Solid(font, "GAME PAUSED", {255,255,255,0}); //Game pause text
    SDL_Texture* pauseText = SDL_CreateTextureFromSurface(renderer, surface); //Make it a texture
    SDL_FreeSurface(surface); //Free the surface to prevent memory leaks
    surface = TTF_RenderText_Solid(font, "RESUME", {255,255,255,0}); //Resume text
    SDL_Texture* resumeText = SDL_CreateTextureFromSurface(renderer, surface); //Make it a texture
    SDL_FreeSurface(surface); //Free the surface to prevent memory leaks
    surface = TTF_RenderText_Solid(font, "QUIT", {255,255,255,0}); //Quit text
    SDL_Texture* quitText = SDL_CreateTextureFromSurface(renderer, surface); //Make it a texture
    SDL_FreeSurface(surface); //Free the surface to prevent memory leaks

    SDL_Rect pause = {(WIDTH/2)-(menuWidth/5)-50, (HEIGHT/2)-(menuHeight/2)+25, 310, 140}; //Location of "GAME PAUSED"
    SDL_Rect resume = {(WIDTH/2)-(menuWidth/5)+5, (HEIGHT)-(menuHeight)+150, 200, 100}; //Location of "RESUME"
    SDL_Rect quit = {(WIDTH/2)-(menuWidth/5)+5, HEIGHT-menuHeight+300, 200, 100}; //Location of "QUIT"
    SDL_RenderCopy(renderer, pauseText, nullptr, &pause); //Render "GAME PAUSED" to the screen
    SDL_RenderCopy(renderer, resumeText, nullptr, &resume); //Render "RESUME" to the screen
    SDL_RenderCopy(renderer, quitText, nullptr, &quit); //Render "QUIT" to the screen

    if(selection) //Change where border is located based on which choice is selected
        {border = {(WIDTH/2)-145, (HEIGHT/2)-50, 300, 100};} //Resume
    else
        {border = {(WIDTH/2)-145, (HEIGHT/2)+100, 300, 100};} //Quit

    SDL_SetRenderDrawColor(renderer,212,175,55,0); //Gold color for border
    SDL_RenderDrawRect(renderer, &border); //Draw the golden rectangle
    SDL_RenderPresent(renderer); //Draw everything to the screen
    SDL_DestroyTexture(pauseText); //Destroy every texture to prevent memory leaks
    SDL_DestroyTexture(resumeText);
    SDL_DestroyTexture(quitText);
}

/********************************
*       Screen Functions        *
*********************************/
bool selectionMenu(SDL_Renderer* renderer, TTF_Font* font, std::function<void(SDL_Renderer*,TTF_Font*,bool)>func)
{ //Generic two prompt selection menu that takes in a draw function as a parameter
    bool selection = true; //True == "resume" || False == "quit"
    func(renderer, font, selection);
    while(true)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event)) //Typical event handler loop
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    {return selection;}
                case SDL_KEYDOWN:
                {
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            {return true;}
                        case SDLK_RETURN:
                            {return selection;}
                        case SDLK_UP: //User selects "RESUME"
                        {
                            selection = true; 
                            func(renderer, font, selection); //Only call the draw function whenever there is a change to be made to the screen   
                            break;
                        }
                        case SDLK_DOWN: //User selects "QUIT"
                        {
                            selection = false; 
                            func(renderer, font, selection);    
                            break;
                        }
                    }
               }
            }
        }
    }
    return selection;
}

#endif