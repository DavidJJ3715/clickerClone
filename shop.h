//! A shop object holds the shop's level, cost of next upgrade, png, etc.
#ifndef _shop_
#define _shop_
#include "func.h"

class shop
{
    public:
        shop(int,int);
        void upgrade(int); 
        bigInt seeAhead(int), seeMax(bigInt);
        int seeMaxCount(bigInt);
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
    upgrade(level);
}

void shop::upgrade(int purchased) 
{
    shopLevel+=purchased;
    for(int i=1;i<purchased;++i)
        {cost = (cost*117)/100;}
}

bigInt shop::seeAhead(int amountToSee)
{
    bigInt costAhead = cost;
    for(int i=1; i<amountToSee; ++i)
        {costAhead = (costAhead*117)/100;}
    return costAhead;
}

bigInt shop::seeMax(bigInt score)
{
    bigInt costAhead = cost;
    while(score >= costAhead)
    {
        score -= costAhead;
        costAhead = (costAhead*117)/100;
    }
    return costAhead;
}

int shop::seeMaxCount(bigInt score)
{
    bigInt costAhead = cost;
    int counter = 0;
    while(score >= costAhead)
    {
        score -= costAhead;
        costAhead = (costAhead*117)/100;
        counter += 1;
    }
    return counter;
}

#endif