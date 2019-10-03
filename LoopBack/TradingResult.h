

#include "talib.h"
#ifndef _TradingResult
#define _TradingResult

#include <algorithm>
#include "load_config.h"

#define MAX_INF 0x3f3f3f3f
#define MIN_EPS 1e-6
/**
 * 记录每次交易的盈利结果
 **/
struct TradingResultNode{
    char code[60];                      //symbol
    char period[60];                    //阶段
    char start_date[60];                //数据开始日期
    char end_date[60];                  //数据结束日期

    double ini_account_money;           //账户初始资金 F
    double rate_of_return;              //年化收益率   F                      important
    int total_open_num;                 //总开仓次数   F                      important
    double win_rate;                    //胜率         F                      important
    double profit_factor;               //盈利因子   净利润总额/亏损总额  F   important
    double max_continue_loss;           //最大连续亏损 F                      important
    double max_windrawal_rate;          //最大回撤比率 F                      important         最大回撤数值 / 高点
    double sharpe;                      //夏普比率     W                      important         每笔交易平均盈利率 / 每笔交易标准差

    int total_trade_days;               //交易日个数   F
    int total_trade_weeks;              //交易周个数   F
    double total_income;                //净利润 F
    double net_loss;                    //总亏损 F
    double net_profits;                 //总盈利 F

    int win_week_num;                   //盈利周数
    int loss_week_num;                  //亏损周数
    int win_cover_num;                  //平仓盈利次数  F
    int loss_cover_num;                 //平仓亏损次数  F
    double loss_rate;                   //赔率          F
    double max_continue_win;            //最大连续盈利  F
    int max_continue_win_num;           //最大连赢次数  F
    int max_continue_loss_num;          //最大连亏次数  F

    double ave_cover_income;            //平均平仓净利润 F      平均每手净利润
    double ave_cover_win;               //平均平仓盈利   F      平均每手盈利
    double ave_cover_loss;              //平均平仓亏损   F      平均每手亏损
    double ave_cover_win_loss_rate;     //平均平仓盈亏比 F

    double max_single_win;              //最大单笔盈利   F
    double max_single_loss;             //最大单笔亏损   F

    double max_windrawl_number;          //最大回撤数值     F
    char max_windraw_start_date[50];     //最大回撤区间开始 F
    char max_windraw_end_date[50];       //最大回撤区间结束 F
    double windrawl_safe_rate;           //回撤安全系数     F    净利润/最大回撤数值

    double total_transcaction_only_fee;  //总交易费用
    double total_transcaction_fee;       //总交易成本     F      总的(保证金 + 手续费)
    double ave_transcation_fee;          //平均交易成本   F
    double transcation_cover_rate;       //交易成本覆盖率 F      净利润/总交易成本

    double real_income_rate;             //实际收益率     F      净利润/初始资金
    double ave_time_income_rate;         //平均每次收益率 F      平均每次收益率

    double stdandard;                    //波动率         F      每笔交易标准差
    // double skwenes;                   //峰度           F
    // double kurtosis;                  //偏度           F
    double sortinoRatio;                 //索提诺比率     F      每周的索提诺

    double targetFunctionRankScore;      //目标函数排名的实际分数 F
    double targetFunctionPromScore;      //目标函数Prom的实际分数 F


    vector<double> canshuSet;            //参数集
    vector<double> saveDayIncome;        //保存的每日累计收益
    vector<string> saveVecTradeDay;      //保存的每个交易日日期
    vector<double> saveWeekIncome;       //保存的每周收益
    vector<double> saveBzjIncome;        //保存的每日保证金
    vector<int> savePositions;           //保存的每日持仓

    vector<tradeNode> saveTradingResult;  //保存的每笔交易报告

    TradingResultNode(){}

    //重载最小比较符号
    bool operator < (const struct TradingResultNode & a) const{
        return this->targetFunctionRankScore > a.targetFunctionRankScore;
        //return this->total_income > a.total_income;
    }
    const double compareValue(){
        return this->targetFunctionPromScore;
        //return this->targetFunctionRankScore;
        //return this->total_income;
    }

    //prom 目标函数
    double targetFunctionProm(){
        return (this->ave_cover_win * (this->win_cover_num - sqrt(this->win_cover_num * 1.0)) +
                this->ave_cover_loss * (this->loss_cover_num - sqrt(this->loss_cover_num * 1.0))) * 250.0 / (this->ini_account_money * this->total_trade_days);
    }
    // 目标函数排名
    // 目标函数排名
    double targetFunctionRank(){
        /*
        double v = this->skwenes * this->kurtosis;
        double eps = 1e-6;
        if(v > eps){
            v = 1.0;
        }else if(v < -eps){
            v = -1.0;
        }else{
            v = 0.0;
        }
        double pdfd = v * sqrt(fabs(this->skwenes * this->kurtosis));
        */
        pair<double ,double> poly = polyfit(this->saveDayIncome , this->ini_account_money);
        //printf("x,y %lf,%lf\n",poly.first , poly.second);
        double ees = 0.0;
        double __y = 0.0;
        for(int i = 0;i < this->saveDayIncome.size();i++){
            double fit = poly.first * (i) + poly.second;
            __y = this->saveDayIncome[i] / this->ini_account_money;
            ees += pow(__y - fit , 2);
        }
        ees = sqrt(ees / this->total_trade_days);
        double poly_score = (poly.first * 100 - ees - fabs(poly.second * 100.0/ __y))*0.1;

        vector<double> saveDayIncome1;
        saveDayIncome1.clear();
        int rr;
        rr=this->saveDayIncome.size()/2;
        for (int i = rr;i<this->saveDayIncome.size();i++)
        {
            saveDayIncome1.push_back(this->saveDayIncome[i]-this->saveDayIncome[rr-1]);
        }
        pair<double ,double> poly1 = polyfit(saveDayIncome1 , this->ini_account_money);
        //printf("x,y %lf,%lf\n",poly.first , poly.second);
        double ees1 = 0.0;
        double __y1 = 0.0;
        for(int i = 0;i < saveDayIncome1.size();i++){
            double fit1 = poly1.first * (i) + poly1.second;
            __y1 = saveDayIncome1[i] / this->ini_account_money;
            ees1 += pow(__y1 - fit1 , 2);
        }
        ees1 = sqrt(ees1 / saveDayIncome1.size());
        double poly_score1 = (poly1.first * 100 - ees1 - fabs(poly1.second * 100.0/ __y1))*0.1;

        poly_score = min(poly_score,poly_score1);
        if (poly_score>1)
        {
            poly_score = 1;
        }

        double rate_windrawal;
        if (this->rate_of_return>=0)
        {
            rate_windrawal = fabs(this->rate_of_return*1.0/this->max_windrawal_rate)*0.4;
        }
        else
        {
            rate_windrawal = fabs(this->rate_of_return*1.0/this->max_windrawal_rate) * -1.0*0.4;
        }

        double prom_windrawal;
        prom_windrawal=(this->targetFunctionProm() + this->max_windrawal_rate * 2) * 100 * 2 * 0.2;
        if (this->max_windrawal_rate<-0.15)
        {
            prom_windrawal = min(0.0,prom_windrawal);
            rate_windrawal = min(0.0,rate_windrawal);
        }


        double max_sum;
        int max_ii;
        int max_dd;
        int max_max_dd;
        max_sum=this->saveDayIncome[0];
        max_ii=0;
        max_dd=0;
        max_max_dd=0;
        for(int i = 0;i < this->saveDayIncome.size();i++)
        {
            if (this->saveDayIncome[i]>max_sum)
            {
                max_sum=this->saveDayIncome[i];
                max_dd=i-max_ii;
                max_ii=i;
                max_max_dd=max(max_max_dd,max_dd);
            }

        }
        double max_per;
        max_per=1.0-max_max_dd*1.0/this->saveDayIncome.size();


        double t = prom_windrawal + rate_windrawal  + this->win_rate * 20 * 0.1
            + poly_score  + this->profit_factor * 10 * 0.1 + this->ave_cover_win_loss_rate*10*0.1;

        if(t>0)
        {
            t=t*1.0*max_per;
        }

        //printf("%lf , %d , %d , %d , %lf , %lf\n",max_sum , max_ii , max_dd , max_max_dd , max_per , t);
        return t;

        //printf("ees = %lf\n", ees);
        // double poly_score = poly.first * 100 - ees - fabs(poly.second * 100.0/ this->total_trade_days);
        // double t = this->rate_of_return * 100 * 0.25 + this->max_windrawal_rate * 100 * 0.5 + this->win_rate * 100 * 0.25
        //            + poly_score * 0.1 + this->profit_factor * 10 * 0.1 + this->sortinoRatio*5*0.2;

        // double t = this->rate_of_return * 100 * 0.3 + this->max_windrawal_rate * 100 * 0.2 + this->win_rate * 50 * 0.1
        // + pdfd * 0.1 + this->profit_factor * 10 * 0.1 + this->sortinoRatio * 5 * 0.2;
        //printf("rate_of_return:%lf max_windrawal_rate:%lf win_rate:%lf pdfd:%lf profit_factor:%lf sortinoRatio:%lf t:%lf\n",
         //      rate_of_return * 100,max_windrawal_rate * 100 ,win_rate * 50,pdfd ,profit_factor * 10,this->sortinoRatio * 5,t);

    }
    // double targetFunctionRank(){
    //     /*
    //     double v = this->skwenes * this->kurtosis;
    //     double eps = 1e-6;
    //     if(v > eps){
    //         v = 1.0;
    //     }else if(v < -eps){
    //         v = -1.0;
    //     }else{
    //         v = 0.0;
    //     }
    //     double pdfd = v * sqrt(fabs(this->skwenes * this->kurtosis));
    //     */
    //     pair<double ,double> poly = polyfit(this->saveDayIncome , this->ini_account_money);
    //     //printf("x,y %lf,%lf\n",poly.first , poly.second);
    //     double ees = 0.0;
    //     double __y = 0.0;
    //     for(int i = 0;i < this->saveDayIncome.size();i++){
    //         double fit = poly.first * (i) + poly.second;
    //         __y = this->saveDayIncome[i] / this->ini_account_money;
    //         ees += pow(__y - fit , 2);
    //     }
    //     ees = sqrt(ees / this->total_trade_days);
    //     double poly_score = poly.first * 100 - ees - fabs(poly.second * 100.0/ __y);
    //    //double t = this->rate_of_return * 100 * 0.25 + this->max_windrawal_rate * 100 * 0.25 + this->win_rate * 50 * 0.1
    //         //+ poly_score * 0.1 + this->profit_factor * 10 * 0.1 + this->sortinoRatio*5*0.2;

