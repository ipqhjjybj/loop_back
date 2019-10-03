#include "load_config.h"
#include "ctaStrategyTemplate.h"

#ifndef _InvestmentProtfolio
#define _InvestmentProtfolio

struct protfolioNode{
	ctaTemplate * obj;
	string code;
	string period;
	vector<double> canshuSet;
    double need_set_money ; 

	protfolioNode(ctaTemplate * _obj , const string &_code , const string &_period , const vector<double> &vec , const double __need_set_money){
		obj = _obj;
		code = _code;
		period = _period;
		canshuSet.clear();
		for(int i = 0; i < vec.size(); i++){
			canshuSet.push_back( vec[i]);
		}
        need_set_money = __need_set_money;
	}
};

class InvestmentProtfolio{

public:

	vector<struct protfolioNode> vPro;
	//vector<struct TradingResultNode > v_tradingResultNum;

    vector< pair<string, string> > code_peroid_vec;     // 存储 code与period的对
    string start_date , end_date;
    vector<double> all_total_income;                    // 保存每日的累计收益
    vector<double> all_week_income;                     // 保存每周的单周收益之和


    double ini_account_money;           //账户初始资金           F
    double rate_of_return;              //年化收益率           F              important
    int total_open_num;                 //总开仓次数           F              important
    double win_rate;                    //胜率                 F              important
    double profit_factor;               //盈利因子   净利润总额/亏损总额  F   important
    double max_continue_loss;           //最大连续亏损                       important
    double max_windrawal_rate;          //最大回撤比率                       important         最大回撤数值 / 高点
    double sharpe;                      //夏普比率                           important         每笔交易平均盈利率 / 每笔交易标准差

    int total_trade_days;               //交易日个数   F
    int total_trade_weeks;              //交易周个数   F
    double total_income;                //净利润      F
    double net_loss;                    //总亏损       F
    double net_profits;                 //总盈利      F

    int win_week_num;                   //盈利周数
    int loss_week_num;                  //亏损周数
    int win_cover_num;                  //平仓盈利次数  F
    int loss_cover_num;                 //平仓亏损次数  F
    double loss_rate;                   //赔率          F
    double max_continue_win;            //最大连续盈利
    int max_continue_win_num;           //最大连赢天数
    int max_continue_loss_num;          //最大连亏天数

    double ave_cover_income;            //平均平仓净利润  F     平均每手净利润
    double ave_cover_win;               //平均平仓盈利    F     平均每手盈利
    double ave_cover_loss;              //平均平仓亏损    F     平均每手亏损
    double ave_cover_win_loss_rate;     //平均平仓盈亏比  F

    double max_single_win;              //最大单日盈利   F
    double max_single_loss;             //最大单日亏损   F

    double max_windrawl_number;          //最大回撤数值
    char max_windraw_start_date[100];     //最大回撤区间开始
    char max_windraw_end_date[100];       //最大回撤区间结束
    double windrawl_safe_rate;           //回撤安全系数         净利润/最大回撤数值

    double total_transcaction_fee;       //总交易成本     F      总的(保证金 + 手续费)
    double ave_transcation_fee;          //平均交易成本   F
    double transcation_cover_rate;       //交易成本覆盖率 F      净利润/总交易成本

    double real_income_rate;             //实际收益率     F      净利润/初始资金
    double ave_time_income_rate;         //平均每次收益率 F      平均每次收益率

    double stdandard;                    //波动率               每笔交易标准差
    double skwenes;                      //峰度
    double kurtosis;                     //偏度
    double sortinoRatio;                 //索提诺比率           每周的索提诺

    double targetFunctionRankScore;      //目标函数排名的实际分数
    double targetFunctionPromScore;      //目标函数Prom的实际分数

    vector<double> saveDayIncome;        //保存的每日收益
    vector<string> all_vec_trade_day;    //保存的每日日期

    // new add
    vector<double> total_bzj;            //总共保证金
    vector<double> total_position;       //显示当前仓位
    vector<double> total_bzj_percent;    //显示保证金占初始资金的比例

