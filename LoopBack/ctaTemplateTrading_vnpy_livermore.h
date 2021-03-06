#ifndef _cta_template_vnpy_livermore_signal
#define _cta_template_vnpy_livermore_signal


#include "load_config.h"
#include "ctaTemplateSignal_vnpy.h"
#include "ctaTemplateTrading_vnpy.h"


const string ShangShenQuShi = "ssqs";
const string ZiRanHuiShen   = "zrhs";
const string CiJiHuiShen    = "cjhs";

const string XiaJiangQushi  = "xjqs";
const string ZiRanHuiChe    = "zrhc";
const string CiJiHuiChe     = "cjhc";

const string HeiMoShui      = "k";        // 上升趋势黑墨水
const string HongMoShui     = "r";        // 下降趋势红墨水
const string QianBi         = "b";        // 其他栏 铅笔

const string NoBelowLine    = "";         // 没有下划线
const string RED_LINE       = "r";        // 上升趋势以及自然回调后的点下标记
const string BLACK_LINE     = "b";        // 下降趋势以及自然回升形成的点下标记


struct __livermore_point{
    double y;
    const char *datetime;
    const string *p_color;
    __livermore_point(const char *_datetime,double _y , const string *_pcolor){
        datetime = _datetime;
        y = _y;
        p_color = _pcolor;
    }
};
struct __livermore_line{
    char datetime[50];
    const string *p_bigCond;  //大的上涨方向
    const string *p_color;    //使用的绘图颜色
    double y;
    __livermore_line(const char *_datetime, double _y, const string *_pbigcond , const string *_pcolor ){
        strcpy(datetime , _datetime);
        p_bigCond = _pbigcond;
        p_color = _pcolor;
        y = _y;
    }
};

class LivermoreStrategy : public CtaTemplateVnpySignal{

public:
    static const int BufferSize = 30;

    /////////////////////////////////////////////////////////////

    double param1;          // param1 = 0.008 策略参数1
    double param2;          // param2 = 0.004 策略参数2
    ///////////////////////////////////////////////////////////////

    int bufferCount ;

    vector<__livermore_point> keyPointArr;          //存储最重要的几个关键点，  (点位,时间, 线的颜色)的格式
    vector<__livermore_line> klinePointArr;         //[(datetime,Y,"上升趋势",'r')] 存储剩下的趋势点, 上升趋势黑墨水 k--black，下降趋势红墨水 ， 其他栏的点，铅笔
    vector<__livermore_point> number_ssqs;          //上升趋势
    vector<__livermore_point> number_zrhs;          //自然回升
    vector<__livermore_point> number_cjhs;          //次级回升
    vector<__livermore_point> number_xjqs;          //下降趋势
    vector<__livermore_point> number_zrhc;          //自然回撤
    vector<__livermore_point> number_cjhc;          //次级回撤


    vector<const string * >big_condArray;           //大方向状态
    int conditionChangeType ;                       //状态变更的原因
    const string *big_condition;


    //突破上升趋势最大点 --> 1
    //W形上升趋势--> 2
    //突破下降趋势最低点 --> 3
    //倒W形下降趋势 --> 4

    LivermoreStrategy(){
        CtaTemplateVnpySignal();
    }

    vector<string> getCanshuSetNames(){
        vector<string> vCanshu; vCanshu.clear();

        vCanshu.push_back("param1");
        vCanshu.push_back("param2");

        return vCanshu;
    }
    void onInit(){
        //printf("init!!\n");
        CtaTemplateVnpySignal::onInit();
        this->setStrategyName("livermoreStrategy");
        //
        param1 = 0.008;
        param2 = 0.004;

        keyPointArr.clear();
        klinePointArr.clear();
        number_ssqs.clear();
        number_zrhs.clear();
        number_cjhs.clear();
        number_xjqs.clear();
        number_zrhc.clear();
        number_cjhc.clear();

        big_condArray.clear();

        this->big_condition = &ShangShenQuShi;
        this->conditionChangeType = 0;

        bufferCount = 0;
    }

    /**
     * 使用 conf 下面的内置的函数来赋予 滑点、手续费比例等。
     ***/
    void setCanshuFromVec(const vector<double> &arr){
        this->param1 = arr[0]; this->param2 = arr[1];
    }

    void addToNumberFigure(const char *_datetime , double y ,  const string *_big_condition){
        const string *pColor = &QianBi;
        if(_big_condition == &ShangShenQuShi){
            this->number_ssqs.push_back( __livermore_point(_datetime , y , &NoBelowLine));
            pColor = &HeiMoShui;
        }
        if(_big_condition == &ZiRanHuiShen){
            this->number_zrhs.push_back( __livermore_point(_datetime , y , &NoBelowLine));
        }
        if(_big_condition == &CiJiHuiShen){
            this->number_cjhs.push_back( __livermore_point(_datetime , y , &NoBelowLine));
        }
        if(_big_condition == &XiaJiangQushi){
            this->number_xjqs.push_back( __livermore_point(_datetime , y , &NoBelowLine));
            pColor = &HongMoShui;
        }
        if(_big_condition == &ZiRanHuiChe){
            this->number_zrhc.push_back( __livermore_point(_datetime , y , &NoBelowLine));
        }
        if(_big_condition == &CiJiHuiChe){
            this->number_cjhc.push_back( __livermore_point(_datetime , y , &NoBelowLine));
        }
        this->klinePointArr.push_back(__livermore_line(_datetime,  y, _big_condition, pColor ));
    }

