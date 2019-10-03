
#include "load_config.h"
#include <vector>
#include <algorithm>
#include "TradingResult.h"
#include "engine.h"


#ifndef _makeReport
#define _makeReport
#include <vector>
#include <algorithm>
/**
 * 获得错误调试报告
 ****/
void debug_report(vector<TradingResultNode > &v_report , const char * filename ,const char * firstLine){
    FILE *fp = NULL;
    if( (fp = fopen(filename, "w")) != NULL){
        fprintf(fp , "%s, total_answer_num:%d\n" , firstLine,allResultNum );
        if(v_report.size() > 0){
            for(int i = 0;i < v_report.size();i++){
                fprintf(fp,"%d,",i);
                struct TradingResultNode * t = &v_report[i];
                for(int j = 0;j < t->canshuSet.size(); j++){
                    fprintf(fp,"%.3lf,",t->canshuSet[j]);
                }
                fprintf(fp,"%.5lf\n", t->compareValue() );
                //fprintf(fp,"%d\n",checkLegalCanshu(t->canshuSet));
                    //out << t.total_income << endl;
            }
        }
        fclose(fp);
    }else{
        printf("cannot write file: %s\n",filename);
    }
}

/**
 * 获得two loop推进报告总的报告
 **/
void debug_two_loop_report( vector<struct TwoTradingReuslt  > &two_loop_report , const char * filename , const char *firstLine){
    FILE *fp = NULL;
    if( (fp = fopen(filename, "w")) != NULL){
        fprintf(fp , "%s\n" , firstLine );
        if(two_loop_report.size() > 0){
            for(int i = 0;i < two_loop_report.size();i++){
                fprintf(fp,"%d,",i);
                struct TwoTradingReuslt * t = &two_loop_report[i];
                for(int j = 0;j < t->n1.canshuSet.size(); j++){
                    fprintf(fp,"%.3lf,",t->n1.canshuSet[j]);
                }
                fprintf(fp,"%.5lf %.5lf\n", t->n1.compareValue() , t->n2.compareValue());
            }
        }
        fclose(fp);
    }else{
        printf("cannot write file: %s\n",filename);
    }
}
/*
 *
 获得具体的交易报告

 */
void debug_tradingResult(struct TradingResultNode *pvec, const char *filename , const char *firstLine){
    FILE *fp = NULL;
    if( (fp = fopen(filename , "w")) != NULL){
        fprintf(fp,"%s",firstLine);
        fprintf(fp,"entryTime,exitTime,entryPrice,exitPrice,volumeDirection\n");
        int llen = pvec->saveTradingResult.size();
        for(int i = 0;i < llen;i++){
            fprintf(fp, "%s,%s,%lf,%lf,%lf\n",pvec->saveTradingResult[i].entryTime,pvec->saveTradingResult[i].exitTime,pvec->saveTradingResult[i].entryPrice,pvec->saveTradingResult[i].exitPrice,pvec->saveTradingResult[i].volume);
        }
        fclose(fp);

    }else{
        printf("cannot write file: %s\n",filename);
    }
}
/**
 * 导出简单float 图表
 **/
void makeFigureSingle(const char *filename , const vector<string> &vecDay ,const vector<double> &data , const char *firstLine){
    FILE *fp = NULL;
    if( (fp = fopen(filename , "w")) != NULL){
        fprintf(fp, "%s" , firstLine);
        for(int i = 0;i < vecDay.size() && i < data.size(); i++){
            fprintf(fp, "%s,%lf\n", vecDay[i].c_str() , data[i]);
        }
        fclose(fp);
        reportFileLists.push_back(filename);
    }else{
        printf("cannot write file: %s\n",filename);
    }
}
/**
 * 制作图表
 **/
void makeFigureCsv(const struct TradingResultNode &no , const string &filename , const char *firstLine){
    FILE *fp = NULL;
    if( (fp = fopen(filename.c_str() , "w")) != NULL){
        fprintf(fp , "%s", firstLine);
        double output_total_money = 0.0;
        for(int i = 0;i < no.saveDayIncome.size() ; i++){
            output_total_money = no.saveDayIncome[i];
            fprintf(fp,"%s,%lf\n",no.saveVecTradeDay[i].c_str(), output_total_money);
        }
        fclose(fp);
        reportFileLists.push_back(filename);
    }else{
        printf("cannot write file: %s\n",filename.c_str());
    }
}
/**
 * 通过几个 tradingResult 获得
 */
void backtesting_assessment_report(const vector<TradingResultNode *> &v_report , const char * filename ,const char * firstLine){
	FILE *fp = NULL;
	if(fp = fopen(filename, "w")){
		const TradingResultNode *best = v_report[0];
		const TradingResultNode *best_sharpe = v_report[1];
		fprintf(fp,"%s\n",firstLine);
		fprintf(fp,"时间段,%s---%s,%s---%s\n",best->start_date,best->end_date,best_sharpe->start_date,best_sharpe->end_date);
        fprintf(fp,"交易日个数,%d,%d\n",best->total_trade_days,best_sharpe->total_trade_days);
        fprintf(fp,"交易周个数,%d,%d\n",best->total_trade_weeks,best_sharpe->total_trade_weeks);
        fprintf(fp,"初始资金,%lf,%lf\n",best->ini_account_money,best_sharpe->ini_account_money);
        fprintf(fp,"----,核心指标,---\n");
        fprintf(fp,"年化收益率(%%),%.1lf%%,%.1lf%%\n",best->rate_of_return * 100.0, best_sharpe->rate_of_return*100.0);
        fprintf(fp,"盈亏比,%.2lf,%.2lf\n",best->profit_factor,best_sharpe->profit_factor);
        fprintf(fp,"胜率,%.1lf,%.1lf\n",best->win_rate,best_sharpe->win_rate);
        fprintf(fp,"最大回撤(%%),%.1lf%%,%.1lf%%\n",best->max_windrawal_rate*100.0,best_sharpe->max_windrawal_rate*100.0);
        fprintf(fp,"索提诺比率,%.2lf,%.2lf\n",best->sortinoRatio,best_sharpe->sortinoRatio);
        fprintf(fp,"目标函数(打分),%.1lf,%.1lf\n",best->targetFunctionRankScore,best_sharpe->targetFunctionRankScore);
        fprintf(fp,"目标函数(PROM)%%,%.2lf,%.2lf\n",best->targetFunctionPromScore,best_sharpe->targetFunctionPromScore * 100.0);
        fprintf(fp,"---,总收益率指标,---\n");
        fprintf(fp,"总盈利,%.1lf,%.1lf\n",best->net_profits,best_sharpe->net_profits);
        fprintf(fp,"总亏损,%.1lf,%.1lf\n",best->net_loss,best_sharpe->net_loss);
        fprintf(fp,"---,交易类指标,---\n");
        fprintf(fp,"总开仓次数,%d,%d\n",best->total_open_num,best_sharpe->total_open_num);
        fprintf(fp,"盈利次数,%d,%d\n",best->win_cover_num,best_sharpe->win_cover_num);
        fprintf(fp,"亏损次数,%d,%d\n",best->loss_cover_num,best_sharpe->loss_cover_num);
        //fprintf(fp,"胜率,%.1lf,%.1lf\n",best->win_rate * 100 ,best_sharpe->win_rate * 100);
        fprintf(fp,"盈利周数,%d,%d\n",best->win_week_num,best_sharpe->win_week_num);
        fprintf(fp,"亏损周数,%d,%d\n",best->loss_week_num,best_sharpe->loss_week_num);
        fprintf(fp,"---,平均收益类指标,---\n");
        fprintf(fp,"平均每手净利润,%.1lf,%.1lf\n",best->ave_cover_income,best_sharpe->ave_cover_income);
        fprintf(fp,"平均每手盈利,%.1lf,%.1lf\n",best->ave_cover_win,best_sharpe->ave_cover_win);
        fprintf(fp,"平均每手亏损,%.1lf,%.1lf\n",best->ave_cover_loss,best->ave_cover_loss);
        fprintf(fp,"---,极端交易类指标,---\n");
        fprintf(fp,"最大单次盈利,%.1lf,%.1lf\n",best->max_single_win,best_sharpe->max_single_win);
        fprintf(fp,"最大单次亏损,%.1lf,%.1lf\n",best->max_single_loss,best_sharpe->max_single_loss);
        fprintf(fp,"最大连赢次数,%d,%d\n",best->max_continue_win_num,best_sharpe->max_continue_win_num);
        fprintf(fp,"最大连续盈利,%.1lf,%.1lf\n",best->max_continue_win,best_sharpe->max_continue_win);
        fprintf(fp,"最大连亏次数,%d,%d\n",best->max_continue_loss_num,best_sharpe->max_continue_loss_num);
        fprintf(fp,"最大连续亏损,%.1lf,%.1lf\n",best->max_continue_loss,best_sharpe->max_continue_loss);
        fprintf(fp,"---,资产回撤指标,---\n");
        fprintf(fp,"最大回撤数值,%.2lf,%.2lf\n",best->max_windrawl_number,best_sharpe->max_windrawl_number);
        //fprintf(fp,"最大回撤比率,%.2lf%%,%.2lf%%\n",best->max_windrawal_rate * 100.0,best_sharpe->max_windrawal_rate*100.0);
        fprintf(fp,"最大回撤发生时间段,%s---%s,%s---%s\n",best->max_windraw_start_date,best->max_windraw_end_date,best_sharpe->max_windraw_start_date,best_sharpe->max_windraw_end_date);
        fprintf(fp,"回撤安全系数,%.1lf,%.1lf\n",best->windrawl_safe_rate,best_sharpe->windrawl_safe_rate);
        fprintf(fp,"---,交易成本类指标,---\n");
        fprintf(fp,"总交易成本,%.1lf,%.1lf\n",best->total_transcaction_fee,best_sharpe->total_transcaction_fee);
        fprintf(fp,"平均交易成本,%.1lf,%.1lf\n",best->ave_transcation_fee,best_sharpe->ave_transcation_fee);
        fprintf(fp,"交易成本覆盖率,%.2lf%%,%.2lf%%\n",best->transcation_cover_rate * 100.0,best_sharpe->transcation_cover_rate * 100.0);
        fprintf(fp,"---,风险类指标,---\n");
        fprintf(fp,"波动率,%.1lf,%.1lf\n",best->stdandard*100,best_sharpe->stdandard*100);
        fprintf(fp,"夏普比率,%.2lf,%.2lf\n",best->sharpe,best_sharpe->sharpe);
        //fprintf(fp,"索提诺比率,%.2lf,%.2lf\n",best->sortinoRatio,best_sharpe->sortinoRatio);
        fclose(fp);
        reportFileLists.push_back(filename);
    }else{
		printf("cannot write file: %s\n",filename);
	}
}
/**
 * 通过几个 tradingResult 获得
 */