    //     double rate_windrawal;
    //     if (this->rate_of_return>=0)
    //     {
    //         rate_windrawal = fabs(this->rate_of_return*1.0/this->max_windrawal_rate);
    //     }
    //     else
    //     {
    //         rate_windrawal = fabs(this->rate_of_return*1.0/this->max_windrawal_rate) * -1.0;
    //     }

    //     double t = (this->targetFunctionProm() + this->max_windrawal_rate * 2) * 100 * 2 * 0.2 + rate_windrawal * 0.4 + this->win_rate * 20 * 0.1
    //         + poly_score * 0.1 + this->profit_factor * 10 * 0.1 + this->sortinoRatio*10*0.1;

    //     return t;

    //     //printf("ees = %lf\n", ees);
    //     // double poly_score = poly.first * 100 - ees - fabs(poly.second * 100.0/ this->total_trade_days);
    //     // double t = this->rate_of_return * 100 * 0.25 + this->max_windrawal_rate * 100 * 0.5 + this->win_rate * 100 * 0.25
    //     //            + poly_score * 0.1 + this->profit_factor * 10 * 0.1 + this->sortinoRatio*5*0.2;

    //     // double t = this->rate_of_return * 100 * 0.3 + this->max_windrawal_rate * 100 * 0.2 + this->win_rate * 50 * 0.1
    //     // + pdfd * 0.1 + this->profit_factor * 10 * 0.1 + this->sortinoRatio * 5 * 0.2;
    //     //printf("rate_of_return:%lf max_windrawal_rate:%lf win_rate:%lf pdfd:%lf profit_factor:%lf sortinoRatio:%lf t:%lf\n",
    //      //      rate_of_return * 100,max_windrawal_rate * 100 ,win_rate * 50,pdfd ,profit_factor * 10,this->sortinoRatio * 5,t);

    // }
    // 存储自身的参数
    double setCanshuSet(const vector<double> &_canshu){
        canshuSet.clear();
        for(int i = 0;i < _canshu.size();i++){
            canshuSet.push_back(_canshu[i]);
        }
    }
    // 得到自身的str值
    string getCanshuSetStr( const string splitFlag=" "){
        stringstream stream;
        for(int i = 0;i < canshuSet.size();i++){
            if(i == 0){
                stream << canshuSet[i] ;
            }else{
                stream << splitFlag <<canshuSet[i] ;
            }
        }
        return string(stream.str());
    }

