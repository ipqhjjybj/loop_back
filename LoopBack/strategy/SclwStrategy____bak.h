
#ifndef _SclwStrategy
#define _SclwStrategy

#include "load_config.h"
#include "ctaStrategyTemplate.h"

/**
 *
12 26 9
4 6
14 3 3
30 70
15
0.8
10
60
 */
class SclwStrategy : public ctaTemplate{

public:
    static const int BufferSize = 30;

    /////////////////////////////////////////////////////////////
    //申明参数
    int macd_fast_length;       //长周期 macd FastLength
    int macd_slow_length;       //长周期 macd SlowLength
    int macd_length;            //长周期 macd Length

    int longHigh_useLength;     //长周期最高点 信号
    int longTupo_length;        //长周期信号突破确立

    int kdj_length;             //中周期 KDJ Length
    int kdj_slowlength;         //中周期 KDJ SlowLength
    int kdj_smoothlength;       //中周期 KDJ SmoothLength

    double midK_buy_value;      //K 值小于该值则买
    double midK_sell_value;     //K 值大于该值则卖

    int HighShortLength;        //短周期 几天内高点与低点

    double trailingPrcnt;       //移动止损

    int midUseBar;              //通过几根 短周期的数据 mi 构造 中周期的数据
    int longUseBar;             //通过几根 短周期的数据 来 构造 长周期的数据
    ///////////////////////////////////////////////////////////////
    //申明变量
    double intraTradeHigh;      //持有内最高点
    double intraTradeLow;       //持有内最低点

    vector<double> midOpenArray;
    vector<double> midCloseArray;
    vector<double> midHighArray;
    vector<double> midLowArray;

    vector<int> midBarMapper;

    vector<double> mid_k_arr;
    vector<double> mid_d_arr;
    vector<double> mid_j_arr;

    vector<double> longOpenArray;
    vector<double> longHighArray;
    vector<double> longLowArray;
    vector<double> longCloseArray;

    vector<int> longBarMapper;

    vector<double> long_macd_arr;
    vector<double> long_avgMacd_arr;
    vector<double> long_macd_diff_arr;

    vector<double> long_highest_arr;
    vector<double> long_highest_highest_arr;
    vector<double> long_lowest_arr;
    vector<double> long_lowest_lowest_arr;


    vector<double> short_highest_arr;
    vector<double> short_lowest_arr;

    double highSinceEntry , lowSinceEntry;


    int signalLong;
    int signalMid;

    int bufferCount ;