    // new add for zuhe huice
    vector<string> save_vec_trade_day;   // 保存的交易日
    vector<double> save_all_income;      // 分段回测时保留的所有收益
    vector<double> save_total_bzj;       // 分段回测时保留的所有保证金
    vector<double> save_total_position;  // 分段回测时保留的总仓位
    vector<double> save_bzj_percent;     // 分段回测时保留的保证金比例

    vector<double> save_last_money;      // 每次交易完，剩下的最后的钱

    bool first_add;


    //返回 投资组合 的结果
    double getInvestmentResult(){
        return this->targetFunctionRankScore;
    }

    //prom 目标函数
    double targetFunctionProm(){
        return (this->ave_cover_win * (this->win_cover_num - sqrt(this->win_cover_num * 1.0)) +
                this->ave_cover_loss * (this->loss_cover_num - sqrt(this->loss_cover_num * 1.0))) * 250.0 / (this->ini_account_money * this->total_trade_days);
    }
    // 目标函数排名
    double targetFunctionRank(){
        double v = this->skwenes * this->kurtosis;
        double eps = 1e-6;
        if(v > eps){
            v = 1.0;
        }else if(v < -eps){
            v = -1.0;
        }else{
            v = 0.0;
        }
        double pdfd = v * sqrt(fabs(this->skwenes * this->kurtosis));
        double t = this->rate_of_return * 100 * 0.3 + this->max_windrawal_rate * 100 * 0.2 + this->win_rate * 50 * 0.1
        + pdfd * 0.1 + this->profit_factor * 10 * 0.1 + this->sortinoRatio * 5 * 0.2;
        //printf("rate_of_return:%lf max_windrawal_rate:%lf win_rate:%lf pdfd:%lf profit_factor:%lf sortinoRatio:%lf t:%lf\n",
         //      rate_of_return * 100,max_windrawal_rate * 100 ,win_rate * 50,pdfd ,profit_factor * 10,this->sortinoRatio * 5,t);

        return t;
    }

	InvestmentProtfolio(){

        vPro.clear();
        all_vec_trade_day.clear();
        code_peroid_vec.clear();
        all_total_income.clear();
        all_week_income.clear();
        all_vec_trade_day.clear();
        total_bzj.clear();
        total_position.clear();
        total_bzj_percent.clear();

        save_vec_trade_day.clear();
        save_all_income.clear();
        save_total_bzj.clear();
        save_total_position.clear();
        save_bzj_percent.clear();

        save_last_money.clear();

        //v_tradingResultNum.clear();
        first_add = 1;
	}

    void clear2(){
        save_vec_trade_day.clear();
        save_all_income.clear();
        save_total_bzj.clear();
        save_total_position.clear();
        save_bzj_percent.clear();
    }
    void clear(){
        vPro.clear();
        all_vec_trade_day.clear();
        code_peroid_vec.clear();
        all_total_income.clear();
        all_week_income.clear();
        all_vec_trade_day.clear();
        total_bzj.clear();
        total_position.clear();
        total_bzj_percent.clear();

        //v_tradingResultNum.clear();
        first_add = 1;
    }
	void addPro(const struct protfolioNode &n){
		vPro.push_back(n);
        code_peroid_vec.push_back( make_pair(string(n.code) , string(n.period)));
	}
	void loadFile(const char *start_date , const char *end_date){
        this->start_date = string(start_date);
        this->end_date   = string(end_date);
		for(int i = 0;i < vPro.size() ;i++){
			struct protfolioNode &t = vPro[i];
			vector<string> useFiles = getFileName(t.code.c_str() , t.period.c_str());
            // for(int kk = 0;kk < useFiles.size(); kk++){
            //     printf("%s\n",useFiles[kk].c_str());
            // }
            //printf("useFile : %s i = %d\n",useFile.c_str(),i);
			t.obj->loadFiles( useFiles , start_date , end_date , t.period);
		}
	}
	void init(){
		for(int i = 0;i < vPro.size();i++){
			struct protfolioNode &t = vPro[i];
			t.obj->init(t.code.c_str());
            t.obj->setCanshuFromVec( t.canshuSet );
		}
	}
	void run(const char *start_date , const char *end_date){
        vector<struct TradingResultNode> ans; ans.clear();
		for(int i = 0;i < vPro.size(); i++){
			struct protfolioNode &t = vPro[i];
			t.obj->run();
            ans.push_back(t.obj->getTradingResult(t.code.c_str() , t.period.c_str() , start_date , end_date));
		}
        for(int i = 0;i < ans.size();i++){
            this->resultAdd(ans[i]);
        }
        this->endCompute();
        //this->showResult();
        //this->exportReport();
	}