    //TradingResultNode(){}
    // 交易函数的构造函数
    //TradingResultNode(const char *_code, const char *_period,const char *_start_date , const char *_end_date ,const vector<struct tradeNode> &tradingResult , const int tradingNum , const double account_money, double slippage,double rate ,double perSize,double cash_deposit,const vector<string> &vecTradeday , const vector<string> &vecTradeWeekDay , const vector<double> &dayIncome , int fee_type){ //
    TradingResultNode(const char *_code, const char *_period,const char *_start_date , const char *_end_date ,const vector<struct tradeNode> &tradingResult , const int tradingNum , const double account_money, double slippage,double rate ,double perSize,double cash_deposit,const vector<string> &vecTradeday , const vector<string> &vecTradeWeekDay , const vector<double> &dayIncome , const vector<double> &weekIncome , const vector<double> &bzjAccount, const vector<double> &dayAccount, const vector<int> &mp, int fee_type){ //
        this->saveTradingResult.clear();
        this->saveTradingResult.assign(tradingResult.begin(),tradingResult.end());
        this->saveBzjIncome.assign(bzjAccount.begin(),bzjAccount.end());
        this->savePositions.assign(mp.begin() , mp.end());
        // for(int i = 0;i < mp.size();i++){
        //     printf("%d\n",mp[i]);
        // }
        //copy(this->saveTradingResult.begin() , tradingResult.begin() ,tradingResult.end());

        strcpy(this->code , _code);
        strcpy(this->period , _period);
        strcpy(this->start_date , _start_date);
        strcpy(this->end_date , _end_date);

        this->total_trade_days = vecTradeday.size();
        //this->total_trade_weeks = vecTradeWeekDay.size();
        this->total_trade_weeks = weekIncome.size();
        this->ini_account_money = account_money;
        this->net_profits = 0; this->net_loss = 0;
        this->win_cover_num = 0; this->loss_cover_num = 0;
        this->total_open_num = tradingNum;
        this->max_single_win = 0.0;   this->max_single_loss = 0.0;
        this->max_continue_win = 0.0; this->max_continue_loss = 0.0;
        this->max_continue_win_num = 0; this->max_continue_loss_num = 0;
        this->total_income = 0.0;
        this->total_transcaction_fee = 0.0;
        this->total_transcaction_only_fee = 0.0;

        double eps = 1e-5;
        double last_pnl = 0.0;                          //上一笔的pnl
        int continue_win_num = 0;                       //连续赚钱次数
        int continue_loss_num = 0;                      //连续亏钱次数
        double continue_income = 0.0;                   //连续盈利
        double continue_loss = 0.0;                     //连续亏损
        //vector<double> v_pnl; v_pnl.clear();            //存储连续的pnl
        //vector<double> v_sum_pnl; v_sum_pnl.clear();    //存储pnl的连续和

        double t_stdandard = 0.0;
        //double t_skwenes = 0.0;
        //double t_kurtosis = 0.0;
        double t_ddex = 0.0;

        this->win_week_num = 0 ;  this->loss_week_num = 0;

        this->saveWeekIncome.clear();
        double t_week_profit = 0.0;
        int tw = 0;

        double t_max_windrawl_number = 0.0;
        int t_max_windraw_start_date = 0, t_max_windraw_end_date = 0;
        int pre_max_sum_i = 0;
        double pre_max_sum_pnl = 0.0;

        //FILE * fp_dd1 = fopen("out_trading.csv","w");
        for(int i = 0;i < tradingNum;i++){
            double turnover = (tradingResult[i].entryPrice + tradingResult[i].exitPrice) * perSize * fabs(tradingResult[i].volume);
            double commission = 0.0;
            if(fee_type == 1){
                commission = turnover * rate;
            }else if(fee_type == 2){
                commission = fabs(tradingResult[i].volume) * rate * 2.0;
            }
            double per_slippage = slippage * 2 * perSize * fabs(tradingResult[i].volume);
            double pnl = (tradingResult[i].exitPrice - tradingResult[i].entryPrice) * tradingResult[i].volume * perSize - commission - per_slippage;

            //printf("t1\n");
            if(last_pnl * pnl > eps){     // 认为这两笔是连续的
                if(pnl > 0){
                    continue_income += pnl;
                    continue_win_num += 1;
                }else{
                    continue_loss += pnl;
                    continue_loss_num += 1;
                }
            }else{  //这一笔的pnl方向与上次的不一样
                continue_win_num = continue_loss_num = 0;
                continue_income = continue_loss = 0;
                if(pnl > eps){
                    continue_win_num += 1;
                    continue_income += pnl;
                }
                if(-pnl > eps){
                    continue_loss_num += 1;
                    continue_loss += pnl;
                }
            }
            //this->total_transcaction_fee += turnover * cash_deposit / 100.0 + commission + per_slippage;
            this->total_transcaction_fee += tradingResult[i].entryPrice * perSize  * fabs(tradingResult[i].volume) * cash_deposit / 100.0 + commission ;
            this->total_transcaction_only_fee += commission + per_slippage;
            this->total_income += pnl;
            //fprintf(fp_dd1,"%d , %s ,%lf , %lf , %s , %lf ,%lf ,%lf , %lf\n", i , tradingResult[i].entryTime , tradingResult[i].entryPrice , tradingResult[i].volume , tradingResult[i].exitTime , tradingResult[i].exitPrice , pnl , commission+per_slippage , this->total_income);
            if(pnl > 0){
                this->net_profits += pnl;
                this->win_cover_num += 1;
            }
            if(pnl < 0){
                this->net_loss += pnl;
                this->loss_cover_num += 1;
            }
            this->max_single_win = max(pnl , this->max_single_win);
            this->max_single_loss = min(pnl , this->max_single_loss);

            this->max_continue_win_num = max(this->max_continue_win_num , continue_win_num);
            this->max_continue_loss_num = max(this->max_continue_loss_num , continue_loss_num);
            this->max_continue_win = max(this->max_continue_win , continue_income);
            this->max_continue_loss = min(this->max_continue_loss , continue_loss);
            last_pnl = pnl;

            double sum_pnl1 = this->total_income;
            if(sum_pnl1 > pre_max_sum_pnl){
                pre_max_sum_pnl = sum_pnl1;
                pre_max_sum_i   = i;
            }
            if( (sum_pnl1 - pre_max_sum_pnl) < t_max_windrawl_number ){
                t_max_windrawl_number = sum_pnl1 - pre_max_sum_pnl;
                t_max_windraw_start_date = pre_max_sum_i;
                t_max_windraw_end_date = i;
            }
        }

        for (int i = 0; i< this->total_trade_weeks; i++)
        {
            this->saveWeekIncome.push_back(weekIncome[i]);
            if (weekIncome[i] >0)
            {
                this->win_week_num += 1;
            }
            if(weekIncome[i]<0)
            {
                this->loss_week_num += 1;
            }
        }
        if(tradingNum > 0){
            this->ave_transcation_fee = this->total_transcaction_fee * 1.0 / this->total_open_num;
            this->transcation_cover_rate = this->total_income * 1.0 / this->total_transcaction_fee;
            this->win_rate = this->win_cover_num * 1.0 / this->total_open_num;
            this->loss_rate = this->loss_cover_num * 1.0 / this->total_open_num;
            this->profit_factor = fabs(this->net_profits / this->net_loss);
            this->ave_cover_income = this->total_income * 1.0 / this->total_open_num;
            this->ave_cover_win = this->net_profits * 1.0 / this->win_cover_num;
            this->ave_cover_loss = this->net_loss * 1.0 / this->loss_cover_num;
            this->ave_cover_win_loss_rate = fabs(this->ave_cover_win * 1.0 / this->ave_cover_loss);
            this->rate_of_return = this->total_income * 250.0 / (this->ini_account_money * this->total_trade_days);
            this->real_income_rate = this->total_income * 1.0 / this->ini_account_money;
            this->ave_time_income_rate = this->ave_cover_income * 1.0 / this->ini_account_money;

            this->saveDayIncome.clear();
            this->saveVecTradeDay.clear();
            this->saveDayIncome.reserve(this->total_trade_days + 10);
            this->saveVecTradeDay.reserve(this->total_trade_days + 10);
            double t_ave_dayIncome_rate = 0.0;
            for(int i = 0;i < this->total_trade_days;i++){
                this->saveDayIncome.push_back(dayAccount[i] - account_money);
                this->saveVecTradeDay.push_back(vecTradeday[i]);
            }
            t_ave_dayIncome_rate = this->saveDayIncome[this->total_trade_days-1] * 1.0 / this->total_trade_days / ini_account_money;

            //printf("saveDayIncome %lf %lf\n",saveDayIncome[0],saveDayIncome[1]);
            for(int i = 0;i < this->total_trade_days;i++){
                //printf("saveDayIncome %.2lf\n", saveDayIncome[i]);
                t_stdandard      += pow( (dayIncome[i] / account_money - t_ave_dayIncome_rate), 2.0);
                // t_skwenes        += pow( (dayIncome[i] / account_money - t_ave_dayIncome_rate), 3.0);
                // t_kurtosis       += pow( (dayIncome[i] / account_money - t_ave_dayIncome_rate), 4.0);
                t_ddex           += pow( min(0.0 , dayIncome[i]/account_money - NoRiseReturn/100.0/250.0 ) , 2);
            }
            t_ddex   = sqrt( t_ddex / this->total_trade_days * 250.0 );
            // mark std
            this->stdandard = sqrt(t_stdandard * 1.0 / (this->total_trade_days - 1)) ;
            this->sharpe    = (this->rate_of_return - NoRiseReturn/100.0 ) / this->stdandard / sqrt(250.0) ;
            // this->skwenes   = t_skwenes * 1.0 / (this->total_trade_days * pow(this->stdandard, 3.0));
            // this->kurtosis  = t_kurtosis * 1.0 / ( this->total_trade_days * pow(this->stdandard , 4.0));

            //this->sortinoRatio = sqrt(52.0) * this->rate_of_return * sqrt(this->total_trade_days - 1) / sqrt(t_ddex);
            this->sortinoRatio = (this->rate_of_return - NoRiseReturn/100.0 ) / t_ddex;

            //printf("TradingResultNode 4\n");
            strcpy(this->max_windraw_start_date, tradingResult[t_max_windraw_start_date].exitTime);
            strcpy(this->max_windraw_end_date , tradingResult[t_max_windraw_end_date].exitTime);
            this->max_windrawl_number = t_max_windrawl_number;
            this->windrawl_safe_rate = this->total_income * 1.0 /fabs(this->max_windrawl_number);
            this->max_windrawal_rate = this->max_windrawl_number * 1.0 / ini_account_money;

            this->targetFunctionRankScore = this->targetFunctionRank();
            this->targetFunctionPromScore = this->targetFunctionProm();
        }else{
            this->ave_transcation_fee = 0;
            this->transcation_cover_rate = 0;
            this->win_rate = 0;
            this->loss_rate = 0;
            this->profit_factor = 0;
            this->ave_cover_income = 0;
            this->ave_cover_win = 0;
            this->ave_cover_loss = 0;
            this->ave_cover_win_loss_rate = 0;
            this->rate_of_return = 0;
            this->real_income_rate = 0;
            this->ave_time_income_rate = 0;

            this->saveDayIncome.clear();
            this->saveVecTradeDay.clear();
            this->saveDayIncome.reserve(this->total_trade_days + 10);
            this->saveVecTradeDay.reserve(this->total_trade_days + 10);

            double t_ave_dayIncome_rate = 0.0;
            for(int i = 0;i < this->total_trade_days;i++){
                this->saveDayIncome.push_back(dayAccount[i] - account_money);
                this->saveVecTradeDay.push_back(vecTradeday[i]);
            }
            t_ave_dayIncome_rate = 0.0;

            t_ddex   = sqrt( t_ddex / this->total_trade_days * 250.0 );
            // mark std
            this->stdandard = 0.0;
            this->sharpe    = 0.0 ;

            this->sortinoRatio = 0.0;

            //printf("TradingResultNode 4\n");
            strcpy(this->max_windraw_start_date, "2050-01-01");
            strcpy(this->max_windraw_end_date , "2050-01-01");
            this->max_windrawl_number = 0.0;
            this->windrawl_safe_rate = 0.0;
            this->max_windrawal_rate = 0.0;

            this->targetFunctionRankScore = 0.0;
            this->targetFunctionPromScore = 0.0;
        }
    }

};