    SclwStrategy(){
        this->setStrategyName("SclwStrategy");
        ctaTemplate();
    }
    void Sell(int size , double price , int iBar){
        ctaTemplate::Sell(size , price , iBar);
    }
    void BuyToCover(int size , double price , int iBar){
        ctaTemplate::BuyToCover(size , price, iBar);
    }
    void Buy(int size , double price , int iBar){
        ctaTemplate::Buy(size ,price , iBar);
    }
    void SellShort(int size , double price , int iBar){
        ctaTemplate::SellShort(size ,price , iBar);
    }
    void init()
    {
        ctaTemplate::init();

        macd_fast_length = 6;
        macd_slow_length = 12;
        macd_length = 6;

        longHigh_useLength = 4;
        longTupo_length = 6;

        kdj_length = 14;
        kdj_slowlength = 3;
        kdj_smoothlength = 3;

        midK_buy_value = 30;
        midK_sell_value = 70;

        HighShortLength = 15;

        trailingPrcnt = 0.8;

        signalLong = 0;
        signalMid = 0;

        intraTradeHigh = 0;
        intraTradeLow = 0;

        midUseBar = 15;
        longUseBar = 60;

        lots = 1;

        //printf("2 n=%d\n",n);
        bufferCount = 0;

        highSinceEntry = 0.0;
        lowSinceEntry = 0.0;
    }
    vector<string> getCanshuSetNames(){
        vector<string> vCanshu; vCanshu.clear();

        vCanshu.push_back("macd_fast_length");
        vCanshu.push_back("macd_slow_length");
        vCanshu.push_back("macd_length");

        vCanshu.push_back("longHigh_useLength");
        vCanshu.push_back("longTupo_length");

        vCanshu.push_back("kdj_length");
        vCanshu.push_back("kdj_slowlength");
        vCanshu.push_back("kdj_smoothlength");

        vCanshu.push_back("midK_buy_value");
        vCanshu.push_back("midK_sell_value");

        vCanshu.push_back("HighShortLength");
        vCanshu.push_back("trailingPrcnt");

        vCanshu.push_back("midUseBar");
        vCanshu.push_back("longUseBar");

        return vCanshu;
    }
    /**
     * 使用 conf 下面的内置的函数来赋予 滑点、手续费比例等。
     ***/
    void init(const char *code){
        ctaTemplate::init(code);
        this->init();
        this->initFee(code);

        if( is_zuhehuice == 1){
            account_money = __const_ini_money;
            lots = __const_ini_money / (__high_history_price ) /2;
        }else{
            account_money = __high_history_price * perSize * cash_deposit * 5 / 100.0;
            lots = 1;
            if(account_money < 1){
                account_money = __const_ini_money;
            }
        }

    }
    void setCanshuFromVec(const vector<double> &arr){

        macd_fast_length = arr[0];
        macd_slow_length = arr[1];
        macd_length = arr[2];

        longHigh_useLength = arr[3];
        longTupo_length = arr[4];

        kdj_length = arr[5];
        kdj_slowlength = arr[6];
        kdj_smoothlength = arr[7];

        midK_buy_value = arr[8];
        midK_sell_value = arr[9];

        HighShortLength = arr[10];

        trailingPrcnt = arr[11];

        midUseBar = arr[12];
        longUseBar = arr[13];
    }

    TradingResultNode getTradingResult( const char *_code , const char *_period,const char *start_date , const char *end_date){
        TradingResultNode a = TradingResultNode( _code, _period ,start_date, end_date ,tradingResult,tradingNum , account_money ,slippage * n_slippages, rate , perSize , cash_deposit, vecTradeDay , vecTradeWeekDay , dayIncome , weekIncome , bzjAccount , dayAccount, positionAccount , fee_type);
        vector<double> d; d.clear();
        d.push_back( this->macd_fast_length); d.push_back( this->macd_slow_length); d.push_back( this->macd_length);
        d.push_back( this->longHigh_useLength); d.push_back(this->longTupo_length);
        d.push_back( this->kdj_length); d.push_back(this->kdj_slowlength);  d.push_back(this->kdj_smoothlength);
        d.push_back( this->midK_buy_value); d.push_back(this->midK_sell_value);
        d.push_back( this->HighShortLength); d.push_back(this->trailingPrcnt);
        d.push_back( this->midUseBar); d.push_back(this->longUseBar);

        a.setCanshuSet(d);
        return a;
    }

    void onBar_bef(int i){
        ctaTemplate::onBar_bef(i);
    }

    void onBar_after(int i){
        ctaTemplate::onBar_after(i);
    }

    int locateUpperBar(int i , int will_use_minute , const vector<int> &mapperBar){
        if( (i + 1) % will_use_minute == 0){
            return mapperBar[i];
        }else{
            return mapperBar[i] - 1;
        }
    }

