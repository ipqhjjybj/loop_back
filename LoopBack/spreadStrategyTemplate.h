#ifndef _spread_template_using
#define _spread_template_using

#include "talib.h"
#include <set>
#include <queue>
#include <map>

#include "TradingResult.h"

class spreadTemplate{

public:
	vector<struct __dataLine> ori_data;

    string cacheStartDate , cacheEndDate;
    vector<__zhuli_data> cacheStructData;
	vector<string> symbolToIndex;
	vector<string > g_datetime , g_date , g_time;
	vector<vector<double> > closeArrayVec , openArrayVec , highArrayVec , lowArrayVec , volArrayVec ;

	vector< struct feeNode > vec_Fee;

	vector< queue<struct tradeNode> > vecQueueTradingUnFinished;
	vector< vector<struct tradeNode> > vecTradingResult;
	vector< int > vecPos;
	vector< int > vecTradingNum;
	vector< double > vec_n_slippages;

    vector<vector<int> > vecMp;
    vector<vector<double> > vecFreeMoney , vecTotalMoney;   // 空闲资金 ,, 总资金 (空闲资金 + 持仓)
    vector<vector<double> > vecDayAccount;                  // 每日空闲资金+持仓的值
    vector<vector<double> > vecDayIncome;                   // 每日赚到的钱
    vector<vector<double> > vecChi_price;                   //持仓价
    vector<vector<double> > vecChu_price;                   //出场价
    vector<vector<double> > vecRu_price;                    //入场价
    vector<vector<double> > vecWeekAccount;                 //
    vector<vector<double> > vecWeekIncome;                  //
    vector<vector<double> > vecBzjAccount;                  //保证金价格
    vector<vector<int> > vecPositionAccount;                //每日的收盘持仓

    vector<int > vecAccountMoney;                   // 账户初始资金
    vector<int > vecLots;                           // 每个交易的手数

    vector<double> vecHighHisotryPrice;             //

    vector<string> vecTradeDay;
    vector<int> jiaoyi_week_num;
    vector<string> vecTradeWeekDay;
    vector<string> vecPchy;

    ////////////////////////

	vector<int> vecBarSinceEntry;					// 是否有持仓
	string strategyName;

    pair<int,string> loopForceCanshu;                   // (loopForceCanshuSize, loopForceCanshuFileName) , 一个是参数的总数，一个是所有需要跑的参数的数量

    vector<int> vec_nn_week;
    int n ;

    // FILE *fp_dd ;

    /**
     *  这里提供一种暴力循环获得 所有参数的例子
     *  本来应该使用递归的方式做的, 但后来想了一下，， 还是用这种来做吧， 毕竟简单，可以添加各种条件约束
     **/
    virtual string getCanshuSetNamesString(){
        vector<string> vCanshu = this->getCanshuSetNames();
        string ret("");
        for(int i = 0;i < vCanshu.size();i++){
            if(i == 0){
                ret = ret + vCanshu[i];
            }else{
                ret = ret + "," + vCanshu[i];
            }
        }
        return ret;
    }
    /**获得参数名字**/
    virtual vector<string> getCanshuSetNames(){
        vector<string> vCanshu; vCanshu.clear();
        return vCanshu;
    }

	/**设置策略名字**/
	virtual void setStrategyName(const string &s){
        this->strategyName = string(s);
    }
    /**获得策略名字**/
    virtual const string& getStrategyName(){
        return this->strategyName;
    }

    /****/
    virtual void loopForce(){

    }
    /**  **/
    virtual void setCanshuFromVec(const vector<double> &arr){

    }
    /** 循环loopforce **/
    virtual const pair<int,string> &getLoopForceInfo(){
        if( 0 == this->loopForceCanshu.first){
            this->loopForce();
        }
        return loopForceCanshu;
    }
	/**清楚数据***/
	virtual void clearAllData(){
		this->vec_Fee.clear();
		this->symbolToIndex.clear();
		this->g_datetime.clear();  this->g_date.clear();   this->g_time.clear();
		this->closeArrayVec.clear();   this->openArrayVec.clear();  this->highArrayVec.clear();  this->lowArrayVec.clear();  this->volArrayVec.clear();
	}

	/** hash 字符串 **/
	virtual string combineHashString(const struct __zhuli_data &d ){
		return string(d.code) + "_" + string(d.zhuliType);
	}