void push_on_backtesting_report(const vector<TradingResultNode *> &v_report , const char * filename ,const char * firstLine){
    FILE *fp = NULL;
    if(fp = fopen(filename, "w")){
        const TradingResultNode *best = v_report[0];
        fprintf(fp,"%s\n",firstLine);
        fprintf(fp,"时间段,%s---%s\n",best->start_date,best->end_date);
        fprintf(fp,"交易日个数,%d\n",best->total_trade_days);
        fprintf(fp,"交易周个数,%d\n",best->total_trade_weeks);
        fprintf(fp,"初始资金,%lf\n",best->ini_account_money);
        fprintf(fp,"----,核心指标,---\n");
        fprintf(fp,"年化收益率(%%),%.1lf%%\n",best->rate_of_return * 100.0);
        fprintf(fp,"盈亏比,%.2lf\n",best->profit_factor);
        fprintf(fp,"胜率,%.1lf\n",best->win_rate);
        fprintf(fp,"最大回撤(%%),%.1lf%%\n",best->max_windrawal_rate*100.0);
        fprintf(fp,"索提诺比率,%.2lf\n",best->sortinoRatio);
        fprintf(fp,"目标函数(打分),%.1lf\n",best->targetFunctionRankScore);
        fprintf(fp,"目标函数(PROM)%%,%.2lf\n",best->targetFunctionPromScore);
        fprintf(fp,"---,总收益率指标,---\n");
        fprintf(fp,"总盈利,%.1lf\n",best->net_profits);
        fprintf(fp,"总亏损,%.1lf\n",best->net_loss);
        fprintf(fp,"---,交易类指标,---\n");
        fprintf(fp,"总开仓次数,%d\n",best->total_open_num);
        fprintf(fp,"盈利次数,%d\n",best->win_cover_num);
        fprintf(fp,"亏损次数,%d\n",best->loss_cover_num);
        fprintf(fp,"盈利周数,%d\n",best->win_week_num);
        fprintf(fp,"亏损周数,%d\n",best->loss_week_num);
        fprintf(fp,"---,平均收益类指标,---\n");
        fprintf(fp,"平均每手净利润,%.1lf\n",best->ave_cover_income);
        fprintf(fp,"平均每手盈利,%.1lf\n",best->ave_cover_win);
        fprintf(fp,"平均每手亏损,%.1lf\n",best->ave_cover_loss);
        fprintf(fp,"---,极端交易类指标,---\n");
        fprintf(fp,"最大单次盈利,%.1lf\n",best->max_single_win);
        fprintf(fp,"最大单次亏损,%.1lf\n",best->max_single_loss);
        fprintf(fp,"最大连赢次数,%d\n",best->max_continue_win_num);
        fprintf(fp,"最大连续盈利,%.1lf\n",best->max_continue_win);
        fprintf(fp,"最大连亏次数,%d\n",best->max_continue_loss_num);
        fprintf(fp,"最大连续亏损,%.1lf\n",best->max_continue_loss);
        fprintf(fp,"---,资产回撤指标,---\n");
        fprintf(fp,"最大回撤数值,%.2lf\n",best->max_windrawl_number);
        //fprintf(fp,"最大回撤比率,%.2lf%%,%.2lf%%\n",best->max_windrawal_rate * 100.0,best_sharpe->max_windrawal_rate*100.0);
        fprintf(fp,"最大回撤发生时间段,%s---%s\n",best->max_windraw_start_date,best->max_windraw_end_date);
        fprintf(fp,"回撤安全系数,%.1lf\n",best->windrawl_safe_rate);
        fprintf(fp,"---,交易成本类指标,---\n");
        fprintf(fp,"总交易成本,%.1lf\n",best->total_transcaction_fee);
        fprintf(fp,"平均交易成本,%.1lf\n",best->ave_transcation_fee);
        fprintf(fp,"交易成本覆盖率,%.2lf%%\n",best->transcation_cover_rate * 100.0);
        fprintf(fp,"---,风险类指标,---\n");
        fprintf(fp,"波动率,%.1lf\n",best->stdandard*100);
        fprintf(fp,"夏普比率,%.2lf\n",best->sharpe);
        //fprintf(fp,"索提诺比率,%.2lf,%.2lf\n",best->sortinoRatio,best_sharpe->sortinoRatio);
        fclose(fp);
        reportFileLists.push_back(filename);
    }else{
        printf("cannot write file: %s\n",filename);
    }
}
/**
 * 输出其他可行解得报告
 ***/
void other_optimal_parameter_sets_report(const vector<TradingResultNode *> &v_report , const char * filename ,const char * firstLine){
	FILE *fp = NULL;
	if( (fp = fopen(filename, "w")) != NULL){
        fprintf(fp , "%s\n" , firstLine);
        if(v_report.size() > 0){
	        const TradingResultNode *best = v_report[0];
	        fprintf(fp,"最优参数,");
	        for(int j = 0;j < best->canshuSet.size();j++){
	            fprintf(fp,"%.3lf,",best->canshuSet[j]);
	        }fprintf(fp,"%.2lf\n",best->total_income);

	        for(int i = 1;i < v_report.size();i++){
	            fprintf(fp,"%d,",i);
	            const struct TradingResultNode * t = v_report[i];
	            for(int j = 0;j < t->canshuSet.size(); j++){
	                fprintf(fp,"%.3lf,",t->canshuSet[j]);
	            }
	            fprintf(fp,"%.2lf\n", t->total_income );
	                //out << t.total_income << endl;
	        }
    	}
        fclose(fp);
        reportFileLists.push_back(filename);
    }else{
    	printf("cannot write file: %s\n",filename);
    }
}

/**
 * 输出稳定性评判的报告
 ****/
void stability_and_distribute_judge_report( const vector<TradingResultNode> &ans , const char *code , const char *use_period , const char *output_start_date , const char *output_end_date , const char * strategyName , const char *firstLine){
    /**
     *  稳定性评判
    *****/
    int total_canshu_num = ans.size();
    int win_canshu_num   = 0;
    int loss_canshu_num  = 0;
    double total_capital = 0.0;
    double win_capital   = 0.0;
    double loss_capital  = 0.0;

    double max_total_capital = 0.0;
    double min_total_capital = 0.0;
    double max_win_capital = 0.0;
    double min_win_capital = 0.0;
    double max_loss_capital = 0.0;
    double min_loss_capital = 0.0;
    for(int i = 0;i < ans.size();i++){
        const struct TradingResultNode &t = ans[i];
        total_capital += t.total_income;
        max_total_capital = max(max_total_capital , t.total_income);
        min_total_capital = min(min_total_capital , t.total_income);

        //printf("total_income:%lf\n",t.total_income);
        if( t.total_income > eps){
            win_capital += t.total_income;
            win_canshu_num += 1;
            max_win_capital = max(max_win_capital , t.total_income);
            min_win_capital = min(min_win_capital , t.total_income);
        }
        if( t.total_income < -eps){
            //printf("why!!\n");
            loss_capital += t.total_income;
            loss_canshu_num += 1;
            max_loss_capital = max(max_loss_capital , t.total_income);
            min_loss_capital = min(min_loss_capital , t.total_income);
        }
    }
    string stability_filename = string(__report_dir_path) + strategyName +"/" + code + "-" + use_period + "-" + output_start_date + "-" + output_end_date + "--stability.csv";
    FILE *fp = NULL;

    if( (fp = fopen(stability_filename.c_str() , "w")) != NULL){
        fprintf(fp,"%s",firstLine);
        //fprintf(fp,"%s,%s,%s--%s\n",code, use_period , output_start_date , output_end_date);
        fprintf(fp,"---,数量,百分比,平均利润\n");
        fprintf(fp,"测试总数,%d,%lf,%.2lf\n", total_canshu_num ,  100.0 , total_capital *1.0 / total_canshu_num);
        fprintf(fp,"获利次数,%d,%lf,%.2lf\n", win_canshu_num   ,  1.0 * win_canshu_num / total_canshu_num , win_capital *1.0 / total_canshu_num);
        fprintf(fp,"亏损次数,%d,%lf,%.2lf\n", loss_canshu_num  ,  1.0 * loss_canshu_num / total_canshu_num , loss_capital *1.0 / total_canshu_num);
        fclose(fp);
        reportFileLists.push_back(stability_filename);
    }else{
        printf("cannot write file: %s\n",stability_filename.c_str());
    }

    double ave_total_capital = 1.0 * total_capital / total_canshu_num;
    double ave_win_capital   = 1.0 * win_capital / win_canshu_num;
    double ave_loss_capital  = 1.0 * loss_capital / loss_canshu_num;

    double t_std_total = 0.0;
    double t_std_win   = 0.0;
    double t_std_loss  = 0.0;
    for(int i = 0;i < ans.size();i++){
        const struct TradingResultNode &t = ans[i];
        t_std_total += pow( ave_total_capital - t.total_income , 2);
        if( t.total_income > eps){
            t_std_win += pow( ave_win_capital - t.total_income , 2);
        }
        if( t.total_income < -eps){
            t_std_loss += pow( ave_loss_capital - t.total_income , 2);
        }
    }
    t_std_total = sqrt( t_std_total / total_canshu_num);
    t_std_win   = sqrt( t_std_win / win_canshu_num);
    if(loss_canshu_num == 0){
        t_std_loss  = 0.0;
    }else{
        t_std_loss  = sqrt( t_std_loss / loss_canshu_num);
    }
    string distribute_filename = string(__report_dir_path) + strategyName +"/" + code + "-" + use_period + "-" + output_start_date + "-" + output_end_date + "--distribute-best.csv";
    fp = NULL;
    if((fp = fopen(distribute_filename.c_str(), "w"))!=NULL){
        fprintf(fp,"%s",firstLine);
        fprintf(fp,"---,所有参数集的,所有盈利的参数集,所有亏损的参数集,最佳策略参数集\n");
        fprintf(fp,"绩效平均值,%.2lf,%.2lf,%.2lf\n",ave_total_capital,ave_win_capital,ave_loss_capital);
        fprintf(fp,"最大值,%.2lf,%.2lf,%.2lf\n",max_total_capital,max_win_capital , max_loss_capital);
        fprintf(fp,"最小值,%.2lf,%.2lf,%.2lf\n",min_total_capital,min_win_capital , min_loss_capital);
        fprintf(fp,"标准差,%.2lf,%.2lf,%.2lf\n",t_std_total,t_std_win,t_std_loss);
        fclose(fp);
        reportFileLists.push_back(distribute_filename);
    }else{
        printf("cannot write file: %s\n",distribute_filename.c_str());
    }
}


/**
 * 扫描过程中 参数不同个数在70%以上，幅度在50%以上，但是结果相差在10%以内的结果同样筛选出来列表暂展示。
 * 假如参数个数为 N ,
 * 1、参数不同个数 > N * 0.7
 * 2、参数的变化幅度Z > N * 0.2

 * 并且实现了 查找最优目标函数 等几个报告
 **/