    void onBar(int i)
    {
        onBar_bef(i);
        //Series类型数据，继承前一根bar的值
        if(i >= longUseBar){

            int midLoc = this->locateUpperBar(i , midUseBar , midBarMapper);
            int longLoc = this->locateUpperBar(i , longUseBar , longBarMapper);

            if(midLoc >= 1 and longLoc >= 1){
                if( longHighArray[longLoc] >= long_highest_highest_arr[longLoc-1]){
                    signalLong = 1;
                }
                if( longLowArray[longLoc] <= long_lowest_lowest_arr[longLoc-1]){
                    signalLong = -1;
                }
                signalMid = 0;
                if( mid_j_arr[midLoc] < midK_buy_value){
                    signalMid = 1;
                }
                if( mid_j_arr[midLoc] > midK_sell_value){
                    signalMid = -1;
                }
                if( this->pos == 0){
                    if( signalLong * signalMid < 0){
                        if( signalLong > 0 && closeArray[i-1] < short_highest_arr[i-1] && highArray[i] > short_highest_arr[i-1] + 2){
                            this->Buy( lots , max(openArray[i] , short_highest_arr[i-1] + 2) , i);
                        }
                        else if( signalLong < 0 && closeArray[i-1] > short_lowest_arr[i-1] && lowArray[i] < short_lowest_arr[i-1] - 2){
                            this->SellShort(lots , min(openArray[i] , short_lowest_arr[i-1] - 2) , i);
                        }
                    }
                    highSinceEntry = highArray[i];
                    lowSinceEntry = lowArray[i];
                }else if( this->pos > 0){
                    double stopValue = highSinceEntry * ( 1 - trailingPrcnt * 1.0 / 100);

                    if( lowArray[i] < stopValue){
                        this->Sell( abs(this->pos) , min(openArray[i] , stopValue) , i);
                    }
                    highSinceEntry = max(highSinceEntry, highArray[i]);
                    lowSinceEntry = lowArray[i];
                }else if( this->pos < 0){
                    double stopValue = lowSinceEntry * ( 1 + trailingPrcnt * 1.0 / 100);
                    if( highArray[i] > stopValue){
                        this->BuyToCover( abs(this->pos) , max(openArray[i] , stopValue) , i);
                    }
                    highSinceEntry = highArray[i];
                    lowSinceEntry = min(lowSinceEntry , lowArray[i]);
                }
            }

        }
        onBar_after(i);
    }

    void beforeRun( )
    {
        ctaTemplate::beforeRun();

        // Strategy Variable
        signalLong = 0;
        signalMid = 0;
        midOpenArray.clear(); midCloseArray.clear(); midHighArray.clear(); midLowArray.clear(); midBarMapper.clear();
        mid_k_arr.clear(); mid_d_arr.clear(); mid_j_arr.clear();
        longOpenArray.clear(); longHighArray.clear(); longLowArray.clear(); longCloseArray.clear(); longBarMapper.clear();
        long_macd_arr.clear(); long_avgMacd_arr.clear(); long_macd_diff_arr.clear();
        long_highest_arr.clear(); long_lowest_arr.clear();
        long_highest_highest_arr.clear(); long_lowest_lowest_arr.clear();
        short_highest_arr.clear(); short_lowest_arr.clear();

        midOpenArray.reserve(n / midUseBar + 10); midHighArray.reserve(n / midUseBar + 10);  midCloseArray.reserve(n / midUseBar + 10);  midCloseArray.reserve(n / midUseBar + 10);  midBarMapper.reserve(n + 10);
        mid_k_arr.reserve(n / midUseBar + 10); mid_d_arr.reserve(n / midUseBar + 10); mid_j_arr.reserve(n / midUseBar + 10);
        longOpenArray.reserve(n / longUseBar + 10); longHighArray.reserve(n / longUseBar + 10); longLowArray.reserve(n / longUseBar + 10); longCloseArray.reserve(n / longUseBar + 10); longBarMapper.reserve(n + 10);
        long_macd_arr.reserve(n / longUseBar + 10); long_avgMacd_arr.reserve(n / longUseBar + 10); long_macd_diff_arr.reserve(n / longUseBar + 10);
        long_highest_arr.reserve(n / longUseBar + 10); long_lowest_arr.reserve(n / longUseBar + 10);
        long_highest_highest_arr.reserve(n / longUseBar + 10);  long_lowest_lowest_arr.reserve(n / longUseBar + 10);
        short_highest_arr.reserve(n + 10);  short_lowest_arr.reserve(n + 10);
        // 通过短周期的数据 构造 中周期的 closeArray

        combineK_line( midBarMapper , midOpenArray , midHighArray , midLowArray , midCloseArray , openArray , highArray , lowArray , closeArray , midUseBar ,n);
        combineK_line( longBarMapper , longOpenArray , longHighArray , longLowArray , longCloseArray , openArray , highArray , lowArray , closeArray , longUseBar ,n);

        KDJ( mid_k_arr , mid_d_arr , mid_j_arr , midHighArray, midLowArray , midCloseArray, kdj_length , kdj_slowlength , kdj_smoothlength , midHighArray.size());
        MACD( long_macd_arr , long_avgMacd_arr , long_macd_diff_arr, longCloseArray , macd_fast_length , macd_slow_length , macd_length , longCloseArray.size());

        Highest(long_highest_arr , longHighArray , longHigh_useLength , longHighArray.size());
        Lowest(long_lowest_arr , longLowArray , longHigh_useLength , longLowArray.size());

        Highest(long_highest_highest_arr , long_highest_arr , longTupo_length , long_highest_arr.size());
        Lowest(long_lowest_lowest_arr , long_lowest_arr , longTupo_length , long_lowest_arr.size());

        Highest(short_highest_arr , highArray , HighShortLength , n);
        Lowest(short_lowest_arr , lowArray , HighShortLength , n);

        signalLong = 0;
        signalMid = 0;
    }

