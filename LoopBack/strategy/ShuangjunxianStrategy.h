
#ifndef _ShuangjunxianStrategy
#define _ShuangjunxianStrategy

#include "../load_config.h"
#include "../ctaStrategyTemplate.h"


class ShuangjunxianStrategy : public ctaTemplate{

public:
    static const int BufferSize = 30;

    /////////////////////////////////////////////////////////////
    //申明参数
    int MABLen;//kb均线的周期数
    int MABRate;//makb均线的周期数

    int initialcp;//用于开仓的资金
    int bl;//开仓资金利用率（十分之几）

    int zhiying_num;//取前zhiying_num根bar的最高价最低价
    int zhiyinglv_l;//多仓止盈率
    int zhiyinglv_s;//空仓zhiyinglv
    int zhiying_per;//每次止盈比例,百分之
    int zhiying_resper;//每次开仓保留比例，十分之


    ///////////////////////////////////////////////////////////////
    //申明变量
    //均线
    int Rate_C;//close在ka值中所占权重
    int Rate_O;//open在ka值中所占权重
    int Rate_L;//low在ka值中所占权重
    int Rate_H;//high在ka值中所占权重
    vector<double> ka;//ka值
    double kasum;
    vector<double> kb;//kb值
    vector<double> makb;//makb值

    //开仓手数
    int hands;//开仓手数

    //入场后的最高价最低价
    vector<double> high_since_entry;//入场后的最高价
    vector<double> low_since_entry;//入场后的最低价

    //止盈
    double zhiying_l;//多仓止盈线
    double zhiying_s;//空仓止盈线
    int zhiying_lots;//每次止盈手数
    int zhiying_kk;//第几次止盈
    int zhiying_reslots;//每次保留手数
    int entry_i;//进场bar编号


