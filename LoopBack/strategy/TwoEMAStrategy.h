
#ifndef _TwoEMAStrategy
#define _TwoEMAStrategy

#include "../load_config.h"
#include "../ctaStrategyTemplate.h"


/**
 *  策略，劲道指数
 */
class TwoEMAStrategy : public ctaTemplate{

public:
    static const int BufferSize = 30;

    /////////////////////////////////////////////////////////////
    //申明参数

    int short_length;		//长周期指标 快线
    int long_length;		//长周期指标 慢线

    ///////////////////////////////////////////////////////////////

    vector<double> short_arr;
    vector<double> long_arr;

    int signalLong;
    int bufferCount ;

    FILE *debug_f ;

    TwoEMAStrategy(){
        this->setStrategyName("TwoEMAStrategy");
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

        short_length = 5;
        long_length = 20;

        lots = 1;

        bufferCount = 0;

    }
    vector<string> getCanshuSetNames(){
        vector<string> vCanshu; vCanshu.clear();

        vCanshu.push_back("short_length");
        vCanshu.push_back("long_length");

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
        short_length = arr[0];
        long_length = arr[1];
    }

    TradingResultNode getTradingResult( const char *_code , const char *_period,const char *start_date , const char *end_date){
        TradingResultNode a = TradingResultNode( _code, _period ,start_date, end_date ,tradingResult,tradingNum , account_money ,slippage * n_slippages, rate , perSize , cash_deposit, vecTradeDay , vecTradeWeekDay , dayIncome , weekIncome , bzjAccount , dayAccount, positionAccount , fee_type);
        vector<double> d; d.clear();
        d.push_back( short_length); d.push_back( long_length);

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
        int preSignal = signalLong;

        if( short_arr[i]  > long_arr[i] ){
            signalLong = 1;
        }else{
            signalLong = -1;
        }

        if(preSignal > 0){
            if( this->pos <= 0){
                this->Buy( lots , closeArray[i] , i);
            }
        }else{
            if( this->pos >= 0){
                this->SellShort(lots , closeArray[i] , i);
            }
        }
        onBar_after(i);
    }

    void beforeRun( )
    {
        ctaTemplate::beforeRun();

        signalLong = 0;

        short_arr.clear(); long_arr.clear();
        short_arr.reserve(n + 10); long_arr.reserve(n + 10);

        // Strategy Variable
        xAverage( short_arr , closeArray , short_length , n);
        xAverage( long_arr , closeArray , long_length , n);
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
            for( int short_length = 4; short_length <= 10 ;short_length++){
                for( int long_length = 15; long_length <= 25; long_length++){
                    vector<double> tmp; tmp.clear();
                    tmp.push_back(short_length);
                    tmp.push_back(long_length);

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


#endif // _TwoEMAStrategy