    /**
     *  这里提供一种暴力循环获得 所有参数的例子
     *  本来应该使用递归的方式做的, 但后来想了一下，， 还是用这种来做吧， 毕竟简单，可以添加各种条件约束
     **/
    virtual void loopForce(){
        // this->CF = arr[0]; this->CM = arr[1]; this->CS = arr[2]; this->d_CF = arr[3] ; this->d_CM = arr[4];  this->d_CS = arr[5];
        // this->N_up = arr[6]; this->N_down = arr[7]; this->zhisunlv_l = arr[8];  this->zhisunlv_s = arr[9];

        FILE *fp = NULL;
        string loopForceFileName = string("./runningTEMP/") + this->strategyName + ".csv";
        printf("loopForceFileName:%s\n",loopForceFileName.c_str());
        int t_canshuNum = 0;
        if( (fp = fopen(loopForceFileName.c_str(), "w")) != NULL){
            int baoben_n_beg = 1;
            int baobenlv_l_beg = 1;
            int baobenlv_s_beg = 1;
            int fuyinglv_l_beg = 1;
            int fuyinglv_s_beg = 1;
            int per_fy_beg = 1;
            int fuying_b_beg = 1;

            for (int baoben_f = 0; baoben_f<1 ; baoben_f +=1){
                for(int zhisun_f = 1; zhisun_f<2; zhisun_f +=1){
                    for(int CF = 3; CF < 8;CF += 2){
                        for(int CM = CF + 1;CM < 16; CM += 3){
                            for(int CS = CM +1  ;CS < 31;CS += 5){
                                for(int d_CF = 2; d_CF < 5; d_CF += 2){
                                    for(int d_CM = d_CF; d_CM < 8; d_CM += 2){
                                        for(int d_CS = d_CM; d_CS < 13; d_CS += 3){
                                            for(int N_up = 1; N_up < 20; N_up += 5){
                                                for(int N_down = 1;N_down < 20; N_down += 5){
                                                    for(int zhisunlv_l = 1; zhisunlv_l < 11; zhisunlv_l += 2){
                                                        for(int zhisunlv_s = 1; zhisunlv_s < 11; zhisunlv_s += 2){
                                                        //for (int baoben_f = 0; baoben_f<3 ; baoben_f +=1){
                                                            baoben_n_beg = 1;
                                                            baobenlv_l_beg = 5;
                                                            baobenlv_s_beg = 5;
                                                            if (0 == baoben_f)
                                                            {
                                                                baoben_n_beg = 5;
                                                                baobenlv_l_beg = 20;
                                                                baobenlv_s_beg = 20;
                                                            }
                                                            if (1 == baoben_f)
                                                            {
                                                                baoben_n_beg = 5;
                                                            }

                                                            for (int baoben_n = baoben_n_beg; baoben_n<6; baoben_n +=2){
                                                                for (int baobenlv_l = baobenlv_l_beg; baobenlv_l<21; baobenlv_l +=5){
                                                                    for (int baobenlv_s = baobenlv_s_beg; baobenlv_s<21; baobenlv_s +=5){
                                                                    //for(int zhisun_f = 1; zhisun_f<4; zhisun_f +=1){
                                                                        fuyinglv_l_beg = 5;
                                                                        fuyinglv_s_beg = 5;
                                                                        per_fy_beg = -3;
                                                                        fuying_b_beg = 2;
                                                                        if (1 == zhisun_f)
                                                                        {
                                                                            fuyinglv_l_beg = 20;
                                                                            fuyinglv_s_beg = 20;
                                                                            per_fy_beg = 3;
                                                                            fuying_b_beg = 6;
                                                                        }
                                                                        if ( 2 == zhisun_f)
                                                                        {
                                                                            fuying_b_beg = 6;
                                                                        }
                                                                        for (int fuyinglv_l= fuyinglv_l_beg; fuyinglv_l<21; fuyinglv_l +=5){
                                                                            for (int fuyinglv_s = fuyinglv_s_beg; fuyinglv_s<21; fuyinglv_s +=5){
                                                                                for (int per_fy = per_fy_beg; per_fy<4; per_fy +=1){
                                                                                    for (int fuying_b = fuying_b_beg; fuying_b<7; fuying_b +=2){

                                                                                        vector<double> tmp; tmp.clear();
                                                                                        tmp.push_back( CF * 1.0);
                                                                                        tmp.push_back( CM * 1.0);
                                                                                        tmp.push_back( CS * 1.0);
                                                                                        tmp.push_back( d_CF * 1.0);
                                                                                        tmp.push_back( d_CM * 1.0);
                                                                                        tmp.push_back( d_CS * 1.0);

                                                                                        tmp.push_back( N_up * 1.0);
                                                                                        tmp.push_back( N_down * 1.0);

                                                                                        tmp.push_back( zhisunlv_l * 1.0);
                                                                                        tmp.push_back( zhisunlv_s * 1.0);

                                                                                        tmp.push_back( baoben_f * 1.0);
                                                                                        tmp.push_back( baoben_n * 1.0);
                                                                                        tmp.push_back( baobenlv_l * 1.0);
                                                                                        tmp.push_back( baobenlv_s * 1.0);

                                                                                        tmp.push_back( zhisun_f * 1.0);
                                                                                        tmp.push_back( fuyinglv_l * 1.0);
                                                                                        tmp.push_back( fuyinglv_s* 1.0);
                                                                                        tmp.push_back( per_fy * 0.1);
                                                                                        tmp.push_back( fuying_b * 1.0);

                                                                                        for(int t_ji = 0; t_ji < tmp.size(); t_ji++){
                                                                                            if(0 == t_ji){
                                                                                                fprintf(fp,"%lf",tmp[t_ji]);
                                                                                            }else{
                                                                                                fprintf(fp,",%lf",tmp[t_ji]);
                                                                                            }
                                                                                        }fprintf(fp,"\n");

                                                                                        t_canshuNum = tmp.size();
                                                                                        //vecLoopArr.push_back(tmp);
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            fclose(fp);
            this->loopForceCanshu = make_pair( t_canshuNum , loopForceFileName );
        }else{
            printf("openfile,loopForce error!\n");
        }
    }

    void run(){

        this->beforeRun();
        for( int i = 1; i < n;i++){ // i = BufferSize - 1
            onBar(i);
        }
        this->afterRun();
    }

    void afterRun(){
        ctaTemplate::afterRun();
    }

};


#endif // _SclwStrategy