    void run2(const char *start_date , const char *end_date){
        vector<struct TradingResultNode> ans; ans.clear();
        this->save_last_money.clear();

        //printf("vpo size: %d\n" , vPro.size());
        for(int i = 0;i < vPro.size(); i++){
            double pre_ini_money = __const_ini_money;
            struct protfolioNode &t = vPro[i];
            this->save_last_money.push_back(vPro[i].need_set_money);
            vector<string> useFiles = getFileName(t.code.c_str() , t.period.c_str());
            __const_ini_money = vPro[i].need_set_money;
            t.obj->loadFiles( useFiles , start_date , end_date , t.period);
            t.obj->init(t.code.c_str());
            t.obj->setCanshuFromVec( t.canshuSet );
            t.obj->run();
            ans.push_back(t.obj->getTradingResult(t.code.c_str() , t.period.c_str() , start_date , end_date));
            __const_ini_money = pre_ini_money;
            //makeFigureCsv(ans[0], (string(__report_dir_path) + t.obj->getStrategyName() + "/figure_trading_result/debug2_" + t.code + "-" + t.period + "-" + replaceDate(string(start_date)) + "-" + replaceDate(string(end_date)) + "--best-figure.csv").c_str() , "sb\n");
        }
        //printf("why!!!! save_last_money.size %d\n" , 1 * this->save_last_money.size());
        for(int i = 0;i < ans.size();i++){
            makeFigureCsv(ans[i], (string(__report_dir_path) + vPro[i].obj->getStrategyName() + "/figure_trading_result/debug_protfolio_2_" + ans[i].code + "-" + ans[i].period + "-" + replaceDate(string(start_date)) + "-" + replaceDate(string(end_date)) + "--best-figure.csv").c_str() , "sb\n");
            string tradingResultFile = string(__report_dir_path) + vPro[i].obj->getStrategyName() + "/figure_trading_result/debug_protfolio_2_" + ans[i].code + "-" + ans[i].period + "-" + replaceDate(start_date) + "-" + replaceDate(end_date) + "--best-trading_result-.csv";
            debug_tradingResult(&ans[i] , tradingResultFile.c_str() , "firstLine");
            this->resultAdd(ans[i]);

            if(ans[i].saveDayIncome.size() > 0){
                this->save_last_money[i] = this->save_last_money[i] + ans[i].saveDayIncome[ans[i].saveDayIncome.size()-1];
            }
        }
        //printf("endCompute\n");
        this->endCompute();
    }

