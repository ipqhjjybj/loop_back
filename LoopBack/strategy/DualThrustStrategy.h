#ifndef _DualThrustStrategy
#define _DualThrustStrategy

#include "../load_config.h"
#include "../ctaStrategyTemplate.h"

class DualThrustStrategy : public ctaTemplate{
public:
	static const int BufferSize = 30;

	/////////////////////////////////////////////////////////////
    //申明参数
	double k1,k2;
	///////////////////////////////////////////////////////////////
    //策略变量
    double longEntry , shortEntry , range;
    double dayHigh , dayLow , dayOpen;

    pair<int , string> need_bar_endtime;
    int longEntered,  shortEntered;

    DualThrustStrategy(){
    	this->setStrategyName("DualThrustStrategy");
        ctaTemplate();
    }

    void init(){
    	ctaTemplate::init();
    	k1 = 0.4;
    	k2 = 0.6;
    	lots = 1;
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
        }
    }

    vector<string> getCanshuSetNames(){
        vector<string> vCanshu; vCanshu.clear();

        vCanshu.push_back("k1");
        vCanshu.push_back("k2");

        return vCanshu;
    }

    void setCanshuFromVec(const vector<double> &arr){
        this->k1 		   = arr[0] ;
        this->k2           = arr[1] ;
    }

    TradingResultNode getTradingResult( const char *_code , const char *_period,const char *start_date , const char *end_date){
        TradingResultNode a = TradingResultNode( _code, _period ,start_date, end_date ,tradingResult,tradingNum , account_money ,slippage * n_slippages, rate , perSize , cash_deposit, vecTradeDay , vecTradeWeekDay , dayIncome , weekIncome , bzjAccount, dayAccount, positionAccount, fee_type);
        vector<double> d; d.clear();
        d.push_back(this->k1);
        d.push_back(this->k2);
        a.setCanshuSet(d);
        return a;
    }

    void beforeRun(){
        ctaTemplate::beforeRun();

        longEntry  = 0.0;
        shortEntry = 0.0;

        dayOpen = 0.0;
        dayHigh = 0.0;
        dayLow  = 0.0;
        range = 0.0;

        longEntered = 0;
        shortEntered = 0;

        //need_bar_endtime = Bar_end_time(this->ttperiod);

    }

    void onBar(int i){
    	onBar_bef(i);

       	if( i > 0 && g_time[i].compare("16:00:00") > 0 && g_time[i-1].compare("16:00:00") < 0){
       		//新的一天
       		if(this->dayHigh > 0.0){
       			//说明已经初始化过了
       			this->range = this->dayHigh - this->dayLow;
       			this->longEntry  = openArray[i] + this->k1 * this->range;
       			this->shortEntry = openArray[i] - this->k2 * this->range;
       		}
       		this->dayOpen = openArray[i];
       		this->dayHigh = highArray[i];
       		this->dayLow  = lowArray[i];

       		this->longEntered = 0;
       		this->shortEntered = 0;
       	}

       	if(this->range > 0.0 ){
       		if( g_time[i].compare( "14:29:00") < 0 or g_time[i].compare( "21:00:00") >= 0){
       			// 尚未到离场时间
       			if(this->pos == 0){
       				if(closeArray[i] > this->dayOpen){
      					if( 0 == this->longEntered  && highArray[i] > this->longEntry){
      						this->Buy( lots , closeArray[i] , i);
      					}
       				}else{
       					if( 0 == this->shortEntered && lowArray[i] < this->shortEntry){
       						this->SellShort( lots , closeArray[i] , i);
       					}
       				}
       			}else if(this->pos > 0){
       				this->longEntered = 1;
       				if(this->lowArray[i] < this->shortEntry){
       					this->Sell(abs(this->pos) , closeArray[i] , i);
       					if( 0 == this->shortEntered){
       						this->SellShort( lots , closeArray[i] , i);
       					}
       				}
       			}else if(this->pos < 0){
       				this->shortEntered = 1;
       				if(this->highArray[i] > this->longEntry){
       					this->BuyToCover(abs(this->pos) , closeArray[i] , i);
       					if( 0 == this->longEntered){
       						this->Buy( lots , closeArray[i] , i);
       					}
       				}
       			}
       		}else{
       			if( this->pos > 0){
       				this->Sell(abs(this->pos) , closeArray[i] , i);
       			}else if(this->pos < 0){
       				this->BuyToCover(abs(this->pos) , closeArray[i] , i);
       			}
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
            for( double k1 = 0.01 ; k1 < 1; k1 += 0.08){
            	for( double k2 = 0.01; k2 < 1; k2 += 0.08){
            		vector<double> tmp; tmp.clear();
                    tmp.push_back(k1);
                    tmp.push_back(k2);

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
    }
};
#endif