void backtesting_assessment_report_and_Other_optimal_parameter_sets_report(ctaTemplate *obj,const char *code, const char *use_period, const char *start_date,const char *end_date,double parma1 = 0.7, double param2 = 0.2 , bool flag_make_report = 1){
    vector<struct TradingResultNode> ans;

    //string filename = getFileName(code , use_period);
    vector<string> filenames = getFileName(code , use_period);

    string output_start_date = replaceDate(string(start_date));
    string output_end_date   = replaceDate(string(end_date));

    if(g_choose_searchAlgorithm == &EngineForceAlgorithm){
        getBestSolution(obj , ans , code , use_period , start_date , end_date);
        reverse(ans.begin() , ans.end());
        string st_name = string(obj->getStrategyName());
        debug_report(ans,(st_name + "_debug_force.csv").c_str(),"debug_force!");
    }
    else if(g_choose_searchAlgorithm == &EngineLiziqunAlgorithm){
        getBestSolutionUsingLiziQun(obj , ans , code , use_period , start_date , end_date , lzq_NumParticle , lzq_max_cycle, lzq_C1 , lzq_C2);
        reverse(ans.begin() , ans.end());
        string st_name = string(obj->getStrategyName());
        debug_report(ans,(st_name + "_debug_liziqun.csv").c_str(),"debug_liziqun!");
    }
    else if(g_choose_searchAlgorithm == &EngineBpSearchAlgorithm){
        getBestSolutionUsingPriority(obj , ans , code , use_period , start_date , end_date , bp_max_cycle , bp_max_NumParticle);
        reverse(ans.begin() , ans.end());
        string st_name = string(obj->getStrategyName());
        debug_report(ans,(st_name + "_debug_bp_serach.csv").c_str(),"debug_bpsearch!");
    }
    else if(g_choose_searchAlgorithm == &EngineRandomAlgorithm){
        getBestSolutionUsingRandomFunction( obj, ans , code ,use_period , start_date , end_date , rm_max_cycle, rm_max_NumParticle , rm_tmp_save_result_num);
        reverse(ans.begin() , ans.end());
        string st_name = string(obj->getStrategyName());
        debug_report(ans,(st_name + "_debug_rm_random_serach.csv").c_str(),"debug_rm_random_search!");
    }
    else{
        printf("Error , please choose search algorithm first!\n");
        return;
    }
    printf("total ans size:%d\n",ans.size());

    if(ans.size() > 0){
        //先找到最优解 净利润最高
        struct TradingResultNode *best = &(ans[0]);
        struct TradingResultNode *best_sharpe = &(ans[0]);
        for(int i = 1; i < ans.size(); i++){
            if(ans[i].compareValue() > best->compareValue()){
                best = &(ans[i]);
            }
            if(ans[i].sharpe > best_sharpe->sharpe){
                best_sharpe = &(ans[i]);
            }
        }
        _config.setBestResult( best->canshuSet);
        string tradingResultFile = string(__report_dir_path) + obj->getStrategyName() + "/figure_trading_result/" + code + "-" + use_period + "-" + output_start_date + "-" + output_end_date + "--best-trading_result.csv";
        char firstLineLine[400];
        sprintf(firstLineLine,"%s,%s,%s--%s,%s\n",code, use_period , start_date , end_date,best->getCanshuSetStr().c_str());
        debug_tradingResult(best , tradingResultFile.c_str() ,firstLineLine);

        makeFigureCsv(*best , (string(__report_dir_path) + obj->getStrategyName() + "/figure_trading_result/" + code + "-" + use_period + "-" + output_start_date + "-" + output_end_date + "--best-figure.csv").c_str() , firstLineLine);

        if(1 == flag_make_report){
            vector<TradingResultNode *> vTradeNodes; vTradeNodes.clear();
            vTradeNodes.push_back(best);
            vTradeNodes.push_back(best_sharpe);

            string fp_filename = string(__report_dir_path) + obj->getStrategyName() + "/" + code + "-" + use_period + "-" + output_start_date + "-" + output_end_date + "--backtesting_assessment_report.csv";
            char firstLine[400];
            sprintf(firstLine,"%s,%s,%s--%s,%s\nLabel,目标函数1(净利润),目标函数2(夏普)",code, use_period , start_date , end_date,best->getCanshuSetStr().c_str());
            backtesting_assessment_report(vTradeNodes , fp_filename.c_str() , firstLine);

            vector<struct TradingResultNode *> outputResult;
            outputResult.clear();
            outputResult.push_back(best);

            const double eps = 1e-6;
            for(int i = 0;i < ans.size(); i++){
                struct TradingResultNode &t = ans[i];
                double score1 = 0.0 , score2 = 0.0;
                for(int j = 0;j < t.canshuSet.size(); j++){
                    if(fabs(best->canshuSet[j] - t.canshuSet[j]) > eps ){
                        score1 += 0.1;
                    }
                    score2 += fabs( (best->canshuSet[j] - t.canshuSet[j]) / best->canshuSet[j] ) ;
                }
                if(score1 >= parma1 && score2 >= best->canshuSet.size() * param2){
                    outputResult.push_back(&ans[i]);
                }
            }
            string similarity_filename = string(__report_dir_path) + obj->getStrategyName() + "/" + code + "-" + use_period + "-" + output_start_date + "-" + output_end_date + "--Other_optimal_parameter_sets.csv";
            sprintf(firstLine , "%s,%s,%s--%s,%s\n参数项目,%s,capital",code, use_period , start_date , end_date,best->getCanshuSetStr().c_str(), obj->getCanshuSetNamesString().c_str());
            other_optimal_parameter_sets_report(outputResult , similarity_filename.c_str() , firstLine);
            sprintf(firstLine,"%s,%s,%s--%s,%s\n",code, use_period , start_date , end_date,best->getCanshuSetStr().c_str());
            stability_and_distribute_judge_report(ans , code , use_period , output_start_date.c_str() , output_end_date.c_str() , obj->getStrategyName().c_str() , firstLine);
        }
    }
}

/**
 * 对于不同的时间窗口，在同组参数下的表现
 * code的品种 a9888, au888,
 * code = "a9888" , 样本内开始时间, 样本内结束时间 , 样本外开始时间,样本外结束时间
 ***/
void Core_index_of_in_or_out_of_sample_report(ctaTemplate *obj ,const char *code,const char *timePeriod, const char* ybn_start_date , const char* ybn_end_date, const char* ybw_start_date, const char* ybw_end_date){

    const vector<double> &vec_canshu = _config.getCanshuSetValues();

    vector<struct TradingResultNode> ans;
    ans.clear();

    //vector<string> useFileNames;
    vector<vector<string> > useFilesArr;
    useFilesArr.clear();

    vector<string> allPeriods = _config.getAllPeriods();
    for(int i = 0;i < allPeriods.size(); i++){
        //useFileNames.push_back( getFileName(code , allPeriods[i].c_str()));
        useFilesArr.push_back( getFileName(code , allPeriods[i].c_str()));
    }

    for(int i = 0;i < useFilesArr.size();i++){
        //printf("%s\n",useFileNames[i].c_str());
        //obj->loadFile(useFileNames[i].c_str() , ybn_start_date , ybn_end_date);
        obj->loadFiles(useFilesArr[i] , ybn_start_date , ybn_end_date , allPeriods[i].c_str());
        obj->init(code);

        obj->setCanshuFromVec(vec_canshu);
        obj->run();
        ans.push_back(obj->getTradingResult(code , timePeriod ,ybn_start_date , ybn_end_date));

        //obj->loadFile(useFileNames[i].c_str() , ybw_start_date , ybw_end_date);
        obj->loadFiles(useFilesArr[i] , ybw_start_date , ybw_end_date , allPeriods[i].c_str());
        obj->init(code);

        obj->setCanshuFromVec(vec_canshu);
        obj->run();
        ans.push_back(obj->getTradingResult(code , timePeriod , ybw_start_date , ybw_end_date));
    }
    string EveryPeriodFileName = string(__report_dir_path) + obj->getStrategyName() + "/" + code + "-" + "all_period" + "-" + replaceDate(ybn_start_date) + "-" + replaceDate(ybn_end_date) + "-" + replaceDate(ybw_start_date) + "-" + replaceDate(ybw_end_date) + "-Core_index_of_in-sample_and_out-of-sample_in_diff_period.csv";
    printf("%s\n",EveryPeriodFileName.c_str());

    FILE *fp = NULL;
    if( (fp = fopen(EveryPeriodFileName.c_str() , "w")) != NULL){
        char firstLine[400];
        sprintf(firstLine,"%s,%s,%s--%s,%s--%s,%s\n",code, timePeriod , ybn_start_date , ybn_end_date, ybw_start_date , ybw_end_date,_config.getCanshuSetStr().c_str());
        fprintf(fp,"%s",firstLine);
        fprintf(fp,"%s,最佳参数,%s,样本内时间段,%s--%s\n",code,timePeriod,ybn_start_date,ybn_end_date);
        const char* firstLine2 = "----,5min样本内,5min样本外,15min样本内,15min样本外,30min样本内,30min样本外,1hour样本内,1hour样本外";
        fprintf(fp,"%s\n",firstLine2);

        fprintf(fp,"年化收益率%%");
        for(int i = 0;i < ans.size();i++){
            fprintf(fp, ",%.2lf%%", ans[i].rate_of_return * 100.0);
        }fprintf(fp, "\n");

        fprintf(fp,"总开仓次数");
        for(int i = 0;i < ans.size();i++){
            fprintf(fp, ",%.2lf", ans[i].total_open_num * 1.0);
        }fprintf(fp, "\n");

        fprintf(fp,"总盈利");
        for(int i = 0;i < ans.size();i++){
            fprintf(fp, ",%.2lf", ans[i].total_income * 1.0);
        }fprintf(fp, "\n");

        fprintf(fp,"初始资金");
        for(int i = 0;i < ans.size();i++){
            fprintf(fp, ",%.2lf", ans[i].ini_account_money * 1.0);
        }fprintf(fp, "\n");


        fprintf(fp,"盈亏比");
        for(int i = 0;i < ans.size();i++){
            fprintf(fp, ",%.2lf", ans[i].profit_factor);
        }fprintf(fp, "\n");

        fprintf(fp,"胜率");
        for(int i = 0;i < ans.size();i++){
            fprintf(fp, ",%.2lf%%", ans[i].win_rate * 100.0);
        }fprintf(fp, "\n");

        fprintf(fp,"最大回撤");
        for(int i = 0;i < ans.size();i++){
            fprintf(fp, ",%.2lf%%", ans[i].max_windrawal_rate * 100.0);
        }fprintf(fp, "\n");

        fprintf(fp,"索提诺比率");
        for(int i = 0;i < ans.size();i++){
            fprintf(fp, ",%.3lf", ans[i].sortinoRatio);
        }fprintf(fp, "\n");

        fprintf(fp,"目标函数_分数");
        for(int i = 0;i < ans.size();i++){
            fprintf(fp, ",%.3lf", ans[i].targetFunctionRankScore);
        }fprintf(fp, "\n");

        fprintf(fp,"目标函数_PROM");
        for(int i = 0;i < ans.size();i++){
            fprintf(fp, ",%.3lf", ans[i].targetFunctionPromScore);
        }fprintf(fp, "\n");
        fclose(fp);
        reportFileLists.push_back(EveryPeriodFileName);
    }else{
        printf("cannot write file: %s\n",EveryPeriodFileName.c_str());
    }
}

/***
 * 用同一个参数集，对指定的品种跑结果
 * 指定一个参数集，然后 对各个品种的都跑
 ***/