    void resultAdd(const struct TradingResultNode & s_data){
        // printf("first_add : %d\n",first_add);
        // printf("total_trade_weeks: %d\n",s_data.total_trade_weeks);
        if(first_add){
            this->total_trade_days = s_data.total_trade_days;
            this->total_trade_weeks = s_data.total_trade_weeks;

            this->all_total_income.reserve(s_data.saveDayIncome.size() + 10);
            this->all_vec_trade_day.reserve(s_data.saveDayIncome.size() + 10);

            this->all_week_income.reserve(s_data.saveWeekIncome.size() + 10);

            for(int i = 0;i < s_data.saveDayIncome.size();i++){
                this->all_total_income.push_back(0.0);
                this->all_vec_trade_day.push_back(s_data.saveVecTradeDay[i]);
            }
            for(int i = 0;i < s_data.saveWeekIncome.size()+2;i++){
                this->all_week_income.push_back(0.0);
            }
            //this->total_bzj.assign(s_data.saveBzjIncome.begin(),s_data.saveBzjIncome.end());
            for(int i = 0;i < s_data.saveBzjIncome.size() ; i++){
                this->total_bzj.push_back(0.0);
            }

            for(int i = 0;i < s_data.savePositions.size() ; i++){
                this->total_position.push_back(0);
            }

            this->ini_account_money = 0;
            this->total_open_num    = 0;
            this->total_income      = 0.0;
            this->net_loss          = 0.0;
            this->net_profits       = 0.0;
            this->win_week_num      = 0;
            this->loss_week_num     = 0;
            this->win_cover_num     = 0;
            this->loss_cover_num    = 0;
            this->max_single_win    = 0.0;
            this->max_single_loss   = 0.0;
            this->max_continue_win  = 0.0;
            this->max_continue_loss = 0.0;

            this->total_transcaction_fee = 0.0;
        }

        for(int i = 0;i < s_data.saveDayIncome.size() && i < this->all_total_income.size();i++){
            this->all_total_income[i] += s_data.saveDayIncome[i];
            // if(i == s_data.saveDayIncome.size() - 1){
            //     printf("fuck %d %lf %lf\n",s_data.saveDayIncome.size(),s_data.saveDayIncome[i],this->all_total_income[i]);
            // }
        }

        for(int i = 0;i < s_data.saveWeekIncome.size() && i < this->all_week_income.size();i++){
            this->all_week_income[i] += s_data.saveWeekIncome[i];
        }

        for(int i = 0;i < s_data.saveBzjIncome.size() && i < this->total_bzj.size() ; i++){
            this->total_bzj[i] += s_data.saveBzjIncome[i];
        }

        for(int i = 0;i < s_data.savePositions.size() && i < this->total_position.size() ; i++){
            this->total_position[i] += abs(s_data.savePositions[i]);
        }


        this->ini_account_money += s_data.ini_account_money;
        this->total_open_num    += s_data.total_open_num;

        this->total_income      += s_data.total_income;
        this->net_loss          += s_data.net_loss;
        this->net_profits       += s_data.net_profits;
        this->win_cover_num     += s_data.win_cover_num;
        this->loss_cover_num    += s_data.loss_cover_num;
        this->max_single_win  = max(this->max_single_win , s_data.max_single_win);
        this->max_single_loss = min(this->max_single_loss , s_data.max_single_loss);
        //this->total_transcaction_fee += s_data.total_transcaction_fee;
        this->total_transcaction_fee += s_data.total_transcaction_only_fee;

        first_add = 0;
    }

