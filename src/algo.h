#ifndef ALGO_H
#define ALGO_H


#include <string>
#include <set>
#include <vector>
#include <cmath>
using namespace std;

class algo
{
public:

    const int INF = 0x3f3f3f3f;
    #define sqr(x) ((x)*(x))
    #define eps 1e-8
        //variables
    string file_name;
    int type;// type == 1 全矩阵, type == 2 二维欧拉距离
    int N;//城市数量
    double **dis;//城市间距离
    double **pheromone;//信息素
    double **herustic;//启发式值
    double **info;// info = pheromone ^ delta * herustic ^ beta
    double pheromone_0;//pheromone初始值，这里是1 / (avg * N)其中avg为图网中所有边边权的平均数。
    int m;//种群数量
    int delta, beta;//参数
    double alpha;
    int *r1, *s, *r;//agent k的出发城市，下一个点，当前点。
    int MAX, iteration;//最大迭代次数，迭代计数变量
    int construction_it;
    set<int> empty, *J;
    struct vertex{
        double x, y;// 城市坐标
        int id;// 城市编号
        int input(const char* data){
            return sscanf(data,"%d %lf %lf", &id,&x, &y);
        }
    }*node;

    void clear(){
        //释放二维数组
        for (int i = 0; i < N; i ++){

            delete[] pheromone[i]; delete[] herustic[i]; delete[] info[i];
        } //释放一维数组
        delete[] pheromone;
        delete[] herustic;
        delete[] info;
        delete[] r1;
        delete[] r;
        delete[] s;
        delete[] J;
        delete[] node;
        delete[] dis;
    }

    ~algo(){
        clear();
    }

public:

    typedef pair<int, int> pair_int;
    struct Tour{//路径
        vector<pair_int> path;//path[i]，存储一                     条边(r,s)
        double L;
        void clean(){
            L = 0x3f3f3f3f;
            path.clear();
            path.shrink_to_fit();
        }//清空
        void calc(double** dis){
            L = 0;
            int sz = path.size();
            for (int i = 0; i < sz; i ++){
                L += dis[path[i].first][path[i].second];
            }
        }//计算长度
        void push_back(int x, int y){
            path.push_back(make_pair(x, y));
        }
        int size(){
            return (int)path.size();
        }
        int r(int i){
            return path[i].first;
        }
        int s(int i){
            return path[i].second;
        }
        void print(FILE *fp){
            int sz = path.size();
            for (int i = 0; i < sz; i ++){
                fprintf(fp, "%d->", path[i].first + 1);
            }
            fprintf(fp, "%d\n", path[sz - 1].second +                    1);
        }
        bool operator <(const Tour &a)const{
            return L < a.L;
        }//重载
    }best_so_far;

    Tour *tour;

    int last = 0x3f3f3f3f;

public :
    double EUC_2D(const vertex &a, const vertex &b){
        return sqrt(sqr(a.x - b.x) + sqr(a.y - b.y));
    }

    void io(const char* NodeList){//输入
        //printf("input file_name and data type\n");
        type = 2;
        node = new vertex[N + 5];
        dis = new double*[N + 5];
        double tmp = 0;
        int cnt = 0;
        if (type == 1){
            for (int i = 0; i < N; i ++){
                dis[i] = new double[N];
                for (int j = 0; j < N; j ++){
                    //fscanf(fp, "%lf", &dis[i][j]);
                    tmp += i != j ? dis[i][j] : 0;// i == j的                           时候 dis不存在，所以不考虑。
                    cnt += i != j ? 1 : 0;// i == j的时候                               dis不存在，所以不考虑。
                }
            }
        }else{
            std::string nodestr(NodeList);
            for (int i = 0; i < N; i ++){
                node[i].input(nodestr.c_str());
                int p = nodestr.find_first_of('|');
                nodestr = nodestr.substr(p+1,nodestr.length()-p);
                //printf("s=%s\n",nodestr.c_str());
                //printf("in_io=%lf %lf %d\n",node[i].x,node[i].y,node[i].id);
            }
            for (int i = 0; i < N; i ++){
                dis[i] = new double[N];
                for (int j = 0; j < N; j ++){
                    dis[i][j] = EUC_2D(node[i],node[j]);// 计算距离
                    //printf("dis[%d][%d]=%lf\n",i,j,dis[i][j]);
                    tmp += i != j ? dis[i][j] : 0;// i == j的                           时候 dis不存在，所以不考虑。
                    cnt += i != j ? 1 : 0;// i == j的时候                               dis不存在，所以不考虑。
                }
            }
        }
        pheromone_0 =  (double)cnt / (tmp * N);//pheromone初始值，这里是1 / (avg * N)其中         avg为图网中所有边边权的平均数。

        return;
    }

