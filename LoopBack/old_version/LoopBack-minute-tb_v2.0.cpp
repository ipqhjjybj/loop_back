/**
 * @copyright JiuXi
 * @author ipqhjjybj
 **/
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <vector>
#include <cmath>

// 中文测试
using namespace std;

//#define debug

/**
 * C++判断今天是星期几
 * 基姆拉尔森计算公式
 * W= (d+2*m+3*(m+1)/5+y+y/4-y/100+y/400+1) mod 7
 * 在公式中d表示日期中的日数，m表示月份数，y表示年数。
 * 注意：在公式中有个与其他公式不同的地方：
 * 把一月和二月看成是上一年的十三月和十四月，例：如果是2004-1-10则换算成：2003-13-10来代入公式计算。
 **/
int CaculateWeekDay(int y, int m, int d)
{
    if(m==1||m==2) //把一月和二月换算成上一年的十三月和是四月
    {
        m+=12;
        y--;
    }
    int Week=(d+2*m+3*(m+1)/5+y+y/4-y/100+y/400)%7;
    return Week;
    //     case 0: PrintfDay(y,m,d); cout << "是星期一" << endl; break;
    //     case 6: PrintfDay(y,m,d); cout << "是星期日" << endl; break;
}
int CaculateWeekDayS(const char * s){
    int y,m,d;
    sscanf(s , "%d/%d/%d" , &y,&m,&d);
    return CaculateWeekDay(y,m,d);
}
//计算xAverage
void xAverage(vector<double> &dstArr , const vector<double> &closeArray ,const int m, const int n){
    for(int i = 0;i < n;i++){
        if(i == 0)  dstArr[i] = closeArray[i];
        else  dstArr[i] = dstArr[i-1] + 2.0 * (closeArray[i] -  dstArr[i-1]) / (m + 1) ;
    }
}
//向左平移单位的算法
void LeftMove(vector<double> &dstArr , const vector<double> &oriArray ,const int m, const int n){
    for(int i = 0;i < n;i++){
        dstArr[i] = oriArray[max(0, i-m)];
    }
}

struct tradeNode{
    char entryTime[50];
    char exitTime[50];
    double entryPrice,exitPrice;
    double volume;
    tradeNode(){
        volume = 0.0;
    }
};

/**
 * 记录每次交易的盈利结果
 **/
struct TradingResultNode{
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

    double max_windrawl_number;         //最大回撤数值     F
    char max_windraw_start_date[50];    //最大回撤区间开始 F
    char max_windraw_end_date[50];      //最大回撤区间结束 F
    double windrawl_safe_rate;          //回撤安全系数     F    净利润/最大回撤数值

    double total_transcaction_fee;      //总交易成本     F      总的(保证金 + 手续费)
    double ave_transcation_fee;         //平均交易成本   F
    double transcation_cover_rate;      //交易成本覆盖率 F      净利润/总交易成本

    double real_income_rate;            //实际收益率     F      净利润/初始资金
    double ave_time_income_rate;        //平均每次收益率 F      平均每次收益率

    double variance;                    //波动率         W      每笔交易标准差
    double kurtosis;                    //峰度           W
    double skewness;                    //偏度           W
    double sortinoRatio;                //索提诺比率     W      每周的索提诺

    double targetFunctionRankScore;     //目标函数排名的实际分数 F
    double targetFunctionPromScore;     //目标函数Prom的实际分数 F

