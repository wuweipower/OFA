// #include"OFA.h"
#include"GA.cpp"
using namespace std;

// void OFATest()
// {
//     OFA test = OFA();
//     test.printIndivs();
//     test.update();
//     test.printIndivs();
// }

void GATest()
{
    GA ga = GA();
    ga.train();
}

int main()
{
    GATest();
}