	/***加载文件中的数据**/
	virtual void loadFile(const vector<struct __zhuli_data> &toCombineDataStruct , const string &start_date , const string &end_date ){
		if(toCombineDataStruct.size() > 0){
			vector<double> tmp_open , tmp_high, tmp_low , tmp_close , tmp_volume;
			string filename = string("./data/spread/cache") + "_" + replaceDate(start_date) + "_" + replaceDate(end_date) + "_" + toCombineDataStruct[0].period ;
			this->clearAllData();

            this->cacheStartDate = start_date;
            this->cacheEndDate = end_date;

            this->cacheStructData.clear();
            this->vecHighHisotryPrice.clear();

			for(int i = 0;i < toCombineDataStruct.size(); i++){
				filename = filename + "_" + toCombineDataStruct[i].code ;
				this->vec_Fee.push_back( _config.getCodeFee(toCombineDataStruct[i].code.c_str()));
                if(1 == this->vec_Fee[i].feeType){
                    this->vec_Fee[i].perMoney = this->vec_Fee[i].perMoney / 100.0;
                }
                this->vecHighHisotryPrice.push_back(0.0);

                this->cacheStructData.push_back( toCombineDataStruct[i]);
			}
            filename = filename + ".csv";
			makeSpreadData( this->ori_data ,toCombineDataStruct , filename , start_date.c_str() , end_date.c_str() );

            // printf("\n%d\n", toCombineDataStruct.size() * 1);
			// 加载手续费节点
			this->symbolToIndex.clear();
			for(int i = 0;i < toCombineDataStruct.size(); i++){
			    vector<double> t_open, t_high ,t_low , t_close , t_volume;

                this->openArrayVec.push_back(t_open);
                this->highArrayVec.push_back(t_high);
                this->lowArrayVec.push_back(t_low);
                this->closeArrayVec.push_back(t_close);
                this->volArrayVec.push_back(t_volume);

                this->symbolToIndex.push_back( this->combineHashString(toCombineDataStruct[i]) );
            }

			// 加载初始数据
			this->n = this->ori_data.size();
			for(int i = 0;i < this->n ; i++){
				string tmp_datetime , tmp_date , tmp_time;
				parseSingleLine( this->ori_data[i], tmp_open , tmp_high , tmp_low , tmp_close , tmp_volume , tmp_datetime , tmp_date , tmp_time );
                // printf("loop i = %d tmp_datetime = %s , tmp_date = %s , tmp_time = %s \n" , i , tmp_datetime.c_str() , tmp_date.c_str() , tmp_time.c_str() );
                for(int j = 0;j < tmp_open.size() ;j++){
					this->openArrayVec[j].push_back( tmp_open[j]);
					this->highArrayVec[j].push_back( tmp_high[j]);
					this->lowArrayVec[j].push_back( tmp_low[j]);
					this->closeArrayVec[j].push_back( tmp_close[j]);
					this->volArrayVec[j].push_back( tmp_volume[j]);

                    this->vecHighHisotryPrice[j] = max(this->vecHighHisotryPrice[j] , tmp_close[j]);
				}
				this->g_datetime.push_back( tmp_datetime);
				this->g_date.push_back( tmp_date );
				this->g_time.push_back( tmp_time );
			}

            this->vecTradeDay.clear();  this->vecTradeWeekDay.clear();  this->jiaoyi_week_num.clear();

            int day_week_beg = 0;
            for(int i = 0;i < n;i++){
                if( i == n-1 ||
                   ( i < n-1 && strcmp(this->g_time[i].c_str() , "15:31:00") <= 0 && strcmp(this->g_time[i+1].c_str() , "15:31:00") >= 0) ||
                   ( strcmp(this->g_time[i].c_str() , "15:31:00") <= 0 && strcmp(this->g_time[i].c_str() , this->g_time[i+1].c_str() ) >= 0 ))
                {
                    //printf("g_date %s\n",g_date[i].c_str());
                    this->vecTradeDay.push_back(string(g_date[i]));
                    int day_num;
                    day_num = datenum(this->vecTradeDay[this->vecTradeDay.size()-1].c_str());
                    if (1 == this->vecTradeDay.size())
                    {
                        day_week_beg = day_num - CaculateWeekDayS(g_date[i].c_str());
                    }

                    this->jiaoyi_week_num.push_back((day_num - day_week_beg )/7);
                };
            }
            for (int i = 1 ;i < this->vecTradeDay.size() ;i++)
            {
                if (this->jiaoyi_week_num[i-1] != this->jiaoyi_week_num[i])
                {
                    this->vecTradeWeekDay.push_back(this->vecTradeDay[i-1]);
                }
            }
            if( this->vecTradeDay.size() > 0){
                this->vecTradeWeekDay.push_back(this->vecTradeDay[this->vecTradeDay.size()-1]);
            }
            printf("num of file data: %d\n",this->n);

            // for(int i = 0;i < this->n ;i++){
            //     printf("%lf %lf %lf %lf %lf\n", this->openArrayVec[0][i] , this->highArrayVec[0][i] , this->lowArrayVec[0][i] , this->closeArrayVec[0][i] , this->volArrayVec[0][i]);
            // }
		}
	}
	// 获得 代码的序号
	virtual int getCodeIndex( const struct __zhuli_data &toCombData){
		int ind = -1;
		string combData = this->combineHashString( toCombData);
		for(int i = 0;i < this->symbolToIndex.size();i++){
			if(this->symbolToIndex[i] == toCombData.code){
				return i;
			}
		}
		return ind;
	}

	/**
     * 获得手续费与滑点
     **/
    virtual double getFee(const struct __zhuli_data &combData , int size , double price){
    	int ind = this->getCodeIndex(combData);
    	return this->getFee(ind , size , price);
    }
    /**
     * 获得手续费与滑点
     **/
    virtual double getFee(int ind , int size , double price){
    	struct feeNode fNode= vec_Fee[ind];
    	double n_slippages = vec_n_slippages[ind];
    	if( fNode.feeType == 1){
    		return price * fNode.csize * size * fNode.perMoney + fNode.minMove * fNode.csize * size * n_slippages;
    	}else if(fNode.feeType == 2){
    		return size * fNode.perMoney + fNode.minMove * fNode.csize * size * n_slippages;
    	}
    	/*
        if( fee_type == 1){         // 表示万分之多少的方式
            return price * perSize * size * rate + slippage * perSize * size * n_slippages;
        }else if(fee_type == 2){    // 表示每手多少元的方式 , 这时候rate就是元了
            return size * rate + slippage * perSize * size * n_slippages;
        }
        */
    }

