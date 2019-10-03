
#ifndef _PeriodDualthrust
#define _PeriodDualthrust

#include "../load_config.h"
#include "../ctaStrategyTemplate.h"


/**
 *  策略，劲道指数
 */
class PeriodDualthrust : public ctaTemplate{

public:
    static const int BufferSize = 30;

    /////////////////////////////////////////////////////////////
    //申明参数

    int TimesMaxToday;              //限制当天开仓最多次数;
    double K1;                      //开多系数
    double K2;                      //开空系数
    int Nday;                       //多天数
    int Mday;                       //空天数
    ///////////////////////////////////////////////////////////////

    vector<double> dayOpenArray;
    vector<double> dayHighArray;
    vector<double> dayLowArray;
    vector<double> dayCloseArray;

    vector<int> dayBarMapper;

    vector<double> hh1 , hc1 , ll1 , lc1;
    vector<double> hh2 , hc2 , ll2 , lc2;

    double buyRange;
    double sellRange;
    double buyTrig;
    double sellTrig;

    int timesToday;

    double longEnterPrice;          //多单进场价位
    double shortEnterPrice;         //空单进场价位

    double __entryPrice;
    double __exitPrice;

    int bufferCount ;

    // FILE *debug_f ;

    PeriodDualthrust(){
        this->setStrategyName("PeriodDualthrust");
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

        TimesMaxToday = 1;
        K1 = 0.5;
        K2 = 0.5;
        Nday = 1;
        Mday = 1;

        lots = 1;

        bufferCount = 0;

    }

    vector<string> getCanshuSetNames(){
        vector<string> vCanshu; vCanshu.clear();

        vCanshu.push_back("TimesMaxToday");
        vCanshu.push_back("K1");
        vCanshu.push_back("K2");
        vCanshu.push_back("Nday");
        vCanshu.push_back("Mday");

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

        TimesMaxToday = arr[0];
        K1 = arr[1];
        K2 = arr[2];
        Nday = arr[3];
        Mday = arr[4];
    }

    TradingResultNode getTradingResult( const char *_code , const char *_period,const char *start_date , const char *end_date){
        TradingResultNode a = TradingResultNode( _code, _period ,start_date, end_date ,tradingResult,tradingNum , account_money ,slippage * n_slippages, rate , perSize , cash_deposit, vecTradeDay , vecTradeWeekDay , dayIncome , weekIncome , bzjAccount , dayAccount, positionAccount , fee_type);
        vector<double> d; d.clear();

        d.push_back(TimesMaxToday);  d.push_back(K1); d.push_back(K2); d.push_back(Nday);  d.push_back(Mday);

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

        // fprintf(debug_f,"%s,%s,%lf,%lf,%lf,%lf,%d,%lf,%lf,%lf,%lf\n",g_date[i].c_str(),g_time[i].c_str(),openArray[i],highArray[i],lowArray[i],closeArray[i],dayBarMapper[i],dayOpenArray[dayBarMapper[i]],dayHighArray[dayBarMapper[i]],dayLowArray[dayBarMapper[i]],dayCloseArray[dayBarMapper[i]]);
        if( i >= 91){
            double HH, HC, LL, LC;

            int longLoc = dayBarMapper[i];
            int isReturnFalse = 1;
            if( longLoc > 0){
                if ( dayBarMapper[i] != dayBarMapper[i-1]){
                    timesToday = 0;
                    isReturnFalse = 0;
                }

                HH = hh1[longLoc - 1];
                HC = hc1[longLoc - 1];
                LL = ll1[longLoc - 1];
                LC = lc1[longLoc - 1];

                buyRange = max(HH - LC , HC - LL);
                buyTrig = K1 * buyRange;
                
                HH = hh2[longLoc - 1];
                HC = hc2[longLoc - 1];
                LL = ll2[longLoc - 1];
                LC = lc2[longLoc - 1];
                sellRange = max(HH - LC , HC - LL);
                sellTrig = K2 * sellRange;
                
                // 开仓，反手开仓
                if( timesToday < TimesMaxToday ){

                    if( isReturnFalse == 1){
                        if( this->pos <= 0){
                            if( highArray[i] >= longEnterPrice){
                                __entryPrice = max(openArray[i] , longEnterPrice);
                                if(this->pos < 0){
                                    this->BuyToCover(abs(this->pos) , __entryPrice , i);
                                }
                                this->Buy(lots , __entryPrice , i);
                                timesToday += 1;
                                isReturnFalse = 0;
                            }
                        }
                    }

                    if(isReturnFalse == 1){
                        if(this->pos >= 0){
                            if( lowArray[i] <= shortEnterPrice){
                                __entryPrice = min(openArray[i] , shortEnterPrice);
                                if(this->pos > 0){
                                    this->Sell(abs(this->pos) , __entryPrice , i);
                                }
                                this->SellShort(lots , __entryPrice , i);
                                timesToday += 1;
                                isReturnFalse = 0;
                            }
                        }
                    }
                }
                // 平仓
                if( isReturnFalse == 1){
                    if( this->pos > 0 && lowArray[i] <= shortEnterPrice){
                        __entryPrice = min( openArray[i] , shortEnterPrice);
                        this->Sell(this->pos , __entryPrice , i);
                        isReturnFalse = 0;
                    }
                }
                if( isReturnFalse == 1){
                    if( this->pos < 0 && highArray[i] >= longEnterPrice){
                        __entryPrice = max( openArray[i] , longEnterPrice);
                        this->BuyToCover( abs(this->pos) , longEnterPrice , i);
                        isReturnFalse = 0;
                    }
                }

                longEnterPrice = dayOpenArray[longLoc] + buyTrig;
                shortEnterPrice = dayOpenArray[longLoc] - sellTrig;
                // fprintf(debug_f,"%s,%s,%lf,%lf,%lf,%lf,%lf,%lf,%d\n",g_date[i].c_str(),g_time[i].c_str(),dayOpenArray[longLoc], longEnterPrice,shortEnterPrice,highArray[i],lowArray[i],closeArray[i],this->pos);
            }
        }

        onBar_after(i);
    }

