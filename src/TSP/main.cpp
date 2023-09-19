#include"OFA.h"
#include"GA.h"
#include"RL.h"
#include<getopt.h>
using namespace std;

void OFATest()
{
    ofa::OFA test = ofa::OFA();
    test.printIndivs();
    test.update();
    test.printIndivs();
}

void GATest()
{
    ga::GA ga_ = ga::GA();
    ga_.train();
}
void RLTest()
{

}

static const struct option long_options[] =
{
    {"method", optional_argument, nullptr, 'f'}
};
int main(int argc, char* argv[])
{
    int opt,opt_idx;
    string method;
    while ((opt = getopt_long(argc, argv, "f:", long_options, &opt_idx)) != -1)
    {
        switch (opt)
        {
        case 'f':
            method = optarg;
            break;
        
        default:
            break;
        }
    }
    std::cout<<method<<std::endl;
    if(method=="ofa")
    {
        OFATest();
    }
    else if(method=="ga")
    {
        GATest();
    }
    else if(method=="rl")
    {
        RLTest();
    }
    
}