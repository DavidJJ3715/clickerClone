//! A shop object holds the shop's level, cost of next upgrade, png, etc.
#ifndef _shop_
#define _shop_
#include "func.h"

class shop
{
    public:
        shop(int,int);
        void upgrade(int); 
        bigInt costOfNext(bigInt), costOfAmount(bigInt,int,int);
        int seeLimit(bigInt,bigInt,int);
        int shopLevel=0;
        bigInt cost;

    private:
        std::string pngPath = "./spritePNGs/shop";
        std::vector<bigInt> costs = {0,35,150,2700,89000,365000,13540685,842321945,42690420690};
};

shop::shop(int level, int shopNumber)
{
    pngPath.append(std::to_string(shopNumber)).append(".png");
    shopLevel = level;
    cost = costOfAmount(costs[shopNumber],0,shopLevel);
}

bigInt shop::costOfNext(bigInt initCost)
    {return ((initCost*117+99)/100);}

bigInt shop::costOfAmount(bigInt initCost, int counter, int limit)
{
    if(counter == limit)
        {return initCost;}
    return costOfAmount(costOfNext(initCost),counter+1,limit);
}

int shop::seeLimit(bigInt playerScore, bigInt initCost, int amount)
{
    if(playerScore < initCost)
        {return amount;}
    return seeLimit(playerScore-initCost,costOfNext(initCost),amount+1);
}

void shop::upgrade(int purchased) 
{
    shopLevel+=purchased;
    cost = costOfAmount(cost,0,purchased);
}

#endif