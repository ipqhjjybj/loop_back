
#ifndef _LivermoreStrategyTest3
#define _LivermoreStrategyTest3

#include "../load_config.h"
#include "../ctaStrategyTemplate.h"

#ifndef __livermore_constant
#define __livermore_constant
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
#endif

class LivermoreStrategyTest3 : public ctaTemplate{

public:
    static const int BufferSize = 30;

    /////////////////////////////////////////////////////////////

    double param1;          // param1 = 0.008 策略参数1
    double param2;          // param2 = 0.004 策略参数2

    double z1;              // z1多单止损
    double z2;              // z2空单止损

    int t2;                 // 时间周期2 在进入交易后，如果是上涨趋势，再出现收盘新高后，(参数T2)个BAR之后未出现新高，则平仓出局。
    int t3;                 // 时间周期3 如果是下跌趋势，再出现收盘新低后，(参数T3)个BAR之后未出现新低，则平仓出局

    int atrLength;          // ATR 波动长度

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

    vector<double> atrArr;                          //ATR波动数组


    vector<const string * >big_condArray;           //大方向状态
    int conditionChangeType ;                       //状态变更的原因

    int refreshFlag;
    int ssqs_jishu;
    int xjqs_jishu;
    double high_since_entry;
    double low_since_entry;

    const string *big_condition;

    //突破上升趋势最大点 --> 1
    //W形上升趋势--> 2
    //突破下降趋势最低点 --> 3
    //倒W形下降趋势 --> 4

    LivermoreStrategyTest3(){
        ctaTemplate();
        this->setStrategyName("LivermoreStrategyTest3");
    }
    void Sell(int size , double price , int iBar){
        ctaTemplate::Sell(size , price , iBar);
    }
    void BuyToCover(int size , double price , int iBar){
        ctaTemplate::BuyToCover(size , price, iBar);
    }
    void Buy(int size , double price , int iBar){
        ctaTemplate::Buy(size ,price , iBar);
    }
    void SellShort(int size , double price , int iBar){
        ctaTemplate::SellShort(size ,price , iBar);
    }
    vector<string> getCanshuSetNames(){
        vector<string> vCanshu; vCanshu.clear();

        vCanshu.push_back("param1");
        vCanshu.push_back("param2");
        vCanshu.push_back("z1");
        vCanshu.push_back("z2");
        vCanshu.push_back("t2");
        vCanshu.push_back("t3");

        vCanshu.push_back("atrLength");

        return vCanshu;
    }
    void init(){
        //printf("init!!\n");
        ctaTemplate::init();

        //
        param1 = 0.008;
        param2 = 0.004;

        z1 = 0.008;
        z2 = 0.008;

        t2 = 400;
        t3 = 400;

        atrLength = 10;

        high_since_entry = 0.0;
        low_since_entry  = 0.0;
        refreshFlag = 0;

        ssqs_jishu = 0;
        xjqs_jishu = 0;

        keyPointArr.clear();
        klinePointArr.clear();
        number_ssqs.clear();
        number_zrhs.clear();
        number_cjhs.clear();
        number_xjqs.clear();
        number_zrhc.clear();
        number_cjhc.clear();

        big_condArray.clear();
        atrArr.clear();

        number_ssqs.reserve(n + 10) ;
        number_zrhs.reserve(n + 10) ;
        number_cjhs.reserve(n + 10) ;
        number_xjqs.reserve(n + 10) ;
        number_zrhc.reserve(n + 10) ;
        number_cjhc.reserve(n + 10) ;

        atrArr.reserve(n + 10);



        this->big_condition = &ShangShenQuShi;
        this->conditionChangeType = 0;

        bufferCount = 0;
    }

    /**
     * 使用 conf 下面的内置的函数来赋予 滑点、手续费比例等。
     ***/
    void init(const char *code){
        ctaTemplate::init(code);
        this->init();
        this->initFee(code);

        if( is_zuhehuice == 1){
            account_money = __const_ini_money;
            lots = 5;
        }else{
            account_money = __high_history_price * perSize * cash_deposit * 5 / 100.0;
            //printf("%lf , %lf , %lf ,%lf\n",__high_history_price , account_money * 1.0, perSize*1.0 , cash_deposit*1.0);
            lots = 1;
            if(account_money < 1){
                account_money = __const_ini_money;
            }
        }
    }

    void setCanshuFromVec(const vector<double> &arr){
        this->param1 = arr[0]; this->param2 = arr[1];  this->z1 = arr[2] ; this->z2 = arr[3] ; this->t2 = arr[4] ; this->t3 = arr[5];
        this->atrLength = arr[6];
    }

