#ifndef _cta_template
#define _cta_template

#include "talib.h"
#include <set>
#include <queue>

class ctaTemplate {

public:
    string ttperiod;
    vector<string> g_datetime , g_date , g_time;
    vector<string> qg_datetime , qg_date , qg_time;

    vector<double> closeArray , openArray , highArray , lowArray , volArray;
    vector<double> qcloseArray , qopenArray , qhighArray , qlowArray , qVolArray;

    set<string> zhuli_qie_dates;

    int tradingNum ;

    vector<struct tradeNode> tradingResult;
    queue<struct tradeNode> queue_trading_unFinished;

    int pos;
    vector<int> mp;                     // 总持仓
    vector<double> free_money , total_money;// 空闲资金 ,, 总资金 (空闲资金 + 持仓)
    vector<double> dayAccount;          // 每日的空闲资金+持仓的值
    vector<double> dayIncome;           // 每日赚到的钱
    vector<double> chi_price;           //持仓价
    vector<double> chu_price;           //出场价
    vector<double> ru_price;            //入场价
    vector<double> weekAccount;
    vector<double> weekIncome;
    vector<double> bzjAccount;          //保证金价格
    vector<int> positionAccount;        //每日的收盘持仓
    int nn_week;

    double kai_huan ;
    double ping_huan ;


    int BarsSinceEntry;                 // 账户是否进入

    double slippage , rate , perSize , minMovement , PriceScale , cash_deposit;
    double n_slippages;                 // 设置几个滑点
    int fee_type ;                      // fee_type == 1,表示手续费是万分之多少, fee_type == 2, 表示手续费是每手多少元

    int n , qn;
    int j_day;                         //计算总共有几个交易日的

    //////  账户信息栏
    int account_money ;   // 账户初始资金
    vector<string> vecTradeDay;
    vector<int> jiaoyi_week_num;
    vector<string> vecTradeWeekDay;

    vector<string> vecPchy;

    int lots;

    double __high_history_price;           //历史最高收盘价

    string strategyName;

    pair<int,string> loopForceCanshu;      // (loopForceCanshuSize, loopForceCanshuFileName) , 一个是参数的总数，一个是所有需要跑的参数的数量

    //FILE * fp_dd;

    /**
     *  这里提供一种暴力循环获得 所有参数的例子
     *  本来应该使用递归的方式做的, 但后来想了一下，， 还是用这种来做吧， 毕竟简单，可以添加各种条件约束
     **/
    virtual string getCanshuSetNamesString(){
        vector<string> vCanshu = this->getCanshuSetNames();
        string ret("");
        for(int i = 0;i < vCanshu.size();i++){
            if(i == 0){
                ret = ret + vCanshu[i];
            }else{
                ret = ret + "," + vCanshu[i];
            }
        }
        return ret;
    }

    virtual void loopForce(){
        printf("Now in ctaTemplate loopForce\n");
    }
    
    virtual const pair<int,string> &getLoopForceInfo(){
        if( 0 == this->loopForceCanshu.first){
            this->loopForce();
        }
        return loopForceCanshu;
    }