void get_PROM_of_each_future_in_diff_period_report(ctaTemplate *obj ,const char * base_code ,const char *base_period , const char *start_date ,const char *end_date){

    vector<string> allCodes   = _config.getAllFeeCodes();
    vector<string> allPeriods = _config.getAllPeriods();

    const vector<double> &vec_canshu = _config.getCanshuSetValues();
    vector<struct TradingResultNode> ans;

    string every_future_filename = string(__report_dir_path) + obj->getStrategyName() + "/" + "PROM_of_each_future_in_diff_period_(Based_on_"+base_code+"-"+base_period+"-"+replaceDate(start_date)+"-"+replaceDate(end_date)+"_best_parameter).csv";
    FILE *fp = NULL;
    if( (fp = fopen(every_future_filename.c_str(), "w")) != NULL){
        char firstLine[400];
        sprintf(firstLine,"%s,%s,%s--%s,%s\n",base_code, base_period , start_date , end_date,_config.getCanshuSetStr().c_str());
        fprintf(fp,"%s",firstLine);
        fprintf(fp,"%s,最佳参数,%s,样本内时间段,%s,%s\n",base_code,base_period,start_date,end_date);
        fprintf(fp,"---");
        for(int j = 0;j < allPeriods.size();j++){
            fprintf(fp,",%s",allPeriods[j].c_str());
        }fprintf(fp,"\n");
        for(int i = 0;i < allCodes.size();i++){ // 选择品种
            string t_code(allCodes[i]);
            ans.clear();
            for(int j = 0;j < allPeriods.size();j++){
                string timePeriod(allPeriods[j]);
                //string useFile = getFileName(t_code.c_str() , timePeriod.c_str());
                vector<string> useFiles = getFileName(t_code.c_str() , timePeriod.c_str());
                //obj->loadFile(useFile.c_str() , start_date , end_date);
                obj->loadFiles(useFiles , start_date , end_date , timePeriod);
                obj->init(t_code.c_str());

                obj->setCanshuFromVec(vec_canshu);
                obj->run();
                ans.push_back(obj->getTradingResult(base_code , base_period ,start_date , end_date));
            }
            fprintf(fp,"%s",t_code.c_str());
            for(int j = 0;j < allPeriods.size();j++){
                fprintf(fp,",%.2lf",ans[j].targetFunctionPromScore );
            }fprintf(fp,"\n");
        }
        fclose(fp);
        reportFileLists.push_back(every_future_filename);
    }else{
        printf("cannot write file: %s\n",every_future_filename.c_str());
    }
}
/**
 * 使用最优参数集, 将 所有品种，所有时间牛熊阶段, min5,min15,min30都跑一遍
 **/
void get_niuxiong_every_future_every_period_report(ctaTemplate *obj ,const char * base_code ,const char *base_period){

    vector<string> allCodes   = _config.getAllFeeCodes();
    vector<string> allPeriods = _config.getAllPeriods();
    const vector<double> &vec_canshu = _config.getCanshuSetValues();
    vector<struct TradingResultNode> ans;
    ans.clear();
    vector<int> state_list;
    state_list.clear();
    for(int i = 0;i < allCodes.size() ;i++){
        string tcode(allCodes[i]);
        vector<niuxiongNode> vNodes = _config.getNiuXiongNode(allCodes[i].c_str());
        for(int j = 0;j < vNodes.size() ;j++){
            niuxiongNode &node_j = vNodes[j];
            for(int k = 0;k < allPeriods.size();k++){
                string timePeriod(allPeriods[k]);
                //string useFile = getFileName(tcode.c_str() , timePeriod.c_str());
                printf("tcode %s timePeriod %s\n",tcode.c_str(),timePeriod.c_str());
                vector<string> useFiles = getFileName(tcode.c_str() , timePeriod.c_str());
                obj->loadFiles(useFiles , node_j.start_date,  node_j.end_date , timePeriod.c_str());
                obj->init( tcode.c_str());
                obj->setCanshuFromVec(vec_canshu);
                obj->run();

                //printf("running ! \n");
                ans.push_back(obj->getTradingResult( tcode.c_str(), timePeriod.c_str() , node_j.start_date , node_j.end_date));
                state_list.push_back(  node_j.state );

                //printf("why\n");
            }
        }
    }
    string niuxiong_filename = string(__report_dir_path) + obj->getStrategyName() + "/" + "capital_of_each_future_in_diff_period_in_diff_niuxiong_state_(Based_on_"+base_code+"-"+base_period+"-_best_parameter).csv";
    FILE *fp = NULL;

    if ((fp = fopen(niuxiong_filename.c_str(), "w")) != NULL){
        char firstLine[400];
        sprintf(firstLine,"%s,%s,%s\n",base_code, base_period , _config.getCanshuSetStr().c_str());
        fprintf(fp,"%s",firstLine);
        fprintf(fp, "code,time_period,start_date,end_date,state,total_income,win_rate,prom,sharpe,rank_score\n");
        for(int i = 0;i < ans.size();i++){
            //if( 1==1){
                //printf("i=%d\n",i);
            if(ans[i].total_open_num > 50 ){
                fprintf(fp,"%s,%s,%s,%s,%d,%.2lf,%.2lf,%.2lf,%.2lf,%.2lf\n",ans[i].code , ans[i].period,  ans[i].start_date , ans[i].end_date , state_list[i], ans[i].total_income , ans[i].win_rate * 100, ans[i].targetFunctionPromScore, ans[i].sharpe, ans[i].targetFunctionRankScore);
            }
        }
        fclose(fp);
        reportFileLists.push_back(niuxiong_filename);
    }else{
        printf("cannot write file: %s\n",niuxiong_filename.c_str());
    }
}

/**
 * 推进分析结果合并
 **/
TradingResultNode mergePushOnResult( vector<pair<struct TradingResultNode  , struct TradingResultNode > > &vec_ans , const char *_code, const char *_period,const char *_start_date , const char *_end_date ){
    vector<struct tradeNode> t_tradingResult ;
    vector<string> vecTradeday , vecTradeWeekDay;
    vector<double> dayIncome , weekIncome , dayAccount , bzjAccount;
    vector<int> savePosition;
    double account_money = 0.0;
    struct feeNode fNode = _config.getCodeFee(_code);
    t_tradingResult.clear();
    vecTradeday.clear(); vecTradeWeekDay.clear();
    dayIncome.clear(); weekIncome.clear(); dayAccount.clear(); bzjAccount.clear(); savePosition.clear();

    if(vec_ans.size() > 0){
        account_money = vec_ans[0].second.ini_account_money;
    }
    printf("vec_ans size : %d\n",vec_ans.size());
    for(int i = 0;i < vec_ans.size();i++){
        struct TradingResultNode * obj = &(vec_ans[i].second);
        for(int j = 0;j < obj->saveTradingResult.size(); j++){
            t_tradingResult.push_back( obj->saveTradingResult[j]);
        }
        for(int j = 0;j < obj->saveVecTradeDay.size(); j++){
            vecTradeday.push_back(obj->saveVecTradeDay[j]);
        }
        for(int j = 0;j < obj->saveWeekIncome.size(); j++){
            //因为TradingResult中，， 并不需要用到 vecTradeWeekDay ,, 所以。。。直接插入个数据就OK了
            vecTradeWeekDay.push_back("2000/01/01");
            weekIncome.push_back(obj->saveWeekIncome[j]);
        }
        for(int j = 0;j < obj->saveDayIncome.size(); j++){
            if(j == 0){
                dayIncome.push_back(obj->saveDayIncome[j]);
            }else{
                dayIncome.push_back(obj->saveDayIncome[j] - obj->saveDayIncome[j-1]);
            }
        }
        for(int j = 0;j < obj->saveBzjIncome.size();j++){
            bzjAccount.push_back(obj->saveBzjIncome[j]);
        }
        for(int j = 0;j < obj->savePositions.size();j++){
            savePosition.push_back(obj->savePositions[j]);
        }
    }
    double sum_val = 0.0;
    for(int i = 0;i < dayIncome.size();i++){
        sum_val += dayIncome[i];
        dayAccount.push_back(sum_val + account_money);
    }
    double use_rate = fNode.perMoney;
    if( 1 == fNode.feeType ){
        use_rate = use_rate / 100.0;
    }
    return TradingResultNode(_code, _period, _start_date , _end_date , t_tradingResult , t_tradingResult.size() ,  account_money, fNode.minMove , use_rate ,fNode.csize, fNode.cash_deposit, vecTradeday , vecTradeWeekDay , dayIncome , weekIncome , bzjAccount , dayAccount, savePosition , fNode.feeType);
}
/**
 * 推进分析
 * 指定 品种,指定周期,指定开始的时间，结束的时间， 样本外检测时间(几个月)
 * 推进步骤
 1、每次按照优化出最佳参数， 然后将这组最佳参数进行样本外周期检验，
 ***/
