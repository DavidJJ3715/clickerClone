#ifndef _func_
#define _func_
#define SDL_MAIN_HANDLED
#include <algorithm>
#include <chrono>
#include <climits>
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <optional>
#include <random>
#include <sstream>
#include <string>
#include <thread>
#include <tuple>
#include <unordered_map>
#include <vector>
#include "../boost_1_86_0/boost/multiprecision/cpp_int.hpp"
#include "../SDL2/include/SDL2/SDL.h"
#include "../SDL2/include/SDL2/SDL_ttf.h"
#include "../SDL2/include/SDL2/SDL_image.h"

//! https://integers.info/large-numbers/googol

/********************************
*       Function Prototypes     *
*********************************/
std::string bundleMap(std::unordered_map<int,int>);

/********************************
*       Global Variables        *
*********************************/
using bigInt = boost::multiprecision::cpp_int;
const int WIDTH = 1200, HEIGHT = 800, frameDelay = 10;
std::mutex scoreLock;

/********************************
*       Save Functions          *
*********************************/
void logSaveData(std::string scoreString, std::string upgradeString)
{
    std::ofstream scoreFile("save.txt");
    scoreFile << scoreString << "\n";
    scoreFile << upgradeString;
}

std::tuple<bigInt,std::string> loadSaveFile()
{
    std::string scoreString;
    std::string upgradeString;
    std::ifstream scoreFile("save.txt");
    scoreFile >> scoreString;
    scoreFile >> upgradeString;
    return std::make_tuple(bigInt(scoreString),upgradeString);
}

