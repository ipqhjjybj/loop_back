/**
 * @copyright JiuXi
 * @author ipqhjjybj
 **/

#ifndef __kingKeltnerStrategy
#define __kingKeltnerStrategy

class kingKeltnerStrategy : public ctaTemplate{
public:
	static const int BufferSize = 30;

	/////////////////////////////////////////////////////////////
    //申明参数
    int kkLength ;			//计算通道中值的窗口数
    double kkDev;			//计算通道宽度的偏差
    double trailingPrcnt;   //移动止损
    ///////////////////////////////////////////////////////////////
    //策略变量
    double atrValue;
    double kkUp;
    double kkDown;
    double high_since_entry;
    double low_since_entry;

    vector<double> atrArray;
    vector<double> kkMid;

    kingKeltnerStrategy(){
    	this->setStrategyName("kingKeltnerStrategy");
        ctaTemplate();
    }

    void init(){
    	ctaTemplate::init();
    	kkLength = 11;
    	kkDev = 1.6;
    	trailingPrcnt = 0.8;

    	lots = 1;

    	atrArray.reserve(n + 10);
        kkMid.reserve(n + 10);
    	atrArray.clear();
        kkMid.clear();
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

        vCanshu.push_back("kkLength");
        vCanshu.push_back("kkDev");
        vCanshu.push_back("trailingPrcnt");

        return vCanshu;
    }
    void setCanshuFromVec(const vector<double> &arr){
        this->kkLength 		   = arr[0] ;
        this->kkDev            = arr[1] ;
        this->trailingPrcnt    = arr[2] ;
    }

    TradingResultNode getTradingResult( const char *_code , const char *_period,const char *start_date , const char *end_date){
        TradingResultNode a = TradingResultNode( _code, _period ,start_date, end_date ,tradingResult,tradingNum , account_money ,slippage * n_slippages, rate , perSize , cash_deposit, vecTradeDay , vecTradeWeekDay , dayIncome , weekIncome , bzjAccount, dayAccount, mp,fee_type);
        vector<double> d; d.clear();
        d.push_back(this->kkLength);
        d.push_back(this->kkDev);
        d.push_back(this->trailingPrcnt);
        a.setCanshuSet(d);
        return a;
    }

    void beforeRun(){
        ctaTemplate::beforeRun();

        high_since_entry = 0.0;
        low_since_entry  = 0.0;

        AvgTrueRange(atrArray , highArray , lowArray , closeArray , kkLength , n);
        Average(kkMid , closeArray , kkLength , n);

    }

    void onBar(int i){
        onBar_bef(i);

        double atrValue = atrArray[i];
        double kkUp   = kkMid[i] + atrValue * this->kkDev;
        double kkDown = kkMid[i] - atrValue * this->kkDev;

        if( 0 == this->pos){
            high_since_entry = highArray[i];
            low_since_entry  = lowArray[i];
            if( highArray[i] > kkUp){
                this->Buy( lots , closeArray[i] , i);
            }else if(lowArray[i] < kkDown){
                this->SellShort( lots , closeArray[i] , i);
            }
        }else if( this->pos > 0){
            high_since_entry = max(high_since_entry , highArray[i]);
            low_since_entry = lowArray[i];
            if( lowArray[i] < high_since_entry * ( 1 - trailingPrcnt / 100.0)){
                this->Sell(abs(this->pos) , closeArray[i] , i);
            }
        }else if( this->pos < 0){
            low_since_entry = min(low_since_entry , lowArray[i]);
            high_since_entry = highArray[i];
            if( highArray[i] > low_since_entry * ( 1 + trailingPrcnt / 100.0)){
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
            for( int kkLength = 3; kkLength <= 30; kkLength += 2){
                for( double kkDev = 1.0; kkDev <= 5.0; kkDev += 0.2){
                    for(double trailingPrcnt = 0.4; trailingPrcnt <= 2.0 ; trailingPrcnt += 0.2){
                        vector<double> tmp; tmp.clear();
                        tmp.push_back(kkLength);
                        tmp.push_back(kkDev);
                        tmp.push_back(trailingPrcnt);

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
