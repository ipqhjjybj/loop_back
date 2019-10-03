
#ifndef _AtrRsiStrategy
#define _AtrRsiStrategy

#include "../load_config.h"
#include "../ctaStrategyTemplate.h"


/**
 *  策略，劲道指数
 */
class AtrRsiStrategy : public ctaTemplate{

public:
    static const int BufferSize = 30;

    /////////////////////////////////////////////////////////////
    //申明参数

    int atrLength;      // 计算ATR指标的窗口数   
    int atrMaLength;    // 计算ATR均线的窗口数
    int rsiLength;      // 计算RSI的窗口数
    double rsiEntry;    // RSI的开仓信号
    double trailingPercent; // RSi的百分比移动止损

    ///////////////////////////////////////////////////////////////

    vector<double> atrValueArray;
    vector<double> atrMaArray;
    vector<double> rsiValueArray;

    int bufferCount ;

    // variable
    double intraTradeHigh , intraTradeLow;
    double use_entryPrice , use_exitPrice;

    FILE *debug_f ;

    AtrRsiStrategy(){
        this->setStrategyName("AtrRsiStrategy");
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

        atrLength = 22;
        atrMaLength = 10;
        rsiLength = 5;
        rsiEntry = 16;
        trailingPercent = 0.8;

        intraTradeHigh = 0.0;
        intraTradeLow = 0.0;

        lots = 1;
        bufferCount = 0;

        use_entryPrice = 0.0;
        use_exitPrice = 0.0;
    }
    vector<string> getCanshuSetNames(){
        vector<string> vCanshu; vCanshu.clear();

        vCanshu.push_back("atrLength");
        vCanshu.push_back("atrMaLength");
        vCanshu.push_back("rsiLength");
        vCanshu.push_back("rsiEntry");
        vCanshu.push_back("trailingPercent");

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
        atrLength = arr[0];
        atrMaLength = arr[1];
        rsiLength = arr[2];
        rsiEntry = arr[3];
        trailingPercent = arr[4];
    }

    TradingResultNode getTradingResult( const char *_code , const char *_period,const char *start_date , const char *end_date){
        TradingResultNode a = TradingResultNode( _code, _period ,start_date, end_date ,tradingResult,tradingNum , account_money ,slippage * n_slippages, rate , perSize , cash_deposit, vecTradeDay , vecTradeWeekDay , dayIncome , weekIncome , bzjAccount , dayAccount, positionAccount , fee_type);
        vector<double> d; d.clear();
        d.push_back( atrLength); d.push_back( atrMaLength); 
        d.push_back(rsiLength); d.push_back(rsiEntry);  d.push_back(trailingPercent);

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

        double rsiBuy = 50 + rsiEntry;
        double rsiSell = 50 - rsiEntry;
        if( i > 0){
            if( 0 == this->pos){
                intraTradeHigh = highArray[i];
                intraTradeLow = lowArray[i];
                if( atrValueArray[i-1] > atrMaArray[i-1]){
                    if( rsiValueArray[i-1] > rsiBuy){
                        use_entryPrice = openArray[i];
                        Buy(lots , use_entryPrice , i);
                    }else if( rsiValueArray[i-1] < rsiSell){
                        use_entryPrice = openArray[i];
                        SellShort(lots , use_entryPrice , i);
                    }
                }
            }else if( this->pos > 0){
                if( lowArray[i] < intraTradeHigh * (1 - trailingPercent/100.0) ){
                    use_exitPrice = min( openArray[i] , intraTradeHigh * ( 1 - trailingPercent / 100.0));
                    Sell( abs(this->pos) , use_exitPrice , i);
                }
                intraTradeHigh = max(intraTradeHigh , highArray[i]);
                intraTradeLow = lowArray[i];
            }else if( this->pos < 0){
                if( highArray[i] > intraTradeLow * ( 1 + trailingPercent/100.0) ){
                    use_exitPrice = max( openArray[i] , intraTradeLow * ( 1 + trailingPercent / 100.0));
                    BuyToCover(abs(this->pos) , use_exitPrice , i);
                }
                intraTradeHigh = highArray[i];
                intraTradeLow = min(intraTradeLow , lowArray[i]);
            }
        }

        onBar_after(i);
    }

    void beforeRun( )
    {
        ctaTemplate::beforeRun();

        atrValueArray.clear() ;  atrValueArray.reserve( n + 10);
        atrMaArray.clear();    atrMaArray.reserve(n + 10);
        rsiValueArray.clear();  rsiValueArray.reserve(n + 10);

        RSI( rsiValueArray , closeArray , rsiLength, n);
        AvgTrueRange( atrValueArray , highArray , lowArray , closeArray , atrLength , n);
        Average( atrMaArray , atrValueArray , atrMaLength , n);

        FILE *fp = fopen("rsi.out", "w");
        for( int i = 0;i < n;i++){
            fprintf(fp , "%lf\n", rsiValueArray[i]);
        }
        fclose(fp);
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

            for( int atrLength = 10; atrLength <= 30; atrLength++){
                for(int atrMaLength = 10; atrMaLength <= 30; atrMaLength++){
                    for(int rsiLength = 3; rsiLength <= 17; rsiLength++){
                        for(double rsiEntry = 10; rsiEntry <= 30; rsiEntry+=3.0){
                            for(double trailingPercent = 0.6; trailingPercent <= 1.0; trailingPercent += 0.1){
                                vector<double> tmp; tmp.clear();
                                tmp.push_back(atrLength);
                                tmp.push_back(atrMaLength);
                                tmp.push_back(rsiLength);
                                tmp.push_back(rsiEntry);
                                tmp.push_back(trailingPercent);

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

    }

    void afterRun(){
        ctaTemplate::afterRun();
    }

};


#endif // _AtrRsiStrategy
