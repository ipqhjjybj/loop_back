
#ifndef _AligatorStrategy
#define _AligatorStrategy

#include "../load_config.h"
#include "../ctaStrategyTemplate.h"

class AligatorStrategy : public ctaTemplate{

public:
    static const int BufferSize = 30;
    /////////////////////////////////////////////////////////////
    //申明参数
    int CF  ,CM , CS , d_CF , d_CM , d_CS ;
    int N_up  , N_down ;//达到鳄鱼线开仓条件后再涨跌一定幅度才开仓
    double zhisunlv_l  , zhisunlv_s ;//止损比例
    double baobenlv_l , baobenlv_s ;//保本比例
    int baoben_n ;//保本线比保本倍数减少多少倍
    int baoben_f ;//是否保本：0：否，1：保本，2：移动保本
    double fuyinglv_l , fuyinglv_s ;//浮盈比例
    double per_fy ;//浮盈比例对止损率的贡献百分比
    int fuying_b ;//从浮盈比例的几倍开始采用浮盈贡献的止损率增加
    int zhisun_f ;//止损方式1,2,3

    ///////////////////////////////////////////////////////////////
    //申明变量
    double zhisun_l , zhisun_s ;
    double kai_up, kai_down;
    int baoben_b_l , baoben_b_s;

    vector<double> lips_N, teeth_N, croco_N , lips , teeth , croco , high_since_entry ,low_since_entry ,entryprice_avg ,fuying ;
    vector<int> zuiArray , eyuArray;
    vector<double> stopline_l , stopline_s , baoben_l , baoben_s;
    vector<int> zhisun_b_l , zhisun_b_s;
    vector<int> zhisun_b_ll , zhisun_b_ss;
    int bufferCount ;

