#include"GA.h"

GA::GA()
{
    this->popSize = DEFALUT_POP_SIZE;
    this->MAX_ITERATIONS = DEFAULT_MAX_ITERATIONS;
    srand(unsigned(time(0)));
    init();   
}

GA::GA(vector<Point>&points)
{
    this->popSize = DEFALUT_POP_SIZE;
    this->MAX_ITERATIONS = DEFAULT_MAX_ITERATIONS;
    srand(unsigned(time(0)));
    this->points = points; 	//set of points for the algorithm to work with.
    init();
}

void swap(int&a,int&b)
{
    int temp = a;
    a = b;
    b = temp; 		//swap a and b's values.
}
void GA::init()
{
    //先随机生成point
    for(int i=0;i<20;i++)
    {
        double x = rand()%100/10.0;//0.0到10.0的随机数,生成20个数字的x和y值。
        double y = rand()%100/10.0;//0.0到10.0的随机数,生成20个数字的y值。
        points.push_back(Point(x,y));
    }
    POINT_NUM = 	points.size();//记录点数量。

    //init pop
    
    vector<int> initGene;
    for(int i=0;i<POINT_NUM;i++)
    {
        initGene.push_back(i);
    }
    for(int i=0;i<popSize;i++)
    {      
        indivs.push_back(Individual(initGene));
    }
    for(int i=0;i<popSize;i++)
    {
        //shuffle
        for(int j=0;j<POINT_NUM;j++)
        {
            swap(indivs[i].gene[j] ,  indivs[i].gene[(j+std::abs(rand()))%POINT_NUM] );
        }
    }
    fit();
    printRes(); 
}

void GA::fit()
{
    double sum=0;
    for(int i=0;i<popSize;i++)
    {
        indivs[i].fitness = calcFitness(indivs[i].gene);//calc fitness of each individual.
        sum+=indivs[i].fitness;
    }

    double cnt = 0;
    for(int i=0;i<popSize;i++)
    {
        indivs[i].end = (indivs[i].fitness+cnt)/sum;
        cnt+=indivs[i].fitness;
    }
  
}

void GA::crossover(Individual& a, Individual& b)
{
    /**
     * 选择两个父代个体作为杂交池。
     * 随机生成两个点，定义作为交叉点。
     * 按顺序将第一个父代中的交叉点之间的城市顺序复制到子代的相应位置。
     * 然后按顺序，从交叉点后的第一个城市开始，依次将第二个父代中没有出现在前面复制过的城市插入子代中。
     * 重复上述步骤，这次反转两个父代中的角色。
     * 如果产生的子代不满足某些约束条件，则需要通过局部优化算法进行修正。
    */
  
    int csa = rand()%POINT_NUM;// cross point of a
    int csb = rand()%POINT_NUM;// cross point of b.

    vector<int> childA;
    vector<int> childB;
    for(int i=0;i<csa;i++)
    {
        childA.push_back(a.gene[i]);
    }
    for(int i=0;i<POINT_NUM;i++)
    {
        if(!inVec(b.gene[i],childA))
        {
            childA.push_back(b.gene[i]);
        }
    }
    for(int j=0;j<csb;j++)
    {
        childB.push_back(b.gene[j]);
    }
    for(int i=0;i<POINT_NUM;i++)
    {
        if(!inVec(a.gene[i],childB))
        {
            childB.push_back(a.gene[i]);
        }
    }
    offsprings.push_back(Individual(childA));
    offsprings.push_back(Individual(childB));

}
bool GA::inVec(int& num,vector<int>&v)
{
    for(int i=0;i<v.size();i++)
    {
        if(num==v[i])
        return true;
    }
    return false;
}
void GA::select()
{
    for(int i=0;i<popSize/2;i++)
    {  
        double random = std::abs(rand()%1000)/1000.0;
        int father = 0;
        for(int j=0;j<popSize;j++)
        {
            if(random<indivs[j].end)
            {
                father=j;
                break;
            }
        }

        random = std::abs(rand()%1000)/1000.0;
        int mother = 0;
        for(int j=0;j<popSize;j++)
        {
            if(random<indivs[j].end)
            {
                mother=j;
                break;
            }
        }

        if(father!=mother)
        {
            crossover(indivs[father],indivs[mother]);
        }
        else
        i--;
              
    }
}

void GA::mutate()
{
    for(int i = 0;i<offsprings.size();i++)
    {
        int random = rand()%1000;
        if(random<100)
        {         
            int preIndex = rand()%POINT_NUM;
            int postIndex = rand()%POINT_NUM;
            swap(offsprings[i].gene[preIndex],offsprings[i].gene[postIndex]);
        }
    }

    indivs = offsprings;
    offsprings.clear();
}

void GA::update()
{
    fit();
    select();
    mutate();   
}

void GA::train()
{
    int t = 0;
    while(t<MAX_ITERATIONS)
    {
        update();
        ++t;
        if(t%100==0) cout << "Iteration: " << t << endl;
    }
    fit();
    printRes();
}

void GA::printRes()
{
    sortIndivs();  
    printIndivs();
}
void GA::sortIndivs()
{
    auto cmp = [](const Individual& a, const Individual& b)
    {
        return a.fitness > b.fitness;
    };
    try
    {
        std::sort(indivs.begin(),indivs.end(),cmp);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    
}

void GA::printIndivs()
{
    using std::cout;
    for(int i =0;i<popSize;i++)
    {
        cout<<"gene: ";
        for(int j =0;j<POINT_NUM;j++)
        {
            cout<<this->indivs[i].gene[j]<<" ";
        }
        cout<<"val: "<<this->indivs[i].fitness<<"\n";
    }
}

double GA::calcFitness(vector<int>& gene)
{
    double sum=0;
    for(int i=0;i<POINT_NUM-1;i++)
    {
        int preIndex = gene[i];
        int postIndex = gene[i+1];
        sum += std::abs(this->points[preIndex].x-this->points[postIndex].x)+
               std::abs(this->points[preIndex].y-this->points[postIndex].y);
    }
    //使用倒数完美解决问题
    double res;
    try
    {
        res = 1/sum;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return res;
}