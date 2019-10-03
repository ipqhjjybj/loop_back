
#ifndef _MACDHaiTrunStrategy
#define _MACDHaiTrunStrategy

#include "../load_config.h"
#include "../ctaStrategyTemplate.h"


/**
 *  策略，劲道指数
 */
class MACDHaiTrunStrategy : public ctaTemplate{

public:
    static const int BufferSize = 30;

    /////////////////////////////////////////////////////////////
    //申明参数

    int FastLength;        // 快线
    int SlowLength;        // 慢线
    int MACDLength;        // MACD 长度

    ///////////////////////////////////////////////////////////////

    vector<double> MA_short;
    vector<double> MA_long;
    vector<double> MACDValue;
    vector<double> AvgMACD;
    vector<double> MACDDiff;

    int signalLong;
    int bufferCount ;

    FILE *debug_f ;

    MACDHaiTrunStrategy(){
        this->setStrategyName("MACDHaiTrunStrategy");
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

        FastLength = 12;
        SlowLength = 26;
        MACDLength = 9;

        lots = 1;

        bufferCount = 0;

    }
    vector<string> getCanshuSetNames(){
        vector<string> vCanshu; vCanshu.clear();

        vCanshu.push_back("FastLength");
        vCanshu.push_back("SlowLength");
        vCanshu.push_back("MACDLength");

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
        FastLength = arr[0];
        SlowLength = arr[1];
        MACDLength = arr[2];
    }

    TradingResultNode getTradingResult( const char *_code , const char *_period,const char *start_date , const char *end_date){
        TradingResultNode a = TradingResultNode( _code, _period ,start_date, end_date ,tradingResult,tradingNum , account_money ,slippage * n_slippages, rate , perSize , cash_deposit, vecTradeDay , vecTradeWeekDay , dayIncome , weekIncome , bzjAccount , dayAccount, positionAccount , fee_type);
        vector<double> d; d.clear();
        d.push_back( FastLength); d.push_back( SlowLength);  d.push_back( MACDLength);

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
        signalLong = 0;
        if( closeArray[i-1] > MA_long[i-1] && MACDValue[i-1] > AvgMACD[i-1] && AvgMACD[i-1] > 0 ){
            signalLong = 1;
        }else if( closeArray[i-1] > MA_long[i-1] && MACDValue[i-1] < AvgMACD[i-1] && MACDValue[i-1] > 0 ){
            signalLong = 0;
        }else if( closeArray[i-1] < MA_long[i-1] && MACDValue[i-1] < AvgMACD[i-1] && AvgMACD[i-1] < 0){
            signalLong = -1;
        }else if( closeArray[i-1] < MA_long[i-1] && MACDValue[i-1] > AvgMACD[i-1] && MACDValue[i-1] < 0){
            signalLong = 0;
        }

        if( this->pos == 0){
            if(signalLong > 0){
                this->Buy( lots , openArray[i] , i);
            }else if(signalLong < 0){
                this->SellShort(lots , openArray[i] , i);
            }
        }else if( this->pos > 0){
            if( signalLong <= 0){
                this->Sell(abs(this->pos) , openArray[i] , i);
            }
            if( signalLong < 0){
                this->SellShort(lots , openArray[i] , i);
            }
        }else if( this->pos < 0){
            if( signalLong >= 0){
                 this->BuyToCover(abs(this->pos)  , openArray[i] , i);
            }
            if( signalLong > 0){
                this->Buy( lots , openArray[i] , i);
            }
        }

        onBar_after(i);
    }

    void beforeRun( )
    {
        ctaTemplate::beforeRun();

        signalLong = 0;

        MA_short.clear(); MA_long.clear(); MACDValue.clear(); AvgMACD.clear(); MACDDiff.clear();
        MA_short.reserve(n + 10); MA_long.reserve(n + 10); MACDValue.reserve(n + 10); AvgMACD.reserve(n + 10); MACDDiff.reserve(n + 10);

        // Strategy Variable
        xAverage( MA_short , closeArray , FastLength , n);
        xAverage( MA_long , closeArray , SlowLength , n);

        MACD( MACDValue , AvgMACD , MACDDiff , closeArray , FastLength , SlowLength , MACDLength , n);

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
            for( int FastLength = 4; FastLength <= 30 ;FastLength++){
                for( int SlowLength = max(15, FastLength) ; SlowLength <= 100; SlowLength++){
                    for( int MACDLength = 7; MACDLength <= 15; MACDLength++){
                        vector<double> tmp; tmp.clear();
                        tmp.push_back(FastLength);
                        tmp.push_back(SlowLength);
                        tmp.push_back(MACDLength);

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
            fclose(fp);
            this->loopForceCanshu = make_pair( t_canshuNum , loopForceFileName );
        }else{
            printf("openfile,loopForce error!\n");
        }
    }

    void run(){

        this->beforeRun();
        // debug_f =  fopen("debug_f.csv" , "w");

        for( int i = 1; i < n;i++){ // i = BufferSize - 1
            onBar(i);
        }
        this->afterRun();

    }

    void afterRun(){
        ctaTemplate::afterRun();
    }

};


#endif // _MACDHaiTrunStrategy
