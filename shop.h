//! A shop object holds the shop's level, cost of next upgrade, png, etc.
#ifndef _shop_
#define _shop_
#include "func.h"

class shop
{
    public:
        shop(int,int);
        void upgrade();
        int shopLevel;
        bigInt cost;

    private:
        std::string pngPath = "./spritePNGs/shop", fileType = ".png";
};

shop::shop(int level, int pngToChoose)
{
    pngPath.append(std::to_string(pngToChoose));
    pngPath.append(fileType);
    shopLevel = level;
}

void shop::upgrade() {shopLevel+=1;}

#endif