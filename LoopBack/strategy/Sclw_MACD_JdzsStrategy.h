
#ifndef _Sclw_MACD_JdzsStrategy
#define _Sclw_MACD_JdzsStrategy

#include "../load_config.h"
#include "../ctaStrategyTemplate.h"


/**
 *  策略，劲道指数
 */
class Sclw_MACD_JdzsStrategy : public ctaTemplate{

public:
    static const int BufferSize = 30;

    /////////////////////////////////////////////////////////////
    //申明参数

    int longUseBar;        //通过几根 中周期的数据 来 构造 长周期的数据
    int mid_P1;            //劲道指数

    int FastLength;        // 快线
    int SlowLength;        // 慢线
    int MACDLength;        // MACD 长度

    ///////////////////////////////////////////////////////////////

    vector<double> longOpenArray;
    vector<double> longHighArray;
    vector<double> longLowArray;
    vector<double> longCloseArray;

    vector<int> longBarMapper;

    vector<double> MA_short;
    vector<double> MA_long;
    vector<double> MACDValue;
    vector<double> AvgMACD;
    vector<double> MACDDiff;


    vector<double> jdzsEmaArr;

    int signalLong;
    int bufferCount ;

    double entryPrice;
    double exitPrice;
    double zhisunPrice;

    FILE *debug_f ;

    Sclw_MACD_JdzsStrategy(){
        this->setStrategyName("Sclw_MACD_JdzsStrategy");
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

        longUseBar = 5;
        mid_P1 = 2;
        FastLength = 12;
        SlowLength = 26;
        MACDLength = 9;

        lots = 1;

        bufferCount = 0;

    }
    vector<string> getCanshuSetNames(){
        vector<string> vCanshu; vCanshu.clear();

        vCanshu.push_back("longUseBar");
        vCanshu.push_back("mid_P1");
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

        longUseBar = arr[0];
        mid_P1 = arr[1];
        FastLength = arr[2];
        SlowLength = arr[3];
        MACDLength = arr[4];

    }

    TradingResultNode getTradingResult( const char *_code , const char *_period,const char *start_date , const char *end_date){
        TradingResultNode a = TradingResultNode( _code, _period ,start_date, end_date ,tradingResult,tradingNum , account_money ,slippage * n_slippages, rate , perSize , cash_deposit, vecTradeDay , vecTradeWeekDay , dayIncome , weekIncome , bzjAccount , dayAccount, positionAccount , fee_type);
        vector<double> d; d.clear();
        d.push_back( this->longUseBar); d.push_back( this->mid_P1); 
        d.push_back( this->FastLength); d.push_back( this->SlowLength); d.push_back(this->MACDLength);

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
        if( i >= longUseBar){

            int longLoc = longBarMapper[i];

            signalLong = 0;

            if( longCloseArray[longLoc-1] > MA_long[longLoc-1] && MACDValue[longLoc-1] > AvgMACD[longLoc-1] && AvgMACD[longLoc-1] > 0){
                signalLong = 1;
            }else if( longCloseArray[longLoc-1] > MA_long[longLoc-1] && MACDValue[longLoc-1] < AvgMACD[longLoc-1] && MACDValue[longLoc-1] > 0){
                signalLong = 0;
            }else if( longCloseArray[longLoc-1] < MA_long[longLoc-1] && MACDValue[longLoc-1] < AvgMACD[longLoc-1] && AvgMACD[longLoc-1] < 0){
                signalLong = -1;
            }else if( longCloseArray[longLoc-1] < MA_long[longLoc-1] && MACDValue[longLoc-1] > AvgMACD[longLoc-1] && MACDValue[longLoc-1] < 0){
                signalLong = 0;
            }

            if( this->pos == 0){
                if( signalLong > 0 && jdzsEmaArr[i - 1] < 0 && highArray[i] > highArray[i-1]){
                    entryPrice = max( openArray[i] , highArray[i-1] + minMovement);
                    this->Buy(lots , entryPrice , i);
                    zhisunPrice = min( lowArray[i-2] , lowArray[i-1]);
                }else if( signalLong < 0 && jdzsEmaArr[i - 1] > 0 && lowArray[i] < lowArray[i-1]){
                    entryPrice = min( openArray[i] , lowArray[i-1] - minMovement);
                    this->SellShort( lots , entryPrice ,i);
                    zhisunPrice = max( highArray[i-2] , highArray[i-1]);
                }
            }else if( this->pos > 0){
                if( signalLong <= 0){
                    exitPrice = closeArray[i];
                    this->Sell( abs(this->pos) , exitPrice , i);
                }else{
                    if( lowArray[i] < zhisunPrice){
                        exitPrice = min(openArray[i] , zhisunPrice - minMovement);
                        this->Sell( abs(this->pos) , exitPrice , i);
                    }
                }
                if( signalLong > 0 && jdzsEmaArr[i-1] < 0 && highArray[i] > highArray[i-1]){
                    zhisunPrice = min( lowArray[i-2] , lowArray[i-1]);
                }
            }else if( this->pos < 0){
                if( signalLong >= 0){
                    exitPrice = closeArray[i];
                    this->BuyToCover( abs(this->pos) , exitPrice , i);
                }else{
                    if( highArray[i] >  zhisunPrice){
                        exitPrice = max( openArray[i] , zhisunPrice + minMovement);
                        this->BuyToCover( abs(this->pos) , exitPrice , i);
                    }
                }
                if( signalLong < 0 && jdzsEmaArr[i-1] > 0 && lowArray[i] < lowArray[i-1]){
                    zhisunPrice = max(highArray[i-2] , highArray[i-1]);
                }
            }

            if(this->pos == 0){
                zhisunPrice = entryPrice = exitPrice = 0.0;
            }
            //fprintf(debug_f , "%d,%d,%lf,%lf,%lf,%lf,%lf,%d,%lf,%lf,%lf\n",i,longLoc,highArray[i],lowArray[i],closeArray[i],volArray[i],jdzsEmaArr[i],this->pos, entryPrice,exitPrice,zhisunPrice);
        }
        onBar_after(i);
    }

