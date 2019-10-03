
#ifndef _SpreadTestStrategy
#define _SpreadTestStrategy

#include "../load_config.h"
#include "../spreadStrategyTemplate.h"

/**
 *  策略说明， 对两个品种， 假如这两个品种的价差为M , 如果价差> canshu_1 ,持有A品种， 价差 < canshu_1 ，持有B品种
 *
 **/
class SpreadTestStrategy : public spreadTemplate{
public:

	double canshu_1;
    
	SpreadTestStrategy(){
		this->setStrategyName("spreadTestStrategy");
	}
	void init(){
		spreadTemplate::init();
	}
	vector<string> getCanshuSetNames(){
		vector<string> vCanshu; vCanshu.clear();
		return vCanshu;
	}
	void setCanshuFromVec(const vector<double> &arr){
		this->canshu_1 = arr[0];
	}

	// TradingResultNode getTradingResult(){

	// }

	void onBar_bef(int i){
        spreadTemplate::onBar_bef(i);
    }

    void onBar_after(int i){
        spreadTemplate::onBar_after(i);
    }

    void onBar(int i){
        onBar_bef(i);

        double jiacha = this->closeArrayVec[0][i] - this->closeArrayVec[1][i];

        // printf("why %d\n",i);
        if(jiacha > this->canshu_1){
            // hold A
            if(this->vecPos[0] < 1){
                if(this->vecPos[0] < 0){
                    this->BuyToCover(0 , abs(this->vecPos[0]) , this->closeArrayVec[0][i] , i);
                }
                this->Buy( 0 , 1 , this->closeArrayVec[0][i] , i);
            }
            if(this->vecPos[1] > 0){
                this->Sell( 1 , abs(this->vecPos[1]) , this->closeArrayVec[1][i] , i);
            }
        }else{
            // hold B
            if(this->vecPos[1] < 1){
                if(this->vecPos[1] < 0){
                    this->BuyToCover(1 , abs(this->vecPos[1]) , this->closeArrayVec[1][i] , i);
                }
                this->Buy( 1 , 1 , this->closeArrayVec[1][i] , i);
            }
            if( this->vecPos[0] > 0){
                this->Sell( 0 , abs(this->vecPos[0]) , this->closeArrayVec[0][i] , i);
            }
        }
        // printf("closeA %lf , closeB %lf , jiacha %lf , A_pos %d , B_pos %d , target %lf\n", this->closeArrayVec[0][i] , this->closeArrayVec[1][i] , jiacha , this->vecPos[0] , this->vecPos[1] , this->canshu_1 );
        onBar_after(i);
    }

    void beforeRun(){
    	spreadTemplate::beforeRun();
    }

    virtual void loopForce(){
    	FILE *fp = NULL;
        string loopForceFileName = string("./runningTEMP/") + this->strategyName + ".csv";
        printf("loopForceFileName:%s\n",loopForceFileName.c_str());
        int t_canshuNum = 0;

        if((fp = fopen(loopForceFileName.c_str(), "w")) != NULL){
        	//for(double canshu_1 = 42600.0 ; canshu_1 < 42610.0; canshu_1 += 1){
        	for(double canshu_1 = 42600.0 ; canshu_1 < 42600.5; canshu_1 += 1){
            	vector<double> tmp; tmp.clear();
        		tmp.push_back(canshu_1);
        		for(int t_ji = 0; t_ji < tmp.size(); t_ji++){
                    if(0 == t_ji){
                        fprintf(fp,"%lf",tmp[t_ji]);
                    }else{
                        fprintf(fp,",%lf",tmp[t_ji]);
                    }
                }fprintf(fp,"\n");

                t_canshuNum = tmp.size();
        	}
        	fclose(fp);
            this->loopForceCanshu = make_pair( t_canshuNum , loopForceFileName );
        }else{
        	printf("openfile , loopForce error!\n");
        }
    }
    // 获得结果集
    struct mergeAllTradingResult getTradingResultList( vector<TradingResultNode> &tradingResultVec ){
        tradingResultVec.clear();
        for(int i = 0;i < this->symbolToIndex.size(); i++){
            //struct __dataLine &d = this->cacheStructData[i];
            // all_code += this->cacheStructData[i].code + "_";
            // all_period += this->cacheStructData[i].period + "_";
            // all_start_date = this->cacheStartDate;
            // all_end_date = this->cacheEndDate;
            // printf("loop debug , %d , %s\n", i , this->symbolToIndex[i].c_str());

            tradingResultVec.push_back( TradingResultNode(this->cacheStructData[i].code.c_str() , this->cacheStructData[i].period.c_str() , this->cacheStartDate.c_str() , this->cacheEndDate.c_str() , this->vecTradingResult[i] ,
                                                           this->vecTradingNum[i] , this->vecAccountMoney[i] ,  this->vec_n_slippages[i] * this->vec_Fee[i].minMove , this->vec_Fee[i].perMoney ,
                                                            this->vec_Fee[i].csize , this->vec_Fee[i].cash_deposit , this->vecTradeDay , this->vecTradeWeekDay , this->vecDayIncome[i] ,
                                                            this->vecWeekIncome[i], this->vecBzjAccount[i] , this->vecDayAccount[i] , this->vecPositionAccount[i] , this->vec_Fee[i].feeType) );
            

            // printf("tradingResultVec[%d].saveDayIncome.size() %d\n",i , tradingResultVec[i].saveDayIncome.size());
        }
        struct mergeAllTradingResult ret = mergeAllTradingResult(tradingResultVec);
        vector<double> d; d.clear();
        d.push_back(this->canshu_1);
        ret.setCanshuSet(d);

        // printf("mergeAllTradingResult tot_saveDayIncome.size() %d\n", ret.tot_saveDayIncome.size());
        return ret;
    }

    void run(){
    	this->beforeRun();
    	for( int i = 1; i < n;i++){ // i = BufferSize - 1
            onBar(i);
        }
        this->afterRun();
    }

    void afterRun(){
        spreadTemplate::afterRun();
    }

};

#endif // _SpreadTestStrategy
