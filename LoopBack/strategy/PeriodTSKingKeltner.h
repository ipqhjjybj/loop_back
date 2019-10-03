
#ifndef _PeriodTSKingKeltner
#define _PeriodTSKingKeltner

#include "../load_config.h"
#include "../ctaStrategyTemplate.h"


/**
 *  策略，劲道指数
 */
class PeriodTSKingKeltner : public ctaTemplate{

public:
    static const int BufferSize = 30;

    /////////////////////////////////////////////////////////////
    //申明参数
    int avgLength;          // 布林通道参数
    int atrLength;          // 真实波幅参数


    ///////////////////////////////////////////////////////////////

    vector<double> threePrices;
    vector<double> MovAvgArray;
    vector<double> atrArray;

    vector<double> upBand;
    vector<double> downBand;

    int bufferCount ;

    FILE *debug_f ;

    PeriodTSKingKeltner(){
        this->setStrategyName("PeriodTSKingKeltner");
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

        avgLength = 40;
        atrLength = 20;


        lots = 1;

        bufferCount = 0;

    }
    vector<string> getCanshuSetNames(){
        vector<string> vCanshu; vCanshu.clear();

        vCanshu.push_back("avgLength");
        vCanshu.push_back("atrLength");

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

        avgLength = arr[0];
        atrLength = arr[1];

    }

    TradingResultNode getTradingResult( const char *_code , const char *_period,const char *start_date , const char *end_date){
        TradingResultNode a = TradingResultNode( _code, _period ,start_date, end_date ,tradingResult,tradingNum , account_money ,slippage * n_slippages, rate , perSize , cash_deposit, vecTradeDay , vecTradeWeekDay , dayIncome , weekIncome , bzjAccount , dayAccount, positionAccount , fee_type);
        vector<double> d; d.clear();
        d.push_back( this->avgLength); d.push_back( this->atrLength); 

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
        if( i >= 2){
            if( this->pos <= 0 && MovAvgArray[i-1] > MovAvgArray[i-2] && highArray[i] >= upBand[i-1]){
                double entryPrice = max(openArray[i] , upBand[i-1]);
                this->Buy( lots , entryPrice , i);
            }
            if( this->pos > 0 && BarsSinceEntry >= 1 && lowArray[i] <= MovAvgArray[i-1]){
                double exitPrice = min( openArray[i] , MovAvgArray[i-1]);
                this->Sell( abs(this->pos) , exitPrice , i);
            }

            if( this->pos >= 0 && MovAvgArray[i-1] < MovAvgArray[i-2] && lowArray[i] <= downBand[i-1]){
                double entryPrice = min( openArray[i] , downBand[i-1]);
                this->SellShort( lots , entryPrice , i);
            }

            if( this->pos < 0 && BarsSinceEntry >= 1 && highArray[i] >= MovAvgArray[i-1]){
                double exitPrice = max( openArray[i] , MovAvgArray[i-1]);
                this->BuyToCover( abs(this->pos) , exitPrice , i);
            }
        }
        onBar_after(i);
    }

    void beforeRun( )
    {
        ctaTemplate::beforeRun();

        threePrices.clear();
        for(int i = 0;i < openArray.size() ;i++){
            threePrices.push_back( (closeArray[i] + highArray[i] + lowArray[i]) / 3.0);
        }
        MovAvgArray.clear();    atrArray.clear();  upBand.clear();  downBand.clear();
        MovAvgArray.reserve(n + 10);   atrArray.reserve(n + 10);   upBand.reserve(n + 10);  downBand.reserve(n + 10);
        Average( MovAvgArray , threePrices , avgLength , n);
        AvgTrueRange( atrArray , highArray , lowArray , closeArray , atrLength , n);

        for(int i = 0;i < n;i++){
            upBand.push_back( MovAvgArray[i] + atrArray[i]);
            downBand.push_back( MovAvgArray[i] - atrArray[i]);
        }
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
            for(int avgLength = 20; avgLength <= 80; avgLength++){
                for( int atrLength = 10; atrLength <= 40; atrLength++){
                    vector<double> tmp; tmp.clear();
                    tmp.push_back( avgLength);
                    tmp.push_back( atrLength);

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

        
        // fflush( debug_f);
        // fclose( debug_f);
    }

    void afterRun(){
        ctaTemplate::afterRun();
    }

};


#endif // _PeriodTSKingKeltner
