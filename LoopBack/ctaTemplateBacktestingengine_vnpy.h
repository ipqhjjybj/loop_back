#ifndef _cta_template_vnpy_backtestingEngine
#define _cta_template_vnpy_backtestingEngine


#include "ctaTemplateTrading.h"
#include "DataLoader.h"
#include "talib.h"

class BacktestingEngine{

    //CTA回测引擎
    //函数接口和策略引擎保持一样，
    //从而实现同一套代码从回测到实盘。
    
    int stopOrderCount ;
    vector<struct Order> vecStopOrders;		 
    vector<struct Order> vecWorkingStopOrders;

    int limitOrderCount;
    vector<struct Order> vecLimitOrders;
    vector<struct Order> vecWorkingLimitOrders;

    CtaTemplateVnpyTrading *p_strategy;

    string startDate;
    string endDate;
    int initDays;

    DataLoader dbClient;

    vector<struct tradeNode> tradingResult;

    vector<string> logList;

    Bar bar;

    BacktestingEngine(){
    	this->stopOrderCount = 0;
    	this->vecStopOrders.clear();  				  // 停止单撤销后不会从本字典中删除
    	this->vecWorkingStopOrders.clear();		  //停止单撤销后会从本字典中删除
    	this->p_strategy = NULL;

    	this->startdate = "";
    	this->endDate   = "";
    	this->initDays  = "";

    	this->capital   = 1000000;				//回测初始资金
    	this->slippage  = 0;					//回测时假设的滑点
    	this->rate      = 0;					//回测时假设的佣金比例（适用于百分比佣金）
    	this->size      = 1;				    //合约大小,默认为1
    	this->priceTick = 0;				 	//价格最小变动

    	this->dbClient  = NULL;					//数据库加载器

    	this->limitOrderCount = 0               //限价单编号
    	this->vecLimitOrders.clear();			//限价单数组
    	this->vecWorkingLimitOrders.clear();    //活动限价单字典，用于进行撮合用

    	this->tradingResult.clear();

    	this->logList.clear();					//日志记录
    }

    void initStrategy( CtaTemplateVnpyTrading *strategyTrading ){
    	this->p_strategy = strategyTrading;
    }

    void runBacktesting(){
    	printf("Start loopback\n");
    	// 需要先用初始数据初始化
    	// self.loadHistoryData()
    	this->p_strategy->inited = 1;
    	this->p_strategy->onInit();
    	printf("Strategy init success!\n");
    	
    }
}
#endif