    void endCompute(){
        this->ave_transcation_fee = this->total_transcaction_fee * 1.0 / this->total_open_num;
        this->transcation_cover_rate = this->total_income * 1.0 / this->total_transcaction_fee;
        this->win_rate         = this->win_cover_num * 1.0 / this->total_open_num;
        this->loss_rate        = this->loss_cover_num * 1.0 / this->total_open_num;
        this->profit_factor    = fabs(this->net_profits / this->net_loss);

        this->ave_cover_income = this->total_income * 1.0 / this->total_open_num;
        this->ave_cover_win    = this->net_profits * 1.0 / this->win_cover_num;
        this->ave_cover_loss   = this->net_loss * 1.0 / this->loss_cover_num;

        this->ave_cover_win_loss_rate = fabs(this->ave_cover_win * 1.0 / this->ave_cover_loss);
        this->rate_of_return = this->total_income * 250.0 / (this->ini_account_money * this->total_trade_days);

        this->real_income_rate = this->total_income * 1.0 / this->ini_account_money;

        this->ave_time_income_rate = this->ave_cover_income * 1.0 / this->ini_account_money;

        double last_pnl = 0.0;                          //上一笔的pnl
        int continue_win_num = 0;                       //连续赚钱次数
        int continue_loss_num = 0;                      //连续亏钱次数
        double continue_income = 0.0;                   //连续盈利
        double continue_loss = 0.0;                     //连续亏损
        //vector<double> v_pnl; v_pnl.clear();            //存储连续的pnl
        //vector<double> v_sum_pnl; v_sum_pnl.clear();    //存储pnl的连续和

        double t_stdandard = 0.0;
        double t_skwenes = 0.0;
        double t_kurtosis = 0.0;
        double t_ddex = 0.0;

        this->win_week_num = 0 ;  this->loss_week_num = 0;

        double t_max_windrawl_number = 0.0;
        int t_max_windraw_start_date = 0, t_max_windraw_end_date = 0;
        int pre_max_sum_i = 0;
        double pre_max_sum_pnl = 0.0;


        for(int i = 0;i < this->total_bzj.size();i++){
            this->total_bzj_percent.push_back( 1.0 * this->total_bzj[i] / this->ini_account_money);
        }

        for(int i = 0;i < this->all_week_income.size();i++){
            double pnl = this->all_week_income[i];
            if(pnl > eps){
                this->win_week_num += 1;
            }
            if(pnl < -eps){
                this->loss_week_num += 1;
            }
        }
        double t_ave_dayIncome_rate = 0.0;
        double tmp_total_income = 0.0;
        //printf("all_total_income : %d\n" , all_total_income.size());
        for(int i = 0;i < all_total_income.size();i++){
            //double pnl = 0.0;
            double pnl = 0.0;
            if(i == 0){
                pnl = this->all_total_income[i];
            }else{
                pnl = this->all_total_income[i] - this->all_total_income[i-1];
            }
            //double pnl  = this->all_total_income[i];        //累计盈利
            //printf("pnl = %lf\n",pnl);
            tmp_total_income += pnl;
            t_ave_dayIncome_rate += pnl;
            if(last_pnl * pnl > eps){ // 认为这两笔是连续的
                if(pnl > 0){
                    continue_income += pnl;
                    continue_win_num += 1;
                }else{
                    continue_loss += pnl;
                    continue_loss_num += 1;
                }
            }else{  //这一笔的pnl方向与上次的不一样
                continue_win_num = continue_loss_num = 0;
                continue_income = continue_loss = 0;
                if(pnl > eps){
                    continue_win_num += 1;
                    continue_income += pnl;
                }
                if(-pnl > eps){
                    continue_loss_num += 1;
                    continue_loss += pnl;
                }
            }
            this->max_single_win = max(pnl , this->max_single_win);
            this->max_single_loss = min(pnl , this->max_single_loss);

            this->max_continue_win_num = max(this->max_continue_win_num , continue_win_num);
            this->max_continue_loss_num = max(this->max_continue_loss_num , continue_loss_num);
            this->max_continue_win = max(this->max_continue_win , continue_income);
            this->max_continue_loss = min(this->max_continue_loss , continue_loss);
            last_pnl = pnl;

            double sum_pnl1 = tmp_total_income;
            if(sum_pnl1 > pre_max_sum_pnl){
                pre_max_sum_pnl = sum_pnl1;
                pre_max_sum_i   = i;
            }
            if( (sum_pnl1 - pre_max_sum_pnl) < t_max_windrawl_number ){
                t_max_windrawl_number = sum_pnl1 - pre_max_sum_pnl;
                t_max_windraw_start_date = pre_max_sum_i;
                t_max_windraw_end_date = i;
            }
        }
        t_ave_dayIncome_rate = 1.0 * total_income / this->ini_account_money / this->total_trade_days;
        for(int i = 0;i < this->all_total_income.size();i++){
            if(i == 0){
                t_stdandard      += pow( (this->all_total_income[i] / this->ini_account_money - t_ave_dayIncome_rate), 2.0);
                t_skwenes        += pow( (this->all_total_income[i] / this->ini_account_money - t_ave_dayIncome_rate), 3.0);
                t_kurtosis       += pow( (this->all_total_income[i] / this->ini_account_money - t_ave_dayIncome_rate), 4.0);
                t_ddex           += pow( min(0.0 , this->all_total_income[i] / this->ini_account_money - NoRiseReturn/100.0/250.0 ) , 2);
            }else{
                t_stdandard      += pow( ((this->all_total_income[i]-this->all_total_income[i-1]) / this->ini_account_money - t_ave_dayIncome_rate), 2.0);
                t_skwenes        += pow( ((this->all_total_income[i]-this->all_total_income[i-1]) / this->ini_account_money - t_ave_dayIncome_rate), 3.0);
                t_kurtosis       += pow( ((this->all_total_income[i]-this->all_total_income[i-1]) / this->ini_account_money - t_ave_dayIncome_rate), 4.0);
                t_ddex           += pow( min(0.0 , (this->all_total_income[i]-this->all_total_income[i-1]) / this->ini_account_money - NoRiseReturn/100.0/250.0 ) , 2);
            }
        }
        t_ddex   = sqrt( t_ddex / this->total_trade_days * 250.0 );
        // mark std
        this->stdandard = sqrt(t_stdandard * 1.0 / (this->total_trade_days - 1)) ;
        this->sharpe    = (this->rate_of_return - NoRiseReturn/100.0 ) / this->stdandard / sqrt(250.0) ;
        this->skwenes   = t_skwenes * 1.0 / (this->total_trade_days * pow(this->stdandard, 3.0));
        this->kurtosis  = t_kurtosis * 1.0 / ( this->total_trade_days * pow(this->stdandard , 4.0));

        this->sortinoRatio = (this->rate_of_return - NoRiseReturn/100.0 ) / t_ddex;

        strcpy(this->max_windraw_start_date, this->all_vec_trade_day[t_max_windraw_start_date].c_str());
        strcpy(this->max_windraw_end_date , this->all_vec_trade_day[t_max_windraw_end_date].c_str());
        this->max_windrawl_number = t_max_windrawl_number;
        this->windrawl_safe_rate = fabs(this->total_income * 1.0 / this->max_windrawl_number);
        this->max_windrawal_rate = this->max_windrawl_number * 1.0 / this->ini_account_money;

        this->targetFunctionRankScore = this->targetFunctionRank();
        this->targetFunctionPromScore = this->targetFunctionProm();

        // for(int i = 0;i < this->all_total_income.size() ;i++){
        //     this->all_total_income[i] = this->all_total_income[i] / this->ini_account_money + 1;
        // }

    }

