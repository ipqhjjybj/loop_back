#ifndef _example
#define _example

#include "load_config.h"
#include "TradingResult.h"
#include "talib.h"

#include <queue>

void printVec(const vector<double> & vec){
    for(int i = 0;i < vec.size();i++){
        printf("%lf,",vec[i]);
    }
    printf("\n");
}

void push_result(priority_queue<struct TradingResultNode > &ans,const struct TradingResultNode &vec){
    if( vec.total_open_num > PushResultMinOpenNum){
        allResultNum += 1;
        ans.push(vec);
    }
}

priority_queue<struct TradingResultNode > Q;
/**
 * 寻找最优的解，并保存到 ans里面
 ****/
void getBestSolution( ctaTemplate * obj,  vector<TradingResultNode> &ans , const char * code , const char * period , const char *start_date , const char *end_date){
    vector<string> filenames = getFileName(code , period);
    obj->loadFiles(filenames , start_date , end_date , period);
    while(!Q.empty()){
        Q.pop();
    }

	ans.clear();
    allResultNum = 0;
	//const vector<vector<double> > &vec = obj->getLoopForce();
    pair<int,string> tmp_loopForceCanshu = obj->getLoopForceInfo();

    FILE *fp = NULL;
    int flag1 = 1;
    int flag2 = 0;
    int kk = 0;
    struct TradingResultNode t1 ;

    //debug_fp = fopen( (string("./log/debug_") + obj->strategyName + ".csv").c_str() , "w");
    if( (fp = fopen(tmp_loopForceCanshu.second.c_str(), "r")) != NULL){
        //for(int i = 0;i < vec.size();i++){
        char ss[1000];
        while( fscanf(fp,"%s",ss) != EOF){
            kk = kk + 1;
            obj->init(code);
            //printVec(vec[i]);
            vector<double> n_canshu =  getParams(ss , ",");
            // for(int i = 0;i < n_canshu.size();i++){
            //     if(i == 0){
            //         fprintf(debug_fp,"%lf",n_canshu[i]);
            //     }else{
            //         fprintf(debug_fp,",%lf",n_canshu[i]);
            //     }
            // }fprintf(debug_fp,"\n");
            // fflush(debug_fp);
            // for(int i = 0;i < n_canshu.size();i++){
            //     printf("%lf ",n_canshu[i]);
            // }printf("\n");
            obj->setCanshuFromVec( n_canshu);
            obj->run();
            struct TradingResultNode t2 = obj->getTradingResult(code , period ,start_date , end_date);
            if( t2.total_open_num > 10 && ( flag1 == 1 || t2 < t1) ){
                t1 = t2;
                flag1 = 0;
                flag2 = 1;
            }
            //break;
            if( flag2 == 1){
                for(int i = 0;i < t1.canshuSet.size();i++){
                    if(i == t1.canshuSet.size()-1){
                        //printf("%lf \n %lf\n %d\n", t1.canshuSet[i] , t1.compareValue() , kk);
                    }else{
                        //printf("%lf ",t1.canshuSet[i]);
                    }
                }
                flag2 = 0;

                push_result(Q , t2);
                if(Q.size() > MaxSaveResultNum){
                    //printf("Q.top().capital %lf\n",Q.top().total_income);
                    Q.pop();
                }
                // printf("t1.total_open_num = %d\n", t1.total_open_num);
                // break;
            }
            //Q.push(obj->getTradingResult(code , period ,start_date , end_date));
        }
        fclose(fp);
        while(!Q.empty()){
            ans.push_back(Q.top());
            Q.pop();
        }
    }else{
        printf("running! open loopforce error!\n");
    }

}

/**
 * 使用粒子群算法 计算最优解
 ***************/
#include <ctime>
#include <cstdlib>

int checkLegalCanshu( const vector<double> &vec){
    int flag = 1;
    vector< pair<int,int> > &mpVec = _config._canshuYueshu;
    for(int i = 0;i < mpVec.size() ;i++){
        pair<int,int> val = mpVec[i];
        if(vec[val.first] + eps >= vec[val.second] ){
            //printf("%d %d , %lf %lf\n",val.first,val.second, vec[val.first] , vec[val.second]);
            flag = 0;
            break;
        }
    }
    return flag;
}

double randNext( const double &low,  const double &high){
    double rand_numf = rand() * 1.0 / (double)(RAND_MAX);
    return rand_numf * (high - low) + low;
}