    virtual vector<string> getCanshuSetNames(){
        vector<string> vCanshu; vCanshu.clear();
        return vCanshu;
    }
    virtual void loadFiles(const vector<string> &vec_files , const char *start_date , const char *end_date , const string __ttperiod){
        //fp_dd = fopen("debug1.csv","w");
        this->ttperiod = __ttperiod;
        this->loopForceCanshu = make_pair(0 , "");
        if( 3 == use_yongan_data){                   // 加载股票板块指数的行情
            this->loadStockPlateSingle( vec_files[0].c_str() , start_date , end_date);       // 因为版块数据跟个股差不多，所以加载函数基本一样
        }else if( 2 == use_yongan_data){             // 加载股票的行情
            this->loadStockSingle( vec_files[0].c_str() , start_date , end_date);
        }else if( 1 == use_yongan_data){
            this->loadFileZhuli(vec_files[0].c_str() , start_date , end_date);      //加载永安的主力合约
            this->loadFileQianZhuli(vec_files[1].c_str() , start_date , end_date);  //加载永安的前主力合约
            this->loadZhuliQiehuan(vec_files[4].c_str() , start_date , end_date);   //加载永安的主力切换日期
        }else{
            this->loadFileZhuli(vec_files[0].c_str() , start_date , end_date);   //加载TB的888合约
        }
    }
    /*
     *
    2005/01/04 15:00:00,2.47715656943,2.4991504472,2.51286648212,2.43087260435,2.43773062181,523051
    2005/01/05 15:00:00,2.43773062181,2.45144665674,2.51029855197,2.44458863927,2.5034405345,479559
    2005/01/06 15:00:00,2.5034405345,2.48401458689,2.55516269166,2.46458863927,2.55202070912,1580033
    2005/01/07 15:00:00,2.55202070912,2.55202070912,2.62573674404,2.54516269166,2.60688484881,4202905
     */
    virtual void loadStockPlateSingle( const char *filename , const char *start_date , const char *end_date){
        printf("StockPlateDay %s%s%s\n" , filename , start_date , end_date);
        FILE *fp = NULL;
        __high_history_price = 0;
        if( (fp = fopen(filename , "r")) != NULL){
            n = 0;
            g_datetime.clear() , g_date.clear() , g_time.clear();
            g_datetime.clear(),g_date.clear(),g_time.clear();
            openArray.clear(),highArray.clear(),lowArray.clear(),closeArray.clear(); volArray.clear();
            char tdate[50],tdatetime[100],ttime[50];
            double preclose,open,high,low,close,volume;

            while( fscanf(fp , "%s %8s,%lf,%lf,%lf,%lf,%lf,%lf" , tdate ,ttime , &preclose , &open, &high , &low, &close ,&volume) != EOF){
                if(strcmp(start_date , tdate) <= 0 && strcmp(tdate , end_date) <= 0){
                    sprintf(tdatetime, "%s %s",tdate,ttime);
                    g_datetime.push_back(string(tdatetime));
                    g_date.push_back(tdate);  g_time.push_back(ttime);

                    openArray.push_back(open); highArray.push_back(high);
                    lowArray.push_back(low);  closeArray.push_back(close);
                    volArray.push_back(volume);

                    __high_history_price = max(__high_history_price , high);
                    n = n + 1;

                }
            }
            fclose(fp);

            vecTradeDay.clear();  vecTradeWeekDay.clear(); jiaoyi_week_num.clear();

            int day_week_beg;
            day_week_beg = 0;
            for(int i = 0;i < n;i++){
                if( i == n-1 ||
                   ( i < n-1 && strcmp(g_time[i].c_str() , "15:31:00") <= 0 && strcmp(g_time[i+1].c_str() , "15:31:00") >= 0) ||
                   ( strcmp(g_time[i].c_str() , "15:31:00") <= 0 && strcmp(g_time[i].c_str() , g_time[i+1].c_str() ) >= 0 ))
                {
                    //printf("g_date %s\n",g_date[i].c_str());
                    vecTradeDay.push_back(string(g_date[i]));
                    int day_num;
                    day_num = datenum(vecTradeDay[vecTradeDay.size()-1].c_str());
                    if (1 == vecTradeDay.size())
                    {
                        day_week_beg = day_num - CaculateWeekDayS(g_date[i].c_str());
                    }

                    jiaoyi_week_num.push_back((day_num - day_week_beg )/7);
                };
            }
            for (int i = 1 ;i < vecTradeDay.size() ;i++)
            {
                if (jiaoyi_week_num[i-1] != jiaoyi_week_num[i])
                {
                    vecTradeWeekDay.push_back(vecTradeDay[i-1]);
                }
            }
            if( vecTradeDay.size() > 0){
                vecTradeWeekDay.push_back(vecTradeDay[vecTradeDay.size()-1]);
            }
            printf("num of file data: %d\n",n);
        }else{
            printf("open zhuliqiehuan file %s Error!\n",filename);
        }
    }
    /*
    2005/01/04 15:00:00,22.93718948,23.49391738,22.77017111,23.3825718,51462.4828395
    2005/01/05 15:00:00,23.49391738,23.82795412,23.15988064,23.71660854,52695.2214897
    2005/01/06 15:00:00,23.77228133,23.9392997,23.3825718,23.60526296,46268.3835317
    2005/01/07 15:00:00,23.60526296,25.94352014,23.49391738,25.94352014,431298.664931
    2005/01/10 15:00:00,27.16832152,28.11475895,26.38890246,27.22399431,2235345.30962
    2005/01/11 15:00:00,27.22399431,28.50446848,27.11264873,27.55803105,1554604.14324
    */
    virtual void loadStockSingle( const char *filename , const char *start_date , const char *end_date){
        printf("StockSingleDay %s%s%s\n" , filename , start_date , end_date);
        FILE *fp = NULL;
        __high_history_price = 0;
        if( (fp = fopen(filename , "r")) != NULL){
            n = 0;
            g_datetime.clear() , g_date.clear() , g_time.clear();
            g_datetime.clear(),g_date.clear(),g_time.clear();
            openArray.clear(),highArray.clear(),lowArray.clear(),closeArray.clear(); volArray.clear();
            char tdate[50],tdatetime[100],ttime[50];
            double open,high,low,close,volume;

            while( fscanf(fp , "%s %8s,%lf,%lf,%lf,%lf,%lf" , tdate ,ttime , &open, &high , &low, &close ,&volume) != EOF){
                if(strcmp(start_date , tdate) <= 0 && strcmp(tdate , end_date) <= 0){
                    sprintf(tdatetime, "%s %s",tdate,ttime);
                    g_datetime.push_back(string(tdatetime));
                    g_date.push_back(tdate);  g_time.push_back(ttime);

                    openArray.push_back(open); highArray.push_back(high);
                    lowArray.push_back(low);  closeArray.push_back(close);
                    volArray.push_back(volume);

                    __high_history_price = max(__high_history_price , high);
                    n = n + 1;

                }
            }
            fclose(fp);

            vecTradeDay.clear();  vecTradeWeekDay.clear(); jiaoyi_week_num.clear();

            int day_week_beg;
            day_week_beg = 0;
            for(int i = 0;i < n;i++){
                if( i == n-1 ||
                   ( i < n-1 && strcmp(g_time[i].c_str() , "15:31:00") <= 0 && strcmp(g_time[i+1].c_str() , "15:31:00") >= 0) ||
                   ( strcmp(g_time[i].c_str() , "15:31:00") <= 0 && strcmp(g_time[i].c_str() , g_time[i+1].c_str() ) >= 0 ))
                {
                    //printf("g_date %s\n",g_date[i].c_str());
                    vecTradeDay.push_back(string(g_date[i]));
                    int day_num;
                    day_num = datenum(vecTradeDay[vecTradeDay.size()-1].c_str());
                    if (1 == vecTradeDay.size())
                    {
                        day_week_beg = day_num - CaculateWeekDayS(g_date[i].c_str());
                    }

                    jiaoyi_week_num.push_back((day_num - day_week_beg )/7);
                };
            }
            for (int i = 1 ;i < vecTradeDay.size() ;i++)
            {
                if (jiaoyi_week_num[i-1] != jiaoyi_week_num[i])
                {
                    vecTradeWeekDay.push_back(vecTradeDay[i-1]);
                }
            }
            if( vecTradeDay.size() > 0){
                vecTradeWeekDay.push_back(vecTradeDay[vecTradeDay.size()-1]);
            }
            printf("num of file data: %d\n",n);
        }else{
            printf("open zhuliqiehuan file %s Error!\n",filename);
        }
    }
    /*
            2004-01-02,zhuli_888,2004-01-02,,ax05
            2004-01-05,zhuli_888,2004-01-05,ax05,ax05
            2004-01-06,zhuli_888,2004-01-06,ax05,ax05
            2004-01-07,zhuli_888,2004-01-07,ax05,ax05
    */
    virtual void loadZhuliQiehuan(const char *filename, const char* start_date , const char * end_date){
        printf("zhuliqiehuan %s%s%s\n", filename , start_date , end_date);
        FILE *fp = NULL;
        if( (fp = fopen(filename , "r")) != NULL){
            char line[500];
            int zz = 0;
            zhuli_qie_dates.clear();
            while( fscanf(fp , "%s" , line) != EOF){
                vector<string> ulines = split(line , ",");
                if( ulines[3].compare(ulines[4]) != 0){
                    zz += 1;
                    ulines[2] = replaceDateXiegang(ulines[2]);
                    zhuli_qie_dates.insert( ulines[2]);
                    //printf("%s,%s,%s,%s,%s\n",ulines[0].c_str(),ulines[1].c_str(),ulines[2].c_str(),ulines[3].c_str(),ulines[4].c_str());
                }
            }
            printf("total_need care qiehuan day:%d\n",zz);
            fclose(fp);
        }else{
            printf("open zhuliqiehuan file %s Error!\n",filename);
        }
    }
    virtual void loadFileQianZhuli(const char *filename , const char *start_date , const char *end_date){
        printf("qianzhuli %s%s%s\n", filename , start_date , end_date);
        FILE *fp = NULL;
        if( (fp = fopen(filename , "r")) != NULL){
            qn = 0;
            qg_datetime.clear(), qg_date.clear() , qg_time.clear();
            qopenArray.clear(), qhighArray.clear() , qlowArray.clear() , qcloseArray.clear();  qVolArray.clear();
            char tdate[50],ttime[50] ,tdatetime[100];
            double open,high,low,close,volume,openInterest;
            while(fscanf(fp,"%s %8s,%lf,%lf,%lf,%lf,%lf,%lf", tdate,ttime, &open,&high,&low,&close,&volume,&openInterest) != EOF){
                if(strcmp(start_date , tdate) <= 0 && strcmp(tdate , end_date) <= 0){
                    sprintf(tdatetime, "%s %s",tdate,ttime);
                    qg_datetime.push_back(string(tdatetime));
                    qg_date.push_back(tdate); qg_time.push_back(ttime);

                    qopenArray.push_back(open); qhighArray.push_back(high);
                    qlowArray.push_back(low);  qcloseArray.push_back(close);

                    qVolArray.push_back(volume);

                    qn++;
                }
            }
            fclose(fp);
            printf("num of qianzhuli file data: %d\n",qn);
        }else{
            printf("open qianzhuli file %s Error!\n" , filename);
        }
    }
    //读取输入的数据
    virtual void loadFileZhuli(const char *filename , const char *start_date , const char *end_date ){
        printf("zhuli %s%s%s\n",filename , start_date , end_date);
        FILE *fp = NULL;
        __high_history_price = 0;
        if( (fp = fopen(filename, "r")) != NULL){
            n = 0;
            g_datetime.clear(),g_date.clear(),g_time.clear();
            openArray.clear(),highArray.clear(),lowArray.clear(),closeArray.clear(); volArray.clear();
            char tdate[50],ttime[50] ,tdatetime[100];
            double open,high,low,close,volume,openInterest;

            while(fscanf(fp,"%s %8s,%lf,%lf,%lf,%lf,%lf,%lf", tdate,ttime, &open,&high,&low,&close,&volume,&openInterest) != EOF){
                if(strcmp(start_date , tdate) <= 0 && strcmp(tdate , end_date) <= 0){
                    //printf("st%s\n",tdate);
                    sprintf(tdatetime, "%s %s",tdate,ttime);
                    g_datetime.push_back(string(tdatetime));
                    g_date.push_back(tdate);  g_time.push_back(ttime);

                    openArray.push_back(open); highArray.push_back(high);
                    lowArray.push_back(low);  closeArray.push_back(close);
                    volArray.push_back(volume);

                    __high_history_price = max(__high_history_price , high);
                    n = n + 1;
                }
            }
            fclose(fp);

            vecTradeDay.clear();  vecTradeWeekDay.clear(); jiaoyi_week_num.clear();

            int day_week_beg;
            day_week_beg = 0;
            for(int i = 0;i < n;i++){
                //printf("%s %d %s %s\n",g_time[i].c_str() , bar_endtime.first ,bar_endtime.second.c_str() , this->ttperiod.c_str() );
                //if( 0 == strcmp(g_time[i].c_str() , bar_endtime.second.c_str() ) )
                if( i == n-1 ||
                   ( i < n-1 && strcmp(g_time[i].c_str() , "15:31:00") <= 0 && strcmp(g_time[i+1].c_str() , "15:31:00") >= 0) ||
                   ( strcmp(g_time[i].c_str() , "15:31:00") <= 0 && strcmp(g_time[i].c_str() , g_time[i+1].c_str() ) >= 0 ))
                {
                    vecTradeDay.push_back(string(g_date[i]));
                    int day_num;
                    day_num = datenum(vecTradeDay[vecTradeDay.size()-1].c_str());
                    if (1 == vecTradeDay.size())
                    {
                        day_week_beg = day_num - CaculateWeekDayS(g_date[i].c_str());
                    }

                    jiaoyi_week_num.push_back((day_num - day_week_beg )/7);
                };
            }
            //printf("s2 vecTradeDay.size()-1 = %d vecTradeDay.size() %d jiaoyi_week_num.size %d\n",vecTradeDay.size()-1 , vecTradeDay.size(),jiaoyi_week_num.size());
            for (int i = 1 ;i < vecTradeDay.size() ;i++)
            {
                if (jiaoyi_week_num[i-1] != jiaoyi_week_num[i])
                {
                    vecTradeWeekDay.push_back(vecTradeDay[i-1]);
                    // printf("vecTradeWeekDay[%d]=%s\n",i,vecTradeWeekDay[vecTradeWeekDay.size()-1].c_str());
                }
            }
            if( vecTradeDay.size() > 0){
                vecTradeWeekDay.push_back(vecTradeDay[vecTradeDay.size()-1]);
            }
            // if (jiaoyi_week_num[vecTradeDay.size()-1] != jiaoyi_week_num[vecTradeDay.size()-2])
            // {
            //     vecTradeWeekDay.push_back(vecTradeDay[vecTradeDay.size()-1]);
            // }
            // printf("s3\n");
            printf("num of file data: %d\n",n);
        }else{
            printf("open zhuli file %s Error!\n" , filename);
        }
    }
    /**
     * 获得手续费
     **/
    virtual double getFee(int size , double price){
        if( fee_type == 1){         // 表示万分之多少的方式
            return price * perSize * size * rate + slippage * perSize * size * n_slippages;
        }else if(fee_type == 2){    // 表示每手多少元的方式 , 这时候rate就是元了
            return size * rate + slippage * perSize * size * n_slippages;
        }
        return 0.0;
    }
    virtual void Sell(int size , double price , int iBar){
        if(pos > 0){
            pos = pos - min( size , abs(pos));
            #ifdef debug
                printf("))pos %d sell %s %lf tradingNum %d\n", pos ,g_datetime[iBar].c_str() , price, tradingNum);
            #endif
            double win_money = 0.0;
            while( !queue_trading_unFinished.empty() && size > 0){
                tradeNode * p_tnode = &(queue_trading_unFinished.front());
                tradeNode new_node = tradeNode();
                strcpy( new_node.entryTime , p_tnode->entryTime);
                strcpy( new_node.exitTime , g_datetime[iBar].c_str());
                new_node.entryPrice = p_tnode->entryPrice;
                new_node.exitPrice = legalTradePrice(price , -1.0 , minMovement);
                if( abs(p_tnode->volume) <= size){
                    new_node.volume = 1 * abs(p_tnode->volume);
                    size -= abs(p_tnode->volume);
                    queue_trading_unFinished.pop();   // 一笔交易完成了，所以删除掉队列中的未完成的
                }else if( abs(p_tnode->volume) > size ){
                    // 多单太多, 这个节点还没有用完
                    new_node.volume = 1 * size;
                    p_tnode->volume = 1 * ( abs(p_tnode->volume) - size);
                    size = 0;
                }
                tradingResult.push_back( new_node );
                tradingNum += 1;
                win_money += (new_node.exitPrice - new_node.entryPrice) * perSize * new_node.volume;
            }

            if(pos == 0){
                BarsSinceEntry = 0;
            }
            chu_price[iBar] = legalTradePrice(price, -1.0 , minMovement);
            //printf("%d ,%s , %d\n", iBar,"sp" , size);
        }

    }
    virtual void BuyToCover(int size , double price , int iBar){
        if(pos < 0){
            pos = pos + min( abs(pos),size);
            #ifdef debug
                printf("}}pos %d buytocover %s %lf tradingNum %d\n", pos ,g_datetime[iBar].c_str()  , price, tradingNum);
            #endif

            double win_money = 0.0;
            while(!queue_trading_unFinished.empty() && size > 0){
                tradeNode * p_tnode = &(queue_trading_unFinished.front());
                tradeNode new_node = tradeNode();
                strcpy( new_node.entryTime , p_tnode->entryTime);
                strcpy( new_node.exitTime , g_datetime[iBar].c_str());
                new_node.entryPrice = p_tnode->entryPrice;
                new_node.exitPrice = legalTradePrice(price , 1.0 , minMovement);
                if( abs(p_tnode->volume) <= size){
                    new_node.volume = -1 * abs(p_tnode->volume);
                    size -= abs(p_tnode->volume);
                    queue_trading_unFinished.pop();   // 一笔交易完成了，所以删除掉队列中的未完成的
                }else if( abs(p_tnode->volume) > size){
                    // 空单太多, 这个节点还没有用完
                    new_node.volume = -1 * size;
                    p_tnode->volume = -1 * ( abs(p_tnode->volume) - size);
                    size = 0;
                }
                tradingResult.push_back( new_node );
                tradingNum += 1;
                win_money += (new_node.exitPrice - new_node.entryPrice) * perSize * new_node.volume;
            }

            free_money[iBar] = free_money[iBar] + size * price - getFee( min(size,abs(pos)) , price) + win_money;
            if(pos == 0){
                BarsSinceEntry = 0;
            }
            chu_price[iBar] = legalTradePrice(price , 1.0 , minMovement);
        }


    }
    virtual void Buy(int size , double price , int iBar){
        int pos_res = pos;
        if(pos <= 0){
            BarsSinceEntry = 0;
        }
        if(pos < 0){
            BuyToCover( abs(pos) , price, iBar);
        }
        pos = pos + size;
        #ifdef debug
            printf("((pos %d buy %s %lf tradingNum %d\n", pos ,g_datetime[iBar].c_str()  , price, tradingNum);
        #endif
        tradeNode new_node = tradeNode();
        strcpy( new_node.entryTime , g_datetime[iBar].c_str());
        new_node.volume = size;
        new_node.entryPrice = legalTradePrice(price, 1.0 , minMovement);
        queue_trading_unFinished.push(new_node);

        ru_price[iBar] = legalTradePrice(price, 1.0 , minMovement);
        if (pos_res<=0)
        {
            chi_price[iBar] = legalTradePrice(price, 1.0 , minMovement);
        }
        else
        {
            chi_price[iBar] = (chi_price[iBar ] * fabs(pos_res) + legalTradePrice(price, 1.0 , minMovement) * fabs(size)) / fabs(pos);
        }

        //printf("%d ,%s , %d\n", iBar,"bk" , size);

    }
    virtual void SellShort(int size , double price , int iBar){
        int pos_res = pos;
        if(pos >= 0){
            BarsSinceEntry = 0;
        }
        if(pos > 0){
            Sell( fabs(pos) , price , iBar);
        }
        pos = pos - size;
        #ifdef debug
            printf("{{pos %d sellshort %s %lf tradingNum %d\n", pos ,g_datetime[iBar].c_str()  , price, tradingNum);
        #endif
        tradeNode new_node = tradeNode();
        strcpy( new_node.entryTime , g_datetime[iBar].c_str());
        new_node.volume = -size;
        new_node.entryPrice = legalTradePrice(price, -1.0 , minMovement);
        queue_trading_unFinished.push(new_node);

        ru_price[iBar] = legalTradePrice(price, -1.0 , minMovement);
        if (pos_res>=0)
        {
            chi_price[iBar] = legalTradePrice(price, -1.0 , minMovement);
        }
        else
        {
            chi_price[iBar] = (chi_price[iBar] * fabs(pos_res) + legalTradePrice(price, -1.0 , minMovement) * fabs(size)) / fabs(pos);
        }

    }