    // 导出 组合报告 中的回测报告
    void exportZuheReport1(const string &backReportName , const char *firstLine){
        FILE *fp = NULL;
        if( (fp = fopen(backReportName.c_str(), "w")) != NULL){
            fprintf(fp,"%s\n",firstLine);
            fprintf(fp,"ini_account_money:%.0lf\n",ini_account_money);
            fprintf(fp,"rate_of_return:%.2lf%%\n",rate_of_return * 100);
            fprintf(fp,"total_open_num:%d\n",total_open_num);
            fprintf(fp,"win_rate:%.2lf\n",win_rate * 100.0);
            fprintf(fp,"profit_factor:%.2lf\n",profit_factor );
            fprintf(fp,"max_continue_loss:%.1lf\n",max_continue_loss);
            fprintf(fp,"max_windrawal_rate:%.6lf%%\n",max_windrawal_rate * 100.0);
            fprintf(fp,"sharpe:%.2lf\n",sharpe);
            fprintf(fp,"total_trade_days:%d\n",this->total_trade_days);
            fprintf(fp,"total_trade_weeks:%d\n",this->total_trade_weeks);
            fprintf(fp,"net_profits:%.2lf\n",net_profits);
            fprintf(fp,"net_loss:%.0lf\n",net_loss);
            fprintf(fp,"total_income:%.0lf\n",total_income);
            fprintf(fp,"win_week_num:%d\n",win_week_num);
            fprintf(fp,"loss_week_num:%d\n",loss_week_num);
            fprintf(fp,"win_cover_num:%d\n",win_cover_num);
            fprintf(fp,"loss_cover_num:%d\n",loss_cover_num);
            fprintf(fp,"loss_rate:%.2lf%%\n",loss_rate*100.0);
            fprintf(fp,"max_continue_win:%.1lf\n",max_continue_win);
            fprintf(fp,"max_continue_win_num:%d\n",max_continue_win_num);
            fprintf(fp,"max_continue_loss_num:%d\n",max_continue_loss_num);
            fprintf(fp,"ave_cover_income:%.0lf\n",ave_cover_income);
            fprintf(fp,"ave_cover_win:%.1lf\n",ave_cover_win);
            fprintf(fp,"ave_cover_loss:%.1lf\n",ave_cover_loss);
            fprintf(fp,"ave_cover_win_loss_rate:%.1lf\n",ave_cover_win_loss_rate);
            fprintf(fp,"max_single_win:%.1lf\n",max_single_win);
            fprintf(fp,"max_single_loss:%.1lf\n",max_single_loss);
            fprintf(fp,"max_windrawl_number:%.0lf\n",max_windrawl_number);
            fprintf(fp,"max_windraw_start_date:%s\n",max_windraw_start_date);
            fprintf(fp,"max_windraw_end_date:%s\n",max_windraw_end_date);
            fprintf(fp,"windrawl_safe_rate:%.2lf%%\n",windrawl_safe_rate * 100);
            fprintf(fp,"total_transcaction_fee:%.1lf\n",total_transcaction_fee);
            fprintf(fp,"ave_transcation_fee:%.1lf\n",ave_transcation_fee);
            fprintf(fp,"transcation_cover_rate:%.2lf%%\n",transcation_cover_rate * 100);
            fprintf(fp,"real_income_rate:%.2lf%%\n",real_income_rate * 100);
            fprintf(fp,"ave_time_income_rate:%.2lf%%\n",ave_time_income_rate * 100);
            fprintf(fp,"stdandard:%.2lf\n",stdandard);

            fprintf(fp,"skwenes:%.2lf\n",skwenes);
            fprintf(fp,"kurtosis:%.2lf\n",kurtosis);
            fprintf(fp,"sortinoRatio:%.2lf\n",sortinoRatio);
            fprintf(fp,"targetFunctionRankScore:%.2lf\n",targetFunctionRankScore);
            fprintf(fp,"targetFunctionPromScore:%.2lf\n",targetFunctionPromScore);
            fclose(fp);

            reportFileLists.push_back(backReportName);
        }else{
            printf("cannot write file: %s\n",backReportName.c_str());
        }
    }
    // 导出 组合报告中的 图表报告
    void exportZuheFigureReport(const string &preName){

        string figureAllIncomeRate = string(__report_dir_path) + string("Combination") + "/" + preName + "-income_all-" + "--best-figure.csv";
        makeFigureSingle(figureAllIncomeRate.c_str() , this->all_vec_trade_day , this->all_total_income , "-all-total-income\n");

        string figureBzj = string(__report_dir_path) + string("Combination") + "/" + preName + "-bzj-" + "--best-figure.csv";
        makeFigureSingle(figureBzj.c_str() , this->all_vec_trade_day , this->total_bzj, "zuhe-bzj\n");

        string figureBzjRate = string(__report_dir_path) + string("Combination") + "/" + preName + "-bzj_rate-" + "--best-figure.csv";
        makeFigureSingle(figureBzjRate.c_str() , this->all_vec_trade_day , this->total_bzj_percent , "zuhe-bzj-percent\n");

        string figurePosition = string(__report_dir_path) + string("Combination") + "/" + preName + "-position_all-" + "--best-figure.csv";
        makeFigureSingle(figurePosition.c_str() , this->all_vec_trade_day , this->total_position , "position_all\n");

    }

