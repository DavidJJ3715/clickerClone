#ifndef _func_
#define _func_
#define SDL_MAIN_HANDLED
#include <iostream>
#include <string>
#include <sstream>
#include <random>
#include <tuple>
#include <memory>
#include <optional>
#include <chrono>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <thread>
#include <functional>
#include <mutex>
#include <limits.h>
#include "../SDL2/include/SDL2/SDL.h"
#include "../SDL2/include/SDL2/SDL_ttf.h"
#include "../SDL2/include/SDL2/SDL_image.h"

/********************************
*       Global Variables        *
*********************************/
const int WIDTH = 1200, HEIGHT = 800, frameDelay = 10;
std::mutex scoreLock;
std::string bundleMap(std::unordered_map<int,int> upgrades); //Function prototype

/********************************
*       Save Functions          *
*********************************/
void saveHighScore(long long score, std::string upgradeString)
{
    std::ofstream scoreFile("save.txt");
    scoreFile << score << "\n";
    scoreFile << upgradeString;
}

std::tuple<long long, std::string> loadHighScore()
{
    long long score = 0;
    std::string upgradeString;
    std::ifstream scoreFile("save.txt");
    scoreFile >> score;
    scoreFile >> upgradeString;
    return std::make_tuple(score, upgradeString);
}

void periodicSave(long long& score, std::unordered_map<int,int> upgrades, bool& running)
{
    while(running)
    {
        {
            std::lock_guard<std::mutex> lock(scoreLock);
            saveHighScore(score, bundleMap(upgrades));
        }
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}

/********************************
*       Core Functionality      *
*********************************/
bool checkClick(int x, int y)
{
    if(((x > (WIDTH/2)-100) and (x < (WIDTH/2)+100)) and ((y < (HEIGHT/2)+100)) and (y > (HEIGHT/2)-100))
        {return true;}
    return false;
}

std::unordered_map<int,int> parseUpgrades(std::string upgradeString)
{
    std::stringstream ss(upgradeString);
    std::string temp;
    std::unordered_map<int,int> map;
    int counter = 1;

    while(std::getline(ss, temp, '+'))
    {
        map.emplace(counter, std::stoi(temp));
        counter++;
    }
    return map;
}

std::string bundleMap(std::unordered_map<int,int> upgrades)
{
    std::string bundle;
    bundle.append(std::to_string(upgrades[1]));
    for(int i=2; i<9; i++)
    {
        bundle.append("+");
        bundle.append(std::to_string(upgrades[i]));
    }
    return bundle;
}

/********************************
*       Draw Functions          *
*********************************/
void drawButton(SDL_Renderer* renderer)
{
    SDL_Rect button = {((WIDTH/2)-100), ((HEIGHT/2)-100), 200, 200};
    // SDL_Rect tester = {1000, 400, 200, 200};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &button);
    // SDL_RenderDrawRect(renderer, &tester);
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

void drawSideBays(SDL_Renderer* renderer, TTF_Font* font, int xPos, int yPos)
{
    bool menu = true;
    int widthVal = -1, heightVal = -1;

    if(xPos > 0 and xPos < WIDTH/3) 
        {widthVal = 0;}
    else if(xPos > WIDTH-(WIDTH/3) and xPos < WIDTH) 
        {widthVal = WIDTH-(WIDTH/3);}
    else
        {menu = false;}
    
    if(menu)
    {
        switch(yPos)
        {
            case 0 ... 99:
                heightVal = 0; break;
            case 100 ... 199:
                heightVal = 100; break;
            case 200 ... 299:
                heightVal = 200; break;
            case 300 ... 399:
                heightVal = 300; break;
            case 400 ... 499:
                heightVal = 400; break;
            case 500 ... 599:
                heightVal = 500; break;
            case 600 ... 699:
                heightVal = 600; break;
            case 700 ... 800:
                heightVal = 700; break;
        }

        SDL_SetRenderDrawColor(renderer, 0,0,0,0);
        SDL_Rect blackBox = {widthVal, 0, WIDTH/3, HEIGHT};
        SDL_RenderFillRect(renderer, &blackBox);

        SDL_SetRenderDrawColor(renderer, 255,255,255,255);
        SDL_Rect box = {widthVal, 0, WIDTH/3, HEIGHT};
        SDL_RenderDrawRect(renderer, &box);
        for(int i=1; i<8; i++)
            {SDL_RenderDrawLine(renderer, widthVal, i*(HEIGHT/8), widthVal+WIDTH/3, i*(HEIGHT/8));}

        SDL_SetRenderDrawColor(renderer,212,175,55,0); //Gold color for border
        for(int i=0; i<5; i++)
        {
            SDL_Rect goldBox = {widthVal+i, heightVal+i, (WIDTH/3)-2*i, 100-2*i};
            SDL_RenderDrawRect(renderer, &goldBox);
        }
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

    int menuWidth = (WIDTH/2), menuHeight = 2*(HEIGHT/3); //Bounds of the black box
    int menuX = (WIDTH - menuWidth)/2, menuY = (HEIGHT - menuHeight)/2; //Starting points of the black box
    int widthVal = (WIDTH/2)-145, heightVal = -1;
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

    SDL_Rect pause = {(WIDTH/2)-200, (HEIGHT/2)-(menuHeight/2)+25, 400, 140}; //Location of "GAME PAUSED"
    SDL_Rect resume = {(WIDTH/2)-100, (HEIGHT/2)-50, 200, 100}; //Location of "RESUME"
    SDL_Rect quit = {(WIDTH/2)-100, (HEIGHT/2)+100, 200, 100}; //Location of "QUIT"
    SDL_RenderCopy(renderer, pauseText, nullptr, &pause); //Render "GAME PAUSED" to the screen
    SDL_RenderCopy(renderer, resumeText, nullptr, &resume); //Render "RESUME" to the screen
    SDL_RenderCopy(renderer, quitText, nullptr, &quit); //Render "QUIT" to the screen

    if(selection) //Change where border is located based on which choice is selected
        {heightVal = (HEIGHT/2)-50;} //Resume
    else
        {heightVal = (HEIGHT/2)+100;} //Quit

    SDL_SetRenderDrawColor(renderer,212,175,55,0); //Gold color for border
    for(int i=0; i<5; i++)
    {
        border = {widthVal+i, heightVal+i, 300-2*i, 100-2*i};
        SDL_RenderDrawRect(renderer, &border); //Draw the golden rectangle
    }

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
                case SDL_MOUSEBUTTONDOWN:
                    {return selection;}
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
                    break;
                }
                case SDL_MOUSEMOTION:
                {
                    int xPos = event.motion.x;
                    int yPos = event.motion.y;
                    if(xPos > (WIDTH/2)-100 and xPos < (WIDTH/2)+100)
                    {
                        if(yPos > (HEIGHT/2)-50 and yPos < (HEIGHT/2)+50)
                            {selection = true;}
                        else if(yPos > (HEIGHT/2)+100 and yPos < (HEIGHT/2)+200)
                            {selection = false;}
                        func(renderer, font, selection);
                    }
                    break;
                }
            }
        }
    }
    return selection;
}

#endif