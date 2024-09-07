//! A shop object holds the shop's level, cost of next upgrade, png, etc.
#ifndef _shop_
#define _shop_

class shop
{
    public:
        shop(int,int);
        int shopLevel;

    private:
        std::string pngPath = "./spritePNGs/shop", fileType = ".png";
};

shop::shop(int level, int pngToChoose)
{
    pngPath.append(std::to_string(pngToChoose));
    pngPath.append(fileType);
    shopLevel = level;
}

#endif