    void init(){//初始化
        last = 0x3f3f3f3f;
        alpha = 0.1;//evaporation parameter，挥发参             数，每次信息素要挥发的量
        //delta = 1;
        //beta = 6;// delta 和 beta分别表示pheromones和herustic的比重
        m = N;
        pheromone = new double*[N + 5];
        herustic = new double*[N + 5];
        info = new double*[N + 5];
        r1 = new int[m + 5];
        r = new int[m + 5];
        s = new int[m + 5];
        J = new set<int>[m + 5];
        empty.clear();
        for (int i = 0; i < N; i ++){
            pheromone[i] = new double[N + 5];
            herustic[i] = new double[N + 5];
            info[i] = new double[N + 5];
            empty.insert(i);
            for (int j = 0; j < N; j ++){
                pheromone[i][j] = pheromone_0;
                herustic[i][j] = 1 / (dis[i][j] + eps);//加一个小数eps，防止被零除
            }
        }
        best_so_far.clean();
        iteration = 0;
        MAX = N * m;
    }

    double power(double x, int y){//快速幂，计算x ^ y，时间复杂度O(logn),感兴趣可以百度
        double ans = 1;
        while (y){
            if (y & 1) ans *= x;
            x *= x;
            y >>= 1;
        }
        return ans;
    }

    void reset(){
        construction_it = 0;
        tour = new Tour[m + 5];
        for (int i = 0; i < m; i ++){
            tour[i].clean();
            r1[i] = i%N;//初始化出发城市，
            J[i] = empty;
            J[i].erase(r1[i]);//初始化agent i需要访问的城市
            r[i] = r1[i];//当前在出发点
        }
        for (int i = 0; i < N; i ++)
            for (int j = 0; j < N; j ++){
                info[i][j] = power(pheromone[i][j], delta) * power(herustic[i][j], beta);
            }//选择公式
    }

    int select_next(int k){
        if (J[k].empty()) return r1[k]; //如果J是空的，那         么返回出发点城市
        double rnd = (double)(rand()) / (double)RAND_MAX;//产生0..1的随机数
        set<int>::iterator it = J[k].begin();
        double sum_prob = 0, sum = 0;
        for (; it != J[k].end(); it ++){
            sum += info[r[k]][*it];
        }//计算概率分布
        rnd *= sum;
        it = J[k].begin();
        for (; it != J[k].end(); it ++){
            sum_prob += info[r[k]][*it];
            if (sum_prob >= rnd){
                return *it;
            }
        }//依照概率选取下一步城市
    }

    void construct_solution(){
        for (int i = construction_it; i < N; i ++){
            for (int k = 0; k < m; k ++){
                int next = select_next(k);//选择下一步的最优决策
                J[k].erase(next);
                s[k] = next;
                tour[k].push_back(r[k], s[k]);

                r[k] = s[k];
            }
        }
        construction_it = N;
    }
    void construct_solution_next(){
        if(construction_it >= N) return;
        construction_it ++;
        for (int k = 0; k < m; k ++){
            int next = select_next(k);//选择下一步的最优决策
            J[k].erase(next);
            s[k] = next;
            tour[k].push_back(r[k], s[k]);
            printf("m=%d,pushback,%d,%d\n",k,r[k],s[k]);
            r[k] = s[k];
        }
    }

    void update_pheromone(){
        Tour now_best;
        now_best.clean();//初始化
        for (int i = 0; i < m; i ++){
            tour[i].calc(dis);
            printf("bestinalgo=%lf\n",tour[i].L);
            if (tour[i] < now_best)
                now_best = tour[i];//寻找当前迭代最优解
        }

        if (now_best < best_so_far){
            best_so_far = now_best;//更新最优解
        }
        for (int i = 0; i < N; i ++)
            for (int j = 0; j < N; j ++)
                pheromone[i][j] *= (1 - alpha);//信息素挥发
        int sz = now_best.size();
        for (int i = 0; i < sz; i ++){
            pheromone[now_best.r(i)][now_best.s(i)] += 1. / (double)now_best.L;
            pheromone[now_best.s(i)][now_best.r(i)] = pheromone[now_best.r(i)][now_best.s(i)];//               对称
            printf("p=%lf\n",pheromone[now_best.r(i)][now_best.s(i)]);
        }//更新信息素含量
    }


};



#endif // ALGO_H
