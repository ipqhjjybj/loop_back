/**
 * @copyright JiuXi
 * @author ipqhjjybj
 **/

#ifndef __BollingerBotStrategyTest
#define __BollingerBotStrategyTest

#include "load_config.h"
#include "ctaStrategyTemplate.h"

class BollingerBotStrategyTest : public ctaTemplate{

public:
	static const int BufferSize = 30;
	/////////////////////////////////////////////////////////////
    //申明参数
    int bollLength;            //通道窗口数
    double buyEntryDev;        //开多仓偏差
    double buyExitDev;		   //平多仓偏差
    double sellEntryDev;       //开空仓偏差
    double sellExitDev;        //平空仓偏差
    double trailingPrcnt;      //移动止损百分比
    int maLongLength;          //过滤用均线多头窗口
    int maShortLength;         //过滤用均线空头窗口
    ///////////////////////////////////////////////////////////////
    //申明变量

    vector<double> bollMid , bollStd , maLongArray , maShortArray , maLongArray;
    double high_since_entry, low_since_entry;
    double longEntry , sellEntry;

    BollingerBotStrategy(){
    	this->setStrategyName("bollingerBotStrategyTest");
        ctaTemplate();
    }
    void init(){
    	ctaTemplate::init();
    	bollLength = 28;
    	buyEntryDev   = 3.2;
    	buyExitDev    = 1.2;
        sellEntryDev  = 3.2;
        sellExitDev   = 1.2;

    	trailingPrcnt = 0.4;
    	maLongLength   = 4;
        maShortLength  = 4;

    	lots = 1;

        maShortArray.reserve(n + 10);
        maLongArray.reserve(n + 10);
        bollMid.reserve(n + 10);
        bollStd.reserve(n + 10);

        maShortArray.clear();
        maLongArray.clear();
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
        vCanshu.push_back("buyEntryDev");
        vCanshu.push_back("buyExitDev");
        vCanshu.push_back("sellEntryDev");
        vCanshu.push_back("sellExitDev");
        vCanshu.push_back("trailingPrcnt");
        vCanshu.push_back("maLongLength");
        vCanshu.push_back("maShortLength");

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
        TradingResultNode a = TradingResultNode( _code, _period ,start_date, end_date ,tradingResult,tradingNum , account_money ,slippage * n_slippages, rate , perSize , cash_deposit, vecTradeDay , vecTradeWeekDay , dayIncome , weekIncome  , bzjAccount, dayAccount, positionAccount , fee_type);
        vector<double> d; d.clear();
        d.push_back(this->bollLength);
        d.push_back(this->buyEntryDev);
        d.push_back(this->buyExitDev);
        d.push_back(this->sellEntryDev);
        d.push_back(this->sellExitDev);
        d.push_back(this->trailingPrcnt);
        d.push_back(this->maLongLength);
        d.push_back(this->maShortLength);

        a.setCanshuSet(d);
        return a;
    }

    void beforeRun(){
        ctaTemplate::beforeRun();

        high_since_entry = 0.0;
        low_since_entry  = 0.0;
        longEntry = 0.0;
        sellEntry = 0.0;

        maLongArray.reserve(n + 10);
        maShortArray.reserve(n + 10);
        bollMid.reserve(n + 10);
        bollStd.reserve(n + 10);

        Average(maLongArray , closeArray , maLongLength , n);
        Average(maShortArray, closeArray , maShortLength , n);

        Average(bollMid , closeArray ,  bollLength , n);
        StandardDev(bollStd , closeArray , bollLength , n);
    }

    void onBar(int i){

        onBar_bef(i);

        double buyentryUp = bollMid[i] + bollStd[i] * this->buyEntryDev;
        double buyexitUp  = bollMid[i] + bollStd[i] * this->buyExitDev;
        double sellentryDown = bollMid[i] - bollStd[i] * this->sellEntryDev;
        double sellexitDown = bollMid[i] - bollStd[i] * this->sellExitDev;

        double maLongFilter = maLongArray[i] , maLongFilter1 = maLongArray[i];
        if(i > 1){
            maLongFilter1 = maLongArray[i-1];
        }
        double maShortFilter = maShortArray[i] , maShortFilter1 = maShortArray[i];
        if(i > 1){
            maShortFilter1 = maShortArray[i-1];
        }
        if(this->pos == 0 ){
            high_since_entry = highArray[i];
            //printf("%lf %lf %lf\n",closeArray[i] , maFilter , maFilter1);
            if( closeArray[i] > maLongFilter && closeArray[i] > maLongFilter1){
                longEntry = buyentryUp;
                if(highArray[i] > longEntry){
                    this->Buy(this->lots , closeArray[i] , i);
                }
            }
            low_since_entry = lowArray[i];
            if( closeArray[i] < maShortFilter && closeArray[i] < maShortFilter1){
                sellEntry = sellentryDown;
                if(lowArray[i] < sellEntry){
                    this->SellShort(this->lots , closeArray[i] , i);
                }
            }
        }else if(this->pos > 0){
            high_since_entry = max(high_since_entry , highArray[i]);
            low_since_entry = lowArray[i];
            double longExit = high_since_entry * ( 1 - this->trailingPrcnt / 100.0);
            longExit = min(longExit , buyexitUp);
            if( lowArray[i] < longExit){
                //printf("Sell \n");
                this->Sell(abs(this->pos) , closeArray[i] , i);
            }
        }else if(this->pos < 0){
            high_since_entry = highArray[i];
            low_since_entry = min(low_since_entry , lowArray[i]);
            double shortExit = low_since_entry * (1 + this->trailingPrcnt / 100.0);
            shortExit = max(shortExit , sellexitDown);
            if( highArray[i] > shortExit){
                this->BuyToCover(abs(this->pos) , closeArray[i] , i);
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
            for(int bollLength = 5; bollLength <= 30 ; bollLength += 2){
                for(double buyEntryDev = 1.8 ; buyEntryDev <= 5.0 ; buyEntryDev += 0.2){
                    for(double buyExitDev = 0 ; buyExitDev <= 1.0 ; buyExitDev += 0.2){
                        for(double sellEntryDev = 1.8; sellEntryDev <= 5.0; sellEntryDev += 0.2){
                            for(double sellExitDev = 0; sellExitDev <= 1.0; sellExitDev += 0.2){
                                for(int maLongLength = 2; maLongLength <= 33; maLongLength += 2){
                                    for(int maShortLength = 2; maShortLength <= 33; maShortLength += 2){
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
