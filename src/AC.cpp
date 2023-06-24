/**
 * 1. 信息素浓度的大小表征路径的远近，信息素浓度越高，表示对应的路径距离越短
 * 2. 蚂蚁会大概率优先选择信息素浓度较高的路径，并释放一定量的信息素，形成正反馈
 * 3. 路径上的信息素浓度会随着时间的推进而逐渐衰退
*/

/**
 * 优化思路：
 * 用蚂蚁行走的路径表示待优化问题的可行解，整个蚂蚁群体的所有路径构成待优化问题的解空间。
*/

/**
    初始化蚂蚁群和城市信息：创建一个蚂蚁群，每只蚂蚁都代表TSP问题的一个可能的路径。初始化城市坐标信息，通常是随机生成。
    设定算法参数：包括蚂蚁数量、迭代次数、信息素重要程度、启发式信息重要程度等。
    更新信息素：根据蚂蚁在路径上的选择情况，更新路径上的信息素浓度。一般会考虑信息素的挥发和增加。挥发可以模拟信息素的逐渐消散，增加则使经过良好路径的信息素浓度增加。
    蚂蚁的路径选择：每只蚂蚁根据当前位置和信息素浓度，使用启发式规则（比如轮盘赌法）选择下一个城市。
    更新路径长度：更新蚂蚁的路径长度，并记录最短路径。
    全局信息素更新：当所有蚂蚁完成路径选择后，通过一定方式更新所有路径上的信息素浓度。
    重复执行3-6步直到达到迭代次数的限制。
    输出结果：输出找到的最短路径及其长度。
*/
#include<vector>
#include<iostream>
#include<cmath>
#include<random>
#include<algorithm>
#include<ctime>
using namespace std;

struct City
{
    double x;
    double y;
    City(double _x, double _y)
    {
        x = _x; y = _y;
    }
};
double distance(const City& c1, const City& c2)
{
    return sqrt(pow(c1.x - c2.x, 2) + pow(c1.y - c2.y, 2));
}
class Ant;
vector<City> cities;
vector<Ant> ants;
vector<vector<double>> pheromones;//信息素矩阵
const int ant_num = 20;
const int initialPheromoneValue = 1;
const double evaporationRate = 0.1; // 信息素挥发率
const int city_num = 20;
const double Alpha = 2; //计算概率中的信息素的重要程度因子
const double Beta = 2;  //计算概率中的启发函数的重要程度因子
const double evapRate = 0.1;
const int ITER_MAX = 200;
vector<int> tour_best;
double  len_best = 0xffffff;
class Ant
{
public:
    vector<int> tour;
    vector<bool> visited;
    double tour_len;
    int city_num;
    
public:
    Ant(int city_num)
    {
        this->city_num = city_num;
        this->tour_len = 0.0;
        visited.resize(city_num,false);
        tour.reserve(city_num);//预分配内存
    }
    void visit_city(int city_index)
    {
        visited[city_index] = true;
        tour.push_back(city_index);
    }
    double cal_total_distance()
    {
        tour_len = 0.0;
        for(int i=0;i<city_num-1;i++)
        {
            int idx_1 = tour[i];
            int idx_2 = tour[i+1];
            double dist = distance(cities[idx_1],cities[idx_2]);
            tour_len += dist;
        }

        //回到出发城市也要一段距离
        int idx_1 = tour.back();
        int idx_2 = tour[0];
        double dist = distance(cities[idx_1],cities[idx_2]);
        tour_len += dist;

        return tour_len;
    }
    void clear_visited_cities()
    {
        fill(visited.begin(),visited.end(),false);
        tour.clear();
    }
    bool is_city_visited(int city_index)const
    {
        return visited[city_index];
    }
    
};

// initialize the cities
void init_cities()
{
    for(int i=0;i<city_num;i++)
    {
        double x = rand()%100/10.0;//0.0到10.0的随机数,生成20个数字的x
        double y = rand()%100/10.0;//0.0到10.0的随机数,生成20个数字的y值。
        cities.push_back(City(x,y));
    }
}

void initialize_ants_pheromones()
{
    int city_num = cities.size();

    ants.resize(ant_num,Ant(city_num));
    for(int i=0;i<ant_num;i++)
    {
        int r = rand()%city_num;
        ants[i].visit_city(r);
    }

    pheromones.resize(city_num,vector<double>(city_num,0.0));
    // 在这里可以对信息素矩阵进行任意初始化，例如：
    for (int i = 0; i < city_num; i++) {
        for (int j = 0; j < city_num; j++) {
            pheromones[i][j] = initialPheromoneValue;
        }
    }
}