/**
 * 比较两个报告。。
 **/
struct TwoTradingReuslt{
    struct TradingResultNode n1 , n2;
    double target_score;
    TwoTradingReuslt(){
        target_score = -10000000.0;
    }
    TwoTradingReuslt( const struct TradingResultNode &tn1 , const struct TradingResultNode &tn2){
        n1 = tn1 , n2 = tn2;
        target_score = (tn1.targetFunctionRankScore + tn2.targetFunctionRankScore) / 2.0;
    }
    //重载最小比较符号
    bool operator < (const struct TwoTradingReuslt & a) const{
        return this->target_score > a.target_score;
        //return this->total_income > a.total_income;
    }
};


/**
 * 合并所有的交易报告
 **/
struct mergeAllTradingResult{
    string code_all;                        //所有的 symbol
    string period_all;                      //各品种的不同的周期
    string start_date;                      //开始的时间
    string end_date;                        //结束的时间

    double tot_ini_account_money;           //总的初始资金
    double tot_rate_of_return;              //总的时年化收益率
    int tot_total_open_num;                   //总的开仓次数
    double tot_win_rate;                    //总的胜率
    double tot_profit_factor;               //总的盈利因子
    double tot_max_continue_loss;           //最大持续亏损
    double tot_max_windrawl_rate;               //最大回撤比率
    double tot_sharpe;                      //总的夏普比率

    int tot_total_trade_days;                     //总的交易天数
    int tot_total_trade_weeks;                    //总的交易周
    double tot_total_income;                //总的交易收入
    double tot_net_loss;                    //总亏损
    double tot_net_profits;                 //总盈利

    int tot_win_week_num;                   //总盈利周数
    int tot_loss_week_num;                  //总亏损周数
    int tot_win_cover_num;                  //总的平仓盈利次数
    int tot_loss_cover_num;                 //总的平仓亏损次数

    double tot_loss_rate;                   //总的赔率
    double tot_max_continue_win;            //总的最大连赢次数
    int tot_max_continue_win_num;        //最大连赢次数
    int tot_max_continue_loss_num;       //最大连亏次数

    double tot_ave_cover_income;            //平均平仓净利润       平均每手净利润
    double tot_ave_cover_win;               //平均平仓盈利         平均每手盈利
    double tot_ave_cover_loss;              //平均平仓亏损         平均每手亏损
    double tot_ave_cover_win_loss_rate;     //平均平仓盈亏比

    double tot_max_single_win;              //最大单笔盈利
    double tot_max_single_loss;             //最大单笔亏损

    double tot_max_windrawl_number;         //最大连续回撤数值
    char tot_max_windraw_start_date[50];     //最大回撤连续开始区间
    char tot_max_windraw_end_date[50];       //最大回撤连续结束区间
    double tot_windrawl_safe_rate;          //总的回撤安全系数

    double tot_total_transcaction_only_fee;     //总交易费用
    double tot_total_transcaction_fee;          //总交易成本   总的(保证金 + 手续费)

    double tot_ave_transcation_fee;         //平均交易费用
    double tot_transcation_cover_rate;      //交易平均覆盖率

    double tot_real_income_rate;            //实际收益率
    double tot_ave_time_income_rate;        //平均每次收益率

    double tot_stdandard;                   //波动率
    double tot_sortinoRatio;                //索提诺比率

    double tot_targetFunctionRankScore;  //目标函数排名的实际分数
    double tot_targetFunctionPromScore;  //目标函数PROM的实际分数

    vector<double> _canshuSet;              //参数集
    vector<double> tot_saveDayIncome;       //保存的每日累计收益
    vector<string> tot_saveVecTradeDay;     //保存的每个交易日日期
    vector<double> tot_saveWeekIncome;      //保存的每周收益
    vector<double> tot_saveBzjIncome;       //保存的每日保证金
    vector<int> tot_savePositions;          //保存的每日持仓

    vector<tradeNode> tot_saveTradingResult; //保存的每笔交易报告

    vector<double> canshuSet;

    mergeAllTradingResult(){}

    //重载最小比较符号
    bool operator < (const struct mergeAllTradingResult & a) const{
        return this->tot_targetFunctionRankScore > a.tot_targetFunctionRankScore;
        //return this->total_income > a.total_income;
    }

    const double compareValue(){
        return this->tot_targetFunctionPromScore;
        //return this->targetFunctionRankScore;
        //return this->total_income;
    }
    // 设置参数集合
    double setCanshuSet(const vector<double> &_canshu){
        canshuSet.clear();
        for(int i = 0;i < _canshu.size();i++){
            canshuSet.push_back(_canshu[i]);
        }
    }
    // 得到自身的str值
    string getCanshuSetStr( const string splitFlag=" "){
        stringstream stream;
        for(int i = 0;i < canshuSet.size();i++){
            if(i == 0){
                stream << canshuSet[i] ;
            }else{
                stream << splitFlag <<canshuSet[i] ;
            }
        }
        return string(stream.str());
    }