void getBestSolutionUsingLiziQun(ctaTemplate * obj,  vector<TradingResultNode> &ans , const char * code , const char * period , const char *start_date , const char *end_date,
                                 const int NumParticle = 40 , const int max_cycle = 100,const double C1 = 1.5 , const double C2 = 1.5){
    vector<canshuNode> _canshuRanges = _config.loadCanshuRange();
    int Dim = _canshuRanges.size();
    //string filename = getFileName(code , period);
    vector<string> filenames = getFileName(code , period);
    double xLo[Dim];               // Lower bound and upper bound for each dimension
    double xHi[Dim];               // Lower bound and upper bound for each dimension
    double v[Dim][NumParticle];                 // current velocity of particle
    double x[Dim][NumParticle];                 // current position of particle
    double individual_optimal_fit[NumParticle]; //individual optimal fitness
    double individual_optimal_x[Dim][NumParticle];   // individual optimal position
    double social_optimal_fit;                  // social optimal fitness
    double social_optimal_x[Dim];               // social optimal position
    allResultNum = 0;
    srand((unsigned)time(0));

    //obj->loadFile(filename.c_str() , start_date , end_date);
    obj->loadFiles(filenames , start_date , end_date , period);
    ans.clear();
    priority_queue<struct TradingResultNode > Q;

    for(int i = 0;i < Dim;i++){
        xLo[i] = _canshuRanges[i].l;  xHi[i] = _canshuRanges[i].r;
    }
    for(int i = 0;i < Dim;i++){
        for(int j = 0;j < NumParticle;j++){
            x[i][j] = individual_optimal_x[i][j]= randNext(xLo[i],xHi[i]);
            double t_range = (xHi[i] - xLo[i]) * 1.0 / NumParticle;
            v[i][j] = randNext( xLo[i]  , xLo[i] + t_range * (j+1));  // 调点
        }
    }
    for(int j=0;j<NumParticle;j++){
        vector<double> use_canshu; use_canshu.clear();
        for(int k = 0;k < Dim;k++){
            use_canshu.push_back(x[k][j]);
        }
        obj->init(code);
        obj->setCanshuFromVec(use_canshu);
        obj->run();
        struct TradingResultNode tnode = obj->getTradingResult(code , period ,start_date , end_date);

        if(checkLegalCanshu(tnode.canshuSet) == 1){
            push_result(Q , tnode );
        }
        //push_result(Q , tnode );
        //Q.push(tnode);
        while(Q.size() > MaxSaveResultNum){
            //printf("Q.top().capital %lf\n",Q.top().total_income);
            Q.pop();
        }
        individual_optimal_fit[j]=tnode.compareValue();

    }
    social_optimal_fit=individual_optimal_fit[0];

    //printf("End 1\n");
    for(int cycle = 0; cycle < max_cycle ; cycle++){
        for(int j = 0;j < NumParticle;j++){
            obj->init();
            vector<double> use_canshu; use_canshu.clear();
            for(int k = 0;k < Dim;k++){
                use_canshu.push_back( x[k][j]);
            }
            obj->init(code);
            obj->setCanshuFromVec(use_canshu);
            struct TradingResultNode tnode = obj->getTradingResult(code , period ,start_date , end_date);
            if(checkLegalCanshu(tnode.canshuSet) == 1){
                // printf("11 tnode compareValue :%lf\n",tnode.compareValue());
                push_result(Q , tnode );
            }
            //Q.push(tnode);
            while(Q.size() > MaxSaveResultNum){
                Q.pop();
            }
            double fitness = tnode.compareValue();  // evaluation function
            if(fitness > individual_optimal_fit[j]){
                individual_optimal_fit[j] = fitness;
                for(int k = 0;k < Dim;k++){
                    individual_optimal_x[k][j] = x[k][j];
                }
            }
        }
        for(int j = 0;j < NumParticle;j++){
            if(individual_optimal_fit[j] > social_optimal_fit){
                social_optimal_fit=individual_optimal_fit[j];
                for(int k = 0;k < Dim;k++){
                    social_optimal_x[k] = individual_optimal_x[k][j];
                }
            }
        }
        for(int i = 0;i < Dim;i++){
            for(int j = 0;j < NumParticle;j++){// Modify the velocity and position of particle
                v[i][j]=v[i][j]+C1*randNext(-1.,1.)*(individual_optimal_x[i][j]-x[i][j]) +C2*randNext(-1.,1.)*(social_optimal_x[i]-x[i][j]);
                x[i][j]=x[i][j]+v[i][j];

                if(x[i][j]>xHi[i])x[i][j]=xHi[i];
                if(x[i][j]<xLo[i])x[i][j]=xLo[i];
            }
        }
    }
    while(!Q.empty()){
        ans.push_back(Q.top());
        Q.pop();
    }
}

