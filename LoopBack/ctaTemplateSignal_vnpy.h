#ifndef _cta_template_vnpy_signal
#define _cta_template_vnpy_signal

#include "talib.h"


class CtaTemplateVnpySignal{
public:
	//策略的基本参数

	int inited;		 	//是否进行了初始化
	int signal;			//当前策略信号


	string strategyName;//策略名字
	string vtSymbol;	//交易的合约
	string name;		//策略信号实例名称

	CtaTemplateVnpySignal( ){
		// 初始化实例
		this->signal   = 0;
	}

	virtual void setStrategyName(const string &s){
		this->strategyName = s;
	}

	virtual void onStart(){
		//启动策略（必须由用户继承实现）
	}

	virtual void onStop(){
		//停止策略（必须由用户继承实现）
	}

	virtual void onTick(){
		//收到行情TICK推送（必须由用户继承实现）
	}

	virtual void onBar(){
		//收到Bar推送（必须由用户继承实现）
	}

	virtual void getSignal(){
		return this->signal;
	}

	virtual void setSignal(int _signal){
		this->signal = _signal;
	}

	virtual void setCanshuFromVec(const vector<double> &arr){

	}
#endif