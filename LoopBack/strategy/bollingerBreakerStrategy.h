/**
 * @copyright JiuXi
 * @author ipqhjjybj
 **/

#ifndef __BollingerBreakerStrategy
#define __BollingerBreakerStrategy

#include "../load_config.h"
#include "../ctaStrategyTemplate.h"

class BollingerBreakerStrategy : public ctaTemplate{

public:
	static const int BufferSize = 30;
	/////////////////////////////////////////////////////////////
    //申明参数
    int bollLength;        //通道窗口数
    double topDev;         //开仓偏差
    double trailingPrcnt;  //移动止损百分比
    int use_range ;	   //use_range天内有突破最高价
    int break_day_n ;  //break_day_n天有突破最高价
    ///////////////////////////////////////////////////////////////
    //申明变量

    vector<double> bollMid , bollStd , buyValue , highestArr;
    double high_since_entry ;
    double longEntry ;

    BollingerBreakerStrategy(){
    	this->setStrategyName("BollingerBreakerStrategy");
        ctaTemplate();
    }
    void init(){
    	ctaTemplate::init();
    	bollLength = 20;
    	topDev     = 1.3;
    	trailingPrcnt = 0.8;
    	use_range   = 10;
    	break_day_n = 10;

    	lots = 1;

        buyValue.reserve(n + 10);
        bollMid.reserve(n + 10);
        bollStd.reserve(n + 10);
        highestArr.reserve(n + 10);

        highestArr.clear();
        buyValue.clear();
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
            //printf("account_money:%lf __high_history_price:%lf perSize:%lf cash_deposit %lf\n",account_money , __high_history_price , perSize ,cash_deposit );
        }
    }
    vector<string> getCanshuSetNames(){
        vector<string> vCanshu; vCanshu.clear();

        vCanshu.push_back("bollLength");
        vCanshu.push_back("topDev");
        vCanshu.push_back("trailingPrcnt");
        vCanshu.push_back("use_range");
        vCanshu.push_back("break_day_n");

        return vCanshu;
    }

    void setCanshuFromVec(const vector<double> &arr){
        this->bollLength 	   = arr[0] ;
        this->topDev     	   = arr[1] ;
        this->trailingPrcnt    = arr[2] ;
        this->use_range        = arr[3] ;
        this->break_day_n      = arr[4] ;
    }

    TradingResultNode getTradingResult( const char *_code , const char *_period,const char *start_date , const char *end_date){
        TradingResultNode a = TradingResultNode( _code, _period ,start_date, end_date ,tradingResult,tradingNum , account_money ,slippage * n_slippages, rate , perSize , cash_deposit, vecTradeDay , vecTradeWeekDay , dayIncome , weekIncome , bzjAccount , dayAccount, positionAccount, fee_type);
        vector<double> d; d.clear();
        d.push_back(this->bollLength);
        d.push_back(this->topDev);
        d.push_back(this->trailingPrcnt);
        d.push_back(this->use_range);
        d.push_back(this->break_day_n);

        a.setCanshuSet(d);
        return a;
    }

    void beforeRun(){
        ctaTemplate::beforeRun();

        high_since_entry = 0.0;
        longEntry = 0.0;

        Average(bollMid , closeArray , bollLength , n);
        Highest(highestArr , highArray , break_day_n ,n);
        StandardDev(bollStd , closeArray , bollLength , n);

    }

    void onBar(int i){

        onBar_bef(i);

        buyValue[i] = bollMid[i] + bollStd[i] * this->topDev;

        int cond1 = 0;
        for( int loop_i = 1; loop_i < i && loop_i < use_range; loop_i++){
        	if( highArray[i - loop_i] > buyValue[i - loop_i]){
        		cond1 = 1;
        		break;
        	}
        }
        int cond2 = 0;
        if( i > 1 && highArray[i] >= highestArr[i] && highArray[i-1] <= highestArr[i-1]){
        	cond2 = 1;
        }

        if(this->pos == 0){
        	high_since_entry = highArray[i];
        	if( cond1 > 0 && cond2 > 0){
        		this->Buy(this->lots , closeArray[i] , i);
        	}
        }else if(this->pos > 0){
        	high_since_entry = max(high_since_entry , highArray[i]);
        	double longExit = high_since_entry * ( 1 - this->trailingPrcnt / 100.0);
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

        if( (fp = fopen(loopForceFileName.c_str(), "w")) != NULL){
        	for(int bollLength = 5; bollLength <= 30 ; bollLength += 2){
        		for(double topDev = 1.0 ; topDev <= 5.0 ; topDev += 0.3){
        			for(double trailingPrcnt = 0.4; trailingPrcnt <= 3.0; trailingPrcnt += 0.2){
        				for(int use_range = 5; use_range <= 30 ; use_range += 2){
        					for(int break_day_n = 8; break_day_n <= 30; break_day_n += 2){
        						vector<double> tmp; tmp.clear();
        						tmp.push_back(bollLength);
        						tmp.push_back(topDev);
        						tmp.push_back(trailingPrcnt);
        						tmp.push_back(use_range);
        						tmp.push_back(break_day_n);

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