    // 更新barsEntry 这个东西
    virtual void updateBarsEntry(int iBar){
        if(pos != 0){
            BarsSinceEntry += 1;
        }
        if(mp[max(0,iBar-1)] != 0 && 0 == mp[iBar]){
            BarsSinceEntry = 0;
        }
    }
    virtual setStrategyName(const string &s){
        this->strategyName = string(s);
    }
    virtual const string& getStrategyName(){
        return this->strategyName;
    }
    virtual void init(){
        n_slippages = 1.0;              //默认有几跳滑点
        slippage = 1.0 , rate = 1.29/10000 , perSize = 10 , minMovement = 1.0 , PriceScale = 1.0 , cash_deposit = 11.0;
        fee_type = 1; // 手续费种类，默认是万分之多少的方式

        this->vecPchy.clear();

        mp.reserve(n + 10);
        mp.clear();
        //////  账户信息栏
        //account_money = __high_history_price * 10;                          // 账户初始资金
        if( is_zuhehuice == 1){
            account_money = __const_ini_money;
            lots = __const_ini_money / (__high_history_price );
        }else{
            account_money = __const_ini_money;
            lots = 1;
        }
        //printf("__high_history_price=%lf lots=%d\n",__high_history_price , lots);

        dayAccount.reserve(vecTradeDay.size() + 10);    // 每日总资金
        dayIncome.reserve(vecTradeDay.size() + 10);     // 每天赚到的钱
        chi_price.reserve(n + 10);
        chu_price.reserve(n + 10);
        ru_price.reserve(n+10);
        free_money.reserve(n + 10);                     // 每笔后的空闲资金
        total_money.reserve(n + 10);                     // 每笔后的总资金 (空闲资金 + 持仓)
        weekAccount.reserve(vecTradeWeekDay.size() + 10);
        weekIncome.reserve(vecTradeWeekDay.size() + 10);

        bzjAccount.reserve(vecTradeDay.size() + 10);
        positionAccount.reserve(vecTradeDay.size() + 10);


        dayAccount.clear();
        dayIncome.clear();
        chi_price.clear();
        chu_price.clear();
        ru_price.clear();
        free_money.clear();
        total_money.clear();
        weekAccount.clear();
        weekIncome.clear();

        bzjAccount.clear();
        positionAccount.clear();
    }
    /**
     * 重新赋值手续费
     */
    virtual void initFee(const char * code){
        struct feeNode o = _config.getCodeFee(code);
        if(o.minMove > 0){
            this->slippage = o.minMove;
            this->cash_deposit = o.cash_deposit;
            this->perSize = o.csize;
            this->rate = o.perMoney;
            this->fee_type = o.feeType;
            this->minMovement = o.minMove;

            if( 1 == this->fee_type ){
                this->rate = this->rate / 100.0;
            }
            //printf("code = %s , this->fee_type %d this->slippage %lf this->perSize %lf this->rate %lf\n",code , this->fee_type , this->slippage ,this->perSize,this->rate);
        }
    }
    /**
     * 使用 conf 下面的内置的函数来赋予 滑点、手续费比例等。
     ***/
    virtual void init(const char *code){
        this->init();
        this->initFee(code);
    }
    virtual void setTimeperiod(const string & t){
        this->ttperiod = t;
    }

