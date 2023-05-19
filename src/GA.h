#include<iostream>
#include<vector>
#include<random>
#include<ctime> 	//for seeding the random number generator
#include<algorithm>
using namespace std;

struct Individual
{
    vector<int> gene;

    double fitness; //the fitness of the individual, higher the better. Maximizes the fitness.
    double end;//概率的右边界

    Individual(vector<int>& gene)
    {
        this->gene = gene; //copy the gene.
    }
    Individual(vector<int>&gene, double fitness)
    {
        this->gene = gene; //copy the gene.
        this->fitness = fitness; //copy the fitness.
    }
    Individual(){}
};

struct Point
{
    double x;
    double y;
    Point(double x,double y)
    {
        this->x = x; 	//copy the x value.
        this->y = y; //copy the y value.
    }
};


class GA
{
public:
    int popSize; //the size of the population.
    int MAX_ITERATIONS;
    int POINT_NUM;
    int DEFAULT_MAX_ITERATIONS = 10000; //default maximum number of iterations.
    int DEFALUT_POP_SIZE = 20; //default population size.

    GA();
    GA(vector<Point>&points);
    vector<Individual> indivs;
    vector<Individual> offsprings;
    vector<Point> points;
    void init();
    void fit();
    void crossover(Individual& a, Individual& b); 	//crossover of the population.
    void select();	//selection of the next generation.
    void mutate();	//mutation of the population.
    void update();
    void train();

    void printRes();
    void printIndivs();
    double calcFitness(vector<int>& gene); //calculates the fitness of the individual.
    void sortIndivs();
    bool inVec(int& num,vector<int>&v);
};