/**
 * 使用类似随机的算法确定最优的解
 **/
double urLo[55];            // Lower bound and upper bound for each dimension
double urHi[55];            // Lower bound and upper bound for each dimension
double urBufa[55];          // 每次
double tmp_urLo[55];        // 缩减范围后的
double tmp_urHi[55];        // 缩减范围后的

int checkLegalCanshuRM( const vector<double> &vec){
    int flag = 1;
    vector< pair<int,int> > &mpVec = _config._canshuYueshu;
    for(int i = 0;i < mpVec.size() ;i++){
        pair<int,int> val = mpVec[i];
        if(vec[val.first] + eps >= vec[val.second] ){
            //printf("%d %d , %lf %lf\n",val.first,val.second, vec[val.first] , vec[val.second]);
            flag = 0;
            break;
        }
    }
    return flag;
}

double getYueshuValueMin( const vector<double> &vec , int obj_i){
    double ret_val = -100000.0;
    vector< pair<int,int> > &mpVec = _config._canshuYueshu;
    for(int i = 0;i < mpVec.size();i++){
        pair<int,int> val = mpVec[i];
        if( obj_i == val.second && obj_i > val.first){
            ret_val = max(ret_val , vec[val.first]);
        }
    }
    return ret_val;
}

void getBestSolutionUsingRandomFunction(ctaTemplate *obj , vector<TradingResultNode> &ans, const char * code ,  const char * period , const char *start_date , const char *end_date , int max_cycle = 100, int __rmNumParticle = 1000, int tmpSaveResultNum = 100){
    srand((unsigned)time(0));
    vector<canshuNode> _canshuRanges = _config.loadCanshuRange();
    int Dim = _canshuRanges.size();
    priority_queue<struct TradingResultNode > Q;   //存储的
    priority_queue<struct TradingResultNode > tQ;  //临时存储的最优解
    vector<double> use_canshu;
    vector<string> filenames = getFileName(code , period);
    obj->loadFiles(filenames , start_date , end_date , period);
    for(int i = 0;i < Dim;i++){
        urLo[i] = _canshuRanges[i].l;  urHi[i] = _canshuRanges[i].r;  urBufa[i] = _canshuRanges[i].bufa;
    }
    while( max_cycle-- > 0){
        while(!tQ.empty()){
            tQ.pop();
        }
        for(int i = 0;i < __rmNumParticle ; i++){
            use_canshu.clear();
            for(int j = 0;j < Dim;j++){
                double yueshuValue = getYueshuValueMin(use_canshu , j );
                double v = randNext( max(urLo[j] , yueshuValue),urHi[j]);
                use_canshu.push_back(v);
            }

            obj->init(code);
            obj->setCanshuFromVec(use_canshu);
            obj->run();
            struct TradingResultNode tnode = obj->getTradingResult(code , period ,start_date , end_date);

            if(checkLegalCanshuRM(tnode.canshuSet) == 1){
                push_result(tQ , tnode);
                while(tQ.size() > tmpSaveResultNum){
                    tQ.pop();
                }
                push_result(Q , tnode);
                while(Q.size() > MaxSaveResultNum){
                    Q.pop();
                }
            }
        }
        // 开始缩减范围
        if(tQ.size() > 0){
            struct TradingResultNode tnode = Q.top();
            for(int i = 0;i < Dim;i++){
                tmp_urLo[i] = tmp_urHi[i] = tnode.canshuSet[i];
            }
            while(!tQ.empty()){
                tnode = tQ.top();
                for(int i = 0;i < Dim;i++){
                    tmp_urLo[i] = min(tmp_urLo[i] , tnode.canshuSet[i]);
                    tmp_urHi[i] = max(tmp_urHi[i] , tnode.canshuSet[i]);
                }
                tQ.pop();
            }
            int change_flag = 0;
            for(int i = 0;i < Dim;i++){
                if( tmp_urLo[i] > urLo[i]){
                    change_flag = 1;
                    urLo[i] = tmp_urLo[i];
                }
                if( tmp_urHi[i] < urHi[i]){
                    change_flag = 1;
                    urHi[i] = tmp_urHi[i];
                }
            }
            if( 0 == change_flag){
                break;
            }
        }else{
            break;
        }
    }
    // 回传最后的解
    while(!Q.empty()){
        ans.push_back(Q.top());
        Q.pop();
    }
}

/**
 * 优先队列 步伐堆
 ***/