    virtual void setSlippageAndRate( double _slippage , double _rate, double _persize,double _minMovement , double _priceScale){
        this->slippage = _slippage ; this->rate = _rate / 10000.0; this->perSize = _persize; this->minMovement = _minMovement; this->PriceScale = _priceScale;
    }
    virtual void setFeeType(int _fee_type){
        this->fee_type = _fee_type;
    }

    virtual TradingResultNode getTradingResult( const char *_code , const char *_period,const char *start_date , const char *end_date){
        TradingResultNode a = TradingResultNode( _code, _period ,start_date, end_date ,tradingResult,tradingNum , account_money ,slippage * n_slippages, rate , perSize , cash_deposit, vecTradeDay , vecTradeWeekDay , dayIncome , weekIncome , bzjAccount ,dayAccount , positionAccount ,fee_type);
        return a;
    }
    virtual void beforeRun(){
        // System Variable
        tradingNum = 0;
        tradingResult.clear();
        while( !queue_trading_unFinished.empty() ){
            queue_trading_unFinished.pop();
        }
        pos = 0;
        BarsSinceEntry = 0;
        for(int i = 0;i < n;i++){
            mp[i] = 0;
        }
        // for(int i = 0;i < vecTradeDay.size();i++){
        //     dayAccount[i] = dayIncome[i] = 0.0;
        // }
        // for(int i = 0;i < vecTradeWeekDay.size();i++){
        //     weekAccount[i] = weekIncome[i] = 0.0;
        // }

        // Account Variable

        nn_week = 0;
        j_day   = 0;
        dayAccount[0]  = account_money;
        dayIncome[0]   = 0;
        weekAccount[0] = account_money;
        weekIncome[0]   = 0;

        chi_price[0] = 0;
        chu_price[0] = 0;
        ru_price[0] = 0;

        free_money[0]  = account_money;
        total_money[0] = account_money;

    }
    virtual void afterRun(){
        // 如果还有 持仓的的话， 在最后一根 bar 平仓掉
        if(n > 0){
            while( !queue_trading_unFinished.empty()){
                tradeNode * p_tnode = &(queue_trading_unFinished.front());
                tradeNode new_node = tradeNode();
                strcpy( new_node.entryTime , p_tnode->entryTime);
                strcpy( new_node.exitTime , g_datetime[n-1].c_str());

                new_node.entryPrice = p_tnode->entryPrice;
                if( pos < 0){
                    new_node.exitPrice = legalTradePrice( closeArray[n-1] , 1.0 , minMovement);
                }else if(pos > 0){
                    new_node.exitPrice = legalTradePrice( closeArray[n-1] , -1.0 , minMovement);
                }
                new_node.volume = abs(p_tnode->volume);

                tradingResult.push_back( new_node );
                tradingNum += 1;
                queue_trading_unFinished.pop();
            }
            double tmp_inc_fee = getFee(fabs(pos) , closeArray[n-1]);
            if( dayIncome.size() > 0){
                dayIncome[dayIncome.size() - 1] = dayIncome[dayIncome.size() - 1] - tmp_inc_fee;
            }
            if( weekIncome.size() > 0){
                weekIncome[weekIncome.size() - 1] = weekIncome[weekIncome.size() - 1] - tmp_inc_fee;
            }
            if( dayAccount.size() > 0){
                dayAccount[dayAccount.size() - 1] = dayAccount[dayAccount.size() - 1] - tmp_inc_fee;
            }
            if(positionAccount.size() > 0){
                positionAccount[positionAccount.size() - 1] = 0;
            }
            pos = 0;
        }
    }

