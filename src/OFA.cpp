#include"OFA.h"
using namespace ofa;
int OFA::null = -1;

const double test[20][2]={
    {1,2},
    {3,4},
    {7,8},
    {10,9},
    {4,6},
    {6,5},
    {14,12},
    {13,76},
    {16,1},
    {7,13},
    {4,11},
    {9,11},
    {19,11},
    {22,4},
    {19,7},
    {6,10},
    {19,8},
    {19,10},
    {8,2},
    {6,0}
};

void OFA::inputPoints(vector<Point>& points)
{
    using std::cin;
    for(int i =0;i<POINT_NUM;i++)
    {
        double x,y;
        //cin>>x>>y;
        points[i].x = test[i][0];
        points[i].y = test[i][1];    
    }
}
void OFA::initIndivs(vector<Individual>& indivs)
{
    //give a initial sequence
    vector<int> initSeq;
    for(int i=0;i<POINT_NUM;i++)
    {
        initSeq.push_back(i);
    }
    for(int i=0;i<N;i++)
    {
        indivs[i].sequence = initSeq;
    }
 

    auto swap = [](int& a, int& b){
        int temp = a;
        a = b;
        b = temp;
    };
    //generate uniformly distributed sequence
    for(int i =0;i<N;i++)
    {
        //shuffle
        for(int j =0;j<POINT_NUM;j++)
        {
            swap(indivs[i].sequence[j] ,  indivs[i].sequence[(j+abs(rand()))%POINT_NUM] );
        }
        //compute the value
        indivs[i].val = computeFucVal(indivs[i].sequence);
        //init the r1 and r2
        for(int j=0;j<POINT_NUM;j++)
        {
            indivs[i].r1.push_back(rand()%2);
            indivs[i].r2.push_back(rand()%2);                 
        }
    }
}

double OFA::computeFucVal(vector<int>& sequence){
    double sum=0;
    for(int i=0;i<POINT_NUM-1;i++)
    {
        int preIndex = sequence[i];
        int postIndex = sequence[i+1];
        sum += abs(this->points[preIndex].x-this->points[postIndex].x)+
               abs(this->points[preIndex].y-this->points[postIndex].y);
    }
    return sum;
}

bool OFA::cmp(const Individual& a, const Individual& b)
{
    return a.val<b.val;
}

void OFA::sortIndivs(vector<Individual>& indivs)
{   
    sort(indivs.begin(),indivs.end(),OFA::cmp);
}

void OFA::printIndivs()
{
    sortIndivs(this->indivs);

    using std::cout;

    for(int i =0;i<N;i++)
    {
        cout<<"seq: ";
        for(int j =0;j<POINT_NUM;j++)
        {
            cout<<this->indivs[i].sequence[j]<<" ";
        }
        cout<<"val: "<<this->indivs[i].val<<"\n";
    }
}

vector<int> OFA::theta(const vector<int>& s1,const vector<int>& s2)
{
    vector<int> res;
    for(int i =0;i<POINT_NUM;i++)
    {
        if(s1[i]==s2[i])
        {
            res.push_back(null);
        }
        else
        {
            res.push_back(s1[i]);
        }
    }
    return res;
}

vector<int> OFA::crossProduct(const int& k,const vector<int>& r)
{
    /**
     * r is a random binary series
     * the series length is similar to delta X
     * scale k belongs to [0,1]
    */
    vector<int> res;
    for(int i =0;i<POINT_NUM;i++)
    {
        if(k<r[i])
        res.push_back(!r[i]);
        else
        res.push_back(r[i]);
    }
    return res;
}

vector<int> OFA::product(const vector<int>& binSeries, const vector<int>& r)
{
    vector<int> res;
    for(int i =0;i<POINT_NUM;i++)
    {
        if(binSeries[i]==1)
        res.push_back(r[i]);
        else
        res.push_back(null);
    }
    return res;
}

vector<int> OFA::add(const vector<int>& seq, const vector<int>& delta)
{
    vector<int> res = seq;
    auto swap = [](int& a, int& b)
    {
        int temp = a;
        b = a;
        a = temp;
    };
    for(int i=0;i<POINT_NUM;i++)
    {
        int flag = false;
        int index=-1;
        for(int j=0;j<POINT_NUM;j++)
        {
            if(seq[i]==delta[j])
            {
                flag = true;
                index = j;
                break;
            }
        }
        if(flag)
        {
            swap(res[i],res[index]);
            i--;
        }
    }
    return res;
}

vector<int> OFA::minus(const vector<int>& seq, const vector<int>& delta)
{
    int i = POINT_NUM-1;
    int j = 0;
    vector<int> res = seq;

    auto swap = [](int& a, int& b)
    {
        int temp;
        temp = a;
        a = b;
        b = temp;
    };
    while(i>=0&&j<POINT_NUM)
    {
        if(res[i]==delta[j] || delta[j]==null)
        {
            i--;
            j++;
            continue;
        }
        
        else
        {
            int k=-1;
            for(k=0;k<POINT_NUM;k++)
            {
                if(res[k]==delta[k])
                break;
            }
            try
            {
                if(k==-1) continue;
                swap(res[i],res[k]);
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
                std::cerr<<"k is -1 in minus operation\n";
            }
                       
        }     
        i--;
        j++;
    }
    return res;

}

void OFA::update()
{
    while(t<Max_t)
    {
        double lambda = (rand()%100)/100.0;
        sortIndivs(this->indivs);
        //计算min的那个 就是最优的那个
        auto newSeq =  calNewSeq(indivs[N-1],indivs[0],true);
        if(isBetter(lambda,computeFucVal(newSeq),indivs[0].val))
        {
            indivs[0].sequence = newSeq;
        }
        //计算其他的
        for(int i =N-1;i>=1;i--)
        {
            double optimalVal = indivs[i].val;
            vector<int> optimalSeq = indivs[i].sequence;
            for(int j=0;j<i;j++)
            {
                auto newSeq = calNewSeq(indivs[j],indivs[i],false);
                if(isBetter(lambda,computeFucVal(newSeq),optimalVal))
                {
                    optimalVal = computeFucVal(newSeq);
                    optimalSeq = newSeq;
                }
            }
            indivs[i].sequence = newSeq;
        }

        //update the val
        for(int i=0;i<N;i++)
        {
            indivs[i].val = computeFucVal(indivs[i].sequence);
        }
        if(t%1000==0)
        {
            std::cout<<"iteraitons: " << t << '\n';  //for checking the progress.
        }
        ++t;
    }   
}

double OFA::getK()
{
    return (double)t/Max_t;
}


bool OFA::isBetter(double lambda, double new_val, double old_val)
{
    return lambda*new_val/(1+lambda*(t+1)) < old_val/t;
}

vector<int> OFA::calNewSeq(const Individual& indiv_a,const Individual& indiv_b, bool flag)
{
    vector<int> res;
    vector<int> delta = theta(indiv_a.sequence,indiv_b.sequence);

    /**
     * @attention r1 and r2的来历不明，先写着
    */
   // printf("cross\n");   
    auto crossRes1 = crossProduct(getK(),indiv_b.r1);//r1 and r2 ?咋来
    //printf("product\n");
    auto productRes1 = product(crossRes1,delta);

    auto crossRes2 = crossProduct(getK(),indiv_b.r2);//r1 and r2?怎么找到？？？
    auto productRes2 = product(crossRes2,delta);//r1 and r2?怎么找到？？？囚了！！

    //printf("minus\n");
    auto minusRes = minus(indiv_b.sequence,productRes1);
    ///printf("add\n");
    auto addRes = add(minusRes,productRes2);

    return addRes;

}