    void beforeRun( )
    {
        ctaTemplate::beforeRun();

        longOpenArray.clear(); longHighArray.clear(); longLowArray.clear(); longCloseArray.clear(); longBarMapper.clear();

        // Strategy Variable

        longOpenArray.reserve(n / longUseBar + 10); longHighArray.reserve(n / longUseBar + 10); longLowArray.reserve(n / longUseBar + 10); longCloseArray.reserve(n / longUseBar + 10); longBarMapper.reserve(n + 10);

        
        // 通过短周期的数据 构造 中周期的 closeArray

        combineK_line( longBarMapper , longOpenArray , longHighArray , longLowArray , longCloseArray , openArray , highArray , lowArray , closeArray , longUseBar ,n);


        // xAverage( long_short_arr , longCloseArray , long_short_length , longCloseArray.size());
        // xAverage( long_long_arr , longCloseArray , long_long_length , longCloseArray.size());

        MA_short.clear(); MA_long.clear(); MACDValue.clear(); AvgMACD.clear(); MACDDiff.clear();
        MA_short.reserve(n + 10); MA_long.reserve(n + 10); MACDValue.reserve(n + 10); AvgMACD.reserve(n + 10); MACDDiff.reserve(n + 10);


        
        // Strategy Variable
        xAverage( MA_short , longCloseArray , FastLength , longCloseArray.size());
        xAverage( MA_long , longCloseArray , SlowLength , longCloseArray.size());

        MACD( MACDValue , AvgMACD , MACDDiff , longCloseArray , FastLength , SlowLength , MACDLength , longCloseArray.size());

        signalLong = 0;

        

        jdzsEmaArr.clear();  jdzsEmaArr.reserve(n + 10);
        ForceIndex( jdzsEmaArr , closeArray , volArray ,  mid_P1 , n );

        

        entryPrice = 0.0 ;  exitPrice = 0.0; zhisunPrice = 0.0;
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
            for( int longUseBar = 4; longUseBar <= 6 ;longUseBar++ ){
                for( int mid_P1 = 2;  mid_P1 <= 13; mid_P1++){
                    for( int FastLength = 4; FastLength <= 12 ;FastLength++){
                        for( int SlowLength = max(15, FastLength); SlowLength <= 40; SlowLength++){
                            for( int MACDLength = 7; MACDLength <= 15; MACDLength++){
                                vector<double> tmp; tmp.clear();
                                tmp.push_back(longUseBar);
                                tmp.push_back(mid_P1);
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

        
        // fflush( debug_f);
        // fclose( debug_f);
    }

    void afterRun(){
        ctaTemplate::afterRun();
    }

};


#endif // _Sclw_MACD_JdzsStrategy