const int MOD_VAL = 1000007;
bool hash_priority_queue_engine[MOD_VAL + 10];
double bpLo[55];            // Lower bound and upper bound for each dimension
double bpHi[55];            // Lower bound and upper bound for each dimension
double bpBufa[55];          // 每次
//priority_queue< TradingResultNode> bpPriResult; // 存储的优先队列结果

int bpLegalCanshuSet(const vector<double> _v){
    int val = 0.0;
    for(int i = 0;i < _v.size();i++){
        val = val * 10 % MOD_VAL + i * _v[i];
        val %= MOD_VAL;
    }
    //printf("val = %d\n",val);
    return (int)val;
}
void getBestSolutionUsingPriority(ctaTemplate * obj,  vector<TradingResultNode> &ans , const char * code , const char * period , const char *start_date , const char *end_date , int max_cycle , int __bpNumParticle){
    vector<canshuNode> _canshuRanges = _config.loadCanshuRange();
    int Dim = _canshuRanges.size();
    memset(hash_priority_queue_engine , 0, sizeof(hash_priority_queue_engine));

    vector<string> filenames = getFileName(code , period);
    obj->loadFiles(filenames , start_date , end_date , period);
    ans.clear();

    struct TradingResultNode maxResultObj ;
    priority_queue<struct TradingResultNode > Q;
    int first_deal = 0;

    vector<double> use_canshu;
    for(int i = 0;i < Dim;i++){
        bpLo[i] = _canshuRanges[i].l;  bpHi[i] = _canshuRanges[i].r;  bpBufa[i] = _canshuRanges[i].bufa;
    }
    for(int i = 0; i < __bpNumParticle; i++){
        use_canshu.clear();
        for(int j = 0;j < Dim;j++){
            double v = randNext(bpLo[j],bpHi[j]);
            use_canshu.push_back(v);
        }
        obj->init(code);
        obj->setCanshuFromVec(use_canshu);
        obj->run();

        struct TradingResultNode tnode = obj->getTradingResult(code , period ,start_date , end_date);
        if(checkLegalCanshu(tnode.canshuSet) == 1){
            push_result(Q , tnode );
            if(0 == first_deal){
                maxResultObj = tnode;
                first_deal = 1;
            }else{
                if( tnode < maxResultObj ){
                    maxResultObj = tnode;
                }
            }
            while(Q.size() > MaxSaveResultNum){
                Q.pop();
            }
        }
        //push_result(Q , tnode );
        //Q.push(tnode);
        while(Q.size() > MaxSaveResultNum){
            //printf("Q.top().capital %lf\n",Q.top().total_income);
            Q.pop();
        }
    }

    vector<vector<double> > nCanshu;
    while( (max_cycle--) > 0){
        nCanshu.clear();
        for(int i = 0;i < Dim;i++){
            vector<double> tmp_n1(maxResultObj.canshuSet);
            vector<double> tmp_n2(maxResultObj.canshuSet);
            double vv = tmp_n1[i] -= bpBufa[i] * randNext( 0 , 4);
            double uu = tmp_n1[i] += bpBufa[i] * randNext( 0 , 4);
            if( vv > bpHi[i]){
                vv = bpHi[i];
            }if(vv < bpLo[i]){
                vv = bpLo[i];
            }
            tmp_n1[i] = vv;

            if(uu > bpHi[i]){
                uu = bpHi[i];
            }if(uu < bpLo[i]){
                uu = bpLo[i];
            }tmp_n2[i] = uu;
            nCanshu.push_back(tmp_n1);
            nCanshu.push_back(tmp_n2);
        }
        int flag_has_new_answer = 0;
        for(int i = 0;i < nCanshu.size();i++){
            int vv = bpLegalCanshuSet(nCanshu[i]);
            if( 0 == hash_priority_queue_engine[vv]){
                obj->init(code);
                obj->setCanshuFromVec(use_canshu);
                obj->run();
                struct TradingResultNode tnode = obj->getTradingResult(code , period ,start_date , end_date);
                if(checkLegalCanshu(tnode.canshuSet) == 1){
                    push_result(Q , tnode );
                    if(0 == first_deal){
                        maxResultObj = tnode;
                        first_deal = 1;
                    }else{
                        if( tnode < maxResultObj ){
                            maxResultObj = tnode;
                        }
                    }
                    flag_has_new_answer += 1;
                }
                hash_priority_queue_engine[vv] = 1;
                while(Q.size() > MaxSaveResultNum){
                    Q.pop();
                }
            }
        }
        if( 0 == flag_has_new_answer){
            break;
        }
    }
    while(!Q.empty()){
        ans.push_back(Q.top());
        Q.pop();
    }
}
#endif

