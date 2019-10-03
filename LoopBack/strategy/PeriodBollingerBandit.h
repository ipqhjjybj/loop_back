
#ifndef _PeriodBollingerBandit
#define _PeriodBollingerBandit

#include "../load_config.h"
#include "../ctaStrategyTemplate.h"


/*  
策略说明:
        基于布林通道的突破系统
系统要素:
        1、基于收盘价计算而来的布林通道
        2、基于收盘价计算而来的进场过滤器
        3、自适应出场均线
入场条件:
        1、满足过滤条件，并且价格上破布林通道上轨，开多单
        2、满足过滤条件，并且价格下破布林通道下轨，开空单
出场条件:
        1、持有多单时，自适应出场均线低于布林通道上轨，并且价格下破自适应出场均线，平多单
        2、持有空单时，自适应出场均线高于布林通道下轨，并且价格上破自适应出场均线，平空单
*/

class PeriodBollingerBandit : public ctaTemplate{

public:
    static const int BufferSize = 30;

    /////////////////////////////////////////////////////////////
    //申明参数
    int bollingerLengths;           // 布林通道参数
    double Offset;            // 布林通道参数
    int rocCalcLength;          // 过滤器参数
    int liqLength;              // 自适应出场均线参数  
    
    ///////////////////////////////////////////////////////////////

    vector<double> MidLine;
    vector<double> Band;

    double upband ;
    double downBand ;
    double rocCalc ; 
    int liqDays ;
    double liqPoint ;


    int bufferCount ;

    // FILE *debug_f ;

    PeriodBollingerBandit(){
        this->setStrategyName("PeriodBollingerBandit");
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

        bollingerLengths = 50;
        Offset = 1.25;
        rocCalcLength = 30;
        liqLength = 50;

        lots = 1;

        bufferCount = 0;

        upband = 0.0;
        downBand = 0.0;
        rocCalc = 0.0;

        liqDays = 50;
        liqPoint = 0.0;

    }

    vector<string> getCanshuSetNames(){
        vector<string> vCanshu; vCanshu.clear();

        vCanshu.push_back("bollingerLengths");
        vCanshu.push_back("Offset");
        vCanshu.push_back("rocCalcLength");
        vCanshu.push_back("liqLength");

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

        bollingerLengths = arr[0];
        Offset = arr[1];
        rocCalcLength = arr[2];
        liqLength = arr[3];
    }

    TradingResultNode getTradingResult( const char *_code , const char *_period,const char *start_date , const char *end_date){
        TradingResultNode a = TradingResultNode( _code, _period ,start_date, end_date ,tradingResult,tradingNum , account_money ,slippage * n_slippages, rate , perSize , cash_deposit, vecTradeDay , vecTradeWeekDay , dayIncome , weekIncome , bzjAccount , dayAccount, positionAccount , fee_type);
        vector<double> d; d.clear();

        d.push_back(bollingerLengths);  d.push_back(Offset); d.push_back(rocCalcLength); d.push_back(liqLength);  

        a.setCanshuSet(d);
        return a;
    }

    void onBar_bef(int i){
        ctaTemplate::onBar_bef(i);
    }

    void onBar_after(int i){
        ctaTemplate::onBar_after(i);
    }

    double SimpleAverage( const vector<double> &closeArray , int length , int nn){
        double ssum = 0.0;
        for( int i = 1; i <= length;i++){
            ssum += closeArray[ max(0 , nn - i)];
        }
        return ssum * 1.0 / length;
    }

    void onBar(int i)
    {
        onBar_bef(i);

        double preLiqPoint = liqPoint;
        if( i > 1){
            if( this->pos <= 0 and rocCalc > 0 and highArray[i] >= upband){
                this->Buy(lots , max(openArray[i] , upband) , i);
            }
            if( this->pos == 0){
                liqDays = liqLength;
            }else{
                liqDays = liqDays - 1;
                liqDays = max(liqDays , 10);
            }

            liqPoint = this->SimpleAverage( closeArray, liqDays , i);

            if( this->pos > 0 and BarsSinceEntry >= 1 and preLiqPoint < upband and lowArray[i] <= preLiqPoint){
                this->Sell( abs(this->pos) , min(openArray[i] , preLiqPoint) , i);
            }

            if( this->pos >= 0 and rocCalc < 0 and lowArray[i] <= downBand){
                this->SellShort(lots, min(openArray[i] , downBand) , i);
            }
                        
            if( this->pos < 0 and BarsSinceEntry >= 1 and preLiqPoint > downBand and highArray[i] >= preLiqPoint){
                this->BuyToCover( abs(this->pos) , max(openArray[i] , preLiqPoint) , i);
            }
        }

        upband = MidLine[i] + Offset * Band[i];
        downBand = MidLine[i] - Offset * Band[i];
        rocCalc = closeArray[i] - closeArray[ max(0 , i - rocCalcLength + 1)];

        //liqPoint = this->SimpleAverage( closeArray, liqDays , i);
        onBar_after(i);
    }

    void beforeRun( )
    {
        ctaTemplate::beforeRun();

        MidLine.clear(); Band.clear();
        MidLine.reserve(n + 10);  Band.reserve(n + 10);

        Average(MidLine , closeArray , bollingerLengths , n);
        StandardDev(Band , closeArray , bollingerLengths , n);

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
            for( int bollingerLengths = 35; bollingerLengths <= 65; bollingerLengths += 3){
                for( double Offset = 0.8; Offset <= 1.5; Offset += 0.25){
                    for( int rocCalcLength = 20; rocCalcLength <= 40; rocCalcLength += 4){
                        for( int liqLength = 40; liqLength <= 60; liqLength += 5){
                            vector<double> tmp; tmp.clear();
                            tmp.push_back(bollingerLengths);
                            tmp.push_back(Offset);
                            tmp.push_back(rocCalcLength);
                            tmp.push_back(liqLength);

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


#endif // _PeriodBollingerBandit
