#ifndef _bigInt_
#define _bigInt_
#include "func.h"

class bigInt
{
    public:
        bigInt(std::string);
        void print();

        std::vector<int> number;

};

bigInt::bigInt(std::string inputString)
{
    for(int i=inputString.size()-1;i>=0;i--)
        {number.push_back(inputString[i]-'0');}
}

bigInt::operator+()
{

}

void bigInt::print()
{
    for(int i=number.size()-1;i>=0;i--)
        {std::cout << number[i];}
}

#endif