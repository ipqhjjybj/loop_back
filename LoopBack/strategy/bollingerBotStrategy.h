/**
 * @copyright JiuXi
 * @author ipqhjjybj
 **/

#ifndef __BollingerBotStrategy
#define __BollingerBotStrategy

#include "../load_config.h"
#include "../ctaStrategyTemplate.h"

class BollingerBotStrategy : public ctaTemplate{

public:
	static const int BufferSize = 30;
	/////////////////////////////////////////////////////////////
    //申明参数
    int bollLength;        //通道窗口数
    double entryDev;       //开仓偏差
    double exitDev;		   //平仓偏差
    double trailingPrcnt;  //移动止损百分比
    int maLength;          //过滤用均线窗口
    ///////////////////////////////////////////////////////////////
    //申明变量

    vector<double> bollMid , bollStd , maArray;
    double high_since_entry, low_since_entry;
    double longEntry , sellEntry;

    BollingerBotStrategy(){
    	this->setStrategyName("bollingerBotStrategy");
        ctaTemplate();
    }
    void init(){
    	ctaTemplate::init();
    	bollLength = 28;
    	entryDev   = 3.2;
    	exitDev    = 1.2;
    	trailingPrcnt = 0.4;
    	maLength   = 0.4;

    	lots = 1;

        maArray.reserve(n + 10);
        bollMid.reserve(n + 10);
        bollStd.reserve(n + 10);

        maArray.clear();
        bollMid.clear();
        bollStd.clear();
    }
    void init(const char *code){
    	ctaTemplate::init(code);
        this->init();
        this->initFee(code);

        if( is_zuhehuice == 1){
            account_money = __const_ini_money;
            lots = __const_ini_money / (__high_history_price );
        }else{
            account_money = __high_history_price * perSize * cash_deposit * 5 / 100.0;
            //printf("%lf , %lf , %lf ,%lf\n",__high_history_price , account_money * 1.0, perSize*1.0 , cash_deposit*1.0);
            lots = 1;
            if(account_money < 1){
                account_money = __const_ini_money;
            }
        }
    }
    vector<string> getCanshuSetNames(){
        vector<string> vCanshu; vCanshu.clear();

        vCanshu.push_back("bollLength");
        vCanshu.push_back("entryDev");
        vCanshu.push_back("exitDev");
        vCanshu.push_back("trailingPrcnt");
        vCanshu.push_back("maLength");

        return vCanshu;
    }

    void setCanshuFromVec(const vector<double> &arr){
        this->bollLength = arr[0] ;
        this->entryDev   = arr[1] ;
        this->exitDev    = arr[2] ;
        this->trailingPrcnt = arr[3];
        this->maLength   = arr[4];
    }

    TradingResultNode getTradingResult( const char *_code , const char *_period,const char *start_date , const char *end_date){
        TradingResultNode a = TradingResultNode( _code, _period ,start_date, end_date ,tradingResult,tradingNum , account_money ,slippage * n_slippages, rate , perSize , cash_deposit, vecTradeDay , vecTradeWeekDay , dayIncome , weekIncome , bzjAccount, dayAccount, positionAccount ,fee_type);
        vector<double> d; d.clear();
        d.push_back(this->bollLength);
        d.push_back(this->entryDev);
        d.push_back(this->exitDev);
        d.push_back(this->trailingPrcnt);
        d.push_back(this->maLength);

        a.setCanshuSet(d);
        return a;
    }

    void beforeRun(){
        ctaTemplate::beforeRun();

        high_since_entry = 0.0;
        low_since_entry  = 0.0;
        longEntry = 0.0;
        sellEntry = 0.0;

        Average(maArray, closeArray , maLength , n);
        Average(bollMid , closeArray ,  bollLength , n);
        StandardDev(bollStd , closeArray , bollLength , n);

    }

    void onBar(int i){

        onBar_bef(i);

        double entryUp = bollMid[i] + bollStd[i] * this->entryDev;
        double exitUp  = bollMid[i] + bollStd[i] * this->exitDev;
        double maFilter = maArray[i] , maFilter1 = maArray[i];
        if(i > 1){
            maFilter1 = maArray[i-1];
        }
        // printf("maLength = %d n = %d\n",maLength,n);
        // printf("close %lf maFilter %lf maFilter1 %lf\n",closeArray[i] , maFilter , maFilter1);
        if(this->pos == 0 ){
            high_since_entry = highArray[i];
            //printf("%lf %lf %lf\n",closeArray[i] , maFilter , maFilter1);
            if( closeArray[i] > maFilter && closeArray[i] > maFilter1){
                longEntry = entryUp;
                //printf("longEntry : %lf\n", longEntry);
                if(highArray[i] > longEntry){
                    //printf("buy!\n");
                    this->Buy(this->lots , closeArray[i] , i);
                }
            }
        }else if(this->pos > 0){
            high_since_entry = max(high_since_entry , highArray[i]);
            double longExit = high_since_entry * ( 1 - this->trailingPrcnt / 100.0);
            longExit = min(longExit , exitUp);
            if( lowArray[i] < longExit){
                //printf("Sell \n");
                this->Sell(abs(this->pos) , closeArray[i] , i);
            }
        }

        onBar_after(i);
    }

    /**
     *  这里提供一种暴力循环获得 所有参数的例子
     *  本来应该使用递归的方式做的, 但后来想了一下，， 还是用这种来做吧， 毕竟简单，可以添加各种条件约束
     **/
    void loopForce(){

        FILE *fp = NULL;
        string loopForceFileName = string("./runningTEMP/") + this->strategyName + ".csv";
        int t_canshuNum = 0;
        // vCanshu.push_back("bollLength");
        // vCanshu.push_back("entryDev");
        // vCanshu.push_back("exitDev");
        // vCanshu.push_back("trailingPrcnt");
        // vCanshu.push_back("maLength");

        if( (fp = fopen(loopForceFileName.c_str(), "w")) != NULL){
            for(int bollLength = 5; bollLength <= 6 ; bollLength += 2){
                for(double entryDev = 1.8 ; entryDev <= 2.0 ; entryDev += 0.2){
                    for(double exitDev = 0 ; exitDev <= 0.2 ; exitDev += 0.2){
                        for(double trailingPrcnt = 0.2; trailingPrcnt <= 0.2; trailingPrcnt += 0.2){
                            for(int maLength = 2; maLength <= 2 ; maLength += 2){
                                vector<double> tmp; tmp.clear();
                                tmp.push_back(bollLength);
                                tmp.push_back(entryDev);
                                tmp.push_back(exitDev);
                                tmp.push_back(trailingPrcnt);
                                tmp.push_back(maLength);

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

        for( int i = 1; i < n;i++){ // i = BufferSize - 1
            onBar(i);
        }

        this->afterRun();
    }

};

#endif