    //prom 目标函数
    double targetFunctionProm(){
        return (this->tot_ave_cover_win * (this->tot_win_cover_num - sqrt(this->tot_win_cover_num * 1.0)) +
                this->tot_ave_cover_loss * (this->tot_loss_cover_num - sqrt(this->tot_loss_cover_num * 1.0))) * 250.0 / (this->tot_ini_account_money * this->tot_total_trade_days);
    }
    // 目标函数排名
    // 目标函数排名
    double targetFunctionRank(){
        pair<double ,double> poly = polyfit(this->tot_saveDayIncome , this->tot_ini_account_money );

        double ees = 0.0;
        double __y = 0.0;
        for(int i = 0;i < this->tot_saveDayIncome.size();i++){
            double fit = poly.first * (i) + poly.second;
            __y = this->tot_saveDayIncome[i] / this->tot_ini_account_money ;
            ees += pow(__y - fit , 2);
        }
        ees = sqrt(ees / this->tot_total_trade_days );
        double poly_score = (poly.first * 100 - ees - fabs(poly.second * 100.0/ __y))*0.1;

        vector<double> tot_saveDayIncome1;
        tot_saveDayIncome1.clear();
        int rr;
        rr=this->tot_saveDayIncome.size()/2;
        for (int i = rr;i<this->tot_saveDayIncome.size();i++)
        {
            tot_saveDayIncome1.push_back(this->tot_saveDayIncome[i]-this->tot_saveDayIncome[rr-1]);
        }
        pair<double ,double> poly1 = polyfit(tot_saveDayIncome1 , this->tot_ini_account_money);

        double ees1 = 0.0;
        double __y1 = 0.0;
        for(int i = 0;i < tot_saveDayIncome1.size();i++){
            double fit1 = poly1.first * (i) + poly1.second;
            __y1 = tot_saveDayIncome1[i] / this->tot_ini_account_money;
            ees1 += pow(__y1 - fit1 , 2);
        }
        ees1 = sqrt(ees1 / tot_saveDayIncome1.size());
        double poly_score1 = (poly1.first * 100 - ees1 - fabs(poly1.second * 100.0/ __y1))*0.1;

        poly_score = min(poly_score,poly_score1);
        if (poly_score>1)
        {
            poly_score = 1;
        }


       //double t = this->rate_of_return * 100 * 0.25 + this->max_windrawal_rate * 100 * 0.25 + this->win_rate * 50 * 0.1
            //+ poly_score * 0.1 + this->profit_factor * 10 * 0.1 + this->sortinoRatio*5*0.2;

        double rate_windrawal;
        if (this->tot_rate_of_return >=0)
        {
            rate_windrawal = fabs(this->tot_rate_of_return*1.0/this->tot_max_windrawl_rate)*0.4;
        }
        else
        {
            rate_windrawal = fabs(this->tot_rate_of_return*1.0/this->tot_max_windrawl_rate) * -1.0*0.4;
        }

        double prom_windrawal;
        prom_windrawal=(this->targetFunctionProm() + this->tot_max_windrawl_rate * 2) * 100 * 2 * 0.2;
        if (this->tot_max_windrawl_rate < -0.15)
        {
            prom_windrawal = min(0.0,prom_windrawal);
            rate_windrawal = min(0.0,rate_windrawal);
        }

        double max_sum;
        int max_ii;
        int max_dd;
        int max_max_dd;
        max_sum=this->tot_saveDayIncome[0];
        max_ii=0;
        max_dd=0;
        max_max_dd=0;
        for(int i = 0;i < this->tot_saveDayIncome.size();i++)
        {
            if (this->tot_saveDayIncome[i]>max_sum)
            {
                max_sum=this->tot_saveDayIncome[i];
                max_dd=i-max_ii;
                max_ii=i;
                max_max_dd=max(max_max_dd,max_dd);
            }

        }
        double max_per;
        max_per=1.0-max_max_dd*1.0/this->tot_saveDayIncome.size();

        double t = prom_windrawal + rate_windrawal  + this->tot_win_rate * 20 * 0.1
            + poly_score  + this->tot_profit_factor * 10 * 0.1 + this->tot_ave_cover_win_loss_rate * 10 * 0.1;

        if(t>0)
        {
            t=t*1.0*max_per;
        }
        return t;
    }

