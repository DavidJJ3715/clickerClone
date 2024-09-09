//! A shop object holds the shop's level, cost of next upgrade, png, etc.
#ifndef _shop_
#define _shop_
#include "func.h"

class shop
{
    public:
        shop(int,int);
        void upgrade(); 
        bigInt seeAhead(int);
        std::tuple<int,bigInt> seeMax(bigInt);
        int shopLevel=0;
        bigInt cost;

    private:
        std::string pngPath = "./spritePNGs/shop";
        std::vector<bigInt> costs = {0,35,150,2700,89000,365000,13540685,842321945,42690420690};
};

shop::shop(int level, int shopNumber)
{
    pngPath.append(std::to_string(shopNumber)).append(".png");
    cost = costs[shopNumber];
    for(int i=0; i<level; ++i) {upgrade();}
}

void shop::upgrade() 
{
    shopLevel+=1;
    cost = (cost*117)/100;
}

bigInt shop::seeAhead(int amountToSee)
{
    bigInt costAhead = cost;
    for(int i=0; i<amountToSee; ++i)
        {costAhead = (costAhead*117)/100;}
    return costAhead;
}

std::tuple<int,bigInt> shop::seeMax(bigInt score)
{
    int counter = 0;
    bigInt costAhead = cost;
    while(score >= costAhead)
    {
        score -= costAhead;
        costAhead = (costAhead*117)/100;
        counter+=1;
    }
    return std::make_tuple(counter,costAhead);
}

#endif