    const string * judge_xjqs( const string *_big_condition , double y ){
        const string *p = _big_condition;
        if( this->number_xjqs.size() > 0){
            if( y < this->number_xjqs[this->number_xjqs.size() - 1].y){
                this->conditionChangeType = 3;
                p = &XiaJiangQushi;
            }
        }
        int to_drop_line = 0;
        if( this->number_zrhc.size() > 0){
            for(int i = 1;i < this->number_zrhc.size() + 1;i++){
                if( this->number_zrhc[this->number_zrhc.size() - i].p_color == &RED_LINE){
                    if( y < (this->number_zrhc[this->number_zrhc.size() - i].y * (1 - this->param2))){
                        this->conditionChangeType = 4;
                        p = &XiaJiangQushi;
                        to_drop_line = 1;
                    }
                    break;
                }
            }
        }
        if (1 == to_drop_line){
            for(int i = 1;i < this->number_zrhc.size() + 1; i++){
                if(this->number_zrhc[this->number_zrhc.size() - i].p_color == &RED_LINE){
                    this->number_zrhc[this->number_zrhc.size() - i].p_color = &NoBelowLine;
                }
            }
        }
        return p;
    }

    const string * judge_ssqs( const string *_big_condition , double y){
        const string *p = _big_condition;
        if( this->number_ssqs.size() > 0){
            if( y > this->number_ssqs[this->number_ssqs.size() - 1].y){
                this->conditionChangeType = 1;
                p = &ShangShenQuShi;
            }
        }
        int to_drop_line = 0;
        if( this->number_zrhs.size() > 0){
            for(int i = 1;i < this->number_zrhs.size() ; i++){
                if(this->number_zrhs[this->number_zrhs.size() - i].p_color == &BLACK_LINE){
                    if(y > (this->number_zrhs[this->number_zrhs.size() - i].y * (1 + this->param2))){
                        this->conditionChangeType = 2;
                        p = &ShangShenQuShi;
                        to_drop_line = 1;
                    }
                    break;
                }
            }
        }
        if(1 == to_drop_line){
            for(int i = 1;i < this->number_zrhs.size() + 1;i++){
                if(this->number_zrhs[this->number_zrhs.size() - i].p_color == &BLACK_LINE){
                    this->number_zrhs[this->number_zrhs.size() - i].p_color = &NoBelowLine;
                }
            }
        }
        return p;
    }