    void exportReport(const string &preName , const string &zuheFirstLine){
        string zuheBacktestingName = string(__report_dir_path) + string("Combination") + "/" + preName + "-back-combination-report.csv";
        this->exportZuheReport1( zuheBacktestingName , zuheFirstLine.c_str());
        this->exportZuheFigureReport(preName);
    }

    void exportSaveFigueReport(const string &preName ){
        string figureAllIncomeRate = string(__report_dir_path) + string("Combination") + "/" + preName + "-save_continue_income_all-" + "--best-figure.csv";
        makeFigureSingle(figureAllIncomeRate.c_str() , this->all_vec_trade_day , this->all_total_income , "-all-total-income\n");

        string figureBzj = string(__report_dir_path) + string("Combination") + "/" + preName + "-save_continue_bzj-" + "--best-figure.csv";
        makeFigureSingle(figureBzj.c_str() , this->all_vec_trade_day , this->total_bzj, "zuhe-bzj\n");

        string figureBzjRate = string(__report_dir_path) + string("Combination") + "/" + preName + "-save_continue_bzj_rate-" + "--best-figure.csv";
        makeFigureSingle(figureBzjRate.c_str() , this->all_vec_trade_day , this->total_bzj_percent , "zuhe-bzj-percent\n");

        string figurePosition = string(__report_dir_path) + string("Combination") + "/" + preName + "-save_continue_position_all-" + "--best-figure.csv";
        makeFigureSingle(figurePosition.c_str() , this->all_vec_trade_day , this->total_position , "position_all\n");
    }

    void getSaveResultMoney(vector<double> &to_get_money){
        for(int i = 0;i < this->save_last_money.size() ; i++){
            printf("%lf\n",this->save_last_money[i]);
            to_get_money.push_back( this->save_last_money[i]);
        }
    }