    //prom 目标函数
    double targetFunctionProm(){
        return (this->ave_cover_win * (this->win_cover_num - sqrt(this->win_cover_num * 1.0)) - this->ave_cover_loss * (this->loss_cover_num - sqrt(this->loss_cover_num * 1.0))) / this->ave_transcation_fee;
    }
    // 目标函数排名
    double targetFunctionRank(){
        return this->rate_of_return * 0.3 + this->max_windrawal_rate * 0.2 + this->win_rate * 0.1 + this->kurtosis * this->skewness * 0.1 + this->profit_factor * 0.1 + this->sortinoRatio * 0.2;
    }
    //TradingResultNode(){}
    // 交易函数的构造函数
    TradingResultNode(const vector<struct tradeNode> &tradingResult , const int tradingNum , const double account_money, double slippage,double rate ,double perSize,const vector<string> &vecTradeDay , const vector<string> &vecTradeWeekDay){ //
        this->total_trade_days = vecTradeDay.size();
        this->total_trade_weeks = vecTradeWeekDay.size();
        this->ini_account_money = account_money;
        this->net_profits = 0; this->net_loss = 0;
        this->win_cover_num = 0; this->loss_cover_num = 0;
        this->total_open_num = tradingNum;
        this->max_single_win = 0.0;   this->max_single_loss = 0.0;
        this->max_continue_win = 0.0; this->max_continue_loss = 0.0;
        this->max_continue_win_num = 0; this->max_continue_loss_num = 0;
        this->total_income = 0.0;
        this->total_transcaction_fee = 0.0;

        double eps = 1e-5;
        double last_pnl = 0.0;                          //上一笔的pnl
        int continue_win_num = 0;                       //连续赚钱次数
        int continue_loss_num = 0;                      //连续亏钱次数
        double continue_income = 0.0;                   //连续盈利
        double continue_loss = 0.0;                     //连续亏损
        vector<double> v_pnl; v_pnl.clear();            //存储连续的pnl
        vector<double> v_sum_pnl; v_sum_pnl.clear();    //存储pnl的连续和
        for(int i = 0;i < tradingNum;i++){
            double turnover = (tradingResult[i].entryPrice + tradingResult[i].exitPrice) * perSize * fabs(tradingResult[i].volume);
            double commission = turnover * rate;
            double per_slippage = slippage * 2 * perSize * fabs(tradingResult[i].volume);
            double pnl = (tradingResult[i].exitPrice - tradingResult[i].entryPrice) * tradingResult[i].volume * perSize - commission - per_slippage;

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
            this->total_transcaction_fee += turnover + commission + per_slippage;
            this->total_income += pnl;
            if(tradingResult[i].volume > 0){
                this->net_profits += pnl;
                this->win_cover_num += 1;
            }
            if(tradingResult[i].volume < 0){
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

            v_pnl.push_back(pnl);
            v_sum_pnl.push_back(this->total_income);
        }
        //this->total_income = this->net_profits + this->net_loss;

        this->ave_transcation_fee = this->total_transcaction_fee * 1.0 / this->total_open_num;
        this->transcation_cover_rate = this->total_income * 1.0 / this->total_transcaction_fee;
        this->win_rate = this->win_cover_num * 1.0 / this->total_open_num;
        this->loss_rate = this->loss_cover_num * 1.0 / this->total_open_num;
        this->profit_factor = fabs(this->net_profits / this->net_loss);
        this->ave_cover_income = this->total_income * 1.0 / this->total_open_num;
        this->ave_cover_win = this->net_profits * 1.0 / this->win_cover_num;
        this->ave_cover_loss = this->net_loss * 1.0 / this->loss_cover_num;
        this->ave_cover_win_loss_rate = this->ave_cover_win * 1.0 / this->ave_cover_loss;
        this->rate_of_return = this->total_income * 250.0 * 100.0 / (this->ini_account_money * this->total_trade_days);
        this->real_income_rate = this->total_income * 1.0 / this->ini_account_money;
        this->ave_time_income_rate = this->ave_cover_income * 1.0 / this->ini_account_money;

        double t_variance = 0.0;
        double t_kurtosis_fenzi = 0.0;
        double t_skewness = 0.0;
        double t_max_windrawl_number = 0.0;
        int t_max_windraw_start_date = 0, t_max_windraw_end_date = 0;
        double t_high_value = ini_account_money;
        double t_ddex = 0.0;

        double t_week_profit = 0.0;
        int tw = 0;
        this->win_week_num = 0 ;  this->loss_week_num = 0;
        for(int i = 0;i < tradingNum; i++){
            double pnl = v_pnl[i];
            t_variance += pow( (pnl - this->ave_cover_income),2.0);
            t_kurtosis_fenzi += pow(pnl - this->ave_cover_income,3.0);
            t_skewness += pow(pnl - this->ave_cover_income, 4.0);
            t_ddex += pow( min(0.0 , pnl) , 2);

            double sum_pnl1 = v_sum_pnl[i];
            for(int j = i+1;j < tradingNum;j++){
                double sum_pnl2 = v_sum_pnl[j];
                if( (sum_pnl2 - sum_pnl1) < t_max_windrawl_number){
                    t_max_windrawl_number = sum_pnl2 - sum_pnl1;
                    t_max_windraw_start_date = i;
                    t_max_windraw_end_date = j;
                    t_high_value = ini_account_money + sum_pnl1;
                }
            }

            if( strncmp( tradingResult[i].exitTime , vecTradeWeekDay[tw].c_str() , 10) <= 0 ){
                //printf("%s %s\n",tradingResult[i].exitTime , vecTradeWeekDay[tw].c_str());
                t_week_profit += pnl;
            }else{
                //printf("t_week_profit %lf\n",t_week_profit);
                if(t_week_profit > eps){
                    this->win_week_num += 1;
                }
                if(t_week_profit < eps){
                    this->loss_week_num += 1;
                }
                tw+=1;
                t_week_profit = pnl;
                //while( strncmp( tradingResult[i].exitTime , vecTradeWeekDay[tw].c_str() , 10) > 0) tw++;
            }
        }
        this->variance = sqrt(t_variance * 1.0 / (tradingNum - 1));
        this->sharpe = this->ave_cover_income * 1.0 * tradingNum / 250.0 / this->variance;
        this->kurtosis = t_kurtosis_fenzi * 1.0 / (tradingNum * pow(this->variance, 3.0));
        this->skewness = t_skewness * 1.0 / ( tradingNum * pow(this->variance , 4.0));
        this->sortinoRatio = sqrt(52.0) * this->rate_of_return * sqrt(tradingNum - 1) / sqrt(t_ddex);

        strcpy(this->max_windraw_start_date, tradingResult[t_max_windraw_start_date].exitTime);
        strcpy(this->max_windraw_end_date , tradingResult[t_max_windraw_end_date].exitTime);
        this->max_windrawl_number = t_max_windrawl_number;
        this->windrawl_safe_rate = fabs(this->total_income * 1.0 / this->max_windrawl_number);
        this->max_windrawal_rate = this->max_windrawl_number * 1.0 / t_high_value;

        this->targetFunctionRankScore = this->targetFunctionRank();
        this->targetFunctionPromScore = this->targetFunctionProm();
    }
    void showResult(){
        printf("账户初始资金:%lf\n",ini_account_money);
        printf("年化收益:%lf\n",rate_of_return);
        printf("总开仓次数:%d\n",total_open_num);
        printf("胜率:%lf\n",win_rate);
        printf("盈利因子:%lf\n",profit_factor);
        printf("最大连续亏损:%lf\n",max_continue_loss);
        printf("最大回撤比率:%lf\n",max_windrawal_rate);
        printf("夏普比率:%lf\n",sharpe);
        printf("交易日个数:%d\n",total_trade_days);
        printf("总共交易周:%d\n",total_trade_weeks);
        printf("总盈利:%lf\n",net_profits);
        printf("总亏损:%lf\n",net_loss);
        printf("净利润:%lf\n",total_income);
        printf("盈利周数:%d\n",win_week_num);
        printf("亏损周数:%d\n",loss_week_num);
        printf("开仓盈利次数:%d\n",win_cover_num);
        printf("平仓亏损次数:%d\n",loss_cover_num);
        printf("赔率:%lf\n",loss_rate);
        printf("最大连续盈利:%lf\n",max_continue_win);
        printf("最大连赢次数:%d\n",max_continue_win_num);
        printf("最大连亏次数:%d\n",max_continue_loss_num);
        printf("平均平仓净利润:%lf\n",ave_cover_income);
        printf("平均平仓盈利:%lf\n",ave_cover_win);
        printf("平均平仓亏损:%lf\n",ave_cover_loss);
        printf("平均平仓盈亏比:%lf\n",ave_cover_win_loss_rate);
        printf("最大单笔盈利:%lf\n",max_single_win);
        printf("最大单笔亏损:%lf\n",max_single_loss);
        printf("最大回撤数值:%lf\n",max_windrawl_number);
        printf("最大回撤区间开始:%s\n",max_windraw_start_date);
        printf("最大回撤区间结束:%s\n",max_windraw_end_date);
        printf("回撤安全系数:%lf\n",windrawl_safe_rate);
        printf("总交易成本:%lf\n",total_transcaction_fee);
        printf("平均交易成本:%lf\n",ave_transcation_fee);
        printf("交易成本覆盖率:%lf\n",transcation_cover_rate);
        printf("实际收益率:%lf\n",real_income_rate);
        printf("平均每次收益率:%lf\n",ave_time_income_rate);
        printf("波动率:%lf\n",variance);
        printf("峰度:%lf\n",kurtosis);
        printf("偏度:%lf\n",skewness);
        printf("索提诺比率:%lf\n",sortinoRatio);
        printf("目标函数排名的实际分数:%lf\n",targetFunctionRankScore);
        printf("目标函数Prom的实际分数:%lf\n",targetFunctionPromScore);

    }
};

// 将报单进行合法化， 需要按照最小一跳进行处理

double legalTradePrice(double price, double volume , double minMovement){
    const int jingdu = 1000;
    if(volume > 0){
        int v = (int)(price*jingdu + 0.9) , k = minMovement * jingdu ;
        if(v % k == 0){
            return price;
        }else{
            return 1.0 * ( v / k * k / jingdu + minMovement);
        }
    }else{
        int v = (int)(price*jingdu) , k = (int)(minMovement * jingdu) ;
        return 1.0 * (v / k * k / jingdu) ;
    }
}

class AligatorStrategy{
    public:
    static const int BufferSize = 30;