    void judge(struct __livermore_line &t_klinePoint , const char * x ,double y){
        const char *pl_x = t_klinePoint.datetime;
        double pl_y = t_klinePoint.y;
        const string * pl_condition = t_klinePoint.p_bigCond;
        const string * pl_color = t_klinePoint.p_color;

        if( this->big_condition == &ShangShenQuShi){
            if( y > pl_y){
                this->addToNumberFigure( x , y , this->big_condition);
            }else if(y < pl_y * (1 - this->param1)){
                this->keyPointArr.push_back( __livermore_point(pl_x, pl_y , &RED_LINE));
                this->number_ssqs[this->number_ssqs.size() - 1].p_color = &RED_LINE;
                this->big_condition = &ZiRanHuiChe;
                this->addToNumberFigure(x, y, this->big_condition);
            }
        }else if( this->big_condition == &ZiRanHuiShen){
            if(y > pl_y){
                this->big_condition = this->judge_ssqs(this->big_condition , y);
                this->addToNumberFigure(x , y , this->big_condition);
            }else if(y < pl_y * (1 - this->param1)){
                this->keyPointArr.push_back( __livermore_point(pl_x, pl_y , &BLACK_LINE));
                this->number_zrhs[this->number_zrhs.size()-1].p_color = &BLACK_LINE;
                //开启下一个状态
                //自然回升转自然回撤
                this->big_condition = &ZiRanHuiChe;
                // 自然回升转次级回撤
                // 当前价格 > 自然回撤的最后一个价格时，为次级回撤
                if( this->number_zrhc.size() > 0 && y > this->number_zrhc[this->number_zrhc.size()-1].y){
                    this->big_condition = &CiJiHuiChe;
                }
                if( this->number_zrhc.size() == 0){
                    this->big_condition = &ZiRanHuiChe;
                }
                // 自然回升转下降趋势
                // 当前点小于下降趋势的最后一个点 或 小于带红线的自然回撤最后一个点
                this->big_condition = this->judge_xjqs(this->big_condition , y );
                this->addToNumberFigure(x , y , this->big_condition);
            }
        }else if( this->big_condition == &CiJiHuiShen){
            if(y > pl_y){
                //判断次级回升上升为自然回升
                if( this->number_zrhs.size() > 0 && y > this->number_zrhs[this->number_zrhs.size()-1].y){
                    this->big_condition = &ZiRanHuiShen;
                }
                if( this->number_zrhs.size() == 0){
                    this->big_condition = &ZiRanHuiShen;
                }
                // 判断次级回升为上升趋势
                this->big_condition = this->judge_ssqs(this->big_condition , y );
                this->addToNumberFigure(x , y, this->big_condition);
            }else if( y < pl_y * ( 1 - this->param1)){
                //次级回升这里之后的状态。。 比较难搞，先简单定义为自然回撤
                //如果小于自然回撤的最后一个点， 则记录在自然回撤里
                this->big_condition = &CiJiHuiChe;
                if( this->number_zrhc.size() > 0 && y < this->number_zrhc[this->number_zrhc.size() - 1].y){
                    this->big_condition = &ZiRanHuiChe;
                }
                if( this->number_zrhc.size() == 0){
                    this->big_condition = &ZiRanHuiChe;
                }
                //判断下降趋势
                this->big_condition = this->judge_xjqs(this->big_condition , y);
                this->addToNumberFigure( x, y , this->big_condition);
            }
        }else if(this->big_condition == &XiaJiangQushi){
            if(y < pl_y){
                this->addToNumberFigure(x , y , this->big_condition);
            }else if(y > pl_y * ( 1+ this->param1)){
                this->keyPointArr.push_back(__livermore_point(pl_x , pl_y , &BLACK_LINE));
                this->number_xjqs[this->number_xjqs.size()-1].p_color = &BLACK_LINE;
                this->big_condition = &ZiRanHuiShen;
                this->addToNumberFigure(x , y, this->big_condition);
            }
        }else if(this->big_condition == &ZiRanHuiChe){
            if( y < pl_y){
                this->big_condition = this->judge_xjqs(this->big_condition , y );
                this->addToNumberFigure(x , y, this->big_condition);
            }else if(y > pl_y * ( 1+ this->param1)){
                this->keyPointArr.push_back( __livermore_point(pl_x , pl_y , &RED_LINE));
                this->number_zrhc[this->number_zrhc.size()-1].p_color = &RED_LINE;
                this->big_condition = &ZiRanHuiShen;
                // 判断是否是次级回升
                if( this->number_zrhs.size() > 0 && y < this->number_zrhs[this->number_zrhs.size() - 1].y){
                    this->big_condition = &CiJiHuiShen;
                }
                if( this->number_zrhs.size() == 0){
                    this->big_condition = &ZiRanHuiShen;
                }
                this->big_condition = this->judge_ssqs(this->big_condition , y );
                this->addToNumberFigure(x , y, big_condition);
            }
        }else if(this->big_condition == &CiJiHuiChe){
            if(y < pl_y){
                if(this->number_zrhc.size() > 0 && y < this->number_zrhc[this->number_zrhc.size() - 1].y){
                    this->big_condition = &ZiRanHuiChe;
                }
                if(this->number_zrhc.size() == 0){
                    this->big_condition = &ZiRanHuiChe;
                }
                // 次级回撤转下降趋势
                this->big_condition = this->judge_xjqs(this->big_condition , y );
                this->addToNumberFigure(x , y, this->big_condition);
            }else if(y > pl_y * ( 1 + this->param1)){
                // 次级回撤这里之后的状态。。 比较难搞，先简单定义为自然回升
                this->big_condition = &CiJiHuiShen;
                if(this->number_zrhs.size() > 0 && y > this->number_zrhs[this->number_zrhs.size() - 1].y){
                    this->big_condition = &ZiRanHuiShen;
                }
                if(this->number_zrhs.size() == 0){
                    this->big_condition = &ZiRanHuiShen;
                }
                this->big_condition = this->judge_ssqs(this->big_condition , y );
                this->addToNumberFigure(x , y, this->big_condition);
            }
        }
    }
    void onBar(const struct Bar &bar){

        if( klinePointArr.size() == 0){  // need initialize
            this->big_condition = &ShangShenQuShi;
            this->addToNumberFigure( bar.datetime , bar.close , this->big_condition);
        }else{
            //printf("judge!\n");
            this->judge(this->klinePointArr[this->klinePointArr.size()-1] , bar.datetime ,bar.close);
        }
        this->big_condArray.push_back(this->big_condition);

        //printf("%s\n",this->big_condition->c_str());
        int buy_cond  = 0;
        int sell_cond = 0;
        if( this->big_condition == &ShangShenQuShi){
            buy_cond  = 1;
        }
        if( this->big_condition == &XiaJiangQushi){
            sell_cond = 1;
        }
        if(buy_cond > 0 && sell_cond > 0){
        	printf("Error in cta_template_vnpy_livermore_signalta! buy_cond > 0 && sell_cond > 0! \n");
        }
        this->setSignal(0);
        if( buy_cond > 0 ){
        	this->setSignal(1);
        }
        if( sell_cond > 0){
        	this->setSignal(-1);
        }
    }
};

class CtaTemplateVnpyTrading_vnpy_livermore : CtaTemplateVnpyTrading{

public:
	CtaTemplateVnpyTrading_vnpy_livermore( ){

	}
};
#endif