    /**
     *  只获得手续费
     **/
    virtual double getSimpleFee(const struct __zhuli_data &combData , int size , double price){
        int ind = this->getCodeIndex(combData);
        return this->getSimpleFee(ind , size , price);
    }
    /**
     *  只获得手续费
     **/
    virtual double getSimpleFee(int ind , int size , double price){
        struct feeNode fNode= vec_Fee[ind];
        double n_slippages = vec_n_slippages[ind];
        if( fNode.feeType == 1){
            return price * fNode.csize * size * fNode.perMoney ;
        }else if(fNode.feeType == 2){
            return size * fNode.perMoney ;
        }
    }

    /**
     * 只获得滑点
     **/
    virtual double getSimpleSlippage(const struct __zhuli_data &combData , int size , double price){
        int ind = this->getCodeIndex(combData);
        return this->getSimpleSlippage(ind , size , price);
    }
    /**
     * 只获得滑点
     **/
    virtual double getSimpleSlippage(int ind , int size , double price){
        struct feeNode fNode= vec_Fee[ind];
        double n_slippages = vec_n_slippages[ind];
        if( fNode.feeType == 1){
            return fNode.minMove * fNode.csize * size * n_slippages;
        }else if(fNode.feeType == 2){
            return fNode.minMove * fNode.csize * size * n_slippages;
        }
    }