    virtual void onBar_bef( int i ){
        kai_huan = 0.0;
        ping_huan = 0.0;
        if( use_yongan_data == 1){
            if( i > 5 && g_time[i-1].compare("14:59:00") <= 0 && (g_date[i-1] +" 14:59:00").compare(g_datetime[i]) < 0  && zhuli_qie_dates.find(g_date[i]) != zhuli_qie_dates.end()){
                //printf("now qie g %s,%s %s\n",g_datetime[i].c_str(),g_date[i].c_str(),g_time[i].c_str());
                if(this->pos != 0){
                    // printf("now qie g %s,%s %s\n",g_datetime[i].c_str(),g_date[i].c_str(),g_time[i].c_str());
                    int j = i;
                    while(j < qn && qg_datetime[j].compare( g_datetime[i]) > 0 ){
                        j--;
                    }
                    while(j < qn && qg_datetime[j].compare( g_datetime[i]) < 0){
                        j++;
                    }
                    if(j < qn){
                        //printf("qg %s, g %s\n" , qg_datetime[j].c_str(), g_datetime[i].c_str());
                        //printf("%d ,%d\n", i , BarsSinceEntry);
                        int pre_use_barsincenEntry = BarsSinceEntry;
                        if(this->pos > 0 ){
                            //以前主力价格平仓
                            int tmp_pos = this->pos;
                            //printf("qc nc: %lf %lf\n", qcloseArray[j] , closeArray[i]);
                            this->Sell( abs(this->pos) , qopenArray[j] , i);
                            this->Buy( abs(tmp_pos) , openArray[i] , i);
                            kai_huan = legalTradePrice(openArray[i] , 1.0 , minMovement);
                            ping_huan = legalTradePrice(qopenArray[j] , -1.0 , minMovement);
                            //printf("%d,%d\n", i,BarsSinceEntry);
                            //printf("%d ,%lf ,%lf \n", i ,kai_huan , ping_huan );
                        }
                        else if(this->pos < 0 ){
                            int tmp_pos = this->pos;
                            //printf("qc nc: %lf %lf\n", qcloseArray[j] , closeArray[i]);
                            this->BuyToCover( abs(this->pos) , qopenArray[j] , i);
                            this->SellShort( abs(tmp_pos) , openArray[i] , i);
                            kai_huan = legalTradePrice(openArray[i] , -1.0 , minMovement);
                            ping_huan = legalTradePrice(qopenArray[j] , 1.0 , minMovement);
                            //printf("%d,%d\n", i,BarsSinceEntry);
                            //printf("%d ,%lf ,%lf \n", i ,kai_huan , ping_huan );
                        }
                        this->vecPchy.push_back( g_datetime[i] );
                        // recover barsinceEntry!
                        this->BarsSinceEntry = pre_use_barsincenEntry;
                    }
                }
            }
        }
        mp[i]               = mp[i-1];
        chi_price[i]        = chi_price[i-1];
        ru_price[i]         = ru_price[i-1];
        chu_price[i]        = chu_price[i-1];
        free_money[i]       = free_money[i-1];
        total_money[i]      = total_money[i-1];

        updateBarsEntry(i);  // 更新持仓
    }