void makePushOnReport( ctaTemplate *obj , const char * code ,const char *timePeriod , const char *start_date , const char *end_date , int ybn_months = 36, int ybw_months = 6){

    int cc_all_num = 200;
    string t_start_date(start_date);
    string t_end_date( timeDelayMonths(t_start_date , ybn_months));
    string t_ybw_date( timeDelayMonths(t_end_date , ybw_months));

    vector<pair<struct TradingResultNode  , struct TradingResultNode > > ans;
    ans.clear();
    while(cc_all_num-- > 0){
        if( t_ybw_date.compare(end_date) < 0){
            // string useFile = getFileName(tcode.c_str() , timePeriod.c_str());
            // obj->loadFile( useFile.c_str() , t_start_date.c_str() , t_end_date.c_str());
            // obj->init( tcode.c_str());
            backtesting_assessment_report_and_Other_optimal_parameter_sets_report(obj, code, timePeriod , t_start_date.c_str() , t_end_date.c_str() ,  0.7,   0.2 , 0);
            vector<string> useFiles = getFileName(code, timePeriod);
            obj->loadFiles( useFiles , t_start_date.c_str() , t_end_date.c_str() , timePeriod);
            obj->init( code);
            obj->setCanshuFromVec(_config.getCanshuSetValues());
            obj->run();
            struct TradingResultNode ta = obj->getTradingResult( code, timePeriod , t_start_date.c_str()  , t_end_date.c_str());

            // makeFigureCsv(ta , (string(__report_dir_path) + obj->getStrategyName() + "/figure_trading_result/zzdebug_ybn_" + code + "-" + timePeriod + "-" + replaceDate(t_start_date) + "-" + replaceDate(t_end_date) +"-" + int2str(ybn_months) + "_" + int2str(ybw_months) + "--best-figure-push-on.csv").c_str() , "debug");
            // debug_tradingResult( &ta , (string(__report_dir_path) + obj->getStrategyName() + "/figure_trading_result/zzdebug_ybn_" + code + "-" + timePeriod + "-" + replaceDate(t_start_date) + "-" + replaceDate(t_end_date) + "--best-trading_result-push-on.csv").c_str() ,"debug");

            obj->loadFiles( useFiles , t_end_date.c_str() ,t_ybw_date.c_str() , timePeriod);
            obj->init( code ) ;
            obj->setCanshuFromVec(_config.getCanshuSetValues());
            obj->run();
            struct TradingResultNode tb = obj->getTradingResult( code, timePeriod , t_end_date.c_str()  , t_ybw_date.c_str());

            // makeFigureCsv(tb , (string(__report_dir_path) + obj->getStrategyName() + "/figure_trading_result/zzdebug_ybw_" + code + "-" + timePeriod + "-" + replaceDate(t_end_date) + "-" + replaceDate(t_ybw_date) +"-" + int2str(ybn_months) + "_" + int2str(ybw_months) + "--best-figure-push-on.csv").c_str() , "debug");
            // debug_tradingResult( &tb , (string(__report_dir_path) + obj->getStrategyName() + "/figure_trading_result/zzdebug_ybw_" + code + "-" + timePeriod + "-" + replaceDate(t_end_date) + "-" + replaceDate(t_ybw_date) + "--best-trading_result-push-on.csv").c_str() ,"debug");
            ans.push_back( make_pair(ta, tb));
        }else{
            //printf( "tuijin end!\n");
        }
        t_start_date = timeDelayMonths(t_start_date , tuijinAnalyseMonthNum);
        t_end_date = timeDelayMonths(t_start_date , ybn_months);
        t_ybw_date = timeDelayMonths(t_end_date , ybw_months);
    }

    string fp_filename = string(__report_dir_path) + obj->getStrategyName() + "/" + code + "-" + timePeriod + "-" + replaceDate(start_date)  + "-" + replaceDate(end_date) +"-" + int2str(ybn_months) + "_" + int2str(ybw_months) + "--push_backtesting_report.csv";
    char firstLine[400];
    sprintf(firstLine,"%s,%s,%s--%s,%s\nLabel,目标函数1(净利润)",code, timePeriod , start_date , end_date,"no canshu set");
    vector<TradingResultNode *> tmp_Node; tmp_Node.clear();

    struct TradingResultNode tnnode = mergePushOnResult(ans, code , timePeriod , start_date , end_date );
    tmp_Node.push_back( &tnnode );
    push_on_backtesting_report(tmp_Node , fp_filename.c_str() , firstLine);
    string tradingResultFile = string(__report_dir_path) + obj->getStrategyName() + "/figure_trading_result/" + code + "-" + timePeriod + "-" + replaceDate(start_date) + "-" + replaceDate(end_date) + "--best-trading_result-push-on.csv";
    char firstLineLine[400];
    sprintf(firstLineLine,"%s,%s,%s--%s,%s\n",code, timePeriod , start_date , end_date,tnnode.getCanshuSetStr().c_str());
    debug_tradingResult( &tnnode , tradingResultFile.c_str() ,firstLineLine);
    makeFigureCsv(tnnode , (string(__report_dir_path) + obj->getStrategyName() + "/figure_trading_result/" + code + "-" + timePeriod + "-" + replaceDate(start_date) + "-" + replaceDate(end_date) +"-" + int2str(ybn_months) + "_" + int2str(ybw_months) + "--best-figure-push-on.csv").c_str() , firstLineLine);

    string push_on_filename = string(__report_dir_path) + obj->getStrategyName() + "/" + "PushOnReport_based_on_" + code + "_" + timePeriod + "_" + replaceDate(start_date) + "_" + replaceDate(end_date) + "_" + int2str(ybn_months) + "_" + int2str(ybw_months) + "_" + int2str(tuijinAnalyseMonthNum) + ".csv";
    FILE *fp = NULL;
    if ((fp = fopen(push_on_filename.c_str(), "w")) != NULL){
        char firstLine[400];
        sprintf(firstLine,"%s,%s,%s--%s,%s,%d,%d,%d\n",code, timePeriod , start_date , end_date,_config.getCanshuSetStr().c_str(),ybn_months,ybw_months,tuijinAnalyseMonthNum);
        fprintf(fp,"%s",firstLine);
        fprintf(fp , "---,%s,%s,%s,%s,样本内月数%d,样本外月数%d\n",code,timePeriod,start_date,end_date,ybn_months,ybw_months);
        fprintf(fp , "样本内开始时间,样本内结束时间,样本外开始时间,样本外结束时间,样本内年化收益,样本外年化收益,推进效率,样本外净损益,样本外最大回撤数值,样本外最大回撤比率,胜率,样本内目标分数,样本外目标分数,样本内最优参数\n");
        double rate = 0.0;
        for(int i = 0;i < ans.size();i++){
            // if(i != 0){
            //     printf("%.2lf,%.2lf\n",ans[i].second.rate_of_return , ans[i-1].second.rate_of_return );
            //     rate = ans[i].second.rate_of_return / ans[i-1].second.rate_of_return;
            // }
            //修改推进效率算法，样本外收益除以样本内收益
            rate = ans[i].second.rate_of_return / ans[i].first.rate_of_return;
            fprintf(fp ,"%s,%s,%s,%s,%.2lf%%,%.2lf%%,%.2lf%%,%.3lf,%.2lf,%.2lf%%,%.2lf%%,%lf,%lf,%s\n",
                ans[i].first.start_date , ans[i].first.end_date , ans[i].second.start_date , ans[i].second.end_date ,
                ans[i].first.rate_of_return * 100.0 , ans[i].second.rate_of_return * 100.0 , rate * 100.0 , ans[i].second.total_income, ans[i].second.max_windrawl_number ,
                ans[i].second.max_windrawal_rate * 100.0, ans[i].second.win_rate * 100.0 , ans[i].first.compareValue() , ans[i].second.compareValue() , ans[i].first.getCanshuSetStr().c_str());
        }
        fclose(fp);
        reportFileLists.push_back(push_on_filename);
    }else{
        printf("cannot write file: %s\n",push_on_filename.c_str());
    }
}

/**
 * 多打印几个 样本1，样本2，样本外，他们的收益、分数情况，进而判断是否有效
 */
void printAllYangben(ctaTemplate *obj , const char *code , const char *timePeriod , const char *start_date , const char *end_date, int ybn_months_1 , int ybn_months_2 , int ybw_months , int use_total_cycle = 30){
    string printAllYangben_filename = string(__report_dir_path) + obj->getStrategyName() + "/" + "printAllYangben_" + code + "_" + timePeriod + "_" + replaceDate(start_date) + "_" + replaceDate(end_date) + "_" + int2str(ybn_months_1) + "_" + int2str(ybn_months_2) + "_" + int2str(ybw_months) + "_" + int2str(use_total_cycle) + ".csv";
    vector<struct TradingResultNode> ans;
    string t_start_date(start_date);
    string t_yangben_end_1( timeDelayMonths(t_start_date , ybn_months_1));
    string t_yangben_end_2( timeDelayMonths(t_yangben_end_1 , ybn_months_2));
    string t_ybw_end( timeDelayMonths(t_yangben_end_2 , ybw_months));
    vector<string> useFiles = getFileName(code, timePeriod);
    FILE *fp = NULL;
    if( t_ybw_end.compare(end_date) <= 0){
        if ((fp = fopen(printAllYangben_filename.c_str(), "w")) != NULL){
            fprintf(fp,"样1分数,样2分数,样外分数,样1年化,样2年化,样外年化,样1胜率,样2胜率,样3胜率,样1回撤,样2回撤,样外回撤,样1盈亏比,样2盈亏比,样3盈亏比\n");
            ans.clear();
            while(use_total_cycle-- > 0){
                if(g_choose_searchAlgorithm == &EngineForceAlgorithm){
                    getBestSolution(obj , ans , code , timePeriod , start_date , t_yangben_end_1.c_str());
                    reverse(ans.begin() , ans.end());
                    string st_name = string(obj->getStrategyName());
                    debug_report(ans,(st_name + "_debug_force.csv").c_str(),"debug_force!");
                }
                else if(g_choose_searchAlgorithm == &EngineLiziqunAlgorithm){
                    getBestSolutionUsingLiziQun(obj , ans , code , timePeriod , start_date , t_yangben_end_1.c_str() , lzq_NumParticle , lzq_max_cycle, lzq_C1 , lzq_C2);
                    reverse(ans.begin() , ans.end());
                    string st_name = string(obj->getStrategyName());
                    debug_report(ans,(st_name + "_debug_liziqun.csv").c_str(),"debug_liziqun!");
                }
                else if(g_choose_searchAlgorithm == &EngineBpSearchAlgorithm){
                    getBestSolutionUsingPriority(obj , ans , code , timePeriod , start_date , t_yangben_end_1.c_str() , bp_max_cycle , bp_max_NumParticle);
                    reverse(ans.begin() , ans.end());
                    string st_name = string(obj->getStrategyName());
                    debug_report(ans,(st_name + "_debug_bp_serach.csv").c_str(),"debug_bpsearch!");
                }
                else if(g_choose_searchAlgorithm == &EngineRandomAlgorithm){
                    getBestSolutionUsingRandomFunction( obj, ans , code ,timePeriod , start_date , t_yangben_end_1.c_str() , rm_max_cycle, rm_max_NumParticle , rm_tmp_save_result_num);
                    reverse(ans.begin() , ans.end());
                    string st_name = string(obj->getStrategyName());
                    debug_report(ans,(st_name + "_debug_rm_random_serach.csv").c_str(),"debug_rm_random_search!");
                }
                else{
                    printf("Error , please choose search algorithm first!\n");
                    return;
                }
                if(ans.size() > 0){
                    struct TradingResultNode *best = &(ans[0]);
                    _config.setBestResult( best->canshuSet);
                    obj->loadFiles( useFiles , t_yangben_end_1.c_str()  , t_yangben_end_2.c_str() , timePeriod);
                    obj->init( code);
                    obj->setCanshuFromVec(_config.getCanshuSetValues());
                    obj->run();
                    struct TradingResultNode ta = obj->getTradingResult( code, timePeriod , t_yangben_end_1.c_str()   , t_yangben_end_2.c_str() );

                    obj->loadFiles( useFiles , t_yangben_end_2.c_str()  , t_ybw_end.c_str() , timePeriod);
                    obj->init( code);
                    obj->setCanshuFromVec(_config.getCanshuSetValues());
                    obj->run();
                    struct TradingResultNode tb = obj->getTradingResult( code, timePeriod , t_yangben_end_2.c_str()   , t_ybw_end.c_str() );

                    fprintf(fp , "%.3lf,%.3lf,%.3lf,%.3lf%%,%.3lf%%,%.3lf%%,%.3lf%%,%.3lf%%,%.3lf%%,%.3lf%%,%.3lf%%,%.3lf%%,%.3lf,%.3lf,%.3lf\n",
                        best->targetFunctionRankScore,ta.targetFunctionRankScore,tb.targetFunctionRankScore ,
                        best->rate_of_return * 100 , ta.rate_of_return * 100 , tb.rate_of_return * 100 ,
                        best->win_rate * 100 , ta.win_rate * 100 , tb.win_rate * 100 ,
                        best->max_windrawal_rate * 100, ta.max_windrawal_rate * 100 , tb.max_windrawal_rate * 100,
                        best->profit_factor , ta.profit_factor , tb.profit_factor);
                }
            }
        }else{
            printf("cannot write file: %s\n",printAllYangben_filename.c_str());
        }
    }
}
/**
 * 目标是 寻找到在某个公式下的最高分数值
 */