    vector<string> g_datetime , g_date , g_time;
    vector<double> closeArray , openArray , highArray , lowArray;
    int tradingNum ;
    vector<struct tradeNode> tradingResult;

    int pos;
    vector<int> mp;                     // 总持仓
    vector<double> free_money , total_money;// 空闲资金 ,, 总资金 (空闲资金 + 持仓)

    int BarsSinceEntry;                 // 账户是否进入
    /////////////////////////////////////////////////////////////
    int CF  ,CM , CS , d_CF , d_CM , d_CS ;
    int N_up  , N_down ;
    double zhisunlv_l  , zhisunlv_s ;
    ///////////////////////////////////////////////////////////////
    double slippage , rate , perSize , minMovement , PriceScale ;

    int lots;
    double zhisun_l , zhisun_s ;
    double kai_up, kai_down;

    vector<double> lips_N, teeth_N, croco_N , lips , teeth , croco , high_since_entry ,low_since_entry;
    vector<int> zuiArray , eyuArray;

    int bufferCount ;
    int n ;

    //////  账户信息栏
    int account_money ;   // 账户初始资金
    vector<string> vecTradeDay;
    vector<string> vecTradeWeekDay;
    //读取输入的数据
    void loadFile(const char *filename , const char *start_date , const char *end_date){
        FILE *fp = NULL;
        if(fp = fopen(filename, "r")){
            n = 0;
            g_datetime.clear(),g_date.clear(),g_time.clear();
            openArray.clear(),highArray.clear(),lowArray.clear(),closeArray.clear();
            
            char tdate[50],ttime[50] ,tdatetime[100];
            double open,high,low,close,volume,openInterest;
            while(fscanf(fp,"%s %5s,%lf,%lf,%lf,%lf,%lf,%lf", tdate,ttime, &open,&high,&low,&close,&volume,&openInterest) != EOF){
                if(strcmp(start_date , tdate) <= 0 && strcmp(tdate , end_date) <= 0){

                    sprintf(tdatetime, "%s %s",tdate,ttime);
                    g_datetime.push_back(string(tdatetime));
                    g_date.push_back(tdate);  g_time.push_back(ttime);

                    openArray.push_back(open); highArray.push_back(high);
                    lowArray.push_back(low);  closeArray.push_back(close);
                    n = n + 1;
                }
            }
            fclose(fp);
            vecTradeDay.clear();  vecTradeWeekDay.clear();
            for(int i = 0;i < n;i++){
                if(i == 0){
                    vecTradeDay.push_back(string(g_date[i]));
                    if(CaculateWeekDayS(g_date[i].c_str()) == 4){
                        vecTradeWeekDay.push_back(g_date[i]);
                    }
                }else{
                    if( strcmp(vecTradeDay[vecTradeDay.size()-1].c_str() , g_date[i].c_str()) != 0){
                        vecTradeDay.push_back( g_date[i]);
                        if(CaculateWeekDayS(g_date[i].c_str()) == 4){
                            vecTradeWeekDay.push_back(g_date[i]);
                        }
                    }
                }
            }
        }else{
            printf("open file %s Error!" , filename);
        }
    }
    void Sell(int size , double price , int iBar){
        if(pos > 0){
            pos = pos - size;
            #ifdef debug
                printf("))pos %d sell %s %lf tradingNum %d\n", pos ,g_datetime[iBar].c_str() , price, tradingNum);
            #endif
            tradingResult[tradingNum].exitPrice = legalTradePrice(price, -1.0 , minMovement);
            strcpy(tradingResult[tradingNum].exitTime , g_datetime[iBar].c_str());
            tradingNum += 1;
            tradingResult.push_back(tradeNode());
        }
    }
    void BuyToCover(int size , double price , int iBar){
        if(pos < 0){
            pos = pos + size;
            #ifdef debug
                printf("}}pos %d buytocover %s %lf tradingNum %d\n", pos ,g_datetime[iBar].c_str()  , price, tradingNum);
            #endif
            tradingResult[tradingNum].exitPrice = legalTradePrice(price , 1.0 , minMovement);
            strcpy(tradingResult[tradingNum].exitTime , g_datetime[iBar].c_str());
            tradingNum += 1;
            tradingResult.push_back(tradeNode());
        }
    }
    void Buy(int size , double price , int iBar){
        if(pos < 0){
            BuyToCover( abs(pos) , price, iBar);
        }
        pos = pos + size;
        #ifdef debug
            printf("((pos %d buy %s %lf tradingNum %d\n", pos ,g_datetime[iBar].c_str()  , price, tradingNum);
        #endif
        tradingResult[tradingNum].entryPrice = legalTradePrice(price, 1.0 , minMovement);
        tradingResult[tradingNum].volume = size;
        strcpy(tradingResult[tradingNum].entryTime , g_datetime[iBar].c_str());
    }
    void SellShort(int size , double price , int iBar){
        if(pos > 0){
            Sell( fabs(pos) , price , iBar);
        }
        pos = pos - size;
        #ifdef debug
            printf("{{pos %d sellshort %s %lf tradingNum %d\n", pos ,g_datetime[iBar].c_str()  , price, tradingNum);
        #endif
        tradingResult[tradingNum].entryPrice = legalTradePrice(price, -1.0 , minMovement);
        tradingResult[tradingNum].volume = -size;
        strcpy(tradingResult[tradingNum].entryTime , g_datetime[iBar].c_str());
    }