    //合并所有的交易结果
    mergeAllTradingResult(vector<TradingResultNode> &tradingResultVec){
        this->tot_ini_account_money = 0;
        this->tot_total_open_num = 0;
        this->tot_total_income = 0.0;
        this->tot_net_loss = 0;
        this->tot_net_profits = 0.0;

        this->tot_win_cover_num = 0;
        this->tot_loss_cover_num = 0;

        this->tot_ave_cover_income = 0.0;
        this->tot_ave_cover_win = 0.0;
        this->tot_ave_cover_loss = 0.0;

        this->tot_total_transcaction_only_fee = 0.0;
        this->tot_total_transcaction_fee = 0.0;
        this->tot_ave_transcation_fee = 0.0;

        this->_canshuSet.clear();
        this->tot_saveVecTradeDay.clear();
        this->tot_saveDayIncome.clear();
        this->tot_saveBzjIncome.clear();
        this->tot_saveWeekIncome.clear();

        this->tot_saveTradingResult.clear();
        for(int i = 0;i < tradingResultVec.size() ;i++){
            if(0 == i){
                this->code_all = string(tradingResultVec[i].code);
                this->period_all = string(tradingResultVec[i].period);
                this->start_date = string(tradingResultVec[i].start_date);
                this->end_date = string(tradingResultVec[i].end_date);

                for(int j = 0;j < tradingResultVec[i].saveVecTradeDay.size();j++){
                    this->tot_saveVecTradeDay.push_back( tradingResultVec[i].saveVecTradeDay[j]);
                    this->tot_saveDayIncome.push_back(  tradingResultVec[i].saveDayIncome[j]);
                    this->tot_saveBzjIncome.push_back( tradingResultVec[i].saveBzjIncome[j] );
                }
                for(int j = 0;j < tradingResultVec[i].saveWeekIncome.size();j++){
                    this->tot_saveWeekIncome.push_back( tradingResultVec[i].saveWeekIncome[j] );
                }
                for(int j = 0;j < tradingResultVec[i].saveTradingResult.size();j++){
                    this->tot_saveTradingResult.push_back( tradingResultVec[i].saveTradingResult[j] );
                }
            }else{
                this->code_all = this->code_all + "_" + string(tradingResultVec[i].code);
                this->period_all = this->period_all + "_" + string(tradingResultVec[i].period);
                for(int j = 0;j < tradingResultVec[i].saveVecTradeDay.size();j++){
                    this->tot_saveDayIncome[j] += tradingResultVec[i].saveDayIncome[j];
                    this->tot_saveBzjIncome[j] += tradingResultVec[i].saveBzjIncome[j];
                }
                for(int j = 0;j < tradingResultVec[i].saveWeekIncome.size();j++){
                    this->tot_saveWeekIncome[j] += tradingResultVec[i].saveWeekIncome[j] ;
                }
                for(int j = 0;j < tradingResultVec[i].saveTradingResult.size();j++){
                    this->tot_saveTradingResult.push_back( tradingResultVec[i].saveTradingResult[j]);
                }
            }
            this->tot_win_cover_num += tradingResultVec[i].win_cover_num;
            this->tot_loss_cover_num += tradingResultVec[i].loss_cover_num;

        }

        for(int i = 0;i < tradingResultVec.size();i++){
            this->tot_ini_account_money += tradingResultVec[i].ini_account_money;
            this->tot_total_open_num += tradingResultVec[i].total_open_num;
            this->tot_total_income += tradingResultVec[i].total_income;
            this->tot_net_loss += tradingResultVec[i].net_loss;
            this->tot_net_profits += tradingResultVec[i].net_profits;
            this->tot_total_transcaction_only_fee += tradingResultVec[i].total_transcaction_only_fee;
            this->tot_total_transcaction_fee += tradingResultVec[i].total_transcaction_fee;
        }


        this->tot_total_trade_days = tradingResultVec[0].total_trade_days;
        this->tot_total_trade_weeks = tradingResultVec[0].total_trade_weeks;

        this->tot_ave_transcation_fee = this->tot_total_transcaction_fee  * 1.0 / this->tot_total_open_num;
        this->tot_transcation_cover_rate = this->tot_total_income * 1.0 / this->tot_total_transcaction_fee;
        this->tot_win_rate = this->tot_win_cover_num * 1.0 / this->tot_total_open_num;
        this->tot_loss_rate = this->tot_loss_cover_num * 1.0 / this->tot_total_open_num;

        if(fabs(this->tot_net_loss) < MIN_EPS){
            this->tot_profit_factor = 0.0;
        }else{
            this->tot_profit_factor = fabs(this->tot_net_profits * 1.0 / this->tot_net_loss);
        }
        this->tot_ave_cover_income = this->tot_total_income * 1.0 / this->tot_total_open_num;
        this->tot_ave_cover_win = this->tot_net_profits * 1.0 / this->tot_win_cover_num;
        this->tot_ave_cover_loss = this->tot_net_loss * 1.0 / this->tot_loss_cover_num;
        this->tot_ave_cover_win_loss_rate = fabs(this->tot_ave_cover_win * 1.0 / this->tot_ave_cover_loss );
        this->tot_rate_of_return = this->tot_total_income * 250.0 / (this->tot_ini_account_money * this->tot_total_trade_days);
        this->tot_real_income_rate = this->tot_total_income * 1.0 / this->tot_ini_account_money;
        this->tot_ave_time_income_rate = this->tot_ave_cover_income * 1.0 / this->tot_ini_account_money ;

        //
        // 下面程序的过程，先对交易的结果进行日期、时间的排序，然后再算连续的方式
        //
        sort( this->tot_saveTradingResult.begin() ,  this->tot_saveTradingResult.end() );

        this->tot_max_single_win = 0.0;
        this->tot_max_single_loss = 0.0;

        double eps = 1e-5;
        double last_pnl = 0.0;                          //上一笔的pnl
        int continue_win_num = 0;                       //连续赚钱次数
        int continue_loss_num = 0;                      //连续亏钱次数
        double continue_income = 0.0;                   //连续盈利
        double continue_loss = 0.0;                     //连续亏损

        double t_stdandard = 0.0;
        double t_ddex = 0.0;

        double t_max_windrawl_number = 0.0;
        int t_max_windraw_start_date = 0, t_max_windraw_end_date = 0;
        int pre_max_sum_i = 0;
        double pre_max_sum_pnl = 0.0;

        int tot_tradingNum = this->tot_saveTradingResult.size();
        for(int i = 0;i < tot_tradingNum ; i++){
            double turnover = this->tot_saveTradingResult[i].turnover;
            double commission = this->tot_saveTradingResult[i].commission;
            double per_slippage = this->tot_saveTradingResult[i].per_slippage;
            double pnl = this->tot_saveTradingResult[i].pnl;

            if(last_pnl * pnl > eps){     // 认为这两笔是连续的
                if(pnl > 0){
                    continue_income += pnl;
                    continue_win_num += 1;
                }else{
                    continue_loss += pnl;
                    continue_loss_num += 1;
                }
            }else{  //这一笔的pnl方向与上次的不一样
                continue_win_num = continue_loss_num = 0;
                continue_income = continue_loss = 0;
                if(pnl > eps){
                    continue_win_num += 1;
                    continue_income += pnl;
                }
                if(-pnl > eps){
                    continue_loss_num += 1;
                    continue_loss += pnl;
                }
            }

            this->tot_max_single_win = max(pnl , this->tot_max_single_win);
            this->tot_max_single_loss = min(pnl , this->tot_max_single_loss);
            this->tot_max_continue_win_num = max(this->tot_max_continue_win_num , continue_win_num);
            this->tot_max_continue_loss_num = max(this->tot_max_continue_loss_num , continue_loss_num);
            this->tot_max_continue_win = max(this->tot_max_continue_win , continue_income);
            this->tot_max_continue_loss = min(this->tot_max_continue_loss , continue_loss);

            last_pnl = pnl;
            double sum_pnl1 = this->tot_total_income;
            if(sum_pnl1 > pre_max_sum_pnl){
                pre_max_sum_pnl = sum_pnl1;
                pre_max_sum_i   = i;
            }
            if( (sum_pnl1 - pre_max_sum_pnl) < t_max_windrawl_number ){
                t_max_windrawl_number = sum_pnl1 - pre_max_sum_pnl;
                t_max_windraw_start_date = pre_max_sum_i;
                t_max_windraw_end_date = i;
            }
        }

        this->tot_win_week_num = 0;
        this->tot_loss_week_num = 0;
        for(int i = 0;i < this->tot_total_trade_weeks; i++){
            if(this->tot_saveWeekIncome[i] > 0){
                this->tot_win_week_num += 1;
            }else{
                this->tot_loss_week_num += 1;
            }
        }

        if( tot_tradingNum > 0){
            double t_ave_dayIncome_rate = this->tot_saveDayIncome[this->tot_total_trade_days-1] * 1.0 / this->tot_total_trade_days / this->tot_ini_account_money ;

            for(int i = 0;i < this->tot_total_trade_days;i++){
                //printf("saveDayIncome %.2lf\n", saveDayIncome[i]);
                t_stdandard      += pow( ( this->tot_saveDayIncome[i] / this->tot_ini_account_money - t_ave_dayIncome_rate), 2.0);
                // t_skwenes        += pow( (dayIncome[i] / account_money - t_ave_dayIncome_rate), 3.0);
                // t_kurtosis       += pow( (dayIncome[i] / account_money - t_ave_dayIncome_rate), 4.0);
                t_ddex           += pow( min(0.0 , this->tot_saveDayIncome[i] / this->tot_ini_account_money  - NoRiseReturn/100.0/250.0 ) , 2);
            }
            t_ddex   = sqrt( t_ddex / this->tot_total_trade_days * 250.0 );
            // mark std
            this->tot_stdandard = sqrt(t_stdandard * 1.0 / (this->tot_total_trade_days - 1)) ;
            this->tot_sharpe    = (this->tot_rate_of_return - NoRiseReturn/100.0 ) / this->tot_stdandard / sqrt(250.0) ;

            //this->sortinoRatio = sqrt(52.0) * this->rate_of_return * sqrt(this->total_trade_days - 1) / sqrt(t_ddex);
            this->tot_sortinoRatio = (this->tot_rate_of_return - NoRiseReturn/100.0 ) / t_ddex;


            strcpy(this->tot_max_windraw_start_date , this->tot_saveTradingResult[t_max_windraw_start_date].exitTime);
            strcpy(this->tot_max_windraw_end_date , this->tot_saveTradingResult[t_max_windraw_end_date].exitTime);
            this->tot_max_windrawl_number = t_max_windrawl_number;
            if(fabs(this->tot_max_windrawl_number) < MIN_EPS){
                this->tot_windrawl_safe_rate = 0.0;
            }else{
                this->tot_windrawl_safe_rate = this->tot_total_income * 1.0 /fabs(this->tot_max_windrawl_number );
            }
            this->tot_max_windrawl_rate = this->tot_max_windrawl_number * 1.0 / this->tot_ini_account_money;

            this->tot_targetFunctionRankScore = this->targetFunctionRank();
            this->tot_targetFunctionPromScore = this->targetFunctionProm();

            if(this->tot_targetFunctionRankScore > MAX_INF || this->tot_targetFunctionRankScore < -MAX_INF){
                this->tot_targetFunctionRankScore = 0.0;
            }
            if(this->tot_targetFunctionPromScore > MAX_INF || this->tot_targetFunctionPromScore < -MAX_INF){
                this->tot_targetFunctionPromScore = 0.0;
            }
        }else{
            this->tot_max_windrawl_number = 0.0;
            this->tot_windrawl_safe_rate = 0.0;
            this->tot_max_windrawl_rate = 0.0;

            this->tot_targetFunctionRankScore = 0.0;
            this->tot_targetFunctionPromScore = 0.0;

            strcpy(this->tot_max_windraw_start_date , "2050-01-01");
            strcpy(this->tot_max_windraw_end_date , "2050-01-01");
        }
    }