struct TwoTradingReuslt new_loop_push_backtesting(ctaTemplate * obj , const char *code , const char * timePeriod,const char * start_date , const char * yangben_end1 , const char * yangben_end2 , int total_cycle ){
    vector< struct TwoTradingReuslt > ret_ans;
    vector<struct TradingResultNode> ans;
    ret_ans.clear();
    int use_total_cycle = total_cycle;
    vector<string> useFiles = getFileName(code, timePeriod);
    priority_queue<struct TwoTradingReuslt> priQue;
    while(use_total_cycle-- > 0){
        ans.clear();
        if(g_choose_searchAlgorithm == &EngineForceAlgorithm){
            getBestSolution(obj , ans , code , timePeriod , start_date , yangben_end1);
            reverse(ans.begin() , ans.end());
            string st_name = string(obj->getStrategyName());
            debug_report(ans,(st_name + "_debug_force.csv").c_str(),"debug_force!");
        }
        else if(g_choose_searchAlgorithm == &EngineLiziqunAlgorithm){
            getBestSolutionUsingLiziQun(obj , ans , code , timePeriod , start_date , yangben_end1 , lzq_NumParticle , lzq_max_cycle, lzq_C1 , lzq_C2);
            reverse(ans.begin() , ans.end());
            string st_name = string(obj->getStrategyName());
            debug_report(ans,(st_name + "_debug_liziqun.csv").c_str(),"debug_liziqun!");
        }
        else if(g_choose_searchAlgorithm == &EngineBpSearchAlgorithm){
            getBestSolutionUsingPriority(obj , ans , code , timePeriod , start_date , yangben_end1 , bp_max_cycle , bp_max_NumParticle);
            reverse(ans.begin() , ans.end());
            string st_name = string(obj->getStrategyName());
            debug_report(ans,(st_name + "_debug_bp_serach.csv").c_str(),"debug_bpsearch!");
        }
        else if(g_choose_searchAlgorithm == &EngineRandomAlgorithm){
            getBestSolutionUsingRandomFunction( obj, ans , code ,timePeriod , start_date , yangben_end1 , rm_max_cycle, rm_max_NumParticle , rm_tmp_save_result_num);
            reverse(ans.begin() , ans.end());
            string st_name = string(obj->getStrategyName());
            debug_report(ans,(st_name + "_debug_rm_random_serach.csv").c_str(),"debug_rm_random_search!");
        }
        else{
            printf("Error , please choose search algorithm first!\n");
            noLegalAns = 1;
            TwoTradingReuslt sb;
            return sb;
        }
        if(ans.size() > 0){
            struct TradingResultNode *best = &(ans[0]);
            _config.setBestResult( best->canshuSet);
            obj->loadFiles( useFiles , yangben_end1  , yangben_end2 , timePeriod);
            obj->init( code);
            obj->setCanshuFromVec(_config.getCanshuSetValues());
            obj->run();
            struct TradingResultNode ta = obj->getTradingResult( code, timePeriod , yangben_end1   , yangben_end2 );

            priQue.push( TwoTradingReuslt(*best , ta));
            while( priQue.size() > 10){
                priQue.pop();
            }
        }
    }
    while(!priQue.empty()){
        ret_ans.push_back(priQue.top());
        priQue.pop();
    }
    reverse(ret_ans.begin() , ret_ans.end());
    string st_name = string(obj->getStrategyName());

    //string two_loop_push_on_filename = string(__report_dir_path) + obj->getStrategyName() + "/" + "TwoLoopReport_based_on_" + code + "_" + timePeriod + "_" + replaceDate(start_date) + "_" + replaceDate(end_date) + "_" + int2str(ybn_months_1) + "_" + int2str(ybn_months_2) + "_" + int2str(ybw_months) + "_" + int2str(use_tuijin_months) + "_" + int2str(use_total_cycle) + ".csv";
    string debug_loop_filename = string(__report_dir_path) + st_name + "/" + "TowLoop_back_result_" + code + "_" + timePeriod + "_" + replaceDate(start_date) + "_" + replaceDate(yangben_end1) + "_" + replaceDate(yangben_end2) + "_" + int2str(total_cycle) + ".csv";
    debug_two_loop_report( ret_ans , (st_name + "_debug_two_loop_ans.csv").c_str(),(string(start_date) + "_" + replaceDate(yangben_end1) + "_" + replaceDate(yangben_end2) + "_" + int2str(total_cycle)).c_str());
    if(ret_ans.size() > 0){
        return ret_ans[0];
    }else{
        noLegalAns = 1;
        TwoTradingReuslt sb;
        return sb;
    }
}
/**
 * 推进分析
 * 指定 品种,指定周期,指定开始的时间，样本一结束的时间，样本二结束的时间， 样本外的时间
 * 推进步骤
   对每组时间，求出样本一的分数，样本二的分数， 样本外的收益
 **/
void makePushOnThreePart(ctaTemplate *obj , const char *code ,const char *timePeriod , const char *start_date , const char *end_date , int ybn_months_1 = 18 , int ybn_months_2 = 18 , int ybw_months = 6 , int use_tuijin_months = 6 , int use_total_cycle = 10){
    int cc_all_num = 200;
    string t_start_date(start_date);
    string t_yangben_end_1( timeDelayMonths(t_start_date , ybn_months_1));
    string t_yangben_end_2( timeDelayMonths(t_yangben_end_1 , ybn_months_2));
    string t_ybw_end( timeDelayMonths(t_yangben_end_2 , ybw_months));
    vector< pair<struct TwoTradingReuslt , struct TradingResultNode> > ans;
    ans.clear();
    while(cc_all_num-- > 0){
        if( t_ybw_end.compare(end_date) < 0){
            noLegalAns = 0;
            struct TwoTradingReuslt out = new_loop_push_backtesting(obj , code , timePeriod ,  t_start_date.c_str() , t_yangben_end_1.c_str() , t_yangben_end_2.c_str() , use_total_cycle);
            vector<string> useFiles = getFileName(code, timePeriod);
            obj->loadFiles( useFiles , t_yangben_end_2.c_str() , t_ybw_end.c_str() , timePeriod);
            obj->init( code);
            obj->setCanshuFromVec(_config.getCanshuSetValues());
            obj->run();
            struct TradingResultNode ta = obj->getTradingResult( code, timePeriod , t_yangben_end_2.c_str()  , t_ybw_end.c_str());
            if(1 == noLegalAns ){
                printf("There is no legal result between %s , %s , %s\n", t_start_date.c_str() , t_yangben_end_1.c_str() , t_yangben_end_2.c_str());
            }else{
                ans.push_back(  make_pair( out, ta));
            }
        }else{
            printf( "tuijin end!\n");
            break;
        }
        t_start_date = timeDelayMonths(t_start_date , use_tuijin_months);
        t_yangben_end_1 = timeDelayMonths(t_yangben_end_1 , use_tuijin_months);
        t_yangben_end_2 = timeDelayMonths(t_yangben_end_2 , use_tuijin_months);
        t_ybw_end = timeDelayMonths(t_ybw_end , use_tuijin_months);
    }
    string two_loop_push_on_filename = string(__report_dir_path) + obj->getStrategyName() + "/" + "TwoLoopReport_based_on_" + code + "_" + timePeriod + "_" + replaceDate(start_date) + "_" + replaceDate(end_date) + "_" + int2str(ybn_months_1) + "_" + int2str(ybn_months_2) + "_" + int2str(ybw_months) + "_" + int2str(use_tuijin_months) + "_" + int2str(use_total_cycle) + ".csv";
    FILE *fp = NULL;
    if ((fp = fopen(two_loop_push_on_filename.c_str(), "w")) != NULL){
        char firstLine[400];
        sprintf(firstLine,"%s,%s,%s--%s,%s,%d,%d,%d,%d,%d\n",code, timePeriod , start_date , end_date,_config.getCanshuSetStr().c_str(),ybn_months_1,ybn_months_2,ybw_months,use_tuijin_months,use_total_cycle);
        fprintf(fp,"%s",firstLine);
        fprintf(fp , "---,%s,%s,%s,%s,样本1内月数%d,样本2内月数%d,样本外月数%d,推进月数%d,循环次数%d\n",code,timePeriod,start_date,end_date,ybn_months_1,ybn_months_2,ybw_months,use_tuijin_months,use_total_cycle);
        fprintf(fp , "样本1内开始时间,样本1内结束时间,样本2内开始时间,样本2内结束时间,样本外开始时间,样本外结束时间,样本1内年化收益,样本2内年化收益,样本外年化收益,样本外最大回撤比率,样本外胜率,样本1内目标分数,样本2内目标分数,样本外目标分数,样本内最优参数\n");
        for(int i = 0;i < ans.size();i++){
            fprintf(fp,"%s,%s,%s,%s,%s,%s,%.2lf%%,%.2lf%%,%.2lf%%,%.2lf%%,%.2lf%%,%.4lf,%.4lf,%.4lf,%s\n",ans[i].first.n1.start_date,ans[i].first.n1.end_date,ans[i].first.n2.start_date,ans[i].first.n2.end_date,ans[i].second.start_date,ans[i].second.end_date,
                ans[i].first.n1.rate_of_return * 100.0,ans[i].first.n2.rate_of_return * 100.0 , ans[i].second.rate_of_return * 100.0 ,ans[i].second.max_windrawal_rate * 100.0 , ans[i].second.win_rate * 100.0, ans[i].first.n1.compareValue() , ans[i].first.n2.compareValue(),ans[i].second.compareValue(),ans[i].first.n1.getCanshuSetStr().c_str());
        }
        fclose(fp);
        reportFileLists.push_back(two_loop_push_on_filename);
    }else{
        printf("cannot write file: %s\n",two_loop_push_on_filename.c_str());
    }
}
/**
 *  对所有品种跑一遍初筛的报告
 *  对每个品种，跑这个周期的最优解，循环跑4遍， 记录样本内最优收益/最优回撤 的最好解

 ****/
