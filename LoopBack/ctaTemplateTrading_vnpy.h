#ifndef _cta_template_vnpy_trading
#define _cta_template_vnpy_trading

class CtaTemplateVnpyTrading{

public:
	//策略的基本参数

	int inited  ; 
	int trading ;
	CtaTemplateVnpyTrading( ){
		this->inited  = 0;
		this->trading = 0;
	}

}
#endif