#ifndef _spread_engine
#define _spread_engine

void spread_push_result(priority_queue<struct mergeAllTradingResult > &ans,const struct mergeAllTradingResult &vec){
    if( vec.tot_total_open_num > PushResultMinOpenNum){
        spread_allResultNum += 1;
        ans.push(vec);
    }
}

priority_queue<struct mergeAllTradingResult > spread_Q;
/**
 * 寻找最优的解，并保存到 ans里面
 ****/
void spread_getBestSolution( spreadTemplate * obj,  vector<struct mergeAllTradingResult> &ans  , vector<struct __zhuli_data> &zhuliData , const char *start_date , const char *end_date){
    vector<TradingResultNode> resultList ;
    obj->loadFile(zhuliData , start_date , end_date);
    while(!spread_Q.empty()){
        spread_Q.pop();
    }

	ans.clear();
    spread_allResultNum = 0;

    pair<int,string> tmp_loopForceCanshu = obj->getLoopForceInfo();

    FILE *fp = NULL;
    int flag1 = 1;
    int flag2 = 0;
    int kk = 0;
    struct mergeAllTradingResult t1 ;

    if( (fp = fopen(tmp_loopForceCanshu.second.c_str(), "r")) != NULL){
        //for(int i = 0;i < vec.size();i++){
        char ss[1000];
        while( fscanf(fp,"%s",ss) != EOF){
            kk = kk + 1;
            // obj->init(code);
            // vector<double> n_canshu = getParams(ss , ",");
            // obj->setCanshuFromVec( n_canshu);
            // obj->run();
            // struct mergeAllTradingResult t2 = obj->getTradingResult(code , period ,start_date , end_date);

            // printf("now canshu %s\n" , ss);
            obj->init();
            vector<double> n_canshu = getParams(ss , ",");
            obj->setCanshuFromVec( n_canshu);
            obj->run();
            struct mergeAllTradingResult t2 = obj->getTradingResultList(resultList);

            if( t2.tot_total_open_num > 0 && ( flag1 == 1 || t2 < t1) ){
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

                spread_push_result(spread_Q , t2);
                if(spread_Q.size() > MaxSaveResultNum){
                    //printf("spread_Q.top().capital %lf\n",spread_Q.top().total_income);
                    spread_Q.pop();
                }
            }
            //spread_Q.push(obj->getTradingResult(code , period ,start_date , end_date));
        }
        fclose(fp);
        while(!spread_Q.empty()){
            ans.push_back(spread_Q.top());
            spread_Q.pop();
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

int spread_checkLegalCanshu( const vector<double> &vec){
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

double spread_randNext( const double &low,  const double &high){
    double rand_numf = rand() * 1.0 / (double)(RAND_MAX);
    return rand_numf * (high - low) + low;
}

void spread_getBestSolutionUsingLiziQun(spreadTemplate * obj,  vector<mergeAllTradingResult> &ans , vector<struct __zhuli_data> &zhuliData , const char *start_date , const char *end_date,
                                 const int NumParticle = 40 , const int max_cycle = 100,const double C1 = 1.5 , const double C2 = 1.5){
    vector<canshuNode> _canshuRanges = _config.loadCanshuRange();
    vector<TradingResultNode> resultList ;

    int Dim = _canshuRanges.size();

    //vector<string> filenames = getFileName(code , period);
    double xLo[Dim];               // Lower bound and upper bound for each dimension
    double xHi[Dim];               // Lower bound and upper bound for each dimension
    double v[Dim][NumParticle];                 // current velocity of particle
    double x[Dim][NumParticle];                 // current position of particle
    double individual_optimal_fit[NumParticle]; //individual optimal fitness
    double individual_optimal_x[Dim][NumParticle];   // individual optimal position
    double social_optimal_fit;                  // social optimal fitness
    double social_optimal_x[Dim];               // social optimal position
    spread_allResultNum = 0;
    srand((unsigned)time(0));

    obj->loadFile(zhuliData , start_date , end_date);
    //obj->loadFile(filename.c_str() , start_date , end_date);
    //obj->loadFiles(filenames , start_date , end_date , period);
    ans.clear();
    priority_queue<struct mergeAllTradingResult > Q;

    for(int i = 0;i < Dim;i++){
        xLo[i] = _canshuRanges[i].l;  xHi[i] = _canshuRanges[i].r;
    }
    for(int i = 0;i < Dim;i++){
        for(int j = 0;j < NumParticle;j++){
            x[i][j] = individual_optimal_x[i][j]= spread_randNext(xLo[i],xHi[i]);
            double t_range = (xHi[i] - xLo[i]) * 1.0 / NumParticle;
            v[i][j] = spread_randNext( xLo[i]  , xLo[i] + t_range * (j+1));  // 调点
        }
    }
    for(int j=0;j<NumParticle;j++){
        vector<double> use_canshu; use_canshu.clear();
        for(int k = 0;k < Dim;k++){
            use_canshu.push_back(x[k][j]);
        }
        //obj->init(code);
        obj->init();
        obj->setCanshuFromVec(use_canshu);
        obj->run();
        //struct mergeAllTradingResult tnode = obj->getTradingResult(code , period ,start_date , end_date);
        struct mergeAllTradingResult tnode = obj->getTradingResultList( resultList );

        if(spread_checkLegalCanshu(tnode.canshuSet) == 1){
            spread_push_result(Q , tnode );
        }
        //spread_push_result(Q , tnode );
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
            vector<double> use_canshu; use_canshu.clear();
            for(int k = 0;k < Dim;k++){
                use_canshu.push_back( x[k][j]);
            }
            obj->init();
            //obj->init(code);
            obj->setCanshuFromVec(use_canshu);
            //struct TradingResultNode tnode = obj->getTradingResult(code , period ,start_date , end_date);
            struct mergeAllTradingResult tnode = obj->getTradingResultList( resultList );

            if(spread_checkLegalCanshu(tnode.canshuSet) == 1){
                // printf("11 tnode compareValue :%lf\n",tnode.compareValue());
                spread_push_result(Q , tnode );
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
                v[i][j]=v[i][j]+C1*spread_randNext(-1.,1.)*(individual_optimal_x[i][j]-x[i][j]) +C2*spread_randNext(-1.,1.)*(social_optimal_x[i]-x[i][j]);
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
double spread_urLo[55];            // Lower bound and upper bound for each dimension
double spread_urHi[55];            // Lower bound and upper bound for each dimension
double spread_urBufa[55];          // 每次
double tmp_spread_urLo[55];        // 缩减范围后的
double tmp_spread_urHi[55];        // 缩减范围后的


int spread_checkLegalCanshuRM( const vector<double> &vec){
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

double spread_getYueshuValueMin( const vector<double> &vec , int obj_i){
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

void spread_getBestSolutionUsingRandomFunction(spreadTemplate *obj , vector<mergeAllTradingResult> &ans,  vector<struct __zhuli_data> &zhuliData , const char *start_date , const char *end_date , int max_cycle = 100, int __rmNumParticle = 1000, int tmpSaveResultNum = 100){
    srand((unsigned)time(0));
    vector<TradingResultNode> resultList ;
    vector<canshuNode> _canshuRanges = _config.loadCanshuRange();
    int Dim = _canshuRanges.size();
    priority_queue<struct mergeAllTradingResult > Q;   //存储的
    priority_queue<struct mergeAllTradingResult > tQ;  //临时存储的最优解
    vector<double> use_canshu;
    //vector<string> filenames = getFileName(code , period);
    //obj->loadFiles(filenames , start_date , end_date , period);
    obj->loadFile(zhuliData , start_date , end_date);
    for(int i = 0;i < Dim;i++){
        spread_urLo[i] = _canshuRanges[i].l;  spread_urHi[i] = _canshuRanges[i].r;  spread_urBufa[i] = _canshuRanges[i].bufa;
    }
    while( max_cycle-- > 0){
        while(!tQ.empty()){
            tQ.pop();
        }
        for(int i = 0;i < __rmNumParticle ; i++){
            use_canshu.clear();
            for(int j = 0;j < Dim;j++){
                double yueshuValue = spread_getYueshuValueMin(use_canshu , j );
                double v = spread_randNext( max(spread_urLo[j] , yueshuValue),spread_urHi[j]);
                use_canshu.push_back(v);
            }

            // obj->init(code);
            obj->init();
            obj->setCanshuFromVec(use_canshu);
            obj->run();

            //struct mergeAllTradingResult tnode = obj->getTradingResult(code , period ,start_date , end_date);
            struct mergeAllTradingResult tnode = obj->getTradingResultList( resultList );


            if(spread_checkLegalCanshuRM(tnode.canshuSet) == 1){
                spread_push_result(tQ , tnode);
                while(tQ.size() > tmpSaveResultNum){
                    tQ.pop();
                }
                spread_push_result(Q , tnode);
                while(Q.size() > MaxSaveResultNum){
                    Q.pop();
                }
            }
        }
        // 开始缩减范围
        if(tQ.size() > 0){
            struct mergeAllTradingResult tnode = Q.top();
            for(int i = 0;i < Dim;i++){
                tmp_spread_urLo[i] = tmp_spread_urHi[i] = tnode.canshuSet[i];
            }
            while(!tQ.empty()){
                tnode = tQ.top();
                for(int i = 0;i < Dim;i++){
                    tmp_spread_urLo[i] = min(tmp_spread_urLo[i] , tnode.canshuSet[i]);
                    tmp_spread_urHi[i] = max(tmp_spread_urHi[i] , tnode.canshuSet[i]);
                }
                tQ.pop();
            }
            int change_flag = 0;
            for(int i = 0;i < Dim;i++){
                if( tmp_spread_urLo[i] > spread_urLo[i]){
                    change_flag = 1;
                    spread_urLo[i] = tmp_spread_urLo[i];
                }
                if( tmp_spread_urHi[i] < spread_urHi[i]){
                    change_flag = 1;
                    spread_urHi[i] = tmp_spread_urHi[i];
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
bool spread_hash_priority_queue_engine[MOD_VAL + 10];
double spread_bpLo[55];            // Lower bound and upper bound for each dimension
double spread_bpHi[55];            // Lower bound and upper bound for each dimension
double spread_bpBufa[55];          // 每次
//priority_queue< TradingResultNode> bpPriResult; // 存储的优先队列结果

int spread_bpLegalCanshuSet(const vector<double> _v){
    int val = 0.0;
    for(int i = 0;i < _v.size();i++){
        val = val * 10 % MOD_VAL + i * _v[i];
        val %= MOD_VAL;
    }
    //printf("val = %d\n",val);
    return (int)val;
}
void spread_getBestSolutionUsingPriority(spreadTemplate * obj,  vector<mergeAllTradingResult> &ans , vector<struct __zhuli_data> &zhuliData, const char *start_date , const char *end_date , int max_cycle , int __bpNumParticle){
    vector<TradingResultNode> resultList ;
    vector<canshuNode> _canshuRanges = _config.loadCanshuRange();
    int Dim = _canshuRanges.size();
    memset(spread_hash_priority_queue_engine , 0, sizeof(spread_hash_priority_queue_engine));

    // vector<string> filenames = getFileName(code , period);
    // obj->loadFiles(filenames , start_date , end_date , period);
    obj->loadFile(zhuliData , start_date , end_date) ;
    ans.clear();

    struct mergeAllTradingResult maxResultObj ;
    priority_queue<struct mergeAllTradingResult > Q;
    int first_deal = 0;

    vector<double> use_canshu;
    for(int i = 0;i < Dim;i++){
        spread_bpLo[i] = _canshuRanges[i].l;  spread_bpHi[i] = _canshuRanges[i].r;  spread_bpBufa[i] = _canshuRanges[i].bufa;
    }
    for(int i = 0; i < __bpNumParticle; i++){
        use_canshu.clear();
        for(int j = 0;j < Dim;j++){
            double v = spread_randNext(spread_bpLo[j],spread_bpHi[j]);
            use_canshu.push_back(v);
        }
        //obj->init(code);
        obj->init();
        obj->setCanshuFromVec(use_canshu);
        obj->run();

        //struct mergeAllTradingResult tnode = obj->getTradingResult(code , period ,start_date , end_date);
        struct mergeAllTradingResult tnode = obj->getTradingResultList( resultList );
        if(spread_checkLegalCanshu(tnode.canshuSet) == 1){
            spread_push_result(Q , tnode );
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
        //spread_push_result(Q , tnode );
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
            double vv = tmp_n1[i] -= spread_bpBufa[i] * spread_randNext( 0 , 4);
            double uu = tmp_n1[i] += spread_bpBufa[i] * spread_randNext( 0 , 4);
            if( vv > spread_bpHi[i]){
                vv = spread_bpHi[i];
            }if(vv < spread_bpLo[i]){
                vv = spread_bpLo[i];
            }
            tmp_n1[i] = vv;

            if(uu > spread_bpHi[i]){
                uu = spread_bpHi[i];
            }if(uu < spread_bpLo[i]){
                uu = spread_bpLo[i];
            }tmp_n2[i] = uu;
            nCanshu.push_back(tmp_n1);
            nCanshu.push_back(tmp_n2);
        }
        int flag_has_new_answer = 0;
        for(int i = 0;i < nCanshu.size();i++){
            int vv = spread_bpLegalCanshuSet(nCanshu[i]);
            if( 0 == spread_hash_priority_queue_engine[vv]){
                //obj->init(code);
                obj->init();
                obj->setCanshuFromVec(use_canshu);
                obj->run();
                //struct TradingResultNode tnode = obj->getTradingResult(code , period ,start_date , end_date);
                struct mergeAllTradingResult tnode = obj->getTradingResultList(resultList);
                if(spread_checkLegalCanshu(tnode.canshuSet) == 1){
                    spread_push_result(Q , tnode );
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
                spread_hash_priority_queue_engine[vv] = 1;
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