    TradingResultNode getTradingResult( const char *_code , const char *_period,const char *start_date , const char *end_date){
        TradingResultNode a = TradingResultNode( _code, _period ,start_date, end_date ,tradingResult,tradingNum , account_money ,slippage * n_slippages, rate , perSize , cash_deposit, vecTradeDay , vecTradeWeekDay , dayIncome , weekIncome , bzjAccount , dayAccount, positionAccount, fee_type);
        vector<double> d; d.clear();
        d.push_back(this->param1);
        d.push_back(this->param2);
        d.push_back(this->z1);
        d.push_back(this->z2);
        d.push_back(this->t2);
        d.push_back(this->t3);
        d.push_back(this->atrLength);
        a.setCanshuSet(d);
        return a;
    }

    void onBar_bef(int i){
        ctaTemplate::onBar_bef(i);
    }

    void onBar_after(int i){
        ctaTemplate::onBar_after(i);
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
    void onBar(int i){

        //printf("i=%d param1=%lf param2=%lf\n",i,param1,param2);
        onBar_bef(i);

        double last_v_xjqs = 0.0;
        double last_v_ssqs = 0.0;

        if(this->number_xjqs.size() > 0){
            last_v_xjqs = this->number_xjqs[this->number_xjqs.size() - 1].y;
        }
        if(this->number_ssqs.size() > 0){
            last_v_ssqs = this->number_ssqs[this->number_ssqs.size() - 1].y;
        }

        if( klinePointArr.size() == 0){  // need initialize
            this->big_condition = &ShangShenQuShi;
            if( closeArray[1] > closeArray[0]){
                this->big_condition = &XiaJiangQushi;
            }
            this->addToNumberFigure( g_datetime[i].c_str() , closeArray[i] , this->big_condition);

        }else{
            //printf("judge!\n");
            this->judge(this->klinePointArr[this->klinePointArr.size()-1] , g_datetime[i].c_str() ,closeArray[i]);
        }

        this->big_condArray.push_back(this->big_condition);

        if(this->big_condArray.size() > 1 && this->big_condArray[this->big_condArray.size() - 1] != this->big_condArray[this->big_condArray.size() - 2]){
            this->refreshFlag = 0;
        }

        if(this->big_condition == &ShangShenQuShi){
            if(highArray[i] < last_v_ssqs && last_v_ssqs > 0.01){
                this->ssqs_jishu += 1;
            }
        }else{
            this->ssqs_jishu = 0;
        }

        if(this->big_condition == &XiaJiangQushi){
            if(lowArray[i] > last_v_xjqs && last_v_xjqs > 0.01){
                this->xjqs_jishu += 1;
            }
        }else{
            this->xjqs_jishu = 0;
        }

        //printf("%s\n",this->big_condition->c_str());
        int buy_cond  = 0;
        int sell_cond = 0;
        if( this->big_condition == &ShangShenQuShi){
            buy_cond  = 1;
        }
        if( this->big_condition == &XiaJiangQushi){
            sell_cond = 1;
        }
        if( this->pos == 0){
            this->high_since_entry = highArray[i];
            this->low_since_entry  = lowArray[i];

            if(buy_cond == 1 && 0 == this->refreshFlag ){
                this->Buy(this->lots , closeArray[i] , i);
            }
            if(sell_cond == 1 && 0 == this->refreshFlag ){
                this->SellShort(this->lots , closeArray[i] , i);
            }
        }else if(this->pos > 0){


            // if( strcmp("2016/11/08" , g_date[i].c_str()) == 0 ){

            //         printf("????!  highArray %lf  z2 %lf , low_since_entry %lf , val %lf\n", highArray[i] , z2 ,low_since_entry , this->low_since_entry * ( 1 + this->z2) );
            // }
            if(buy_cond == 0){
                this->Sell(abs(this->pos) , closeArray[i] , i);

                // if( strcmp("2016/11/08" , g_date[i].c_str()) == 0 ){

                //     printf("fuck111111!  highArray %lf  z2 %lf , low_since_entry %lf , val %lf\n", highArray[i] , z2 ,low_since_entry , this->low_since_entry * ( 1 + this->z2) );
                // }
            }
            if(sell_cond == 1){
                this->SellShort(abs(this->pos) , closeArray[i] , i);

                // if( strcmp("2016/11/08" , g_date[i].c_str()) == 0 ){

                //     printf("fuck22222!  highArray %lf  z2 %lf , low_since_entry %lf , val %lf\n", highArray[i] , z2 ,low_since_entry , this->low_since_entry * ( 1 + this->z2) );
                // }
            }

            if( BarsSinceEntry > 0 && lowArray[i] < this->high_since_entry - this->atrArr[i] * this->z1){
                this->refreshFlag = 1;
                this->Sell(abs(this->pos) , closeArray[i] , i);

                // if( strcmp("2016/11/08" , g_date[i].c_str()) == 0 ){

                //     printf("fuck!  highArray %lf  z2 %lf , low_since_entry %lf , val %lf\n", highArray[i] , z2 ,low_since_entry , this->low_since_entry * ( 1 + this->z2) );
                // }
            }

            this->high_since_entry = max(this->high_since_entry , highArray[i]);
            this->low_since_entry  = lowArray[i];

        }else if(this->pos < 0){


            if(sell_cond == 0){
                this->BuyToCover(abs(this->pos) , closeArray[i] , i);
            }
            if(buy_cond == 1){
                this->Buy(this->lots , closeArray[i] , i);
            }
            if( BarsSinceEntry > 0 && highArray[i] > this->low_since_entry + this->atrArr[i] * this->z2 ){
                this->refreshFlag = 1;
                this->BuyToCover(abs(this->pos) , closeArray[i] , i);
            }

            this->high_since_entry = highArray[i];
            this->low_since_entry  = min(this->low_since_entry , lowArray[i]);
        }

        // if( strcmp("2016/11/08" , g_date[i].c_str()) == 0 ){
        //     printf("pos: %d , %s , %s ,%lf ,%lf\n", this->pos , g_date[i].c_str() , g_time[i].c_str() , highArray[i] , lowArray[i]);
        // }
        // if(this->pos == 0){
        //     this->high_since_entry = highArray[i];
        //     this->low_since_entry  = lowArray[i];
        // }else if(this->pos > 0){
        //     this->high_since_entry = max(this->high_since_entry , highArray[i]);
        //     this->low_since_entry  = lowArray[i];
        //     if(lowArray[i] < this->high_since_entry * ( 1 - this->z1)){
        //         this->refreshFlag = 1;
        //         this->Sell(abs(this->pos) , closeArray[i] , i);
        //     }
        // }else if(this->pos < 0){
        //     this->high_since_entry = highArray[i];
        //     this->low_since_entry  = min(this->low_since_entry , lowArray[i]);
        //     if(highArray[i] > this->low_since_entry * ( 1 + this->z2)){
        //         this->refreshFlag = 1;
        //         this->BuyToCover(abs(this->pos) , closeArray[i] , i);
        //     }
        // }

        // printf("");
        // if(this->pos > 0 && this->ssqs_jishu >= this->t2){
        //     this->refreshFlag = 1;
        //     this->Sell(abs(this->pos) , closeArray[i] , i);
        // }else if(this->pos < 0 && this->xjqs_jishu >= this->t3){
        //     this->refreshFlag = 1;
        //     this->BuyToCover(abs(this->pos) , closeArray[i] , i);
        // }
        //printf("pos -> %lf\n" , this->pos);
        onBar_after(i);
    }

    void beforeRun( ){
        ctaTemplate::beforeRun();

        AvgTrueRange(atrArr , highArray , lowArray , closeArray , atrLength , n);
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
            for(double t_param1 = 0.008; t_param1 < 0.040;t_param1 += 0.002){
                for(double t_param2 = t_param1 / 3.0 ; t_param2 < t_param1 * 2.0 / 3.0 ; t_param2 += 0.002){
                    for(double t_z1 = 0.005; t_z1 < 0.050; t_z1 += 0.005){
                        for(double t_z2 = max(t_z1 - 0.02,0.05); t_z2 < t_z1 + 0.02 ; t_z2 += 0.005){
                            for(int tt2 = 200 ; tt2 < 201; tt2 += 100){
                                for(int tt3 = 200; tt3 < 201; tt3 += 100){
                                    for(int t_atrLength = 10; t_atrLength <= 30; t_atrLength += 2){
                                        vector<double> tmp; tmp.clear();
                                        tmp.push_back( t_param1 * 1.0);
                                        tmp.push_back( t_param2 * 1.0);
                                        tmp.push_back( t_z1 * 1.0);
                                        tmp.push_back( t_z2 * 1.0);
                                        tmp.push_back( tt2 * 1.0);
                                        tmp.push_back( tt3 * 1.0);
                                        tmp.push_back( atrLength * 1.0);
                                        // vecLoopArr.push_back(tmp);
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
        }else{
            printf("openfile,loopForce error!\n");
        }
        this->loopForceCanshu = make_pair( t_canshuNum , loopForceFileName );
    }

    void run(){

        this->beforeRun();

        //printf("run param1:%lf param2:%lf\n",param1,param2);
        for( int i = 1; i < n;i++){ // i = BufferSize - 1
            onBar(i);
        }

        this->afterRun();
    }

    void afterRun(){
        ctaTemplate::afterRun();
    }

};

#endif // _LivermoreStrategy