    // 保存到要输出的 report 里
    void saveToNowReport( ){
        for(int i = 0;i < all_vec_trade_day.size();i++){
            this->save_vec_trade_day.push_back(all_vec_trade_day[i]);
        }
        for(int i = 0;i < all_total_income.size();i++){
            this->save_all_income.push_back(all_total_income[i]);
        }
        for(int i = 0;i < total_bzj.size() ;i++){
            this->save_total_bzj.push_back(total_bzj[i]);
        }
        for(int i = 0;i < total_bzj_percent.size() ;i++){
            this->save_bzj_percent.push_back(total_bzj_percent[i]);
        }
        for(int i = 0;i < total_position.size() ;i++){
            this->save_total_position.push_back(total_position[i]);
        }
    }
    void showResult(){
        printf("ini_account_money:%lf\n",ini_account_money);
        printf("rate_of_return:%lf\n",rate_of_return);
        printf("total_open_num:%d\n",total_open_num);
        printf("win_rate:%lf\n",win_rate);
        printf("profit_factor:%lf\n",profit_factor);
        printf("max_continue_loss:%lf\n",max_continue_loss);
        printf("max_windrawal_rate:%lf\n",max_windrawal_rate);
        printf("sharpe:%lf\n",sharpe);
        printf("total_trade_days:%d\n",this->total_trade_days);
        printf("total_trade_weeks:%d\n",this->total_trade_weeks);
        printf("net_profits:%lf\n",net_profits);
        printf("net_loss:%lf\n",net_loss);
        printf("total_income:%lf\n",total_income);
        printf("win_week_num:%d\n",win_week_num);
        printf("loss_week_num:%d\n",loss_week_num);
        printf("win_cover_num:%d\n",win_cover_num);
        printf("loss_cover_num:%d\n",loss_cover_num);
        printf("loss_rate:%lf%%\n",loss_rate*100.0);
        printf("max_continue_win:%lf\n",max_continue_win);
        printf("max_continue_win_num:%d\n",max_continue_win_num);
        printf("max_continue_loss_num:%d\n",max_continue_loss_num);
        printf("ave_cover_income:%lf\n",ave_cover_income);
        printf("ave_cover_win:%lf\n",ave_cover_win);
        printf("ave_cover_loss:%lf\n",ave_cover_loss);
        printf("ave_cover_win_loss_rate:%lf\n",ave_cover_win_loss_rate);
        printf("max_single_win:%lf\n",max_single_win);
        printf("max_single_loss:%lf\n",max_single_loss);
        printf("max_windrawl_number:%lf\n",max_windrawl_number);
        printf("max_windraw_start_date:%s\n",max_windraw_start_date);
        printf("max_windraw_end_date:%s\n",max_windraw_end_date);
        printf("windrawl_safe_rate:%lf\n",windrawl_safe_rate);
        printf("total_transcaction_fee:%lf\n",total_transcaction_fee);
        printf("ave_transcation_fee:%lf\n",ave_transcation_fee);
        printf("transcation_cover_rate:%lf\n",transcation_cover_rate);
        printf("real_income_rate:%lf\n",real_income_rate);
        printf("ave_time_income_rate:%lf\n",ave_time_income_rate);
        printf("stdandard:%lf\n",stdandard);

        printf("skwenes:%lf\n",skwenes);
        printf("kurtosis:%lf\n",kurtosis);
        printf("sortinoRatio:%lf\n",sortinoRatio);
        printf("targetFunctionRankScore:%lf\n",targetFunctionRankScore);
        printf("targetFunctionPromScore:%lf\n",targetFunctionPromScore);

    }
};


// 组合结果节点
struct investment_result_node{
    string str_canshu;
    double score;
    investment_result_node(){}
    investment_result_node(vector<double> &_canshuSet , double _score){
        score = _score;
        str_canshu = "";
        for(int i = 0;i < _canshuSet.size();i++){
            if(i == 0){
                str_canshu = float2str(_canshuSet[i]);
            }else{
                str_canshu = str_canshu + ","+ float2str(_canshuSet[i]);
            }
        }
    }
    //重载最小比较符号
    bool operator < (const struct investment_result_node & a) const{
        return this->score > a.score;
        //return this->total_income > a.total_income;
    }
};
#endif