    virtual void onBar_after(int i){


        // double win_money = 0.0;
        // if(pos != 0){
        //     win_money = pos * (closeArray[i] - tradingResult[tradingNum].entryPrice) * perSize ;
        // }
        // total_money[i] = free_money[i] + abs(pos) * closeArray[i] + win_money;
        // if( strcmp(g_date[i].c_str() , g_date[i-1].c_str()) != 0){
        //     //printf("%lf\n",dayAccount[j_day-1]);
        //     j_day += 1;
        // }
        // dayAccount[j_day] = total_money[i];
        // if(j_day > 0){
        //     dayIncome[j_day] = dayAccount[j_day] - dayAccount[j_day-1];
        // }else{
        //     dayIncome[j_day] = dayAccount[j_day] - account_money;
        // }
        mp[i] = pos;

        // if( 0 == pos || mp[i-1] != mp[i]){
        //     BarsSinceEntry = 0;
        // }
        if (0 == mp[i])
        {
            chi_price[i] = 0;
        }
        if ((0 !=mp[i] && mp[i] == mp[i-1] ) || ( 0 != mp[i] && mp[i] * mp[i-1] >0 && fabs(mp[i]) < fabs(mp[i-1])))
        {
            chi_price[i] = closeArray[i-1];
        }
        if (( 0 != mp[i] &&  mp[i] == mp[i-1] ) || (0 != mp[i] && 0 == mp[i-1]) || ( 0 != mp[i] && mp[i] * mp[i-1] >0 && fabs(mp[i]) > fabs(mp[i-1])))
        {
            chu_price[i] = closeArray[i];
        }
        if (0 == mp[i] && 0 == mp[i-1])
        {
            chu_price[i] = 0;
        }


        double fuying_1;
        double fei_1;
        double bzj_1;
        fuying_1 = 0.0;
        fei_1 = 0.0;
        bzj_1 = 0.0;
        if( ( 0 != mp[i] && mp[i] == mp[i-1]) || (0 != mp[i] && 0 == mp[i-1]) || ( 0 != mp[i] && mp[i] * mp[i-1] >0 ))//持仓中 or  新开仓 or 加仓 or 减仓
        {
            //fuying_1 = (chu_price[i] - chi_price[i]) * mp[i] * perSize + (ping_huan - kai_huan) * mp[i] * perSize;
            fuying_1 = (chu_price[i] - chi_price[i]) * mp[i] * perSize;
            if(0 == mp[i-1])//新开仓
            {
                fei_1 = getFee(fabs(mp[i]),chi_price[i]);
            }
            else
            {
                if( kai_huan > 0 )
                {
                    if (mp[i] == mp[i-1])//持仓中
                    {
                        fei_1 = getFee(fabs(mp[i]) , kai_huan) + getFee(fabs(mp[i]), ping_huan);
                        fuying_1 = (closeArray[i] - closeArray[i-1]) * mp[i] * perSize + (ping_huan - kai_huan) * mp[i] * perSize;
                    }
                    else
                    {
                        if (fabs(mp[i]) > fabs(mp[i-1]))//加仓
                        {
                            fei_1 = getFee(fabs(mp[i-1]) , kai_huan) + getFee(fabs(mp[i-1]), ping_huan) + getFee(fabs(mp[i-1]-mp[i]), ru_price[i]);
                            fuying_1 = (closeArray[i] - closeArray[i-1]) * mp[i-1] * perSize + (ping_huan - kai_huan) * mp[i-1] * perSize + (closeArray[i] - ru_price[i]) * (mp[i]-mp[i-1]) * perSize ;
                        }
                        else//减仓
                        {
                            fei_1 = getFee(fabs(mp[i-1]) , kai_huan) + getFee(fabs(mp[i-1]), ping_huan) + getFee(fabs(mp[i-1]-mp[i]), chu_price[i]);
                            fuying_1 = (ping_huan - closeArray[i-1]) * mp[i-1] * perSize + (closeArray[i] - kai_huan) * mp[i] * perSize + (chu_price[i] - kai_huan) * (mp[i-1]-mp[i]) * perSize ;
                        }

                    }

                }
                else
                {
                    if (mp[i] == mp[i-1])//持仓中
                    {
                        fei_1 = 0;
                    }
                    else
                    {
                        if (fabs(mp[i]) > fabs(mp[i-1]))//加仓
                        {
                            fei_1 = getFee(fabs(mp[i-1]-mp[i]), ru_price[i]);
                        }
                        else//减仓
                        {
                            fei_1 = getFee(fabs(mp[i-1]-mp[i]), chu_price[i]);
                            fuying_1 = (closeArray[i] - chi_price[i]) * mp[i] * perSize + (chu_price[i] - chi_price[i]) * (mp[i-1]-mp[i]) * perSize ;
                        }

                    }

                }
            }
        }
        if (0 != mp[i] && 0!=mp[i-1] && mp[i] * mp[i-1]<0)//反手开仓
        {
            if( kai_huan > 0)
            {
                fuying_1 = (ping_huan - closeArray[i-1]) * mp[i-1] * perSize + (chu_price[i] - kai_huan) * mp[i-1] * perSize  + (closeArray[i] - chi_price[i]) * mp[i] * perSize;
                fei_1 = getFee(fabs(mp[i]),chi_price[i]) + getFee(fabs(mp[i-1]),chu_price[i]) + getFee(fabs(mp[i-1]),kai_huan) +getFee(fabs(mp[i-1]),ping_huan);
            }
            else
            {
                fuying_1 = (chu_price[i] - closeArray[i-1]) * mp[i-1] *perSize + (closeArray[i] - chi_price[i]) * mp[i] * perSize;
                fei_1 = getFee(fabs(mp[i]),chi_price[i]) + getFee(fabs(mp[i-1]),chu_price[i]);
            }
        }
        if (0 == mp[i] && 0 != mp[i-1])//平仓
        {
            if( kai_huan > 0)
            {
                fuying_1 = (ping_huan - closeArray[i-1]) * mp[i-1] * perSize + (chu_price[i] - kai_huan) * mp[i-1] * perSize;
                fei_1 = getFee(fabs(mp[i-1]),ping_huan) + getFee(fabs(mp[i-1]),kai_huan) + getFee(fabs(mp[i-1]),chu_price[i]);
            }
            else
            {
                fuying_1 = (chu_price[i] - closeArray[i-1]) * mp[i-1] * perSize;
                fei_1 = getFee(fabs(mp[i-1]),chu_price[i]);
            }

        }
        if (0 == mp[i] && 0 == mp[i-1])//无持仓
        {
            fuying_1 = 0;
            fei_1 = 0;
        }

        if (0 != mp[i] || ( 0 == mp[i] && 0 != mp[i-1]))
        {
            bzj_1 = closeArray[i] * perSize * max(fabs(mp[i]),fabs(mp[i-1])) * cash_deposit * 1.0/100;
        }
        else
        {
            bzj_1 = 0;
        }

        total_money[i] = total_money[i] + fuying_1 - fei_1;
        free_money[i] = total_money[i] - bzj_1;

        fprintf( fp_dd , "%d , %s , %lf , %d, %lf ,%lf , %lf , %lf , %lf , %lf , %lf , \n", i ,g_datetime[i].c_str(), closeArray[i],mp[i], chi_price[i] , ru_price[i], chu_price[i] , fuying_1 , fei_1  , kai_huan , ping_huan);

        //pair<int , string> bar_endtime = Bar_end_time(this->ttperiod);

        // printf("after 33 %s %d %s\n",g_datetime[i].c_str() , bar_endtime.first , bar_endtime.second.c_str());
        //if( 0 == strcmp(g_time[i].c_str() , bar_endtime.second.c_str() ) )
        if( i == n-1 ||
          ( i < n-1 && strcmp(g_time[i].c_str() , "15:31:00") <= 0 && strcmp(g_time[i+1].c_str() , "15:31:00") >= 0) ||
          ( strcmp(g_time[i].c_str() , "15:31:00") <= 0 && strcmp(g_time[i].c_str() , g_time[i+1].c_str() ) >= 0 ))
        {
            dayAccount.push_back(total_money[i]);
            bzjAccount.push_back(bzj_1);                     // 保留保证金的数据
            positionAccount.push_back(pos);                  // 保留每日的持仓数据
            if (1 == dayAccount.size())
            {
                dayIncome.push_back(total_money[i]-account_money);
            }
            else
            {
                dayIncome.push_back(dayAccount[dayAccount.size()-1]-dayAccount[dayAccount.size()-2]);
            }
            //printf("%d , %s, %lf , %lf \n", i , g_date[i].c_str() , dayAccount[dayAccount.size()-1], dayIncome[dayIncome.size()-1]);

            // printf("g_date[%d]=%s nn_week=%d vecTradeDay.size %d %s\n",i,g_date[i].c_str(),nn_week , vecTradeDay.size() , vecTradeWeekDay[nn_week].c_str());

            // if(nn_week >= vecTradeWeekDay.size()){
            //     printf("duo yi zhou %s tradeweek %s\n",g_date[i].c_str() , vecTradeWeekDay[vecTradeWeekDay.size()-1].c_str());
            //     printf("%s--%s\n",vecTradeDay[0].c_str(),vecTradeDay[vecTradeDay.size()-1].c_str());
            // }
            if ( nn_week < vecTradeWeekDay.size() && 0 == strcmp(g_date[i].c_str() , vecTradeWeekDay[nn_week].c_str() ))
            {
                // printf("total_money[%d]=%lf\n",i,total_money[i]);
                weekAccount.push_back(total_money[i]);

                if (0 == nn_week)
                {
                    weekIncome.push_back(total_money[i]-account_money);
                }
                else
                {
                    weekIncome.push_back(weekAccount[weekAccount.size()-1]-weekAccount[weekAccount.size()-2]);
                }
                nn_week +=1;
                //printf("%d , %s, %lf , %lf \n", i , g_date[i].c_str() , weekAccount[weekAccount.size()-1], weekIncome[weekIncome.size()-1]);
            }
            //printf("%d\n", i);
        }

        if( is_zuhehuice == 1 && this->pos == 0){
            //this->lots = this->account_money * cash_deposit / closeArray[i] / 100.0;
            this->lots = this->account_money * 100 / ( cash_deposit * perSize * closeArray[i]) / 2.0;
            //this->lots = __const_ini_money  / closeArray[i] / 4.0;
            //printf("this->lots=%d account_money = %lf cash_deposit %lf \n",this->lots , this->account_money , cash_deposit);
        }

    }
    virtual void onBar(int i){
        onBar_bef(i);
        onBar_after(i);
    }
    virtual void run(){
        this->beforeRun();

        this->afterRun();
    }

    virtual void setCanshuFromVec(const vector<double> &arr){

    }
    // 设置初始资金
    virtual void setAccountMoney( int ini_money){
        this->account_money = ini_money;
    }
};


#endif