    /**
     * 卖出函数
     **/
    virtual void Sell( const struct __zhuli_data &combData , int size , double price , int iBar){
    	int ind = this->getCodeIndex(combData);
    	return this->Sell( ind , size , price , iBar);
    }
    /**
     * 平仓函数
     **/
    virtual void BuyToCover(const struct __zhuli_data &combData , int size , double price , int iBar){
    	int ind = this->getCodeIndex(combData);
    	return this->BuyToCover( ind , size , price , iBar);
    }
    /**
     * 买的函数
     ***/
    virtual void Buy(const struct __zhuli_data &combData , int size , double price , int iBar){
    	int ind = this->getCodeIndex(combData);
    	return this->Buy( ind , size , price , iBar);
    }
    /**
     * 开空函数
     **/
   	virtual void SellShort(const struct __zhuli_data &combData , int size , double price , int iBar){
   		int ind = this->getCodeIndex(combData);
   		return this->SellShort( ind , size , price , iBar);
   	}
    /**
     * 卖出函数
     **/
    virtual void Sell( int ind , int size , double price , int iBar){
    	if(this->vecPos[ind] > 0){
    		this->vecPos[ind] -= min( size , abs(this->vecPos[ind]));
    		double win_money = 0.0;
    		queue< struct tradeNode > & queue_trading_unFinished = vecQueueTradingUnFinished[ind] ;
    		while( !queue_trading_unFinished.empty() && size > 0){
    			tradeNode * p_tnode = &(queue_trading_unFinished.front());
                tradeNode new_node = tradeNode();

                strcpy( new_node.symbol_code , p_tnode->symbol_code);
                strcpy( new_node.entryTime , p_tnode->entryTime);
                strcpy( new_node.exitTime , g_datetime[iBar].c_str());

                new_node.entryPrice = p_tnode->entryPrice;
                new_node.exitPrice = legalTradePrice(price , -1.0 , vec_Fee[ind].minMove);
                if( abs(p_tnode->volume) <= size){
                    new_node.volume = 1 * abs(p_tnode->volume);
                    size -= abs(p_tnode->volume);
                    queue_trading_unFinished.pop();   // 一笔交易完成了，所以删除掉队列中的未完成的
                }else if( abs(p_tnode->volume) > size ){
                    // 多单太多, 这个节点还没有用完
                    new_node.volume = 1 * size;
                    p_tnode->volume = 1 * ( abs(p_tnode->volume) - size);
                    size = 0;
                }
                if(this->vecPos[ind] == 0){
                	this->vecBarSinceEntry[ind] = 0;
                }

                new_node.turnover = (new_node.exitPrice + new_node.entryPrice) * vec_Fee[ind].csize * fabs(new_node.volume);
                new_node.commission = getSimpleFee( ind, abs(new_node.volume) , price) + getSimpleFee( ind, abs(new_node.volume) , new_node.entryPrice);
                new_node.per_slippage = getSimpleSlippage(ind , abs(new_node.volume) , price) + getSimpleSlippage(ind , abs(new_node.volume) , new_node.entryPrice);
                new_node.pnl = (new_node.exitPrice - new_node.entryPrice) * new_node.volume * vec_Fee[ind].csize - new_node.commission - new_node.per_slippage;
                new_node.bzj = new_node.turnover * vec_Fee[ind].cash_deposit / 100.0;

                vecTradingResult[ind].push_back( new_node);

                vecTradingNum[ind] += 1;
                win_money += (new_node.exitPrice - new_node.entryPrice) * vec_Fee[ind].csize * new_node.volume;
    		}

            vecChu_price[ind][iBar] = legalTradePrice(price , -1.0 , vec_Fee[ind].minMove);
    	}

        // if(ind == 0){
        //     fprintf(fp_dd, "sell %d size %lf price %d iBar\n", size , price , iBar);
        // }
    }
    /**
     * 平仓函数
     **/
    virtual void BuyToCover(int ind , int size , double price , int iBar){
    	if(this->vecPos[ind] < 0){
    		this->vecPos[ind] += min( size , abs(this->vecPos[ind]));
    		double win_money = 0.0;
    		queue< struct tradeNode > & queue_trading_unFinished = vecQueueTradingUnFinished[ind];
    		while(!queue_trading_unFinished.empty() && size > 0){
    			tradeNode * p_tnode = &(queue_trading_unFinished.front());
                tradeNode new_node = tradeNode();

                strcpy( new_node.symbol_code , p_tnode->symbol_code);
                strcpy( new_node.entryTime , p_tnode->entryTime);
                strcpy( new_node.exitTime , g_datetime[iBar].c_str());

                new_node.entryPrice = p_tnode->entryPrice;
                new_node.exitPrice = legalTradePrice(price , 1.0 , vec_Fee[ind].minMove);
                if( abs(p_tnode->volume) <= size){
                    new_node.volume = -1 * abs(p_tnode->volume);
                    size -= abs(p_tnode->volume);
                    queue_trading_unFinished.pop();   // 一笔交易完成了，所以删除掉队列中的未完成的
                }else if( abs(p_tnode->volume) > size){
                    // 空单太多, 这个节点还没有用完
                    new_node.volume = -1 * size;
                    p_tnode->volume = -1 * ( abs(p_tnode->volume) - size);
                    size = 0;
                }

                if(this->vecPos[ind] == 0){
                    this->vecBarSinceEntry[ind] = 0;
                }

                // new added
                new_node.turnover = (new_node.exitPrice + new_node.entryPrice) * vec_Fee[ind].csize * fabs(new_node.volume);
                new_node.commission = getSimpleFee( ind, abs(new_node.volume) , price) + getSimpleFee( ind, abs(new_node.volume) , new_node.entryPrice);
                new_node.per_slippage = getSimpleSlippage(ind , abs(new_node.volume) , price) + getSimpleSlippage(ind , abs(new_node.volume) , new_node.entryPrice);
                new_node.pnl = (new_node.exitPrice - new_node.entryPrice) * new_node.volume * vec_Fee[ind].csize - new_node.commission - new_node.per_slippage;
                new_node.bzj = new_node.turnover * vec_Fee[ind].cash_deposit / 100.0;
                
                // end
                vecTradingResult[ind].push_back( new_node);
                vecTradingNum[ind] += 1;
                win_money += (new_node.exitPrice - new_node.entryPrice) * vec_Fee[ind].csize * new_node.volume;
    		}
            vecFreeMoney[ind][iBar] += size * price - getFee( ind, min(size,abs(this->vecPos[ind])) , price) + win_money;
    		if(this->vecPos[ind] == 0){
    			this->vecBarSinceEntry[ind] = 0;
    		}
            vecChu_price[ind][iBar] = legalTradePrice(price , 1.0 , vec_Fee[ind].minMove);
    	}
    }
    /**
     * 买的函数
     ***/
    virtual void Buy(int ind , int size , double price , int iBar){
        int pos_res = vecPos[ind];
    	if( this->vecPos[ind] <= 0){
    		this->vecBarSinceEntry[ind] = 0;
    	}
    	if( this->vecPos[ind] < 0){
    		this->BuyToCover(ind , abs(this->vecPos[ind]) , price , iBar);
    	}
    	this->vecPos[ind] += size;
    	tradeNode new_node = tradeNode();
    	strcpy( new_node.entryTime , g_datetime[iBar].c_str());
        strcpy( new_node.symbol_code  ,  this->symbolToIndex[ind].c_str() );

    	new_node.volume = size;
    	new_node.entryPrice = legalTradePrice(price, 1.0 , vec_Fee[ind].minMove);

        // new added
        new_node.commission = getSimpleFee( ind, abs(new_node.volume) , price);
        new_node.per_slippage += getSimpleSlippage(ind , abs(new_node.volume) , price);
        // end

    	this->vecQueueTradingUnFinished[ind].push( new_node);

        vecRu_price[ind][iBar] = legalTradePrice(price, 1.0 , vec_Fee[ind].minMove);
        if(pos_res <= 0){
            vecChi_price[ind][iBar] = legalTradePrice(price, 1.0 , vec_Fee[ind].minMove);
        }else{
            vecChi_price[ind][iBar] = (vecChi_price[ind][iBar] * fabs(pos_res) + legalTradePrice(price, 1.0 , vec_Fee[ind].minMove) *  fabs(size) ) / fabs(this->vecPos[ind]);
        }

        // if(ind == 0){
        //     fprintf(fp_dd, "buy %d size %lf price %d iBar\n", size , price , iBar);
        // }
    }
    /**
     * 开空函数
     **/
   	virtual void SellShort(int ind , int size , double price , int iBar){
        int pos_res = this->vecPos[ind];
   		if( this->vecPos[ind] >= 0){
   			this->vecBarSinceEntry[ind] = 0;
   		}
   		if( this->vecPos[ind] > 0){
   			this->Sell( ind , abs(this->vecPos[ind]) , price , iBar);
   		}
   		this->vecPos[ind] -= size;
   		tradeNode new_node = tradeNode();
   		strcpy( new_node.entryTime , g_datetime[iBar].c_str());
        strcpy( new_node.symbol_code  ,  this->symbolToIndex[ind].c_str() );

   		new_node.volume = -size;
        new_node.entryPrice = legalTradePrice(price, -1.0 , vec_Fee[ind].minMove);

        // new added
        new_node.commission = getSimpleFee( ind, abs(new_node.volume) , price);
        new_node.per_slippage += getSimpleSlippage(ind , abs(new_node.volume) , price);
        // end

        this->vecQueueTradingUnFinished[ind].push( new_node);

        vecRu_price[ind][iBar] = legalTradePrice(price, -1.0 , vec_Fee[ind].minMove);
        if(pos_res>=0){
            vecChi_price[ind][iBar] = legalTradePrice(price, -1.0 , vec_Fee[ind].minMove);
        }else{
            vecChi_price[ind][iBar] = (vecChi_price[ind][iBar] * fabs(pos_res) + legalTradePrice(price, -1.0 , vec_Fee[ind].minMove) );
        }
   	}