    // 更新barsEntry 这个东西
    void updateBarsEntry(int iBar){
        if(pos != 0){
            BarsSinceEntry += 1;
        }
        if(mp[max(0,iBar-1)] != 0 && 0 == mp[iBar]){
            BarsSinceEntry = 0;
        }
    }
    void init(){
        CF = 5 , CM = 8 , CS = 13 , d_CF = 3 , d_CM = 5 , d_CS = 8;
        N_up = 10 , N_down = 10;
        zhisunlv_l = 10 , zhisunlv_s = 10 ;


        slippage = 1.0 , rate = 1.29/10000 , perSize = 10 , minMovement = 1.0 , PriceScale = 1.0;

        lots = 1;
        zhisun_l = 0 , zhisun_s = 0;
        kai_up=0, kai_down=0;

        lips_N.reserve(n + 10) , teeth_N.reserve(n + 10) , croco_N.reserve(n + 10) ;
        lips.reserve(n + 10)  , teeth.reserve(n + 10)  , croco.reserve(n + 10) ;

        zuiArray.reserve(n + 10) , eyuArray.reserve(n + 10);
        high_since_entry.reserve(n + 10) , low_since_entry.reserve(n + 10);

        bufferCount = 0;
        mp.reserve(n + 10);
        //////  账户信息栏
        account_money = 40000;   // 账户初始资金
    }
    void showResult2(){
        printf("showResult2\n");
        TradingResultNode a(tradingResult,tradingNum , account_money ,slippage, rate , perSize , vecTradeDay , vecTradeWeekDay);
        a.showResult();
    }
    void run(){
        // System Variable
        tradingNum = 0;
        tradingResult.clear();
        tradingResult.push_back(tradeNode());
        pos = 0;
        BarsSinceEntry = 0;
        for(int i = 0;i < n;i++){
            mp[i] = 0;
        }
        // Strategy Variable
        zhisun_l = 0 , zhisun_s = 0;
        kai_up, kai_down;
        bufferCount = 0;
        zuiArray[0] = eyuArray[0] = 0;
        high_since_entry[0] = low_since_entry[0] = 0.0;

        xAverage(lips_N  ,closeArray , CF , n);
        xAverage(teeth_N ,closeArray , CM , n);
        xAverage(croco_N ,closeArray , CS , n);

        LeftMove(lips , lips_N , d_CF , n);
        LeftMove(teeth , teeth_N , d_CM ,n);
        LeftMove(croco , croco_N , d_CS ,n);


        for( int i = 1; i < n;i++){ // i = BufferSize - 1

            zuiArray[i]         = zuiArray[i-1];
            eyuArray[i]         = eyuArray[i-1];
            high_since_entry[i] = high_since_entry[i-1];
            low_since_entry[i]  = low_since_entry[i-1];
            mp[i]               = mp[i-1];
            updateBarsEntry(i);  // 更新持仓


            if(lips[i] > teeth[i] && teeth[i] > croco[i]){
                zuiArray[i] = 1;
            }else if(lips[i] < teeth[i] && teeth[i] < croco[i]){
                zuiArray[i] = -1;
            }else{
                zuiArray[i] = 0;
            }

            printf("i=%d\n",i);
            if( 1 == zuiArray[i-1] && closeArray[i-1] > openArray[i-1] && lowArray[i-1] > lips[i-1] && openArray[i] > lips[i]){
                eyuArray[i] = 1;
            }
            if( 1 == eyuArray[i-1] && lowArray[i] < lips[i]){
                eyuArray[i] = 0;
            }
            if(-1 == zuiArray[i-1] && closeArray[i-1] < openArray[i-1] && highArray[i-1] < lips[i-1] && openArray[i] < lips[i]){
                eyuArray[i] = -1;
            }
            if(-1 == eyuArray[i-1] && highArray[i] > lips[i]){
                eyuArray[i] = 0;
            }

            if(0 == eyuArray[i-1] && 1 == eyuArray[i]){
                kai_up = openArray[i] + N_up * minMovement * PriceScale;
            }
            if(0 == eyuArray[i-1] && -1 == eyuArray[i]){
                kai_down = openArray[i] - N_down * minMovement * PriceScale;
            }

            if(eyuArray[i] > 0 && highArray[i] > kai_up && mp[i] < 1){
                if( openArray[i] > kai_up){
                    #ifdef debug
                        printf("date %s time %s ,buy price %lf , volume %lf\n", g_date[i].c_str()  , g_time[i].c_str()  ,openArray[i] , 1.0*lots);
                    #endif
                    Buy(lots , openArray[i] , i);
                    zhisun_l = openArray[i] * ( 1 - zhisunlv_l * 1.0 / 1000.0);
                }else{
                    #ifdef debug
                        printf("date %s time %s ,buy price %lf , volume %lf\n", g_date[i].c_str()  , g_time[i].c_str()  ,kai_up , 1.0*lots);
                    #endif // DEBUG
                    Buy(lots , kai_up , i);
                    zhisun_l = kai_up * ( 1 - zhisunlv_l * 1.0 / 1000.0);
                }
                high_since_entry[i] = highArray[i];
            }

            if(eyuArray[i] < 0 && lowArray[i] < kai_down && mp[i] > -1){

                if(openArray[i] < kai_down){
                    #ifdef debug
                    printf("date %s time %s ,SellShort price %lf , volume %lf\n", g_date[i].c_str()  , g_time[i].c_str()  ,openArray[i] , 1.0*lots);
                    #endif // debug
                    SellShort(lots , openArray[i] , i);
                    zhisun_s = openArray[i] * ( 1 + zhisunlv_s * 1.0 / 1000.0);
                }else{
                    #ifdef debug
                    printf("date %s time %s ,SellShort price %lf , volume %lf\n", g_date[i].c_str() , g_time[i].c_str()  ,kai_down , 1.0*lots);
                    #endif // debug
                    SellShort(lots , kai_down , i);
                    zhisun_s = kai_down * ( 1 + zhisunlv_s * 1.0 / 1000.0);
                }

                low_since_entry[i] = lowArray[i];
            }

            if( mp[i] > 0 && highArray[i] > high_since_entry[i]){
                high_since_entry[i] = highArray[i];
            }
            if( mp[i] > 0 && mp[i-1] > 0){
                zhisun_l = high_since_entry[i-1] * ( 1-zhisunlv_l *1.0 / 1000.0);
            }
            if( mp[i] < 0){
                low_since_entry[i] = min(low_since_entry[i] , lowArray[i]);
            }
            if( mp[i] < 0 && mp[i-1] < 0){
                zhisun_s = low_since_entry[i-1] * ( 1+zhisunlv_s *1.0 / 1000.0);
            }

            if( mp[i] > 0 && lowArray[i] < zhisun_l && BarsSinceEntry > 0){
                if( openArray[i] < zhisun_l){
                    #ifdef debug
                    printf("date %s time %s ,Sell price %lf , volume %lf\n", g_date[i].c_str()  , g_time[i].c_str()  ,openArray[i] , 1.0*lots);
                    #endif // debug
                    Sell(lots , openArray[i] , i);
                }else{
                    #ifdef debug
                    printf("date %s time %s ,Sell price %lf , volume %lf\n", g_date[i].c_str()  , g_time[i].c_str()  ,zhisun_l , 1.0*lots);
                    #endif // debug
                    Sell(lots , zhisun_l , i );
                }
                eyuArray[i] = 0;
            }
            if( mp[i] < 0 && highArray[i] > zhisun_s && BarsSinceEntry > 0){
                if( openArray[i] > zhisun_s){
                    #ifdef debug
                        printf("date %s time %s ,BuyToCover price %lf , volume %lf\n", g_date[i].c_str()  , g_time[i].c_str()  ,openArray[i] , 1.0*lots);
                    #endif // debug
                    BuyToCover(lots , openArray[i] , i);
                }else{
                    #ifdef debug
                    printf("date %s time %s ,BuyToCover price %lf , volume %lf\n", g_date[i].c_str()  , g_time[i].c_str()  ,zhisun_s , 1.0*lots);
                    #endif // debug
                    BuyToCover(lots , zhisun_s , i);
                }
                eyuArray[i] = 0;
            }

            mp[i] = pos;

            if( 0 == pos || mp[i-1] != mp[i]){
                BarsSinceEntry = 0;
            }
            #ifdef debug
            printf("date %s time %s ,zui %d eyu %d kai_up %lf kai_down %lf\n", g_date[i].c_str()  , g_time[i].c_str()  ,zuiArray[i], eyuArray[i] ,kai_up , kai_down);
            printf("date %s time %s ,lips %lf teeth %lf croco %lf\n", g_date[i].c_str()  , g_time[i].c_str()  , lips[i] , teeth[i], croco[i]);
            printf("date %s time %s ,mp %d barsSinceEntry %d\n" ,g_date[i].c_str()  , g_time[i].c_str()  , mp[i] , BarsSinceEntry);
            #endif // debug
        }
    }

