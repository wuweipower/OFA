#include<vector>
#include<iostream>
#include<algorithm>
#include<random>
#include<time.h>
using std::vector;

struct Individual
{
    vector<int> sequence;
    double val;
    vector<int> r1;
    vector<int> r2;
    vector<int> beta;
};

struct Point
{
    double x;
    double y;
};

class OFA
{
    private:
    int N;//the size of fauna
    int POINT_NUM;//the number of points, thus the sequence length
    static const int DEFAULT_N=20;
    static const int DEFAULT_POINT_NUM= 20;
    double res=2147483647;//INT_MAX
    int t=1;//init times
    int Max_t = 20000;//max iterations
    vector<Point> points;
    vector<Individual> indivs;

    static int null;//用来标识null 在theta操作中

    public:
    OFA():N(DEFAULT_N),POINT_NUM(DEFAULT_POINT_NUM)
    {
        this->points = vector<Point>(POINT_NUM);
        this->indivs = vector<Individual>(N);
        int seed = (unsigned int)(time(0));
        srand(seed);

        inputPoints(this->points);
        initIndivs(this->indivs);    
    }
    OFA(int N,int Xlen);

    void inputPoints(vector<Point>& points);

    //init indivs
    void initIndivs(vector<Individual>& indivs);


    //operators
    vector<int> minus(const vector<int>& binSeries, const vector<int>& delta);
    vector<int> theta(const vector<int>& s1,const vector<int>& s2);
    vector<int> crossProduct(const int& k,const vector<int>& r);
    vector<int> product(const vector<int>& binSeries, const vector<int>& delta);
    vector<int> add(const vector<int>& binSeries, const vector<int>& delta);


    //generate uniformly distributed random group
    //void generateUDRG();

    //compute F val;
    double computeFucVal(vector<int>& sequence);

    //order function value form best to worse
    static bool cmp(const Individual& a, const Individual& b);

    //sort indivs according to the val
    void sortIndivs(vector<Individual>& indivs);

    //produce new foraging position
    //compute new F for each individual
    //move
    void update();

    //for showing the result of the algorithm
    void printIndivs();

    double getK();

    bool isBetter(double lambda, double new_val, double old_val);

    vector<int> calNewSeq(const Individual& indiv_a,const Individual& indiv_b, bool flag);//flag is true when it is the min
};