    // 获得结果集
    virtual struct mergeAllTradingResult getTradingResultList( vector<TradingResultNode> &tradingResultVec ){
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
            
            // debug_tradingResult_spread(  &tradingResultVec[i] , (string("test_report_") + string(this->symbolToIndex[i]) + ".csv").c_str() , "test" );
            // makeSpreadSingleReport( tradingResultVec[i] , (string("test_single_report_") + string(this->symbolToIndex[i]) + ".csv").c_str() , "test" );
        }

        return mergeAllTradingResult(tradingResultVec);
        // struct mergeAllTradingResult ret = mergeAllTradingResult(tradingResultVec);
        // ret.
        // return ;
    }

    // 更新barsEntry 这个东西
    virtual void updateBarsEntry(int ind , int iBar){
        if(vecPos[ind] != 0){
            vecBarSinceEntry[ind] += 1;
        }
        if( vecMp[ind][max(0,iBar-1)] != 0 && 0 == vecMp[ind][iBar]){
            vecBarSinceEntry[ind] = 0;
        }
    }

    /**初始化数据**/
    virtual void init(){
    	this->vec_n_slippages.clear();

        this->vecAccountMoney.clear();
        this->vecLots.clear();

        for(int i = 0;i < this->symbolToIndex.size();i++){
            this->vecAccountMoney.push_back(200000.0);
            this->vecLots.push_back(1);
        }

        ///////
        this->vecMp.clear();
        this->vecFreeMoney.clear(); this->vecTotalMoney.clear();
        this->vecDayAccount.clear(); this->vecDayIncome.clear();
        this->vecChi_price.clear(); this->vecChu_price.clear();
        this->vecRu_price.clear();  this->vecWeekAccount.clear();
        this->vecWeekIncome.clear();  this->vecBzjAccount.clear();
        this->vecPositionAccount.clear();

        for(int i = 0;i < this->symbolToIndex.size(); i++){
            vector<int> mp , positionAccount;
            vector<double> free_money , dayAccount , dayIncome, chi_price , chu_price , ru_price , total_money , weekAccount , weekIncome, bzjAccount ;

            mp.clear();  free_money.clear();  dayAccount.clear(); dayIncome.clear();  chi_price.clear(); chu_price.clear(); ru_price.clear(); total_money.clear();
            weekAccount.clear();  weekIncome.clear();  bzjAccount.clear();  positionAccount.clear();

            mp.reserve( this->n + 10); free_money.reserve( this->n + 10);   dayAccount.reserve( this->n + 10); dayIncome.reserve( this->n + 10); chi_price.reserve( this->n + 10);
            chu_price.reserve( this->n + 10);  ru_price.reserve( this->n + 10); total_money.reserve( this->n + 10); weekAccount.reserve( this->n + 10); weekIncome.reserve( this->n + 10);
            bzjAccount.reserve( this->n + 10); positionAccount.reserve( this->n + 10);

            vecMp.push_back(mp); vecFreeMoney.push_back(free_money); vecDayAccount.push_back(dayAccount); vecDayIncome.push_back(dayIncome); vecChi_price.push_back(chi_price);
            vecChu_price.push_back(chu_price); vecRu_price.push_back(ru_price); vecTotalMoney.push_back(total_money);  vecWeekAccount.push_back(weekAccount); vecWeekIncome.push_back(weekIncome);
            vecBzjAccount.push_back(bzjAccount);  vecPositionAccount.push_back( positionAccount);
        }
        //printf("size %d\n",this->vecDayAccount.size());
    }
    /**
     *  运行前函数
     **/
    virtual void beforeRun(){
        // DEBUG
        // fp_dd = fopen("out.csv", "w");
        // printf("1\n");
    	this->vecTradingResult.clear();
    	this->vecQueueTradingUnFinished.clear();
    	this->vecTradingNum.clear();
    	this->vecPos.clear();
    	this->vecBarSinceEntry.clear();

        this->vec_nn_week.clear();
        //////
        // printf("2\n");
    	for(int i = 0;i < this->symbolToIndex.size();i++){
    		queue<struct tradeNode> q_trade;
    		vector<struct tradeNode> v_tradingResult;

    		this->vecQueueTradingUnFinished.push_back( q_trade);
    		this->vecTradingResult.push_back( v_tradingResult );
    		this->vec_n_slippages.push_back( 1.0 );
    		this->vecTradingNum.push_back( 0 );
    		this->vecPos.push_back( 0);
    		this->vecBarSinceEntry.push_back(0);

            this->vec_nn_week.push_back(0);
    	}
        for(int i = 0;i < this->symbolToIndex.size();i++){
            // this->vecChi_price[i][0] = 0;
            this->vecChi_price[i].push_back(0);
            // this->vecChu_price[i][0] = 0;
            this->vecChu_price[i].push_back(0);
            // this->vecRu_price[i][0] = 0;
            this->vecRu_price[i].push_back(0);
            // this->vecFreeMoney[i][0] = vecAccountMoney[i];
            this->vecFreeMoney[i].push_back(vecAccountMoney[i]);
            // this->vecTotalMoney[i][0] = vecAccountMoney[i];
            this->vecTotalMoney[i].push_back(vecAccountMoney[i]);

            this->vecMp[i].push_back(0);
        }
    }

    /**
     *  运行结束后函数
     **/
    virtual void afterRun(){
        if(this->n > 0){
            for(int ind = 0;ind < this->symbolToIndex.size() ; ind++){
                queue<struct tradeNode> &queue_trading_unFinished = vecQueueTradingUnFinished[ind];
                vector<struct tradeNode> &tradingResult = vecTradingResult[ind];
                vector<double> &dayIncome = vecDayIncome[ind];
                vector<double> &dayAccount = vecDayAccount[ind];
                vector<double> &weekIncome = vecWeekIncome[ind];
                vector<int> &positionAccount = vecPositionAccount[ind];
                vector<double> &closeArray = closeArrayVec[ind];

                double &minMovement = vec_Fee[ind].minMove;
                int &tradingNum = vecTradingNum[ind];
                int &pos = vecPos[ind];

                while( !queue_trading_unFinished.empty()){
                    tradeNode * p_tnode = &(queue_trading_unFinished.front());
                    tradeNode new_node = tradeNode();

                    strcpy( new_node.symbol_code , p_tnode->symbol_code);
                    strcpy( new_node.entryTime , p_tnode->entryTime);
                    strcpy( new_node.exitTime , g_datetime[n-1].c_str());
                    
                    new_node.entryPrice = p_tnode->entryPrice;
                    if( pos < 0){
                        new_node.exitPrice = legalTradePrice( closeArray[n-1] , 1.0 , minMovement);
                    }else if(pos > 0){
                        new_node.exitPrice = legalTradePrice( closeArray[n-1] , -1.0 , minMovement);
                    }
                    new_node.volume = abs(p_tnode->volume);

                    new_node.turnover = (new_node.exitPrice + new_node.entryPrice) * vec_Fee[ind].csize * fabs(new_node.volume);
                    new_node.commission = getSimpleFee( ind, abs(new_node.volume) , new_node.exitPrice) + getSimpleFee( ind, abs(new_node.volume) , new_node.entryPrice);
                    new_node.per_slippage = getSimpleSlippage(ind , abs(new_node.volume) , new_node.exitPrice) + getSimpleSlippage(ind , abs(new_node.volume) , new_node.entryPrice);
                    new_node.pnl = (new_node.exitPrice - new_node.entryPrice) * new_node.volume * vec_Fee[ind].csize - new_node.commission - new_node.per_slippage;
                    new_node.bzj = new_node.turnover * vec_Fee[ind].cash_deposit / 100.0;

                    tradingResult.push_back( new_node );
                    tradingNum += 1;
                    queue_trading_unFinished.pop();
                }

                double tmp_inc_fee = getFee(ind, fabs(pos) , closeArray[n-1]);
                if( dayIncome.size() > 0){
                    dayIncome[dayIncome.size() - 1] = dayIncome[dayIncome.size() - 1] - tmp_inc_fee;
                }
                if( weekIncome.size() > 0){
                    weekIncome[weekIncome.size() - 1] = weekIncome[weekIncome.size() - 1] - tmp_inc_fee;
                }
                if( dayAccount.size() > 0){
                    dayAccount[dayAccount.size() - 1] = dayAccount[dayAccount.size() - 1] - tmp_inc_fee;
                }
                if(positionAccount.size() > 0){
                    positionAccount[positionAccount.size() - 1] = 0;
                }
                pos = 0;
            }
        }

        // debug
        // fclose(fp_dd);
    }

    /**
     *  每根 bar 线之前执行的函数
     **/
    virtual void onBar_bef( int i){
        if(i > 0){
            // printf("start onBar_bef i = %d\n", i);
            for(int ind = 0;ind < this->symbolToIndex.size() ; ind++){
                // printf("this->vecMp[%d].size %d\n", ind , this->vecMp[ind].size());
                // printf("this->vecMp[ind][i-1] %d\n",this->vecMp[ind][i-1]);
                // this->vecMp[ind][i] = this->vecMp[ind][i-1];
                this->vecMp[ind].push_back(this->vecMp[ind][i-1]);
                // this->vecChi_price[ind][i] = this->vecChi_price[ind][i-1];
                this->vecChi_price[ind].push_back(this->vecChi_price[ind][i-1]);
                // this->vecRu_price[ind][i] = this->vecRu_price[ind][i-1];
                this->vecRu_price[ind].push_back(this->vecRu_price[ind][i-1]);
                // this->vecChu_price[ind][i] = this->vecChu_price[ind][i-1];
                this->vecChu_price[ind].push_back(this->vecChu_price[ind][i-1]);
                // this->vecFreeMoney[ind][i] = this->vecFreeMoney[ind][i-1];
                this->vecFreeMoney[ind].push_back(this->vecFreeMoney[ind][i-1]);
                // this->vecTotalMoney[ind][i] = this->vecTotalMoney[ind][i-1];
                this->vecTotalMoney[ind].push_back(this->vecTotalMoney[ind][i-1]);

                this->updateBarsEntry(ind , i);

                // printf("%d\n",i);
            }
            // printf("end onBar_bef i = %d\n", i);
        }
    }

    /**
     *  每根 bar 线之后执行的函数
     **/
    virtual void onBar_after( int i){

        for(int ind = 0; ind < this->symbolToIndex.size() ;ind++){
            vector<int> &mp = vecMp[ind];
            vector<double> &chi_price = vecChi_price[ind];
            vector<double> &chu_price = vecChu_price[ind];
            vector<double> &closeArray = closeArrayVec[ind];
            vector<double> &total_money = vecTotalMoney[ind];
            vector<double> &free_money = vecFreeMoney[ind];
            vector<double> &dayAccount = vecDayAccount[ind];
            vector<double> &bzjAccount = vecBzjAccount[ind];
            vector<double> &dayIncome = vecDayIncome[ind];
            vector<double> &weekAccount = vecWeekAccount[ind];
            vector<double> &weekIncome = vecWeekIncome[ind];
            vector<int> &positionAccount = vecPositionAccount[ind];
            vector<double> &ru_price = vecRu_price[ind];

            int &nn_week = vec_nn_week[ind];
            int &pos = vecPos[ind];
            double perSize = vec_Fee[ind].csize;
            double cash_deposit = vec_Fee[ind].cash_deposit;
            double account_money = vecAccountMoney[ind];

            double kai_huan = 0.0;
            double ping_huan = 0.0;

            mp[i] = pos;

            if (0 == mp[i])
            {
                chi_price[i] = 0;
            }
            if ((0 !=mp[i] && mp[i] == mp[i-1] ) || ( 0 != mp[i] && mp[i] * mp[i-1] >0 && fabs(mp[i]) < fabs(mp[i-1])))
            {
                chi_price[i] = closeArray[i-1];
            }
            if (( 0 != mp[i] &&  mp[i] == mp[i-1] ) || (0 != mp[i] && 0 == mp[i-1]) || ( 0 != mp[i] && mp[i] * mp[i-1] >0 && fabs(mp[i]) > fabs(mp[i-1])))
            {
                chu_price[i] = closeArray[i];
            }
            if (0 == mp[i] && 0 == mp[i-1])
            {
                chu_price[i] = 0;
            }

            double fuying_1;
            double fei_1;
            double bzj_1;

            fuying_1 = 0.0;
            fei_1 = 0.0;
            bzj_1 = 0.0;
            if( ( 0 != mp[i] && mp[i] == mp[i-1]) || (0 != mp[i] && 0 == mp[i-1]) || ( 0 != mp[i] && mp[i] * mp[i-1] >0 ))//持仓中 or  新开仓 or 加仓 or 减仓
            {
                //fuying_1 = (chu_price[i] - chi_price[i]) * mp[i] * perSize + (ping_huan - kai_huan) * mp[i] * perSize;
                fuying_1 = (chu_price[i] - chi_price[i]) * mp[i] * perSize;
                if(0 == mp[i-1])//新开仓
                {
                    fei_1 = getFee(ind , fabs(mp[i]),chi_price[i]);
                }
                else
                {
                    if( kai_huan > 0 )
                    {
                        if (mp[i] == mp[i-1])//持仓中
                        {
                            fei_1 = getFee( ind , fabs(mp[i]) , kai_huan) + getFee( ind , fabs(mp[i]), ping_huan);
                            fuying_1 = (closeArray[i] - closeArray[i-1]) * mp[i] * perSize + (ping_huan - kai_huan) * mp[i] * perSize;
                        }
                        else
                        {
                            if (fabs(mp[i]) > fabs(mp[i-1]))//加仓
                            {
                                fei_1 = getFee(ind , fabs(mp[i-1]) , kai_huan) + getFee(ind , fabs(mp[i-1]), ping_huan) + getFee(ind , fabs(mp[i-1]-mp[i]), ru_price[i]);
                                fuying_1 = (closeArray[i] - closeArray[i-1]) * mp[i-1] * perSize + (ping_huan - kai_huan) * mp[i-1] * perSize + (closeArray[i] - ru_price[i]) * (mp[i]-mp[i-1]) * perSize ;
                            }
                            else//减仓
                            {
                                fei_1 = getFee(ind , fabs(mp[i-1]) , kai_huan) + getFee(ind , fabs(mp[i-1]), ping_huan) + getFee(ind , fabs(mp[i-1]-mp[i]), chu_price[i]);
                                fuying_1 = (ping_huan - closeArray[i-1]) * mp[i-1] * perSize + (closeArray[i] - kai_huan) * mp[i] * perSize + (chu_price[i] - kai_huan) * (mp[i-1]-mp[i]) * perSize ;
                            }

                        }

                    }
                    else
                    {
                        if (mp[i] == mp[i-1])//持仓中
                        {
                            fei_1 = 0;
                        }
                        else
                        {
                            if (fabs(mp[i]) > fabs(mp[i-1]))//加仓
                            {
                                fei_1 = getFee(ind, fabs(mp[i-1]-mp[i]), ru_price[i]);
                            }
                            else//减仓
                            {
                                fei_1 = getFee(ind , fabs(mp[i-1]-mp[i]), chu_price[i]);
                                fuying_1 = (closeArray[i] - chi_price[i]) * mp[i] * perSize + (chu_price[i] - chi_price[i]) * (mp[i-1]-mp[i]) * perSize ;
                            }

                        }

                    }
                }
            }
            if (0 != mp[i] && 0!=mp[i-1] && mp[i] * mp[i-1]<0)//反手开仓
            {
                if( kai_huan > 0)
                {
                    fuying_1 = (ping_huan - closeArray[i-1]) * mp[i-1] * perSize + (chu_price[i] - kai_huan) * mp[i-1] * perSize  + (closeArray[i] - chi_price[i]) * mp[i] * perSize;
                    fei_1 = getFee(ind, fabs(mp[i]),chi_price[i]) + getFee(ind, fabs(mp[i-1]),chu_price[i]) + getFee(ind, fabs(mp[i-1]),kai_huan) +getFee(ind, fabs(mp[i-1]),ping_huan);
                }
                else
                {
                    fuying_1 = (chu_price[i] - closeArray[i-1]) * mp[i-1] *perSize + (closeArray[i] - chi_price[i]) * mp[i] * perSize;
                    fei_1 = getFee(ind, fabs(mp[i]),chi_price[i]) + getFee(ind, fabs(mp[i-1]),chu_price[i]);
                }
            }
            if (0 == mp[i] && 0 != mp[i-1])//平仓
            {
                if( kai_huan > 0)
                {
                    fuying_1 = (ping_huan - closeArray[i-1]) * mp[i-1] * perSize + (chu_price[i] - kai_huan) * mp[i-1] * perSize;
                    fei_1 = getFee(ind, fabs(mp[i-1]),ping_huan) + getFee(ind, fabs(mp[i-1]),kai_huan) + getFee(ind, fabs(mp[i-1]),chu_price[i]);
                }
                else
                {
                    fuying_1 = (chu_price[i] - closeArray[i-1]) * mp[i-1] * perSize;
                    fei_1 = getFee(ind, fabs(mp[i-1]),chu_price[i]);
                }

            }
            if (0 == mp[i] && 0 == mp[i-1])//无持仓
            {
                fuying_1 = 0;
                fei_1 = 0;
            }

            if (0 != mp[i] || ( 0 == mp[i] && 0 != mp[i-1]))
            {
                bzj_1 = closeArray[i] * perSize * max(fabs(mp[i]),fabs(mp[i-1])) * cash_deposit * 1.0/100;
            }
            else
            {
                bzj_1 = 0;
            }

            total_money[i] = total_money[i] + fuying_1 - fei_1;
            free_money[i] = total_money[i] - bzj_1;

            // if( ind == 0){
            //     fprintf( fp_dd , "abc %d , %s , %lf , %d, %lf ,%lf , %lf , %lf , %lf , %lf , %lf , %lf\n", i ,g_datetime[i].c_str(), closeArray[i],mp[i], chi_price[i] , ru_price[i], chu_price[i] , fuying_1 , fei_1  , kai_huan , ping_huan , total_money[i]);
            // }
            //pair<int , string> bar_endtime = Bar_end_time(this->ttperiod);

            // printf("after 33 %s %d %s\n",g_datetime[i].c_str() , bar_endtime.first , bar_endtime.second.c_str());
            //if( 0 == strcmp(g_time[i].c_str() , bar_endtime.second.c_str() ) )

            if( i == n-1 ||
              ( i < n-1 && strcmp(g_time[i].c_str() , "15:31:00") <= 0 && strcmp(g_time[i+1].c_str() , "15:31:00") >= 0) ||
              ( strcmp(g_time[i].c_str() , "15:31:00") <= 0 && strcmp(g_time[i].c_str() , g_time[i+1].c_str() ) >= 0 ))
            {

                dayAccount.push_back(total_money[i]);
                bzjAccount.push_back(bzj_1);                     // 保留保证金的数据
                positionAccount.push_back(pos);                  // 保留每日的持仓数据
                if (1 == dayAccount.size())
                {
                    dayIncome.push_back(total_money[i]-account_money);
                }
                else
                {
                    dayIncome.push_back(dayAccount[dayAccount.size()-1]-dayAccount[dayAccount.size()-2]);
                }

                // if( ind == 0){
                //     printf("fuck fuckfuckfuckfuckfuckfuckfuckfuckfuck\n\n\n");
                //     fprintf( fp_dd , "%d , %s , %lf \n", i ,g_datetime[i].c_str(), dayIncome[dayIncome.size()-1]);
                // }
                //printf("%d , %s, %lf , %lf \n", i , g_date[i].c_str() , dayAccount[dayAccount.size()-1], dayIncome[dayIncome.size()-1]);

                // printf("g_date[%d]=%s nn_week=%d vecTradeDay.size %d %s\n",i,g_date[i].c_str(),nn_week , vecTradeDay.size() , vecTradeWeekDay[nn_week].c_str());

                // if(nn_week >= vecTradeWeekDay.size()){
                //     printf("duo yi zhou %s tradeweek %s\n",g_date[i].c_str() , vecTradeWeekDay[vecTradeWeekDay.size()-1].c_str());
                //     printf("%s--%s\n",vecTradeDay[0].c_str(),vecTradeDay[vecTradeDay.size()-1].c_str());
                // }
                if ( nn_week < vecTradeWeekDay.size() && 0 == strcmp(g_date[i].c_str() , vecTradeWeekDay[nn_week].c_str() ))
                {
                    // printf("total_money[%d]=%lf\n",i,total_money[i]);
                    weekAccount.push_back(total_money[i]);

                    if (0 == nn_week)
                    {
                        weekIncome.push_back(total_money[i]-account_money);
                    }
                    else
                    {
                        weekIncome.push_back(weekAccount[weekAccount.size()-1]-weekAccount[weekAccount.size()-2]);
                    }
                    nn_week +=1;
                    //printf("%d , %s, %lf , %lf \n", i , g_date[i].c_str() , weekAccount[weekAccount.size()-1], weekIncome[weekIncome.size()-1]);
                }
                //printf("%d\n", i);
            }
        }
    }

    /**
     *  每根 bar 时执行的函数
     **/
    virtual void onBar(int i){

    }

    /**
     *  需要重载的函数
        总运行过程
     **/
    virtual void run(){
        this->beforeRun();

        this->afterRun();
    }

};



#endif