    AligatorStrategy(){
        this->setStrategyName("aligatorStrategy");
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


        CF = 5 , CM = 8 , CS = 13 , d_CF = 3 , d_CM = 5 , d_CS = 8;
        N_up = 10 , N_down = 10;
        zhisunlv_l = 10 , zhisunlv_s = 10 ;
        baobenlv_l = 10 , baobenlv_s = 10 ;
        baoben_n = 2 ;
        baoben_f = 0 ;
        fuyinglv_l = 10 , fuyinglv_s = 10 ;
        per_fy = 0.1;
        fuying_b = 5;
        zhisun_f = 1;

        lots = 1;

        lips_N.clear() , teeth_N.clear(), croco_N.clear();
        lips.clear() , teeth.clear() , croco.clear();

        zuiArray.clear() ,eyuArray.clear();
        high_since_entry.clear() , low_since_entry.clear();
        entryprice_avg.clear();
        fuying.clear();
        stopline_l.clear() , stopline_s.clear();
        baoben_l.clear() , baoben_s.clear();
        zhisun_b_l.clear() , zhisun_b_s.clear();
        zhisun_b_ll.clear() , zhisun_b_ss.clear();


        lips_N.reserve(n + 10) , teeth_N.reserve(n + 10) , croco_N.reserve(n + 10) ;
        lips.reserve(n + 10)  , teeth.reserve(n + 10)  , croco.reserve(n + 10) ;

        zuiArray.reserve(n + 10) , eyuArray.reserve(n + 10);
        high_since_entry.reserve(n + 10) , low_since_entry.reserve(n + 10);
        entryprice_avg.reserve(n+10);
        fuying.reserve(n+10);
        stopline_l.reserve(n+10) , stopline_s.reserve(n+10) ;
        baoben_l.reserve(n+10) , baoben_s.reserve(n+10);
        zhisun_b_l.reserve(n+10), zhisun_b_s.reserve(n+10);
        zhisun_b_ll.reserve(n+10), zhisun_b_ss.reserve(n+10);

        //printf("2 n=%d\n",n);
        bufferCount = 0;
    }
    vector<string> getCanshuSetNames(){
        vector<string> vCanshu; vCanshu.clear();

        vCanshu.push_back("CF");
        vCanshu.push_back("CM");
        vCanshu.push_back("CS");
        vCanshu.push_back("d_CF");
        vCanshu.push_back("d_CM");
        vCanshu.push_back("d_CS");

        vCanshu.push_back("N_up");
        vCanshu.push_back("N_down");

        vCanshu.push_back("zhisunlv_l");
        vCanshu.push_back("zhisunlv_s");

        vCanshu.push_back("baoben_f");
        vCanshu.push_back("baoben_n");
        vCanshu.push_back("baobenlv_l");
        vCanshu.push_back("baobenlv_s");

        vCanshu.push_back("zhisun_f");
        vCanshu.push_back("fuyinglv_l");
        vCanshu.push_back("fuyinglv_s");
        vCanshu.push_back("per_fy");
        vCanshu.push_back("fuying_b");


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
            //printf("%lf , %lf , %lf ,%lf\n",__high_history_price , account_money * 1.0, perSize*1.0 , cash_deposit*1.0);
            lots = 1;
            if(account_money < 1){
                account_money = __const_ini_money;
            }
        }
    }

    void setCanshuFromVec(const vector<double> &arr){

        this->CF = arr[0] + eps;
        this->CM = arr[1] + eps;
        this->CS = arr[2] + eps;
        this->d_CF = arr[3] +eps;
        this->d_CM = arr[4] + eps;
        this->d_CS = arr[5] + eps;

        this->N_up = arr[6] + eps;
        this->N_down = arr[7] + eps;

        this->zhisunlv_l = arr[8];
        this->zhisunlv_s = arr[9];

        this->baoben_f      = arr[10];
        this->baoben_n      = arr[11];
        this->baobenlv_l    = arr[12];
        this->baobenlv_s    = arr[13];

        this->zhisun_f      = arr[14];
        this->fuyinglv_l    = arr[15];
        this->fuyinglv_s    = arr[16];
        this->per_fy        = arr[17];
        this->fuying_b      = arr[18];

    }
    TradingResultNode getTradingResult( const char *_code , const char *_period,const char *start_date , const char *end_date){
        TradingResultNode a = TradingResultNode( _code, _period ,start_date, end_date ,tradingResult,tradingNum , account_money ,slippage * n_slippages, rate , perSize , cash_deposit, vecTradeDay , vecTradeWeekDay , dayIncome , weekIncome , bzjAccount , dayAccount, positionAccount , fee_type);
        vector<double> d; d.clear();
        d.push_back(this->CF);     d.push_back(this->CM);    d.push_back(this->CS);     d.push_back(this->d_CF);       d.push_back(this->d_CM);
        d.push_back(this->d_CS);   d.push_back(this->N_up);  d.push_back(this->N_down); d.push_back(this->zhisunlv_l); d.push_back(this->zhisunlv_s);

        d.push_back(this->baoben_f);   d.push_back(this->baoben_n);    d.push_back(this->baobenlv_l);    d.push_back(this->baobenlv_s);

        d.push_back(this->zhisun_f);   d.push_back(this->fuyinglv_l);     d.push_back(this->fuyinglv_s);     d.push_back(this->per_fy);    d.push_back(this->fuying_b);

        a.setCanshuSet(d);
        return a;
    }

    void onBar_bef(int i){
        ctaTemplate::onBar_bef(i);
    }

    void onBar_after(int i){
        ctaTemplate::onBar_after(i);
    }

    void onBar(int i)
    {
        onBar_bef(i);
        //Series类型数据，继承前一根bar的值
        zuiArray[i]         = zuiArray[i-1];
        eyuArray[i]         = eyuArray[i-1];
        high_since_entry[i] = high_since_entry[i-1];
        low_since_entry[i]  = low_since_entry[i-1];
        entryprice_avg[i]   = entryprice_avg[i-1];
        fuying[i]           = fuying[i-1];
        stopline_l[i]       = stopline_l[i-1];
        stopline_s[i]       = stopline_s[i-1];
        baoben_l[i]         = baoben_l[i-1];
        baoben_s[i]         = baoben_s[i-1];
        zhisun_b_l[i]       = zhisun_b_l[i-1];
        zhisun_b_s[i]       = zhisun_b_s[i-1];
        zhisun_b_ll[i]       = zhisun_b_ll[i-1];
        zhisun_b_ss[i]       = zhisun_b_ss[i-1];


        //===================================================================================================================
        //鳄鱼线嘴张开情况
        if(lips[i] > teeth[i] && teeth[i] > croco[i])
        {
            zuiArray[i] = 1;
        }
        else if(lips[i] < teeth[i] && teeth[i] < croco[i])
        {
            zuiArray[i] = -1;
        }
        else
        {
            zuiArray[i] = 0;
        }


        //===================================================================================================================
        //鳄鱼线持仓信号
        //多仓信号
        if( 1 == zuiArray[i-1] && closeArray[i-1] > openArray[i-1] && lowArray[i-1] > lips[i-1] && openArray[i] > lips[i])
        {
            eyuArray[i] = 1;
        }
        if( 1 == eyuArray[i-1] && lowArray[i] < lips[i])
        {
            eyuArray[i] = 0;
        }
        //空仓信号
        if(-1 == zuiArray[i-1] && closeArray[i-1] < openArray[i-1] && highArray[i-1] < lips[i-1] && openArray[i] < lips[i])
        {
            eyuArray[i] = -1;
        }
        if(-1 == eyuArray[i-1] && highArray[i] > lips[i])
        {
            eyuArray[i] = 0;
        }


        //===================================================================================================================
        //开仓涨跌线
        if(0 == eyuArray[i-1] && 1 == eyuArray[i])
        {
            kai_up = openArray[i] + N_up * minMovement * PriceScale;
        }
        if(0 == eyuArray[i-1] && -1 == eyuArray[i])
        {
            kai_down = openArray[i] - N_down * minMovement * PriceScale;
        }


        //===================================================================================================================
        //进场
        //开多仓
        if(eyuArray[i] > 0 && highArray[i] >= kai_up && mp[i] < 1)
        {
            if( openArray[i] >= kai_up)
            {
                Buy(lots , openArray[i] , i);
                zhisun_l = openArray[i] * ( 1 - zhisunlv_l * 1.0 / 1000.0);
                entryprice_avg[i] = openArray[i];
            }
            else
            {
                Buy(lots , kai_up , i);
                zhisun_l = kai_up * ( 1 - zhisunlv_l * 1.0 / 1000.0);
                entryprice_avg[i] = legalTradePrice(kai_up, 1.0 , minMovement);
            }
            high_since_entry[i] = highArray[i];
            fuying[i] = high_since_entry[i] - entryprice_avg[i];
        }
        //开空仓
        if(eyuArray[i] < 0 && lowArray[i] <= kai_down && mp[i] > -1)
        {

            if(openArray[i] <= kai_down)
            {
                SellShort(lots , openArray[i] , i);
                zhisun_s = openArray[i] * ( 1 + zhisunlv_s * 1.0 / 1000.0);
                entryprice_avg[i] = openArray[i];
            }
            else
            {
                SellShort(lots , kai_down , i);
                zhisun_s = kai_down * ( 1 + zhisunlv_s * 1.0 / 1000.0);
                entryprice_avg[i] = legalTradePrice(kai_down, -1.0 , minMovement);
            }
            low_since_entry[i] = lowArray[i];
            fuying[i] = entryprice_avg[i] - low_since_entry[i];
        }


        //===================================================================================================================
        //入场后的最高价和最低价
        //最高价
        if( mp[i] > 0 && highArray[i] > high_since_entry[i] && BarsSinceEntry > 0)
        {
            high_since_entry[i] = highArray[i];
        }
        if (mp[i-1]<1 && mp[i]<1)
        {
            high_since_entry[i] = highArray[i];
        }
        //最低价
         if( mp[i] < 0 && BarsSinceEntry > 0)
        {
            low_since_entry[i] = min(low_since_entry[i] , lowArray[i]);
        }
        if (mp[i-1]>-1 && mp[i]>-1)
        {
            low_since_entry[i] = lowArray[i];
        }

        //===================================================================================================================
        //浮盈
        if (mp[i]>0 && BarsSinceEntry > 0)
        {
            fuying[i] = high_since_entry[i] - entryprice_avg[i];
        }
        else if (mp[i]<0 && BarsSinceEntry > 0)
        {
            fuying[i] = entryprice_avg[i] - low_since_entry[i];
        }


        //===================================================================================================================
        //保本
        //保本倍数
        if (mp[i] > 0)
        {
            baoben_b_l = floor(1000.0 * (high_since_entry[i-1] - entryprice_avg[i]) / entryprice_avg[i] / baobenlv_l ) ;
        }
        else
        {
            baoben_b_l = 0.0;
        }
        if (mp[i] < 0)
        {
            baoben_b_s= floor(1000.0 * (entryprice_avg[i] - low_since_entry[i-1]) / entryprice_avg[i] / baobenlv_s ) ;
        }
        else
        {
            baoben_b_s = 0.0;
        }


        //保本线1
        baoben_l[i] = entryprice_avg[i] * (1 + baobenlv_l*1.0 / 1000);
        baoben_s[i] = entryprice_avg[i] * (1 - baobenlv_s*1.0 / 1000);

        //保本线2
        if (baoben_b_l >= baoben_n)
        {
            stopline_l[i] = entryprice_avg[i] * (1 + (baoben_b_l - baoben_n) * baobenlv_l*1.0 / 1000);
        }
        else
        {
            stopline_l[i] = entryprice_avg[i];
        }

        if (baoben_b_s >= baoben_n)
        {
            stopline_s[i] = entryprice_avg[i] * (1 - (baoben_b_s - baoben_n) * baobenlv_s*1.0/ 1000);
        }
        else
        {
            stopline_s[i] = entryprice_avg[i];
        }



        //===================================================================================================================
        //止损线
        //止损倍数
        if (entryprice_avg[i] > 0)
        {
            zhisun_b_l[i] = floor(1000.0 * fuying[i] / entryprice_avg[i] /fuyinglv_l);
            zhisun_b_s[i] = floor(1000.0 * fuying[i] / entryprice_avg[i] /fuyinglv_s);
        }


        if (zhisun_b_l[i] < fuying_b)
        {
            zhisun_b_ll[i] = 0;
        }
        else
        {
            zhisun_b_ll[i] = zhisun_b_l[i] - fuying_b;
        }

        if (zhisun_b_s[i] < fuying_b)
        {
            zhisun_b_ss[i] = 0;
        }
        else
        {
            zhisun_b_ss[i] = zhisun_b_s[i] - fuying_b;
        }


        //多仓止损线
        if( mp[i] > 0 && mp[i-1] > 0)
        {
            if (1 == zhisun_f)//一般移动止损
            {
                zhisun_l = high_since_entry[i-1] * ( 1-zhisunlv_l *1.0 / 1000.0);
            }
            if (2 == zhisun_f)//止损率随着浮盈倍数增加的移动止损
            {
                zhisun_l = high_since_entry[i-1] * ( 1 - (1 + zhisun_b_l[i-1]* 1.0 * per_fy ) * zhisunlv_l *1.0 / 1000.0);
            }
            if (3 == zhisun_f)//浮盈倍数达到一定程度后，止损率随着浮盈倍数增加的移动止损
            {
                zhisun_l = high_since_entry[i-1] * ( 1 - (1 + zhisun_b_ll[i-1]* 1.0 * per_fy ) * zhisunlv_l *1.0 / 1000.0);
            }

            if (high_since_entry[i-1] >= baoben_l[i] && entryprice_avg[i] > zhisun_l && 1==baoben_f)//加入保本止损
            {
                zhisun_l= entryprice_avg[i];
            }
            if (baoben_b_l >= baoben_n && stopline_l[i] > zhisun_l && 0==baoben_f)//加入移动保本止损
            {
                zhisun_l = stopline_l[i];
            }
        }



        //空仓止损线
        if( mp[i] < 0 && mp[i-1] < 0)
        {
            if (1 == zhisun_f)//一般移动止损
            {
                zhisun_s = low_since_entry[i-1] * ( 1+zhisunlv_s *1.0 / 1000.0);
            }
            if (2 == zhisun_f)//止损率随着浮盈倍数增加的移动止损
            {
                 zhisun_s = low_since_entry[i-1] * ( 1+ (1 + zhisun_b_s[i-1]* 1.0 * per_fy ) * zhisunlv_s *1.0 / 1000.0);
            }
            if (3 == zhisun_f)//浮盈倍数达到一定程度后，止损率随着浮盈倍数增加的移动止损
            {
                zhisun_s = low_since_entry[i-1] * ( 1+ (1 + zhisun_b_ss[i-1]* 1.0 * per_fy ) * zhisunlv_s *1.0 / 1000.0);
            }

            if (low_since_entry[i-1] <= baoben_s[i]  &&  entryprice_avg[i] < zhisun_s && 1==baoben_f)//加入保本止损
            {
                zhisun_s= entryprice_avg[i];
            }
            if (baoben_b_s >= baoben_n  && stopline_s[i] < zhisun_s && 0==baoben_f)//加入移动保本止损
            {
                zhisun_s = stopline_s[i];
            }
        }



        //===================================================================================================================
        //移动止损
        //多仓止损
        if( mp[i] > 0 && lowArray[i] < zhisun_l && BarsSinceEntry > 0)
        {
            if( openArray[i] < zhisun_l)
            {
                Sell(lots , openArray[i] , i);
            }
            else
            {

                Sell(lots , zhisun_l , i );
            }
            eyuArray[i] = 0;
        }
        //空仓止损
        if( mp[i] < 0 && highArray[i] > zhisun_s && BarsSinceEntry > 0)
        {
            if( openArray[i] > zhisun_s)
            {
                BuyToCover(lots , openArray[i] , i);
            }
            else
            {
                BuyToCover(lots , zhisun_s , i);
            }
            eyuArray[i] = 0;
        }



        onBar_after(i);



        //===================================================================================================================
        //入场价
        if (0 == mp[i] && 0==mp[i-1])
        {
            entryprice_avg[i] = closeArray[i];
        }

        //===================================================================================================================
        //浮盈
        if (0 == mp[i] && 0==mp[i-1])
        {
            fuying[i] = 0;
        }


        //===================================================================================================================


        onBar_after(i);
    }

    void beforeRun( )
    {
        ctaTemplate::beforeRun();

        // Strategy Variable
        //变量赋初始值
        zhisun_l = 0 , zhisun_s = 0;
        kai_up=0, kai_down=0;
        bufferCount = 0;
        zuiArray[0] = eyuArray[0] = 0;
        high_since_entry[0] = low_since_entry[0] = 0.0;
        entryprice_avg[0] = 0.0;
        fuying[0] = 0.0;
        baoben_b_l = 0;
        baoben_b_s = 0;
        stopline_l[0] = 0.0;
        stopline_s[0] = 0.0;
        baoben_l[0] = 0.0;
        baoben_s[0] = 0.0;
        zhisun_b_l[0] = 0;
        zhisun_b_s[0] = 0;
        zhisun_b_ll[0] = 0;
        zhisun_b_ss[0] = 0;



        xAverage(lips_N  ,closeArray , CF , n);
        xAverage(teeth_N ,closeArray , CM , n);
        xAverage(croco_N ,closeArray , CS , n);

        LeftMove(lips , lips_N , d_CF , n);
        LeftMove(teeth , teeth_N , d_CM ,n);
        LeftMove(croco , croco_N , d_CS ,n);
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


#endif // _AligatorStrategy