void makeFirstFilterReport( ctaTemplate * obj, const char *timePeriod , const char *end_date , int loop_times = 4){
    vector<struct TradingResultNode> ans;
    vector<string> outputFileLines;  outputFileLines.clear();
    char outLine[1000] , firstLineLine[1000] , outFileLine[1000];
    FILE * fout_ybw = NULL;

    sprintf(outFileLine , "./simpleNeedReport/debug_%s_%s.csv", timePeriod , obj->getStrategyName().c_str());
    fout_ybw = fopen(outFileLine , "w");

    for(int i = 0;i < _config._pinzhongNodes.size();i++){
        pinzhongNode p = _config._pinzhongNodes[i];
        struct TradingResultNode need_print_trading_result;
        int ans_changes = 0;
        double bef_best_ans = -1000.0;
        string output_start_date = replaceDate(string(p.our_start_date));
        string output_end_date   = replaceDate(string(end_date));
        for(int j = 0;j < loop_times;j++ ){
            ans.clear();
            if(g_choose_searchAlgorithm == &EngineForceAlgorithm){
                getBestSolution(obj , ans , p.code , timePeriod , p.our_start_date , end_date);
                reverse(ans.begin() , ans.end());
                string st_name = string(obj->getStrategyName());
                debug_report(ans,(st_name + "_debug_force.csv").c_str(),"debug_force!");
            }
            else if(g_choose_searchAlgorithm == &EngineLiziqunAlgorithm){
                getBestSolutionUsingLiziQun(obj , ans , p.code , timePeriod , p.our_start_date , end_date , lzq_NumParticle , lzq_max_cycle, lzq_C1 , lzq_C2);
                reverse(ans.begin() , ans.end());
                string st_name = string(obj->getStrategyName());
                debug_report(ans,(st_name + "_debug_liziqun.csv").c_str(),"debug_liziqun!");
            }
            else if(g_choose_searchAlgorithm == &EngineBpSearchAlgorithm){
                getBestSolutionUsingPriority(obj , ans , p.code , timePeriod , p.our_start_date , end_date , bp_max_cycle , bp_max_NumParticle);
                reverse(ans.begin() , ans.end());
                string st_name = string(obj->getStrategyName());
                debug_report(ans,(st_name + "_debug_bp_serach.csv").c_str(),"debug_bpsearch!");
            }
            else if(g_choose_searchAlgorithm == &EngineRandomAlgorithm){
                getBestSolutionUsingRandomFunction( obj, ans , p.code ,timePeriod , p.our_start_date , end_date , rm_max_cycle, rm_max_NumParticle , rm_tmp_save_result_num);
                reverse(ans.begin() , ans.end());
                string st_name = string(obj->getStrategyName());
                debug_report(ans,(st_name + "_debug_rm_random_serach.csv").c_str(),"debug_rm_random_search!");
            }
            else{
                printf("Error , please choose search algorithm first!\n");
                return;
            }
            printf("total ans size:%d\n",ans.size());
            if(ans.size() > 0){
                struct TradingResultNode *best = &(ans[0]);
                // sprintf(firstLineLine,"%s,%s,%s--%s,%s\n",p.code, timePeriod , output_start_date.c_str()  , output_end_date.c_str()  ,need_print_trading_result.getCanshuSetStr().c_str());
                // makeFigureCsv(*best , (string(__report_dir_path) + obj->getStrategyName() + "/figure_trading_result/" + p.code + "-" + timePeriod + "-" + output_start_date + "-" + output_end_date + "-filter_loop-"+int2str(j)+"--best-figure.csv").c_str() , firstLineLine);
                //double now_total_income = best->total_income , now_with_draw = best->max_windrawl_number;
                //double use_rate = now_total_income * 1.0 / fabs(now_with_draw);
                double use_rate = best->compareValue();
                if(use_rate > bef_best_ans){
                    bef_best_ans = use_rate;
                    need_print_trading_result = *best;
                    ans_changes += 1;
                }
            }
            //printf("!!!!!! ans.size %d\n",1 * ans.size());
        }
        if( ans_changes > 0){
            sprintf(firstLineLine,"%s,%s,%s--%s,%s\n",p.code, timePeriod , output_start_date.c_str() , output_end_date.c_str() ,need_print_trading_result.getCanshuSetStr().c_str());
            //sprintf(firstLineLine , "%s,%s,%s--%s,%s\n参数项目,%s,capital",p.code, timePeriod , output_start_date.c_str() , output_end_date.c_str() ,need_print_trading_result.getCanshuSetStr().c_str(), obj->getCanshuSetNamesString().c_str());
            makeFigureCsv(need_print_trading_result , (string(__report_dir_path) + obj->getStrategyName() + "/figure_trading_result/" + p.code + "-" + timePeriod + "-" + output_start_date + "-" + output_end_date + "-filter_loop-"+"--print--"+"--best-figure.csv").c_str() , firstLineLine);
            double use_rate = need_print_trading_result.rate_of_return * 1.0 / fabs(need_print_trading_result.max_windrawal_rate);
            sprintf(outLine,"%s,%s,%s,%s,%.2lf%%,%.2lf%%,%.2lf,%.2lf",p.code,timePeriod,p.our_start_date , end_date , need_print_trading_result.rate_of_return * 100.0 ,need_print_trading_result.max_windrawal_rate * 100.0 ,need_print_trading_result.compareValue(),use_rate );
            outputFileLines.push_back( outLine);

            if( fout_ybw != NULL){
                fprintf(fout_ybw , "%s %s %s %s %s\n", p.code , timePeriod , "2017/01/01","2017/10/13", need_print_trading_result.getCanshuSetStr(",").c_str());
            }
        }

        //break;
        // vector<string> useFiles = getFileName( p.code , timePeriod);
        // obj->loadFiles( useFiles , p.our_start_date , end_date , timePeriod);
    }
    if(outputFileLines.size() > 0){
        FILE * fp = NULL;
        string outputFirstFilterFileName = string(__report_dir_path) + obj->getStrategyName() + "/" + "FirstFilter_" + timePeriod + "_" + replaceDate(end_date) + + "_" + int2str(loop_times) + ".csv";
        if( (fp = fopen(outputFirstFilterFileName.c_str() , "w")) != NULL){
            fprintf(fp , "初筛结束时间:%s,筛选最优解次数:%d\n", end_date , loop_times);
            fprintf(fp , "code,timePeriod,start_time,end_time,ybn_year_rate,ybn_winthdraw_rate,use_score,income/with_draw\n");
            for(int i = 0;i < outputFileLines.size() ;i++){
                fprintf(fp , "%s\n", outputFileLines[i].c_str());
            }
            fclose(fp);
            reportFileLists.push_back(outputFirstFilterFileName);
        }else{
            printf("cannot write file: %s\n",outputFirstFilterFileName.c_str());
        }
    }else{
        printf("makeFirstFilterReport result is None!\n");
    }

    if( fout_ybw != NULL){
        fclose(fout_ybw);
    }
}

/**
 * 制作简单的策略回测报告
 *******/
void makeSimpleReport(ctaTemplate * obj , const char *fileName, const char *outFileName){
    FILE *fp = NULL;
    FILE *out_fp = NULL;
    char firstLine[200];
    char symbol[30] , start_date[40] , end_date[40] , period[40] , canshuSet[200];
    if( (fp = fopen(fileName , "r")) != NULL){
        out_fp = fopen( outFileName , "w");

        while( fscanf(fp , "%s %s %s %s %s",symbol,period,start_date,end_date,canshuSet) != EOF){
            vector<double> now_cansh_j = getParams( canshuSet , ",");
            vector<string> filenames = getFileName( symbol , period);
            obj->loadFiles(filenames , start_date , end_date , period);
            obj->init(symbol);
            obj->setCanshuFromVec(now_cansh_j);
            obj->run();

            sprintf( firstLine , "%s,%s_income\n", symbol , symbol);
            struct TradingResultNode tnode = obj->getTradingResult(symbol , period ,start_date , end_date);
            makeFigureCsv(tnode, (string(__report_dir_path) + obj->getStrategyName() + "/figure_trading_result/simple_report_" + symbol + "-" + period + "-" + replaceDate(string(start_date)) + "-" + replaceDate(string(end_date)) + "--best-figure.csv").c_str() , firstLine);
            string tradingResultFile = string(__report_dir_path) + obj->getStrategyName() + "/figure_trading_result/simple_report_" + symbol + "-" + period + "-" + replaceDate(string(start_date)) + "-" + replaceDate(string(end_date)) + "--best-trading_result.csv";
            debug_tradingResult( &tnode , tradingResultFile.c_str() , firstLine);

            fprintf(out_fp, "%s,%s,%s,%s,%.2lf%%,%.2lf%%,%.2lf,%.2lf,%.2lf%%\n" ,symbol, tnode.getCanshuSetStr(" ").c_str() ,start_date , end_date , tnode.rate_of_return*100.0 , tnode.max_windrawal_rate * 100.0 , tnode.targetFunctionPromScore, tnode.targetFunctionRankScore , (tnode.net_profits + tnode.net_loss) / fabs( tnode.max_windrawl_number ) * 100);
        }
        fclose(fp);
        fclose(out_fp);
    }
}
/**
 *  最佳化参数
 *  对某个策略，针对所有品种，所有周期调整出最优参数
 ******/
void makeBestCanshuUsingOneAlgorithm( ctaTemplate * obj , const char *start_date , const char *end_date ){
    vector<string> allCodes   = _config.getAllFeeCodes();
    vector<string> allPeriods = _config.getAllPeriods();
    vector<struct TradingResultNode > ans;
    ans.clear();
    for(int i = 0;i < allCodes.size() ;i++){
        for(int j = 0;j < allPeriods.size(); j++){
            backtesting_assessment_report_and_Other_optimal_parameter_sets_report(obj, allCodes[i].c_str(), allPeriods[j].c_str() , start_date , end_date ,  0.7,   0.2 , 0);
            vector<string> useFiles = getFileName(allCodes[i].c_str() , allPeriods[j].c_str());
            obj->loadFiles( useFiles , start_date, end_date , allPeriods[j]);
            obj->init(allCodes[i].c_str());
            obj->setCanshuFromVec(_config.getCanshuSetValues());
            obj->run();
            ans.push_back(obj->getTradingResult( allCodes[i].c_str(), allPeriods[j].c_str() , start_date , end_date ));
        }
    }
    string best_canshu_filename = string(__report_dir_path) + obj->getStrategyName() + "/" + "best_canshu_"  + replaceDate(start_date) + "_" + replaceDate(end_date) + ".csv";
    FILE *fp = NULL;
    if ((fp = fopen(best_canshu_filename.c_str(), "w")) != NULL){
        char firstLine[400];
        sprintf(firstLine,"%s--%s\n",start_date , end_date);
        fprintf(fp,"%s",firstLine);
        fprintf(fp,"code,period,start_date,end_date,%s\n",obj->getCanshuSetNamesString().c_str());
        for(int i = 0;i < ans.size() ;i++){
            fprintf(fp , "%s,%s,%s,%s",ans[i].code, ans[i].period , ans[i].start_date , ans[i].end_date);
            for(int j = 0;j < ans[i].canshuSet.size(); j++){
                fprintf(fp,",%lf",ans[i].canshuSet[j]);
            }
            fprintf(fp,",%.2lf\n",ans[i].compareValue());
        }
        fclose(fp);
        reportFileLists.push_back(best_canshu_filename);
    }else{
        printf("cannot write file: %s\n",best_canshu_filename.c_str());
    }
}


#include "InvestmentProtfolio.h"
/**
 * 通过 combination 文件, 组合回测
19 2015/9/27
j min30 3 5 9 1 4 9 17 3 9.32035 15.9689 0 4 19.0477 19.1264 1 19.1362 19.5105 2.30146 5
rb min30 3 4 7 1 2 7 13 9 7.07517 16.3113 0 4 19.4789 19.87 1 19.0312 19.7609 2.74616 5
ma min30 3 9 18 2 4 6 11 9 10.9297 9.71074 0 4 19.9287 19.7351 1 19.3767 19.8285 2.96234 5
oi min30 3 4 8 1 4 8 17 15 1.78038 12.2253 0 4 19.0187 19.1352 1 19.1075 19.7048 2.63259 5
rs min30 3 4 5 1 2 7 15 1 2.12205 2.10931 0 4 19.6176 19.6979 1 19.3085 19.5257 2.59493 5
sr min30 3 8 24 1 3 7 15 15 5.76119 6.44307 0 4 19.8843 19.1846 1 19.5291 19.6048 2.79536 5
ta min30 3 6 25 1 2 7 16 8 6.40822 18.2596 0 4 19.3423 19.121 1 19.5758 19.8154 2.74626 5
 */
