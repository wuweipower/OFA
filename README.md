# 简单的复述论文的意思

目前的问题：CNC code中的路径规划如zig, zig-tag, radial, spiral tool paths 不是最优的路径。

DPO (drlling path optimization) 。我们想通过优化路径，提高生产效率。

literature：文献；文学

算法：

理论：解决优化问题就类似于动物觅食(animal foraging)。全局最优解的附近，也就是一些局部最优解可以被看作动物觅食栖息地的各种斑块（小块），优化过程就可以被看作动物根据动物觅食理论，在不同的小块上寻找更好的net rate of energy gain。在找到最佳斑块后，动物会根据最佳猎物的模型搜索斑块内的最佳位置。当找到最终最佳位置时，即可获得最佳解。

通俗来说就是先找到好的地方，然后找到最优的解？

难道先确定求解空间，然后再找？

以下是chatGPT的回答：

动物觅食理论，也称最优觅食理论，是行为生态学的一个分支，研究动物在选择食物和获取食物的地点上做出决策以最大化其能量收益并最小化能量消耗。

该理论假定动物在觅食活动中有限的时间和精力，必须选择最有利可图的食物或补给地来摄取营养。在做出这些决策时，动物通常会权衡预期的能量获取和搜索时间、处理时间和被捕食风险等成本。

根据这个理论，动物应当选择提供净能量摄入率最高的猎物，净能量摄入率的计算为每单位时间获得的能量减去采集和处理猎物的能量支出。动物还被预测为将采取不同的觅食策略，取决于其资源分布和丰度，灰斑或不可预测的环境更倾向于采取冒险行为。



算法基础：

每个在某个patch中的位置的个体被看作一个目标函数的解。以这个解为起点，然后开始搜索，选择一个最优的

x~i~^t+1^ = x~i~^t^ - k* r~1i~ * Δx^t^ + k*r~2i~ * Δx^t^

x~i~^t+1^ ：t+1次过后，一个position中的第i个vector

X𝑡+1 = [𝑥~1~ ^𝑡+1^, ⋯ , 𝑥𝑖 𝑡+1, ⋯ 𝑥𝑑 𝑡+1] ^T^ 是一个新的position

k指的是scale factor

r~1i~ r~2i~ 是uniform random number

Δ是指增量



如何摆脱local optimal solution

recruitment：招收，招募，招聘

当某些动物找到猎物后，会唤来其他动物，就像猎狗。这里就提出Group和Recruitment的概念。

Group就是一个分散到不同patch不同位置的动物群（fauna）。就是限制空间。

recruitment就是在搜索过后，计算Group中的每个个体中的位置的一个value，然后从优到差排序。然后优的个体吸引差的个体，差的个体就会离开原来的位置。

简单来说就是发现更优解了，其他的解随机向他靠拢。

然后最优的那个会向差的方向移动。目的是为了增大搜索空间，避免陷入局部最优。

# problem

代码跑出来后的结构基本都是原来的最优的个体。

主要问题是如何更新个体。

# 以下是杂记不用看

animal foraging： 动物觅食

namely：即

patch：补丁，碎片，斑点

核心理论：The optimization  process can be viewed as an animal, following the optimal  foraging theory, foraging in different patches to find the  optimal patch where the net rate of energy gain can be  maximized. After the optimal patch is found, the animal will  search the optimal position within the patch according to the  model of the best prey.

the individual is regarded as a foraging animal whose  position in a patch denotes a solution of the objective function

According to the Optimal Foraging Theory,  animals always forage in the area of greatest food abundance  and always allocate most foraging time to the patch of greatest  food

The animal foraging position is expressed by a  two-dimensional variable of longitude and latitude and can  move along the two directions.

The position in OFA is  expressed by a d-dimensional variable;

**According to the optimal foraging theory, animals always  allocate the greatest amount of time to the patch of greatest food  abundance and progressively less time to progressively worse  areas**

The marginal value theorem of optimal foraging  theory states that an animal should **leave a patch when its rate of  food intake in the patch drops to the average rate of the habitat**

recruitment：招聘

fauna：动物群

scatter：分散



使用

Boost

openGL