    void showResult(){
        printf("tradingNum: %d\n",tradingNum);
        double capital = 0.0;
        double long_capital = 0.0, short_capital = 0.0;
        double total_commission = 0.0, total_slippages = 0.0;
        double max_long_income = 0.0 ;
        for(int i = 0;i < tradingNum; i++){
            double turnover = (tradingResult[i].entryPrice + tradingResult[i].exitPrice) * perSize * fabs(tradingResult[i].volume);
            double commission = turnover * rate;
            double per_slippage = slippage * 2 * perSize * fabs(tradingResult[i].volume);
            double pnl = (tradingResult[i].exitPrice - tradingResult[i].entryPrice) * tradingResult[i].volume * perSize - commission - per_slippage;
            #ifdef debug
            printf("entryTime: %s , entry price:%lf ,exitTime%s : exit price:%lf\n",tradingResult[i].entryTime, tradingResult[i].entryPrice, tradingResult[i].exitTime, tradingResult[i].exitPrice);
            #endif // debug
            total_commission += commission;
            capital += pnl ;
            total_slippages += per_slippage;
            if(tradingResult[i].volume > 0){
                long_capital += pnl ;
                max_long_income = max(max_long_income , pnl );
            }
            if(tradingResult[i].volume < 0){
                short_capital += pnl ;
                max_long_income = max(max_long_income , pnl );
            }
        }
        printf("capital:%lf\nlong_capital:%lf\nshort_capital %lf\n",capital,long_capital,short_capital);
        printf("total_commision:%lf\ntotal_slippages:%lf\n",total_commission,total_slippages);
        printf("max_long_income:%lf\n",max_long_income);
    }
};


int main(){
    printf("Hello world\n");
    class AligatorStrategy a;
    a.loadFile("rb888_30_minutes.csv" , "2009/03/27" , "2009/12/01");
    //a.loadFile("rb888_30_minutes.csv" , "2009/03/27" , "2050/03/31");
    a.init();
    a.run();
    a.showResult();
    a.showResult2();
    //loadFile("rb888_30_minutes.csv" , "2009/03/27" , "2009/12/01");

    /*
    for(int i = 0;i < vb.size() && i < 10; i++){
        Bar a = vb[i];
        cout << a.printOut() << endl;

    }
    */
    //LoopBack(vb , "2009/03/27" , "2050/03/31"); // 2050/03/31 will never be reached !! Take tare of it .
    return 0;
}