void autoSave(bool& running, bigInt& score, std::unordered_map<int,int>& upgrades)
{
    while(running)
    {
        {
            std::lock_guard<std::mutex> lock(scoreLock);
            logSaveData(score.str(),bundleMap(upgrades));
        }
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}

/********************************
*       Core Functionality      *
*********************************/
int checkClick(int x, int y)
{
    int retVal = -1;
    if(((x > (WIDTH/2)-100) and (x < (WIDTH/2)+100)) and ((y < (HEIGHT/2)+100)) and (y > (HEIGHT/2)-100))
        {return 0;}
    else if(x > WIDTH-(WIDTH/3))
    {
        switch(y)
        {
            case 0 ... 99:
                retVal = 1; break;
            case 100 ... 199:
                retVal = 2; break;
            case 200 ... 299:
                retVal = 3; break;
            case 300 ... 399:
                retVal = 4; break;
            case 400 ... 499:
                retVal = 5; break;
            case 500 ... 599:
                retVal = 6; break;
            case 600 ... 699:
                retVal = 7; break;
            case 700 ... 800:
                retVal = 8; break;
        }
    }
    return retVal; 
}

template<typename shopType>
void upgradeShop(std::shared_ptr<shopType>& shop, bigInt& score)
{
    if(score >= shop->cost)
    {
        score = score-shop->cost;
        shop->upgrade();
    }
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

std::string giveLabel(bigInt score)
{
    switch(score.str().size()-1)
    {
        case 3  ...  5: return "Thousand";
        case 6  ...  8: return "Million";
        case 9  ... 11: return "Billion";
        case 12 ... 14: return "Trillion";
        case 15 ... 17: return "Quadrillion";
        case 18 ... 20: return "Quintillion";
        case 21 ... 23: return "Sextillion";
        case 24 ... 26: return "Septillion";
        case 27 ... 29: return "Octillion";
        case 30 ... 32: return "Nonillion";
        case 33 ... 35: return "Decillion";
        case 36 ... 38: return "Undecillion";
        case 39 ... 41: return "Duodecillion";
        case 42 ... 44: return "Tredecillion";
        case 45 ... 47: return "Quattuordecillion";
        case 48 ... 50: return "Quindecillion";
        case 51 ... 53: return "Sexdecillion";
        case 54 ... 56: return "Septendecillion";
        case 57 ... 59: return "Octodecillion";
        case 60 ... 62: return "Novemdecillion";
        case 63 ... 65: return "Vigintillion";
        case 66 ... 68: return "Unvigintillion";
        case 69 ... 71: return "Duovigintillion";
        case 72 ... 74: return "Trevigintillion";
        case 75 ... 77: return "Quattuorvigintillion";
        case 78 ... 80: return "Quinvigintillion";
        case 81 ... 83: return "Sexvigintillion";
        case 84 ... 86: return "Septenvigintillion";
        case 87 ... 89: return "Octovigintillion";
        case 90 ... 92: return "Novemvigintillion";
        case 93 ... 95: return "Trigintillion";
        case 96 ... 98: return "Untrigintillion";
    }
    return "";
}

/********************************
*       Draw Functions          *
*********************************/
void drawButton(SDL_Renderer* renderer)
{
    SDL_Rect button = {((WIDTH/2)-100), ((HEIGHT/2)-100), 200, 200};
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    SDL_RenderFillRect(renderer, &button);
}

void drawScore(SDL_Renderer* renderer, TTF_Font* font, bigInt score)
{
    std::string scoreString = "";
    SDL_Rect textBox = {(WIDTH/2)-100, 75, 200, 100};
    SDL_Rect labelBox = {(WIDTH/2)-100, 165, 200, 100};

    if(score < 1000)
        {scoreString = score.str();}
    else
    {
        switch(score.str().size()%3)
        {
            case 0:
                scoreString.append(score.str().substr(0,3)).append(".").append(score.str().substr(3,3));
                break;
            case 1:
                scoreString.append(score.str().substr(0,1)).append(".").append(score.str().substr(1,3));
                break;
            case 2:
                scoreString.append(score.str().substr(0,2)).append(".").append(score.str().substr(2,3));
                break;
        }
        
    }

    SDL_Surface* surface = TTF_RenderText_Solid(font, scoreString.c_str(), {255,255,255,0});
    SDL_Texture* scoreText = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Surface* surface2 = TTF_RenderText_Solid(font, giveLabel(score).c_str(), {255,255,255,0});
    SDL_Texture* labelText = SDL_CreateTextureFromSurface(renderer, surface2);

    SDL_SetRenderDrawColor(renderer, 0,0,0,0);
    SDL_RenderFillRect(renderer, &textBox);
    SDL_SetRenderDrawColor(renderer, 255,255,255,255);
    SDL_RenderCopy(renderer, scoreText, nullptr, &textBox);
    SDL_RenderCopy(renderer, labelText, nullptr, &labelBox);
    
    SDL_FreeSurface(surface);
    SDL_FreeSurface(surface2);
    SDL_DestroyTexture(scoreText);
    SDL_DestroyTexture(labelText);
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

template<typename shopType>
void drawShopLevels(SDL_Renderer* renderer, TTF_Font* font, std::vector<std::shared_ptr<shopType>>& shopList, int xPos)
{
    char levelString[128];
    if(xPos > WIDTH-(WIDTH/3) and xPos < WIDTH)
    {
        for(uint64_t i=0; i<shopList.size(); i++)
        {
            levelString[0] = '\0';
            sprintf(levelString, "%d", shopList[i]->shopLevel);
            
            SDL_Rect box = {WIDTH-40,int(i)*(HEIGHT/8),35,35};
            SDL_Surface* surface = TTF_RenderText_Solid(font, levelString, {255,255,255,0});
            SDL_Texture* writing = SDL_CreateTextureFromSurface(renderer, surface);

            SDL_SetRenderDrawColor(renderer,0,0,0,0);
            SDL_RenderCopy(renderer, writing, nullptr, &box);

            SDL_FreeSurface(surface);
            SDL_DestroyTexture(writing);
        }
    }
}

template<typename shopType>
void drawShopCosts(SDL_Renderer* renderer, TTF_Font* font, std::vector<std::shared_ptr<shopType>>& shopList, int xPos)
{
    if(xPos > WIDTH-(WIDTH/3) and xPos < WIDTH)
    {

    }
}

void drawSaveScreen(SDL_Renderer* renderer, TTF_Font* font)
{
    char message[16];
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