    ShuangjunxianStrategy(){
        this->setStrategyName("ShuangjunxianStrategy");
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

        MABLen = 20;
        MABRate = 10;

        initialcp = 152500;
        bl = 5;

        zhiying_num = 3;
        zhiyinglv_l = 10;
        zhiyinglv_s = 10;
        zhiying_per = 3;
        zhiying_resper = 5;


        ka.clear();
        kb.clear();
        makb.clear();
        high_since_entry.clear();
        low_since_entry.clear();

        ka.reserve(n+10);
        kb.reserve(n+10);
        makb.reserve(n+10);
        high_since_entry.reserve(n+10);
        low_since_entry.reserve(n+10);




        //printf("2 n=%d\n",n);
        //bufferCount = 0;
    }
    vector<string> getCanshuSetNames(){
        vector<string> vCanshu; vCanshu.clear();

        vCanshu.push_back("MABLen");
        vCanshu.push_back("MABRate");
        vCanshu.push_back("initialcp");
        vCanshu.push_back("bl");
        vCanshu.push_back("zhiying_num");
        vCanshu.push_back("zhiyinglv_l");
        vCanshu.push_back("zhiyinglv_s");
        vCanshu.push_back("zhiying_per");
        vCanshu.push_back("zhiying_resper");




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
            lots = __const_ini_money / (__high_history_price * 10);
        }else{
            /*account_money = __high_history_price * perSize * cash_deposit * 5 / 100.0;
            //printf("%lf , %lf , %lf ,%lf\n",__high_history_price , account_money * 1.0, perSize*1.0 , cash_deposit*1.0);
            lots = 1;
            if(account_money < 1){
                account_money = __const_ini_money;
            }*/
            account_money = __const_ini_money;
        }
    }

    void setCanshuFromVec(const vector<double> &arr){

        this->MABLen = arr[0] + eps;
        this->MABRate = arr[1] + eps;
        this->initialcp = arr[2] + eps;
        this->bl = arr[3] + eps;
        this->zhiying_num = arr[4] + eps;
        this->zhiyinglv_l = arr[5] + eps;
        this->zhiyinglv_s = arr[6] + eps;
        this->zhiying_per = arr[7] + eps;
        this->zhiying_resper = arr[8] + eps;





    }
    TradingResultNode getTradingResult( const char *_code , const char *_period,const char *start_date , const char *end_date){
        TradingResultNode a = TradingResultNode( _code, _period ,start_date, end_date ,tradingResult,tradingNum, account_money ,slippage * n_slippages, rate , perSize , cash_deposit, vecTradeDay , vecTradeWeekDay , dayIncome , weekIncome ,bzjAccount , dayAccount, positionAccount, fee_type);
        vector<double> d; d.clear();

        d.push_back(this->MABLen);
        d.push_back(this->MABRate);
        d.push_back(this->initialcp);
        d.push_back(this->bl);
        d.push_back(this->zhiying_num);
        d.push_back(this->zhiyinglv_l);
        d.push_back(this->zhiyinglv_s);
        d.push_back(this->zhiying_per);
        d.push_back(this->zhiying_resper);


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
        ka[i] = ka[i-1];
        kb[i] = kb[i-1];
        makb[i] = makb[i-1];
        high_since_entry[i] = high_since_entry[i-1];
        low_since_entry[i] = low_since_entry[i-1];


        //===================================================================================================================
        //均线
        ka[i] = (Rate_O * 1.0 * openArray[i] + Rate_H * 1.0 * highArray[i] + Rate_L * 1.0 * lowArray[i] + Rate_C * 1.0 * closeArray[i]) * 1.0 / (Rate_O + Rate_H + Rate_L + Rate_C);
        kasum = 0.0;
        for (int ii=0; ii<MABLen; ii++)
        {
            kasum = (MABLen-ii) * 1.0 * ka[i-ii] + kasum;
        }
        kb[i] = kasum * 2.0 / MABLen / (MABLen + 1);
        Average(makb, kb, MABRate , n);

        //===================================================================================================================
        //开仓手数
        hands = (initialcp * 100.0 * bl)/ (10.0 * openArray[i] * perSize * cash_deposit);

        //===================================================================================================================
        //进场
        //开多仓
        if (kb[i-1]>makb[i-1] and kb[i-2]<makb[i-2] and mp[i]<=0)
        {
            Buy(hands , openArray[i] , i);
            high_since_entry[i] = highArray[i];
            low_since_entry[i] = lowArray[i];
            zhiying_kk = 1;
            zhiying_lots = hands * 0.01 * zhiying_per * zhiying_kk;
            zhiying_lots = max(1,zhiying_lots);
            zhiying_reslots = hands * 0.1 * zhiying_resper;
            zhiying_reslots = max(1,zhiying_reslots);
            entry_i = i;
            //printf("%d , %s , %d\n", i , "bk" , hands);
        }

        //开空仓
        if (kb[i-1]<makb[i-1] and kb[i-2]>makb[i-2] and mp[i]>=0)
        {
            SellShort(hands , openArray[i] , i);
            high_since_entry[i] = highArray[i];
            low_since_entry[i] = lowArray[i];
            zhiying_kk = 1;
            zhiying_lots = hands * 0.01 * zhiying_per * zhiying_kk;
            zhiying_lots = max(1,zhiying_lots);
            zhiying_reslots = hands * 0.1 * zhiying_resper;
            zhiying_reslots = max(1,zhiying_reslots);
            entry_i = i;
            //printf("%d , %s , %d\n", i , "sk" , hands);
        }


        //===================================================================================================================
        //入场后的最高价最低价
        if (mp[i]!=0 and BarsSinceEntry>0)
        {
            high_since_entry[i] = max(highArray[i] , high_since_entry[i]);
            low_since_entry[i] = min(lowArray[i] , low_since_entry[i]);
        }


        //===================================================================================================================
        //止盈线
        //多仓止盈线
        if (mp[i]>0)
        {
            zhiying_l = high_since_entry[i-zhiying_num] * 1.0 * (1 + zhiyinglv_l * 1.0 / 1000.0);
        }
        else
        {
            zhiying_l = highArray[i] + 1;
        }

        //空仓止盈线
        if (mp[i]<0)
        {
            zhiying_s = low_since_entry[i-zhiying_num] * 1.0 * (1  - zhiyinglv_s * 1.0 / 1000.0);
        }
        else
        {
            zhiying_s = lowArray[i] - 1;
        }


        //===================================================================================================================
        //止盈
        //多仓止盈
        if (highArray[i]>=zhiying_l and mp[i]>0 and mp[i-1]>0 and i - entry_i>=zhiying_num and fabs(mp[i])>zhiying_reslots)
        {
            Sell(zhiying_lots , max(openArray[i],zhiying_l), i );
            zhiying_kk = zhiying_kk + 1;
            zhiying_lots = (abs(mp[i]) - zhiying_lots) * 0.01 * zhiying_per * zhiying_kk;
            zhiying_lots = max(1,zhiying_lots);
            //printf("%d , %s , %d\n", i , "sp" , zhiying_lots);
        }

        //空仓止盈
        if (lowArray[i]<=zhiying_s and mp[i]<0 and mp[i-1]<0 and i - entry_i>=zhiying_num and fabs(mp[i])>zhiying_reslots)
        {
            BuyToCover(zhiying_lots , min(openArray[i] , zhiying_s) , i );
            zhiying_kk = zhiying_kk + 1;
            zhiying_lots = (abs(mp[i]) - zhiying_lots) * 0.01 * zhiying_per * zhiying_kk;
            zhiying_lots = max(1,zhiying_lots);
            //printf("%d , %s , %d\n", i , "bp" , zhiying_lots);
        }



        onBar_after(i);
        //printf("%d ,%lf , %lf ,%lf ,%lf , %lf , %lf , %lf ,%d ,%lf , %lf , %lf ,%lf ,%d , %d ,%d ,%d ,%d\n",i ,openArray[i] , highArray[i] , lowArray[i] ,closeArray[i] ,ka[i] ,kb[i] , makb[i] ,mp[i] , high_since_entry[i], low_since_entry[i] , zhiying_l , zhiying_s ,zhiying_kk , zhiying_lots , zhiying_reslots , BarsSinceEntry ,i - entry_i);


    }

    void beforeRun( )
    {
        ctaTemplate::beforeRun();

        // Strategy Variable
        //变量赋初始值
        Rate_C = 3;
        Rate_O = 1;
        Rate_L = 1;
        Rate_H = 1;
        ka[0] = 0.0;
        kasum = 0.0;
        kb[0] = 0.0;
        makb[0] = 0.0;

        hands = 0;

        high_since_entry[0] = 0.0;
        low_since_entry[0] = 0.0;

        zhiying_l = 0.0;
        zhiying_s = 0.0;
        zhiying_lots = 0;
        zhiying_kk = 0;
        zhiying_reslots = 0;
        entry_i = 0;

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
            int MABLen = 20;
            int MABRate = 10;
            int initialcp = 152500;
            int bl = 5;

            int zhiying_num = 3;
            int zhiyinglv_l =10;
            int zhiyinglv_s = 10;
            int zhiying_per = 3;
            int zhiying_resper = 5;



            vector<double> tmp; tmp.clear();
            tmp.push_back( MABLen * 1.0);
            tmp.push_back( MABRate * 1.0);
            tmp.push_back( initialcp * 1.0);
            tmp.push_back( bl * 1.0);
            tmp.push_back( zhiying_num * 1.0);
            tmp.push_back( zhiyinglv_l * 1.0);
            tmp.push_back( zhiyinglv_s * 1.0);
            tmp.push_back( zhiying_per * 1.0);
            tmp.push_back( zhiying_resper * 1.0);


            for(int t_ji = 0; t_ji < tmp.size(); t_ji++){
                if(0 == t_ji){
                    fprintf(fp,"%lf",tmp[t_ji]);
                }else{
                    fprintf(fp,",%lf",tmp[t_ji]);
                }
            }fprintf(fp,"\n");
            t_canshuNum = tmp.size();

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