void init()
{
    srand(time(NULL));
    init_cities();
    initialize_ants_pheromones(); 
}
//huristic function eta
double eta(double dist)
{
    return 1/dist;
}

void local_update_pheromones(int c1, int c2)
{
    static const double rho = 0.1;
    //方法1
    double delta_pheronmone = 1;
    //方法2
    #if 0
        delta_pheronmone = Q_learning;
    #endif
    pheromones[c1][c2] = (1-rho)*pheromones[c1][c2] + rho*delta_pheronmone;
    pheromones[c2][c1] = pheromones[c1][c2];
}

void global_update_pheromones(const vector<int>& tour_best,double len_best)
{
    static const double alpha = 0.1;
    for(int i=0;i<tour_best.size();++i)
    {
        pheromones[tour_best[i]][tour_best[(i+1)%tour_best.size()]] =
        (1-alpha) * pheromones[tour_best[i]][tour_best[(i+1)%tour_best.size()]]
        + alpha*(1/len_best);
        pheromones[tour_best[(i+1)%tour_best.size()]][tour_best[i]] = pheromones[tour_best[i]][tour_best[(i+1)%tour_best.size()]];
    }
}


int select_next_city(int antIndex) 
{
    static const double p0= 0.7;
    int city_num = cities.size();
    int currentCity = ants[antIndex].tour.back();
    vector<int> index;
    vector<double> probabilities;
    double sum = 0.0;
    
    // 计算蚂蚁从当前城市到其他城市的转移概率
    for(int i = 0;i<city_num;++i)
    {
        if(!ants[antIndex].is_city_visited(i))
        {
            double heuristic_val = pow(eta(distance(cities[currentCity],cities[i])),Beta);
            double pheromone_val = pow(pheromones[currentCity][i],1);
    	    probabilities.push_back(heuristic_val * pheromone_val);
            index.push_back(i);
        }
    }
    
    for(int i=0;i<probabilities.size();++i)
    {
        sum+=probabilities[i];
    }
    
    for(int i=0;i<probabilities.size();++i)
    {
        probabilities[i] /= sum;
    }

    if((rand()%1000/1000.0) < p0)
    {
        double p_max = -1;
        int i_max;
        for(int i=0;i<probabilities.size();++i)
        {
            if(probabilities[i]>p_max)
            {
                p_max = probabilities[i];
                i_max = index[i];
            }
        }
        return i_max;
    }
    else
    {
        for(int i=0;i<probabilities.size()-1;++i)
        {
            probabilities[i+1] += probabilities[i];
        }
        double random = (rand()%100)/100.0;
        for(int i=0;i<probabilities.size();++i)
        {
            if(random<=probabilities[i])
            {
                return index[i];
            }
        }
    }
    cerr<<"Error !"<<endl;
    exit(1);  //This should not happen.
    return -1;
}

void ant_move()//最多move city_num-1次
{
    for(int i=0;i<ant_num;++i)//move the ant to the next city
    {
        int next_city = select_next_city(i);
        ants[i].visit_city(next_city);
        // update pheromone
        local_update_pheromones(ants[i].tour[ants[i].tour.size()-1],ants[i].tour[ants[i].tour.size()-2]);
    }
}
void build_tour()
{
    for(int i=1;i<city_num;++i)
    {
        ant_move();
    }
}
void select_best_tour()
{
    for(int i=1;i<ant_num;++i)
    {
        if(ants[i].cal_total_distance()<len_best)
        {
            tour_best = ants[i].tour;
            len_best = ants[i].cal_total_distance();
        }
    }
}
void flash_ants()//刷新
{
    for(int i=0;i<ant_num;++i)
    {
        ants[i].clear_visited_cities();  
        int r = rand()%city_num;
        ants[i].visit_city(r);
    }
}
int main()
{    
    init();
    for(int i=0;i<ITER_MAX;++i)
    {
        build_tour();      
        select_best_tour();     
        global_update_pheromones(tour_best,len_best);
        flash_ants();
        if((i+1)%10==0)
        {
            cout<<"Iteration "<<i+1
                <<"\tTour length: "<<len_best<<endl;
        }
    }
    cout<<"best tour: ";
    for(int i=0;i<tour_best.size();++i)
    {
        cout<<tour_best[i]<<" ";
    }
    cout<<endl;
    cout<<"best len: "<<len_best<<endl;
}