void makeCombinationReport( const char *filePath , const double will_use_ini_money , const char *end_date , const char *preName , const char *zuheFirstLine){
    char start_date[200] , tmp_code[200] , tmp_period[200];
    int num_all;
    double canshu_i;
    __const_ini_money = will_use_ini_money;
    //AligatorStrategy t1 = AligatorStrategy();
    LivermoreStrategyTest t1 = LivermoreStrategyTest();

    is_zuhehuice = 1;
    InvestmentProtfolio investment;
    investment.clear();

    FILE *fp = NULL;
    if( (fp = fopen(filePath , "r")) != NULL){
        fscanf(fp , "%d %s",&num_all , start_date);
        while( fscanf(fp , "%s %s" , tmp_code , tmp_period) != EOF){
            printf("%s %s\n",tmp_code , tmp_period);
            vector<double> ncanshu;  ncanshu.clear();
            for(int i = 0;i < num_all;i++){
                fscanf(fp,"%lf",&canshu_i);
                ncanshu.push_back( canshu_i);

                printf("%lf ",canshu_i);
            }printf("\n");

            protfolioNode a = protfolioNode(&t1 , tmp_code , tmp_period , ncanshu , will_use_ini_money) ;
            investment.addPro(a);
        }
        fclose(fp);

        investment.run2(start_date , end_date);

        investment.exportReport( preName , zuheFirstLine );
    }else{
        printf("cannot write file: %s\n",filePath);
    }

    is_zuhehuice = 0;
}

/**
 * 通过 combination_continue 文件, 进行组合回测
    6 3 2010/03/27 2010/06/27
    if min1 0.028 0.015333 0.02 0 200 200
    ic min1 0.03 0.02 0.015 0.01 200 200
    ih min1 0.028 0.009333 0.025 0.015 200 200

    6 3 2010/06/28 2010/09/27
    if min1 0.028 0.015333 0.02 0 200 200
    ic min1 0.03 0.02 0.015 0.01 200 200
    ih min1 0.028 0.009333 0.025 0.015 200 200
 **/
void makeContinueCombinationReport( const char *filePath , const double will_use_ini_money , const char *preName , const char *zuheFirstLine){
    char start_date[200] , end_date[200] , tmp_code[200] , tmp_period[200] , char_end_date[200];
    FILE *fp = NULL;
    is_zuhehuice = 1;
    int param_num , pinzhong_num;
    double canshu_i;
    vector<double> vec_ini_use_money;
    InvestmentProtfolio investment;
    LivermoreStrategyTest t1 = LivermoreStrategyTest();
    vec_ini_use_money.clear();
    if( (fp = fopen(filePath , "r")) != NULL){
        while( fscanf(fp , "%d %d %s %s",&param_num , &pinzhong_num , start_date , end_date) != EOF){
            // printf("param_num %d\n" , param_num);
            // printf("pinzhong_num %d\n" , pinzhong_num);
            investment.clear();
            for(int i = 0;i < pinzhong_num;i++){
                fscanf(fp , "%s %s" , tmp_code , tmp_period);
                printf("%s %s\n", tmp_code , tmp_period);
                vector<double> ncanshu; ncanshu.clear();
                for(int j = 0;j < param_num;j++){
                    fscanf(fp , "%lf", &canshu_i);
                    ncanshu.push_back(canshu_i);
                }
                for(int j = 0;j < ncanshu.size();j++){
                    printf("%lf ",ncanshu[j]);
                }printf("\n");
                if(vec_ini_use_money.size() > i){
                    protfolioNode a = protfolioNode(&t1 , tmp_code , tmp_period ,  ncanshu , vec_ini_use_money[i] );
                    investment.addPro(a);
                }else{
                    protfolioNode a = protfolioNode(&t1 , tmp_code , tmp_period , ncanshu , will_use_ini_money );
                    investment.addPro(a);
                }
            }
            investment.run2(start_date , end_date);
            //investment.exportReport( preName , zuheFirstLine );
            investment.saveToNowReport();

            vec_ini_use_money.clear();
            investment.getSaveResultMoney( vec_ini_use_money);
            investment.exportSaveFigueReport( preName);
        }
        fclose(fp);
    }else{
        printf("cannot write file: %s\n",filePath);
    }
    is_zuhehuice = 0;
}


/**
 *  对所有 rqalpha 上的股票，跑一遍筛选
 *  对每个品种，跑这个周期的最优解，循环跑4遍， 记录样本内最优收益/最优回撤 的最好解
  如果 use_yongan_data == 3 , 那么
 ****/
void makeFirstFilterStockReport( ctaTemplate * obj, const char *timePeriod , const char * start_date , const char *end_date ,  int loop_times = 1){
    vector<struct TradingResultNode> ans;
    vector<string> outputFileLines;  outputFileLines.clear();
    char outLine[1000] , firstLineLine[1000];
    vector<string> *useStockCodes = &(_config._all_rqalpha_codes);
    if( 3 == use_yongan_data){
        useStockCodes = &(_config._all_plate_codes);
    }
    for(int i = 0;i < useStockCodes->size();i++){
        string stock_code = (*useStockCodes)[i];
        struct TradingResultNode need_print_trading_result;
        int ans_changes = 0;
        double bef_best_ans = -1000.0;
        string output_start_date = replaceDate(string(start_date));
        string output_end_date   = replaceDate(string(end_date));
        for(int j = 0;j < loop_times;j++ ){
            ans.clear();
            if(g_choose_searchAlgorithm == &EngineForceAlgorithm){
                getBestSolution(obj , ans , stock_code.c_str() , timePeriod , start_date , end_date);
                reverse(ans.begin() , ans.end());
                string st_name = string(obj->getStrategyName());
                debug_report(ans,(st_name + "_debug_force.csv").c_str(),"debug_force!");
            }
            else if(g_choose_searchAlgorithm == &EngineLiziqunAlgorithm){
                getBestSolutionUsingLiziQun(obj , ans , stock_code.c_str() , timePeriod , start_date , end_date , lzq_NumParticle , lzq_max_cycle, lzq_C1 , lzq_C2);
                reverse(ans.begin() , ans.end());
                string st_name = string(obj->getStrategyName());
                debug_report(ans,(st_name + "_debug_liziqun.csv").c_str(),"debug_liziqun!");
            }
            else if(g_choose_searchAlgorithm == &EngineBpSearchAlgorithm){
                getBestSolutionUsingPriority(obj , ans , stock_code.c_str() , timePeriod , start_date , end_date , bp_max_cycle , bp_max_NumParticle);
                reverse(ans.begin() , ans.end());
                string st_name = string(obj->getStrategyName());
                debug_report(ans,(st_name + "_debug_bp_serach.csv").c_str(),"debug_bpsearch!");
            }
            else if(g_choose_searchAlgorithm == &EngineRandomAlgorithm){
                getBestSolutionUsingRandomFunction( obj, ans , stock_code.c_str() ,timePeriod , start_date , end_date , rm_max_cycle, rm_max_NumParticle , rm_tmp_save_result_num);
                reverse(ans.begin() , ans.end());
                string st_name = string(obj->getStrategyName());
                debug_report(ans,(st_name + "_debug_rm_random_serach.csv").c_str(),"debug_rm_random_search!");
            }
            else{
                printf("Error , please choose search algorithm first!\n");
                return;
            }
            printf("total ans size:%d\n",ans.size());
            if(ans.size() > 0){
                struct TradingResultNode *best = &(ans[0]);
                double use_rate = best->compareValue();
                if(use_rate > bef_best_ans){
                    bef_best_ans = use_rate;
                    need_print_trading_result = *best;
                    ans_changes += 1;
                }
            }
            //printf("!!!!!! ans.size %d\n",1 * ans.size());
        }
        if( ans_changes > 0){
            sprintf(firstLineLine,"%s,%s,%s--%s,%s\n",stock_code.c_str(), timePeriod , output_start_date.c_str() , output_end_date.c_str() ,need_print_trading_result.getCanshuSetStr().c_str());
            //sprintf(firstLineLine , "%s,%s,%s--%s,%s\n参数项目,%s,capital",p.code, timePeriod , output_start_date.c_str() , output_end_date.c_str() ,need_print_trading_result.getCanshuSetStr().c_str(), obj->getCanshuSetNamesString().c_str());
            makeFigureCsv(need_print_trading_result , (string(__report_dir_path) + obj->getStrategyName() + "/figure_trading_result/" + stock_code.c_str() + "-" + timePeriod + "-" + output_start_date + "-" + output_end_date + "-filter_loop-"+"--print--"+"--best-figure.csv").c_str() , firstLineLine);
            double use_rate = need_print_trading_result.rate_of_return * 1.0 / fabs(need_print_trading_result.max_windrawal_rate);
            sprintf(outLine,"%s,%s,%s,%s,%.2lf%%,%.2lf%%,%.2lf,%.2lf",stock_code.c_str(),timePeriod,start_date , end_date , need_print_trading_result.rate_of_return * 100.0 ,need_print_trading_result.max_windrawal_rate * 100.0 ,need_print_trading_result.compareValue(),use_rate );
            outputFileLines.push_back( outLine);
        }

    }
    if(outputFileLines.size() > 0){
        FILE * fp = NULL;
        string outputFirstFilterFileName = string(__report_dir_path) + obj->getStrategyName() + "/" + "FirstFilter_" + timePeriod + "_" + replaceDate(end_date) + + "_" + int2str(loop_times) + ".csv";
        if( (fp = fopen(outputFirstFilterFileName.c_str() , "w")) != NULL){
            fprintf(fp , "初筛结束时间:%s,筛选最优解次数:%d\n", end_date , loop_times);
            fprintf(fp , "code,timePeriod,start_time,end_time,ybn_year_rate,ybn_winthdraw_rate,use_score,income/with_draw\n");
            for(int i = 0;i < outputFileLines.size() ;i++){
                fprintf(fp , "%s\n", outputFileLines[i].c_str());
            }
            fclose(fp);
            reportFileLists.push_back(outputFirstFilterFileName);
        }else{
            printf("cannot write file: %s\n",outputFirstFilterFileName.c_str());
        }
    }else{
        printf("makeFirstFilterStockReport result is None!\n");
    }
}


/**
 *  对相关性 比较明显的品种，进行组合回测，得到最优参数
 *  制作组合回测报告
 **/
void makeProtfolioReport( vector<struct investment_result_node> &investResult, ctaTemplate * obj , vector< string > &code_list , const char *timePeriod , const char * start_date , const char *end_date , const char *firstLine){
    is_zuhehuice = 1;
    __const_ini_money = 500000;
    InvestmentProtfolio investment;
    char ss[1000];

    investResult.clear();
    // 生成需要的  遍历参数文件
    obj->loopForce();
    pair<int,string> tmp_loopForceCanshu = obj->loopForceCanshu;
    FILE *fp = NULL;

    if( (fp = fopen(tmp_loopForceCanshu.second.c_str(), "r")) != NULL){

        while( fscanf(fp,"%s",ss) != EOF){
            vector<double> now_cansh = getParams(ss , ",");
            investment.clear();
            for(int i = 0;i < code_list.size();i++){
                protfolioNode a = protfolioNode(obj , code_list[i], timePeriod , now_cansh , __const_ini_money) ;
                investment.addPro(a);
            }

            investment.loadFile(start_date , end_date);
            investment.init();
            investment.run2(start_date , end_date);

            double score = investment.getInvestmentResult();
            investResult.push_back( investment_result_node( now_cansh , score));
        }
        fclose(fp);
        // 从大道小排序
        sort(investResult.begin() , investResult.end());
    }else{
        printf("running! open loopforce error!\n");
    }
}

#endif