    void beforeRun( )
    {
        ctaTemplate::beforeRun();

        dayOpenArray.clear(); dayHighArray.clear(); dayLowArray.clear(); dayCloseArray.clear(); dayBarMapper.clear();

        // Strategy Variable

        dayOpenArray.reserve(n + 10); dayHighArray.reserve(n + 10); dayLowArray.reserve(n + 10); dayCloseArray.reserve(n + 10); dayBarMapper.reserve(n + 10);


        // 通过短周期的数据 构造 中周期的 closeArray

        combineK_dayLine( dayBarMapper , dayOpenArray , dayHighArray , dayLowArray , dayCloseArray , openArray , highArray , lowArray , closeArray , g_time ,n);

        

        hh1.clear();   hc1.clear();   ll1.clear();   lc1.clear();
        hh2.clear();   hc2.clear();   ll2.clear();   lc2.clear();

        hh1.reserve(n + 10);  hc1.reserve(n + 10);   ll1.reserve(n + 10);   lc2.reserve(n + 10);
        hh2.reserve(n + 10);  hc2.reserve(n + 10);   ll2.reserve(n + 10);   lc2.reserve(n + 10);

        Highest( hh1 , dayHighArray , Nday , dayHighArray.size());
        Highest( hc1 , dayCloseArray , Nday , dayCloseArray.size());
        Lowest( ll1 , dayLowArray , Nday , dayLowArray.size());
        Lowest( lc1 , dayCloseArray , Nday , dayCloseArray.size());

        Highest( hh2 , dayHighArray , Mday , dayHighArray.size());
        Highest( hc2 , dayCloseArray , Mday , dayCloseArray.size());
        Lowest( ll2 , dayLowArray , Mday , dayLowArray.size());
        Lowest( lc2 , dayCloseArray , Mday , dayCloseArray.size());

        timesToday = 0;

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
            for(int TimesMaxToday = 1; TimesMaxToday < 5; TimesMaxToday++){
                for(double K1 = 0.4;  K1 < 0.9 ; K1 += 0.05){
                    for(double K2 = 0.4; K2 < 0.9 ; K2 += 0.05){
                        for(int Nday = 1; Nday <= 10 ;Nday += 1){
                            for(int Mday = 1; Mday <= 10; Mday += 1){
                                vector<double> tmp; tmp.clear();
                                tmp.push_back(TimesMaxToday);
                                tmp.push_back(K1);
                                tmp.push_back(K2);
                                tmp.push_back(Nday);
                                tmp.push_back(Mday);

                                for(int t_ji = 0; t_ji < tmp.size(); t_ji++){
                                    if(0 == t_ji){
                                        fprintf(fp,"%lf",tmp[t_ji]);
                                    }else{
                                        fprintf(fp,",%lf",tmp[t_ji]);
                                    }
                                }fprintf(fp,"\n");

                                t_canshuNum = tmp.size();
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

        
        // debug_f =  fopen("debug_f.csv" , "w");
        this->beforeRun();

        for( int i = 1; i < n;i++){ // i = BufferSize - 1
            onBar(i);
        }
        this->afterRun();


        // fflush( debug_f);
        // fclose( debug_f);
    }

    void afterRun(){
        ctaTemplate::afterRun();
    }

};


#endif // _PeriodDualthrust