    mergeAllTradingResult(vector<struct __zhuli_data> &zhuliData , const char * _start_date , const char * _end_date ,  const vector<struct tradeNode> &tradingResult , const int tradingNum , const double account_money,
            const vector<string> &vecTradeday , const vector<string> &vecTradeWeekDay , const vector<double> &dayIncome , const vector<double> &weekIncome , const vector<double> &bzjAccount,
            const vector<double> &dayAccount, const vector<int> &mp){

        this->tot_saveTradingResult.clear();
        this->tot_saveTradingResult.assign(tradingResult.begin(),tradingResult.end());
        this->tot_saveBzjIncome.assign(bzjAccount.begin(),bzjAccount.end());
        this->tot_savePositions.assign(mp.begin() , mp.end());

        this->code_all = "";
        this->period_all = "";
        for(int i = 0;i < zhuliData.size(); i++){
            if(i == 0){
                this->code_all = zhuliData[i].code;
                this->period_all = zhuliData[i].period;
            }else{
                this->code_all = this->code_all + string("_") + zhuliData[i].code;
                this->period_all = this->period_all + string("_") + zhuliData[i].period;
            }
        }
        this->start_date = _start_date;
        this->end_date = _end_date;

        this->tot_total_trade_days = vecTradeday.size();
        this->tot_total_trade_weeks = weekIncome.size();
        this->tot_ini_account_money = account_money;
        this->tot_net_profits = 0; this->tot_net_loss = 0;
        this->tot_win_cover_num = 0; this->tot_loss_cover_num = 0;
        this->tot_total_open_num = tradingNum;
        this->tot_max_single_win = 0.0;   this->tot_max_single_loss = 0.0;
        this->tot_max_continue_win = 0.0; this->tot_max_continue_loss = 0.0;
        this->tot_max_continue_win_num = 0; this->tot_max_continue_loss_num = 0;
        this->tot_total_income = 0.0;
        this->tot_total_transcaction_fee = 0.0;
        this->tot_total_transcaction_only_fee = 0.0;

        double eps = 1e-5;
        double last_pnl = 0.0;                          //上一笔的pnl
        int continue_win_num = 0;                       //连续赚钱次数
        int continue_loss_num = 0;                      //连续亏钱次数
        double continue_income = 0.0;                   //连续盈利
        double continue_loss = 0.0;                     //连续亏损

        double t_stdandard = 0.0;
        double t_ddex = 0.0;

        this->tot_win_week_num = 0 ;  this->tot_loss_week_num = 0;

        this->tot_saveWeekIncome.clear();
        double t_week_profit = 0.0;
        int tw = 0;

        double t_max_windrawl_number = 0.0;
        int t_max_windraw_start_date = 0, t_max_windraw_end_date = 0;
        int pre_max_sum_i = 0;
        double pre_max_sum_pnl = 0.0;

        //FILE * fp_dd1 = fopen("out_trading.csv","w");
        for(int i = 0;i < tradingNum;i++){
            double turnover = tradingResult[i].turnover;
            double commission =  tradingResult[i].commission;
            double per_slippage = tradingResult[i].per_slippage;
            double pnl = tradingResult[i].pnl;
            double bzj = tradingResult[i].bzj;

            // printf("tradingNum %d , %lf , %lf , %lf, %lf , %lf\n",  i ,turnover , commission ,per_slippage , pnl ,bzj);
            //printf("t1\n");
            if(last_pnl * pnl > eps){     // 认为这两笔是连续的
                if(pnl > 0){
                    continue_income += pnl;
                    continue_win_num += 1;
                }else{
                    continue_loss += pnl;
                    continue_loss_num += 1;
                }
            }else{  //这一笔的pnl方向与上次的不一样
                continue_win_num = continue_loss_num = 0;
                continue_income = continue_loss = 0;
                if(pnl > eps){
                    continue_win_num += 1;
                    continue_income += pnl;
                }
                if(-pnl > eps){
                    continue_loss_num += 1;
                    continue_loss += pnl;
                }
            }
            //this->total_transcaction_fee += turnover * cash_deposit / 100.0 + commission + per_slippage;
            this->tot_total_transcaction_fee += bzj + commission;
            this->tot_total_transcaction_only_fee += commission + per_slippage;
            this->tot_total_income += pnl;
            //fprintf(fp_dd1,"%d , %s ,%lf , %lf , %s , %lf ,%lf ,%lf , %lf\n", i , tradingResult[i].entryTime , tradingResult[i].entryPrice , tradingResult[i].volume , tradingResult[i].exitTime , tradingResult[i].exitPrice , pnl , commission+per_slippage , this->total_income);
            if(pnl > 0){
                this->tot_net_profits += pnl;
                this->tot_win_cover_num += 1;
            }
            if(pnl < 0){
                this->tot_net_loss += pnl;
                this->tot_loss_cover_num += 1;
            }
            this->tot_max_single_win = max(pnl , this->tot_max_single_win);
            this->tot_max_single_loss = min(pnl , this->tot_max_single_loss);

            this->tot_max_continue_win_num = max(this->tot_max_continue_win_num , continue_win_num);
            this->tot_max_continue_loss_num = max(this->tot_max_continue_loss_num , continue_loss_num);
            this->tot_max_continue_win = max(this->tot_max_continue_win , continue_income);
            this->tot_max_continue_loss = min(this->tot_max_continue_loss , continue_loss);
            last_pnl = pnl;

            double sum_pnl1 = this->tot_total_income;
            if(sum_pnl1 > pre_max_sum_pnl){
                pre_max_sum_pnl = sum_pnl1;
                pre_max_sum_i   = i;
            }
            if( (sum_pnl1 - pre_max_sum_pnl) < t_max_windrawl_number ){
                t_max_windrawl_number = sum_pnl1 - pre_max_sum_pnl;
                t_max_windraw_start_date = pre_max_sum_i;
                t_max_windraw_end_date = i;
            }
        }

        for (int i = 0; i < this->tot_total_trade_weeks; i++)
        {
            this->tot_saveWeekIncome.push_back(weekIncome[i]);
            if (weekIncome[i] >0)
            {
                this->tot_win_week_num += 1;
            }
            if(weekIncome[i]<0)
            {
                this->tot_loss_week_num += 1;
            }
        }
        if(tradingNum > 0){
            this->tot_ave_transcation_fee = this->tot_total_transcaction_fee * 1.0 / this->tot_total_open_num;
            this->tot_transcation_cover_rate = this->tot_total_income * 1.0 / this->tot_total_transcaction_fee;
            this->tot_win_rate = this->tot_win_cover_num * 1.0 / this->tot_total_open_num;
            this->tot_loss_rate = this->tot_loss_cover_num * 1.0 / this->tot_total_open_num;
            if(fabs(this->tot_net_loss) < MIN_EPS){
                this->tot_profit_factor = 0.0;
            }else{
                this->tot_profit_factor = fabs(this->tot_net_profits / this->tot_net_loss);
            }
            this->tot_ave_cover_income = this->tot_total_income * 1.0 / this->tot_total_open_num;
            if(this->tot_win_cover_num > 0){
                this->tot_ave_cover_win = this->tot_net_profits * 1.0 / this->tot_win_cover_num;
            }else{
                this->tot_ave_cover_win = 0.0;
            }
            if( this->tot_loss_cover_num > 0){
                this->tot_ave_cover_loss = this->tot_net_loss * 1.0 / this->tot_loss_cover_num;
            }else{
                this->tot_ave_cover_loss = 0.0;
            }
            this->tot_ave_cover_win_loss_rate = fabs(this->tot_ave_cover_win * 1.0 / this->tot_ave_cover_loss);
            this->tot_rate_of_return = this->tot_total_income * 250.0 / (this->tot_ini_account_money * this->tot_total_trade_days);
            this->tot_real_income_rate = this->tot_total_income * 1.0 / this->tot_ini_account_money;
            this->tot_ave_time_income_rate = this->tot_ave_cover_income * 1.0 / this->tot_ini_account_money;

            this->tot_saveDayIncome.clear();
            this->tot_saveVecTradeDay.clear();
            this->tot_saveDayIncome.reserve(this->tot_total_trade_days + 10);
            this->tot_saveVecTradeDay.reserve(this->tot_total_trade_days + 10);
            double t_ave_dayIncome_rate = 0.0;
            for(int i = 0;i < this->tot_total_trade_days;i++){
                this->tot_saveDayIncome.push_back(dayAccount[i] - account_money);
                this->tot_saveVecTradeDay.push_back(vecTradeday[i]);
            }
            t_ave_dayIncome_rate = this->tot_saveDayIncome[this->tot_total_trade_days-1] * 1.0 / this->tot_total_trade_days / account_money;

            //printf("saveDayIncome %lf %lf\n",saveDayIncome[0],saveDayIncome[1]);
            for(int i = 0;i < this->tot_total_trade_days;i++){
                //printf("saveDayIncome %.2lf\n", saveDayIncome[i]);
                t_stdandard      += pow( (dayIncome[i] / account_money - t_ave_dayIncome_rate), 2.0);
                // t_skwenes        += pow( (dayIncome[i] / account_money - t_ave_dayIncome_rate), 3.0);
                // t_kurtosis       += pow( (dayIncome[i] / account_money - t_ave_dayIncome_rate), 4.0);
                t_ddex           += pow( min(0.0 , dayIncome[i]/account_money - NoRiseReturn/100.0/250.0 ) , 2);
            }
            t_ddex   = sqrt( t_ddex / this->tot_total_trade_days * 250.0 );
            // mark std
            this->tot_stdandard = sqrt(t_stdandard * 1.0 / (this->tot_total_trade_days - 1)) ;
            this->tot_sharpe    = (this->tot_rate_of_return - NoRiseReturn/100.0 ) / this->tot_stdandard / sqrt(250.0) ;
            // this->skwenes   = t_skwenes * 1.0 / (this->total_trade_days * pow(this->stdandard, 3.0));
            // this->kurtosis  = t_kurtosis * 1.0 / ( this->total_trade_days * pow(this->stdandard , 4.0));

            //this->sortinoRatio = sqrt(52.0) * this->rate_of_return * sqrt(this->total_trade_days - 1) / sqrt(t_ddex);
            this->tot_sortinoRatio = (this->tot_rate_of_return - NoRiseReturn/100.0 ) / t_ddex;

            //printf("TradingResultNode 4\n");
            strcpy(this->tot_max_windraw_start_date, tradingResult[t_max_windraw_start_date].exitTime);
            strcpy(this->tot_max_windraw_end_date , tradingResult[t_max_windraw_end_date].exitTime);
            this->tot_max_windrawl_number = t_max_windrawl_number;
            if(fabs(this->tot_max_windrawl_number) < MIN_EPS){
                this->tot_windrawl_safe_rate = 0.0;
            }else{
                this->tot_windrawl_safe_rate = this->tot_total_income * 1.0 /fabs(this->tot_max_windrawl_number);
            }
            this->tot_max_windrawl_rate = this->tot_max_windrawl_number * 1.0 / account_money;

            this->tot_targetFunctionRankScore = this->targetFunctionRank();
            this->tot_targetFunctionPromScore = this->targetFunctionProm();

            if(this->tot_targetFunctionRankScore > MAX_INF || this->tot_targetFunctionRankScore < -MAX_INF){
                this->tot_targetFunctionRankScore = 0.0;
            }
            if(this->tot_targetFunctionPromScore > MAX_INF || this->tot_targetFunctionPromScore < -MAX_INF){
                this->tot_targetFunctionPromScore = 0.0;
            }
        }else{
            this->tot_ave_transcation_fee = 0;
            this->tot_transcation_cover_rate = 0;
            this->tot_win_rate = 0;
            this->tot_loss_rate = 0;
            this->tot_profit_factor = 0;
            this->tot_ave_cover_income = 0;
            this->tot_ave_cover_win = 0;
            this->tot_ave_cover_loss = 0;
            this->tot_ave_cover_win_loss_rate = 0;
            this->tot_rate_of_return = 0;
            this->tot_real_income_rate = 0;
            this->tot_ave_time_income_rate = 0;

            this->tot_saveDayIncome.clear();
            this->tot_saveVecTradeDay.clear();
            this->tot_saveDayIncome.reserve(this->tot_total_trade_days + 10);
            this->tot_saveVecTradeDay.reserve(this->tot_total_trade_days + 10);

            double t_ave_dayIncome_rate = 0.0;
            for(int i = 0;i < this->tot_total_trade_days;i++){
                this->tot_saveDayIncome.push_back(dayAccount[i] - account_money);
                this->tot_saveVecTradeDay.push_back(vecTradeday[i]);
            }
            t_ave_dayIncome_rate = 0.0;

            t_ddex   = sqrt( t_ddex / this->tot_total_trade_days * 250.0 );
            // mark std
            this->tot_stdandard = 0.0;
            this->tot_sharpe    = 0.0 ;

            this->tot_sortinoRatio = 0.0;

            //printf("TradingResultNode 4\n");
            strcpy(this->tot_max_windraw_start_date, "2050-01-01");
            strcpy(this->tot_max_windraw_end_date , "2050-01-01");
            this->tot_max_windrawl_number = 0.0;
            this->tot_windrawl_safe_rate = 0.0;
            this->tot_max_windrawl_rate = 0.0;

            this->tot_targetFunctionRankScore = 0.0;
            this->tot_targetFunctionPromScore = 0.0;
        }
    }
};

#endif // _TradingResult
