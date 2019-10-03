
#include "spreadStrategyTemplate.h"
#include "spread_engine.h"
#ifndef _report_spread
#define _report_spread

/**
 * 获得错误调试报告
 ****/
void spread_debug_report(vector<mergeAllTradingResult > &v_report , const char * filename ,const char * firstLine){
    FILE *fp = NULL;
    if( (fp = fopen(filename, "w")) != NULL){
        fprintf(fp , "%s, total_answer_num:%d\n" , firstLine,allResultNum );
        if(v_report.size() > 0){
            for(int i = 0;i < v_report.size();i++){
                fprintf(fp,"%d,",i);
                struct mergeAllTradingResult * t = &v_report[i];
                for(int j = 0;j < t->canshuSet.size(); j++){
                    fprintf(fp,"%.3lf,",t->canshuSet[j]);
                }
                fprintf(fp,"%.5lf\n", t->compareValue() );
                //fprintf(fp,"%d\n",checkLegalCanshu(t->canshuSet));
            }
        }
        fclose(fp);
    }else{
        printf("cannot write file: %s\n",filename);
    }
}

// debug 日志
void spread_debug_tradingResult_test( struct TradingResultNode *pvec, const char *filename , const char *firstLine){
    FILE *fp = NULL;
    if( (fp = fopen(filename , "w")) != NULL){
        fprintf(fp,"%s",firstLine);
        fprintf(fp,"symbol_code,entryTime,exitTime,entryPrice,exitPrice,volumeDirection,turnover,commission,per_slippage,pnl\n");
        int llen = pvec->saveTradingResult.size();
        for(int i = 0;i < llen;i++){
            fprintf(fp, "%s,%s,%s,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",pvec->saveTradingResult[i].symbol_code ,pvec->saveTradingResult[i].entryTime,pvec->saveTradingResult[i].exitTime,
                        pvec->saveTradingResult[i].entryPrice,pvec->saveTradingResult[i].exitPrice,pvec->saveTradingResult[i].volume,
                        pvec->saveTradingResult[i].turnover , pvec->saveTradingResult[i].commission , pvec->saveTradingResult[i].per_slippage,pvec->saveTradingResult[i].pnl );
        }
        // debug other info
        fprintf(fp, "\n" );
        llen = pvec->saveDayIncome.size();
        // printf("income llen = %d\n",llen);
        // for(int i = 0;i < llen;i++){
        //     fprintf(fp, "saveDayIncome[%d] = %lf\n", i, pvec->saveDayIncome[i]);
        //     // printf( "saveDayIncome[%d] = %lf\n", i, pvec->saveDayIncome[i] );
        // }

        // llen = pvec->saveBzjIncome.size();
        // printf("bzj llen = %d\n" , llen);

        // llen = pvec->savePositions.size();
        // printf("positions llen = %d\n",llen);

        for(int i = 0;i < llen;i++){
            fprintf(fp, "tradeDay[%d]=%s,saveDayIncome[%d] = %lf,saveBzjIncome[%d]=%lf ,savePositions[%d] = %d\n", i, pvec->saveVecTradeDay[i].c_str() ,i, pvec->saveDayIncome[i],i,pvec->saveBzjIncome[i] , i, pvec->savePositions[i]);
        }

        llen = pvec->saveWeekIncome.size();
        for(int i = 0;i < llen;i++){
            fprintf(fp, "saveWeekIncome[%d]=%lf\n",i,pvec->saveWeekIncome[i]);
        }

        fclose(fp);
    }else{
        printf("cannot write file: %s\n",filename);
    }
}

/**
 * 导出价差的交易总情况
 **/
void spread_debug_tradingResult( struct mergeAllTradingResult *pvec, const char *filename , const char *firstLine){
    FILE *fp = NULL;
    if( (fp = fopen(filename , "w")) != NULL){
        fprintf(fp,"%s",firstLine);
        fprintf(fp,"symbol_code,entryTime,exitTime,entryPrice,exitPrice,volumeDirection,turnover,commission,per_slippage,pnl\n");
        int llen = pvec->tot_saveTradingResult.size();
        for(int i = 0;i < llen;i++){
            fprintf(fp, "%s,%s,%s,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",pvec->tot_saveTradingResult[i].symbol_code ,pvec->tot_saveTradingResult[i].entryTime,pvec->tot_saveTradingResult[i].exitTime,
                        pvec->tot_saveTradingResult[i].entryPrice,pvec->tot_saveTradingResult[i].exitPrice,pvec->tot_saveTradingResult[i].volume,
                        pvec->tot_saveTradingResult[i].turnover , pvec->tot_saveTradingResult[i].commission , pvec->tot_saveTradingResult[i].per_slippage,pvec->tot_saveTradingResult[i].pnl );
        }
        fclose(fp);
    }else{
        printf("cannot write file: %s\n",filename);
    }
}

/**
 * 制作图表
 **/
void spread_makeFigureCsv(const struct mergeAllTradingResult &no , const string &filename , const char *firstLine){
    // printf("spread figure Csv %s\n" , filename.c_str());
    FILE *fp = NULL;
    if( (fp = fopen(filename.c_str() , "w")) != NULL){
        fprintf(fp , "%s", firstLine);
        double output_total_money = 0.0;
        for(int i = 0;i < no.tot_saveDayIncome.size() ; i++){
            output_total_money = no.tot_saveDayIncome[i];
            fprintf(fp,"%s,%lf\n",no.tot_saveVecTradeDay[i].c_str(), output_total_money);
        }
        fclose(fp);
        reportFileLists.push_back(filename);
    }else{
        printf("cannot write file: %s\n",filename.c_str());
    }
}


// // 制作价差的总报告
// void makeSpreadTotalReport( const vector<TradingResultNode>& spread_tradings , const char * filename ,const char * firstLine){
// 	FILE *fp = NULL;
// 	if( (fp = fopen(filename , "w")) != NULL){

// 		fclose(fp);
// 	}
// }

// 制作价差的单品种报告 --> 对所有的交易品种
void makeSpredMergeFixReport(struct mergeAllTradingResult & singleSpread , const char * filename , const char * firstLine){
    FILE *fp = NULL;
    if((fp = fopen(filename , "w")) != NULL){
        const mergeAllTradingResult *best = &singleSpread;
        fprintf(fp , "%s\n" , firstLine);
        // fprintf(fp,"所有的品种,%s\n",best->code_all.c_str());
        fprintf(fp,"时间段,%s---%s\n",best->start_date.c_str(),best->end_date.c_str());
        fprintf(fp,"交易日个数,%d\n",best->tot_total_trade_days);
        fprintf(fp,"交易周个数,%d\n",best->tot_total_trade_weeks);
        fprintf(fp,"初始资金,%lf\n",best->tot_ini_account_money);
        fprintf(fp,"----,核心指标,---\n");
        fprintf(fp,"年化收益率(%%),%.1lf%%\n",best->tot_rate_of_return * 100.0);
        fprintf(fp,"盈亏比,%.2lf\n",best->tot_profit_factor);
        fprintf(fp,"胜率,%.1lf\n",best->tot_win_rate);
        fprintf(fp,"最大回撤(%%),%.1lf%%\n",best->tot_max_windrawl_rate*100.0);
        fprintf(fp,"索提诺比率,%.2lf\n",best->tot_sortinoRatio);
        fprintf(fp,"目标函数(打分),%.1lf\n",best->tot_targetFunctionRankScore);
        fprintf(fp,"目标函数(PROM)%%,%.2lf\n",best->tot_targetFunctionPromScore);
        fprintf(fp,"---,总收益率指标,---\n");
        fprintf(fp,"总盈利,%.1lf\n",best->tot_net_profits);
        fprintf(fp,"总亏损,%.1lf\n",best->tot_net_loss);
        fprintf(fp,"---,交易类指标,---\n");
        fprintf(fp,"总开仓次数,%d\n",best->tot_total_open_num);
        fprintf(fp,"盈利次数,%d\n",best->tot_win_cover_num);
        fprintf(fp,"亏损次数,%d\n",best->tot_loss_cover_num);
        //fprintf(fp,"胜率,%.1lf,%.1lf\n",best->win_rate * 100 ,best_sharpe->win_rate * 100);
        fprintf(fp,"盈利周数,%d\n",best->tot_win_week_num);
        fprintf(fp,"亏损周数,%d\n",best->tot_loss_week_num);
        fprintf(fp,"---,平均收益类指标,---\n");
        fprintf(fp,"平均每手净利润,%.1lf\n",best->tot_ave_cover_income);
        fprintf(fp,"平均每手盈利,%.1lf\n",best->tot_ave_cover_win);
        fprintf(fp,"平均每手亏损,%.1lf\n",best->tot_ave_cover_loss);
        fprintf(fp,"---,极端交易类指标,---\n");
        fprintf(fp,"最大单次盈利,%.1lf\n",best->tot_max_single_win);
        fprintf(fp,"最大单次亏损,%.1lf\n",best->tot_max_single_loss);
        fprintf(fp,"最大连赢次数,%d\n",best->tot_max_continue_win_num);
        fprintf(fp,"最大连续盈利,%.1lf\n",best->tot_max_continue_win);
        fprintf(fp,"最大连亏次数,%d\n",best->tot_max_continue_loss_num);
        fprintf(fp,"最大连续亏损,%.1lf\n",best->tot_max_continue_loss);
        fprintf(fp,"---,资产回撤指标,---\n");
        fprintf(fp,"最大回撤数值,%.2lf\n",best->tot_max_windrawl_number);

        fprintf(fp,"最大回撤发生时间段,%s---%s\n",best->tot_max_windraw_start_date , best->tot_max_windraw_end_date);
        fprintf(fp,"回撤安全系数,%.1lf\n",best->tot_windrawl_safe_rate);
        fprintf(fp,"---,交易成本类指标,---\n");
        fprintf(fp,"总交易成本,%.1lf\n",best->tot_total_transcaction_fee);
        fprintf(fp,"平均交易成本,%.1lf\n",best->tot_ave_transcation_fee);
        fprintf(fp,"交易成本覆盖率,%.2lf%%\n",best->tot_transcation_cover_rate * 100.0);
        fprintf(fp,"---,风险类指标,---\n");
        fprintf(fp,"波动率,%.1lf\n",best->tot_stdandard*100);
        fprintf(fp,"夏普比率,%.2lf\n",best->tot_sharpe);
        //fprintf(fp,"索提诺比率,%.2lf,%.2lf\n",best->sortinoRatio,best_sharpe->sortinoRatio);
        fclose(fp);

        reportFileLists.push_back(filename);
    }
}

/**
 * 通过几个 tradingResult 获得
 */
void spread_backtesting_assessment_report(const vector<mergeAllTradingResult *> &v_report , const char * filename ,const char * firstLine){
    FILE *fp = NULL;
    if(fp = fopen(filename, "w")){
        const mergeAllTradingResult *best = v_report[0];
        const mergeAllTradingResult *best_sharpe = v_report[1];
        fprintf(fp,"%s\n",firstLine);
        // fprintf(fp,"所有的品种,%s,%s\n",best->code_all.c_str(),best_sharpe->code_all.c_str());
        fprintf(fp,"时间段,%s---%s,%s---%s\n",best->start_date.c_str(),best->end_date.c_str(),best_sharpe->start_date.c_str(),best_sharpe->end_date.c_str());
        fprintf(fp,"交易日个数,%d,%d\n",best->tot_total_trade_days,best_sharpe->tot_total_trade_days);
        fprintf(fp,"交易周个数,%d,%d\n",best->tot_total_trade_weeks,best_sharpe->tot_total_trade_weeks);
        fprintf(fp,"初始资金,%lf,%lf\n",best->tot_ini_account_money,best_sharpe->tot_ini_account_money);
        fprintf(fp,"----,核心指标,---\n");
        fprintf(fp,"年化收益率(%%),%.1lf%%,%.1lf%%\n",best->tot_rate_of_return * 100.0, best_sharpe->tot_rate_of_return*100.0);
        fprintf(fp,"盈亏比,%.2lf,%.2lf\n",best->tot_profit_factor,best_sharpe->tot_profit_factor);
        fprintf(fp,"胜率,%.1lf,%.1lf\n",best->tot_win_rate,best_sharpe->tot_win_rate);
        fprintf(fp,"最大回撤(%%),%.1lf%%,%.1lf%%\n",best->tot_max_windrawl_rate*100.0,best_sharpe->tot_max_windrawl_rate*100.0);
        fprintf(fp,"索提诺比率,%.2lf,%.2lf\n",best->tot_sortinoRatio,best_sharpe->tot_sortinoRatio);
        fprintf(fp,"目标函数(打分),%.1lf,%.1lf\n",best->tot_targetFunctionRankScore,best_sharpe->tot_targetFunctionRankScore);
        fprintf(fp,"目标函数(PROM)%%,%.2lf,%.2lf\n",best->tot_targetFunctionPromScore,best_sharpe->tot_targetFunctionPromScore * 100.0);
        fprintf(fp,"---,总收益率指标,---\n");
        fprintf(fp,"总盈利,%.1lf,%.1lf\n",best->tot_net_profits,best_sharpe->tot_net_profits);
        fprintf(fp,"总亏损,%.1lf,%.1lf\n",best->tot_net_loss,best_sharpe->tot_net_loss);
        fprintf(fp,"---,交易类指标,---\n");
        fprintf(fp,"总开仓次数,%d,%d\n",best->tot_total_open_num,best_sharpe->tot_total_open_num);
        fprintf(fp,"盈利次数,%d,%d\n",best->tot_win_cover_num,best_sharpe->tot_win_cover_num);
        fprintf(fp,"亏损次数,%d,%d\n",best->tot_loss_cover_num,best_sharpe->tot_loss_cover_num);
        //fprintf(fp,"胜率,%.1lf,%.1lf\n",best->win_rate * 100 ,best_sharpe->win_rate * 100);
        fprintf(fp,"盈利周数,%d,%d\n",best->tot_win_week_num,best_sharpe->tot_win_week_num);
        fprintf(fp,"亏损周数,%d,%d\n",best->tot_loss_week_num,best_sharpe->tot_loss_week_num);
        fprintf(fp,"---,平均收益类指标,---\n");
        fprintf(fp,"平均每手净利润,%.1lf,%.1lf\n",best->tot_ave_cover_income,best_sharpe->tot_ave_cover_income);
        fprintf(fp,"平均每手盈利,%.1lf,%.1lf\n",best->tot_ave_cover_win,best_sharpe->tot_ave_cover_win);
        fprintf(fp,"平均每手亏损,%.1lf,%.1lf\n",best->tot_ave_cover_loss,best->tot_ave_cover_loss);
        fprintf(fp,"---,极端交易类指标,---\n");
        fprintf(fp,"最大单次盈利,%.1lf,%.1lf\n",best->tot_max_single_win,best_sharpe->tot_max_single_win);
        fprintf(fp,"最大单次亏损,%.1lf,%.1lf\n",best->tot_max_single_loss,best_sharpe->tot_max_single_loss);
        fprintf(fp,"最大连赢次数,%d,%d\n",best->tot_max_continue_win_num,best_sharpe->tot_max_continue_win_num);
        fprintf(fp,"最大连续盈利,%.1lf,%.1lf\n",best->tot_max_continue_win,best_sharpe->tot_max_continue_win);
        fprintf(fp,"最大连亏次数,%d,%d\n",best->tot_max_continue_loss_num,best_sharpe->tot_max_continue_loss_num);
        fprintf(fp,"最大连续亏损,%.1lf,%.1lf\n",best->tot_max_continue_loss,best_sharpe->tot_max_continue_loss);
        fprintf(fp,"---,资产回撤指标,---\n");
        fprintf(fp,"最大回撤数值,%.2lf,%.2lf\n",best->tot_max_windrawl_number,best_sharpe->tot_max_windrawl_number);
        //fprintf(fp,"最大回撤比率,%.2lf%%,%.2lf%%\n",best->max_windrawal_rate * 100.0,best_sharpe->max_windrawal_rate*100.0);
        fprintf(fp,"最大回撤发生时间段,%s---%s,%s---%s\n",best->tot_max_windraw_start_date,best->tot_max_windraw_end_date,best_sharpe->tot_max_windraw_start_date,best_sharpe->tot_max_windraw_end_date);
        fprintf(fp,"回撤安全系数,%.1lf,%.1lf\n",best->tot_windrawl_safe_rate,best_sharpe->tot_windrawl_safe_rate);
        fprintf(fp,"---,交易成本类指标,---\n");
        fprintf(fp,"总交易成本,%.1lf,%.1lf\n",best->tot_total_transcaction_fee,best_sharpe->tot_total_transcaction_fee);
        fprintf(fp,"平均交易成本,%.1lf,%.1lf\n",best->tot_ave_transcation_fee,best_sharpe->tot_ave_transcation_fee);
        fprintf(fp,"交易成本覆盖率,%.2lf%%,%.2lf%%\n",best->tot_transcation_cover_rate * 100.0,best_sharpe->tot_transcation_cover_rate * 100.0);
        fprintf(fp,"---,风险类指标,---\n");
        fprintf(fp,"波动率,%.1lf,%.1lf\n",best->tot_stdandard*100,best_sharpe->tot_stdandard*100);
        fprintf(fp,"夏普比率,%.2lf,%.2lf\n",best->tot_sharpe,best_sharpe->tot_sharpe);
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
void spread_push_on_backtesting_report(const vector<mergeAllTradingResult *> &v_report , const char * filename ,const char * firstLine){
    FILE *fp = NULL;
    if(fp = fopen(filename, "w")){
        const mergeAllTradingResult *best = v_report[0];
        fprintf(fp,"%s\n",firstLine);
        fprintf(fp,"时间段,%s---%s\n",best->start_date.c_str(),best->end_date.c_str());
        fprintf(fp,"交易日个数,%d\n",best->tot_total_trade_days);
        fprintf(fp,"交易周个数,%d\n",best->tot_total_trade_weeks);
        fprintf(fp,"初始资金,%lf\n",best->tot_ini_account_money);
        fprintf(fp,"----,核心指标,---\n");
        fprintf(fp,"年化收益率(%%),%.1lf%%\n",best->tot_rate_of_return * 100.0);
        fprintf(fp,"盈亏比,%.2lf\n",best->tot_profit_factor);
        fprintf(fp,"胜率,%.1lf\n",best->tot_win_rate);
        fprintf(fp,"最大回撤(%%),%.1lf%%\n",best->tot_max_windrawl_rate*100.0);
        fprintf(fp,"索提诺比率,%.2lf\n",best->tot_sortinoRatio);
        fprintf(fp,"目标函数(打分),%.1lf\n",best->tot_targetFunctionRankScore);
        fprintf(fp,"目标函数(PROM)%%,%.2lf\n",best->tot_targetFunctionPromScore);
        fprintf(fp,"---,总收益率指标,---\n");
        fprintf(fp,"总盈利,%.1lf\n",best->tot_net_profits);
        fprintf(fp,"总亏损,%.1lf\n",best->tot_net_loss);
        fprintf(fp,"---,交易类指标,---\n");
        fprintf(fp,"总开仓次数,%d\n",best->tot_total_open_num);
        fprintf(fp,"盈利次数,%d\n",best->tot_win_cover_num);
        fprintf(fp,"亏损次数,%d\n",best->tot_loss_cover_num);
        fprintf(fp,"盈利周数,%d\n",best->tot_win_week_num);
        fprintf(fp,"亏损周数,%d\n",best->tot_loss_week_num);
        fprintf(fp,"---,平均收益类指标,---\n");
        fprintf(fp,"平均每手净利润,%.1lf\n",best->tot_ave_cover_income);
        fprintf(fp,"平均每手盈利,%.1lf\n",best->tot_ave_cover_win);
        fprintf(fp,"平均每手亏损,%.1lf\n",best->tot_ave_cover_loss);
        fprintf(fp,"---,极端交易类指标,---\n");
        fprintf(fp,"最大单次盈利,%.1lf\n",best->tot_max_single_win);
        fprintf(fp,"最大单次亏损,%.1lf\n",best->tot_max_single_loss);
        fprintf(fp,"最大连赢次数,%d\n",best->tot_max_continue_win_num);
        fprintf(fp,"最大连续盈利,%.1lf\n",best->tot_max_continue_win);
        fprintf(fp,"最大连亏次数,%d\n",best->tot_max_continue_loss_num);
        fprintf(fp,"最大连续亏损,%.1lf\n",best->tot_max_continue_loss);
        fprintf(fp,"---,资产回撤指标,---\n");
        fprintf(fp,"最大回撤数值,%.2lf\n",best->tot_max_windrawl_number);
        //fprintf(fp,"最大回撤比率,%.2lf%%,%.2lf%%\n",best->max_windrawal_rate * 100.0,best_sharpe->max_windrawal_rate*100.0);
        fprintf(fp,"最大回撤发生时间段,%s---%s\n",best->tot_max_windraw_start_date,best->tot_max_windraw_end_date);
        fprintf(fp,"回撤安全系数,%.1lf\n",best->tot_windrawl_safe_rate);
        fprintf(fp,"---,交易成本类指标,---\n");
        fprintf(fp,"总交易成本,%.1lf\n",best->tot_total_transcaction_fee);
        fprintf(fp,"平均交易成本,%.1lf\n",best->tot_ave_transcation_fee);
        fprintf(fp,"交易成本覆盖率,%.2lf%%\n",best->tot_transcation_cover_rate * 100.0);
        fprintf(fp,"---,风险类指标,---\n");
        fprintf(fp,"波动率,%.1lf\n",best->tot_stdandard*100);
        fprintf(fp,"夏普比率,%.2lf\n",best->tot_sharpe);
        //fprintf(fp,"索提诺比率,%.2lf,%.2lf\n",best->sortinoRatio,best_sharpe->sortinoRatio);
        fclose(fp);
        reportFileLists.push_back(filename);
    }else{
        printf("cannot write file: %s\n",filename);
    }
}

void spread_backtesting_assessment_report_running( spreadTemplate *obj , vector<struct __zhuli_data> &zhuliData,const char *start_date,const char *end_date , bool flag_make_report = 1){
    vector<struct mergeAllTradingResult> ans;

    string output_start_date = replaceDate(string(start_date));
    string output_end_date   = replaceDate(string(end_date));

    if(g_choose_searchAlgorithm == &EngineForceAlgorithm){
        // printf("g_choose_searchAlgorithm == &EngineForceAlgorithm\n");
        spread_getBestSolution(obj , ans, zhuliData , start_date , end_date);
        //getBestSolution(obj , ans , code , use_period , start_date , end_date);
        reverse(ans.begin() , ans.end());
        string st_name = string(obj->getStrategyName());

        spread_debug_report(ans,(st_name + "_debug_force.csv").c_str(),"debug_force!");
        //debug_report(ans,(st_name + "_debug_force.csv").c_str(),"debug_force!");
    }
    else if(g_choose_searchAlgorithm == &EngineLiziqunAlgorithm){
        spread_getBestSolutionUsingLiziQun(obj , ans , zhuliData , start_date , end_date);
        //getBestSolutionUsingLiziQun(obj , ans , code , use_period , start_date , end_date , lzq_NumParticle , lzq_max_cycle, lzq_C1 , lzq_C2);
        reverse(ans.begin() , ans.end());
        string st_name = string(obj->getStrategyName());
        spread_debug_report(ans,(st_name + "_debug_liziqun.csv").c_str(),"debug_liziqun!");
        //debug_report(ans,(st_name + "_debug_liziqun.csv").c_str(),"debug_liziqun!");
    }
    else if(g_choose_searchAlgorithm == &EngineBpSearchAlgorithm){
        spread_getBestSolutionUsingPriority(obj , ans , zhuliData , start_date , end_date , bp_max_cycle , bp_max_NumParticle);
        reverse(ans.begin() , ans.end());
        string st_name = string(obj->getStrategyName());
        spread_debug_report(ans,(st_name + "_debug_bp_serach.csv").c_str(),"debug_bpsearch!");
        //debug_report(ans,(st_name + "_debug_bp_serach.csv").c_str(),"debug_bpsearch!");
    }
    else if(g_choose_searchAlgorithm == &EngineRandomAlgorithm){
        spread_getBestSolutionUsingRandomFunction( obj, ans , zhuliData , start_date , end_date , rm_max_cycle, rm_max_NumParticle , rm_tmp_save_result_num);
        reverse(ans.begin() , ans.end());
        string st_name = string(obj->getStrategyName());
        spread_debug_report(ans,(st_name + "_debug_rm_random_serach.csv").c_str(),"debug_rm_random_search!");
        //debug_report(ans,(st_name + "_debug_rm_random_serach.csv").c_str(),"debug_rm_random_search!");
    }
    else{
        printf("Error , please choose search algorithm first!\n");
        return;
    }
    printf("total ans size:%d\n",ans.size());

    if(ans.size() > 0){
        // printf("spread_backtesting_assessment_report_running ans.size()=%d\n",ans.size());
        //先找到最优解 净利润最高
        struct mergeAllTradingResult *best = &(ans[0]);
        struct mergeAllTradingResult *best_sharpe = &(ans[0]);
        for(int i = 1; i < ans.size(); i++){
            if(ans[i].compareValue() > best->compareValue()){
                best = &(ans[i]);
            }
            if(ans[i].tot_sharpe > best_sharpe->tot_sharpe){
                best_sharpe = &(ans[i]);
            }
        }
        // printf("mergeAllTradingResult *best tot_saveDayIncomeSize %d\n", best->tot_saveDayIncome.size());
        _config.setBestResult( best->canshuSet);
        string tradingResultFile = string(__report_dir_path) + obj->getStrategyName() + "/figure_trading_result/" + best->code_all + "-" + best->period_all + "-" + output_start_date + "-" + output_end_date + "--best-trading_result.csv";
        char firstLineLine[400];
        sprintf(firstLineLine,"%s,%s,%s--%s,%s\n",best->code_all.c_str(), best->period_all.c_str(), start_date , end_date,best->getCanshuSetStr().c_str());
        spread_debug_tradingResult(best , tradingResultFile.c_str() ,firstLineLine);

        spread_makeFigureCsv(*best , (string(__report_dir_path) + obj->getStrategyName() + "/figure_trading_result/" + best->code_all + "-" + best->period_all + "-" + output_start_date + "-" + output_end_date + "--best-figure.csv").c_str() , firstLineLine);

        if(1 == flag_make_report){
            vector<mergeAllTradingResult *> vTradeNodes; vTradeNodes.clear();
            vTradeNodes.push_back(best);
            vTradeNodes.push_back(best_sharpe);

            string fp_filename = string(__report_dir_path) + obj->getStrategyName() + "/" + best->code_all + "-" +  best->period_all + "-" + output_start_date + "-" + output_end_date + "--backtesting_assessment_report.csv";
            char firstLine[400];
            sprintf(firstLine,"%s,%s,%s--%s,%s\nLabel,目标函数1(净利润),目标函数2(夏普)",best->code_all.c_str(), best->period_all.c_str() , start_date , end_date,best->getCanshuSetStr().c_str());
            spread_backtesting_assessment_report(vTradeNodes , fp_filename.c_str() , firstLine);
        }
    }
}

/**
 * 推进分析结果合并
 **/
struct mergeAllTradingResult spread_mergePushOnResult( vector<pair<struct mergeAllTradingResult  , struct mergeAllTradingResult > > &vec_ans , vector<struct __zhuli_data> &zhuliData ,const char *_start_date , const char *_end_date ){
    vector<struct tradeNode> t_tradingResult ;
    vector<string> vecTradeday , vecTradeWeekDay;
    vector<double> dayIncome , weekIncome , dayAccount , bzjAccount;
    vector<int> savePosition;
    double account_money = 0.0;

    t_tradingResult.clear();
    vecTradeday.clear(); vecTradeWeekDay.clear();
    dayIncome.clear(); weekIncome.clear(); dayAccount.clear(); bzjAccount.clear(); savePosition.clear();

    if(vec_ans.size() > 0){
        account_money = vec_ans[0].second.tot_ini_account_money;
    }

    for(int i = 0;i < vec_ans.size();i++){
        struct mergeAllTradingResult * obj = &(vec_ans[i].second);
        for(int j = 0;j < obj->tot_saveTradingResult.size(); j++){
            t_tradingResult.push_back( obj->tot_saveTradingResult[j]);
        }
        for(int j = 0;j < obj->tot_saveVecTradeDay.size(); j++){
            vecTradeday.push_back(obj->tot_saveVecTradeDay[j]);
        }
        for(int j = 0;j < obj->tot_saveWeekIncome.size(); j++){
            //因为TradingResult中，， 并不需要用到 vecTradeWeekDay ,, 所以。。。直接插入个数据就OK了
            vecTradeWeekDay.push_back("2000/01/01");
            weekIncome.push_back(obj->tot_saveWeekIncome[j]);
        }
        for(int j = 0;j < obj->tot_saveDayIncome.size(); j++){
            if(j == 0){
                dayIncome.push_back(obj->tot_saveDayIncome[j]);
            }else{
                dayIncome.push_back(obj->tot_saveDayIncome[j] - obj->tot_saveDayIncome[j-1]);
            }
        }
        for(int j = 0;j < obj->tot_saveBzjIncome.size();j++){
            bzjAccount.push_back(obj->tot_saveBzjIncome[j]);
        }
        for(int j = 0;j < obj->tot_savePositions.size();j++){
            savePosition.push_back(obj->tot_savePositions[j]);
        }
    }
    double sum_val = 0.0;
    for(int i = 0;i < dayIncome.size();i++){
        sum_val += dayIncome[i];
        dayAccount.push_back(sum_val + account_money);
    }

    // debug
    // for(int i = 0;i < dayIncome.size();i++){
    //     printf("vecTradeday[%d]=%s , dayIncome %lf\n",i,vecTradeday[i].c_str() , dayIncome[i]);
    // }
    // printf("vecTradeday sz %d dayIncome sz %d\n", vecTradeday.size() , dayIncome.size());

    return mergeAllTradingResult( zhuliData , _start_date , _end_date , t_tradingResult , t_tradingResult.size() ,  account_money,  vecTradeday , vecTradeWeekDay , dayIncome , weekIncome , bzjAccount , dayAccount, savePosition );
}
/**
 * 推进分析
 * 指定 品种,指定周期,指定开始的时间，结束的时间， 样本外检测时间(几个月)
 * 推进步骤
 1、每次按照优化出最佳参数， 然后将这组最佳参数进行样本外周期检验，
 ***/
void spread_makePushOnReport( spreadTemplate *obj , vector<struct __zhuli_data> &zhuliData , const char *start_date , const char *end_date , int ybn_months = 36, int ybw_months = 6){
    vector<TradingResultNode> resultList ;
    int cc_all_num = 200;
    string t_start_date(start_date);
    string t_end_date( timeDelayMonths(t_start_date , ybn_months));
    string t_ybw_date( timeDelayMonths(t_end_date , ybw_months));

    vector<pair<struct mergeAllTradingResult  , struct mergeAllTradingResult > > ans;
    ans.clear();
    while(cc_all_num-- > 0){
        // printf("cc_all_num %d\n",cc_all_num);
        if( t_ybw_date.compare(end_date) < 0){
            // printf("spread_makePushOnReport %s %s\n", t_start_date.c_str() , t_end_date.c_str() );
            //backtesting_assessment_report_and_Other_optimal_parameter_sets_report(obj, zhuliData , t_start_date.c_str() , t_end_date.c_str() ,  0.7,   0.2 , 0);
            spread_backtesting_assessment_report_running(obj , zhuliData , t_start_date.c_str() , t_end_date.c_str() , 0);

            obj->loadFile( zhuliData , t_start_date.c_str() , t_end_date.c_str());
            obj->init();

            // printf("_config.getCanshuSetValues() = %s\n", _config.getCanshuSetStr().c_str());
            obj->setCanshuFromVec(_config.getCanshuSetValues());
            obj->run();

            struct mergeAllTradingResult ta = obj->getTradingResultList( resultList );

            // spread_makeFigureCsv(ta , (string(__report_dir_path) + obj->getStrategyName() + "/figure_trading_result/zzdebug_ybn_" + ta.code_all + "-" + ta.period_all + "-" + replaceDate(t_start_date) + "-" + replaceDate(t_end_date) +"-" + int2str(ybn_months) + "_" + int2str(ybw_months) + "--best-figure-push-on.csv").c_str() , "debug");
            // spread_debug_tradingResult( &ta ,  (string(__report_dir_path) + obj->getStrategyName() + "/figure_trading_result/zzdebug_ybn_" + ta.code_all  + "-" + ta.period_all + "-" + replaceDate(t_start_date) + "-" + replaceDate(t_end_date) + "--best-trading_result-push-on.csv").c_str() ,"debug" );

            obj->loadFile(zhuliData, t_end_date.c_str() ,t_ybw_date.c_str());
            obj->init();
            // obj->loadFiles( useFiles , t_end_date.c_str() ,t_ybw_date.c_str() , timePeriod);
            //obj->init( code ) ;
            obj->setCanshuFromVec(_config.getCanshuSetValues());
            obj->run();

            struct mergeAllTradingResult tb = obj->getTradingResultList( resultList );
            //struct TradingResultNode tb = obj->getTradingResult( code, timePeriod , t_end_date.c_str()  , t_ybw_date.c_str());

            // spread_makeFigureCsv(tb , (string(__report_dir_path) + obj->getStrategyName() + "/figure_trading_result/zzdebug_ybw_" + tb.code_all + "-" + tb.period_all + "-" + replaceDate(t_end_date) + "-" + replaceDate(t_ybw_date) +"-" + int2str(ybn_months) + "_" + int2str(ybw_months) + "--best-figure-push-on.csv").c_str() , "debug");
            // spread_debug_tradingResult( &tb , (string(__report_dir_path) + obj->getStrategyName() + "/figure_trading_result/zzdebug_ybw_" + tb.code_all + "-" + tb.period_all + "-" + replaceDate(t_end_date) + "-" + replaceDate(t_ybw_date) + "--best-trading_result-push-on.csv").c_str() ,"debug");

            //debug_tradingResult( &tb , (string(__report_dir_path) + obj->getStrategyName() + "/figure_trading_result/zzdebug_ybw_" + code + "-" + timePeriod + "-" + replaceDate(t_end_date) + "-" + replaceDate(t_ybw_date) + "--best-trading_result-push-on.csv").c_str() ,"debug");
            ans.push_back( make_pair(ta, tb));
        }else{
            //printf( "tuijin end!\n");
        }
        t_start_date = timeDelayMonths(t_start_date , tuijinAnalyseMonthNum);
        t_end_date = timeDelayMonths(t_start_date , ybn_months);
        t_ybw_date = timeDelayMonths(t_end_date , ybw_months);
    }

    if(ans.size() > 0){

        string fp_filename = string(__report_dir_path) + obj->getStrategyName() + "/" + ans[0].first.code_all + "-" + ans[0].first.period_all + "-" + replaceDate(start_date)  + "-" + replaceDate(end_date) +"-" + int2str(ybn_months) + "_" + int2str(ybw_months) + "--push_backtesting_report.csv";
        char firstLine[400];
        sprintf(firstLine,"%s,%s,%s--%s,%s\nLabel,目标函数1(净利润)",ans[0].first.code_all.c_str(), ans[0].first.period_all.c_str() , start_date , end_date,"no canshu set");
        vector<mergeAllTradingResult *> tmp_Node; tmp_Node.clear();

        //struct TradingResultNode tnnode = mergePushOnResult(ans, code , timePeriod , start_date , end_date );
        struct mergeAllTradingResult tnnode = spread_mergePushOnResult(ans , zhuliData , start_date , end_date);
        tmp_Node.push_back( &tnnode );

        spread_push_on_backtesting_report(tmp_Node , fp_filename.c_str() , firstLine);
        string tradingResultFile = string(__report_dir_path) + obj->getStrategyName() + "/figure_trading_result/" + ans[0].first.code_all + "-" + ans[0].first.period_all + "-" + replaceDate(start_date) + "-" + replaceDate(end_date) + "--best-trading_result-push-on.csv";
        char firstLineLine[400];
        sprintf(firstLineLine,"%s,%s,%s--%s,%s\n",ans[0].first.code_all.c_str(), ans[0].first.period_all.c_str() , start_date , end_date,tnnode.getCanshuSetStr().c_str());
        spread_debug_tradingResult( &tnnode , tradingResultFile.c_str() ,firstLineLine);
        spread_makeFigureCsv(tnnode , (string(__report_dir_path) + obj->getStrategyName() + "/figure_trading_result/" + ans[0].first.code_all + "-" + ans[0].first.period_all + "-" + replaceDate(start_date) + "-" + replaceDate(end_date) +"-" + int2str(ybn_months) + "_" + int2str(ybw_months) + "--best-figure-push-on.csv").c_str() , firstLineLine);

        string push_on_filename = string(__report_dir_path) + obj->getStrategyName() + "/" + "PushOnReport_based_on_" + ans[0].first.code_all + "_" + ans[0].first.period_all + "_" + replaceDate(start_date) + "_" + replaceDate(end_date) + "_" + int2str(ybn_months) + "_" + int2str(ybw_months) + "_" + int2str(tuijinAnalyseMonthNum) + ".csv";
        FILE *fp = NULL;
        if ((fp = fopen(push_on_filename.c_str(), "w")) != NULL){
            char firstLine[400];
            sprintf(firstLine,"%s,%s,%s--%s,%s,%d,%d,%d\n",ans[0].first.code_all.c_str(),  ans[0].first.period_all.c_str() , start_date , end_date,_config.getCanshuSetStr().c_str(),ybn_months,ybw_months,tuijinAnalyseMonthNum);
            fprintf(fp,"%s",firstLine);
            fprintf(fp , "---,%s,%s,%s,%s,样本内月数%d,样本外月数%d\n",ans[0].first.code_all.c_str(),ans[0].first.period_all.c_str(),start_date,end_date,ybn_months,ybw_months);
            fprintf(fp , "样本内开始时间,样本内结束时间,样本外开始时间,样本外结束时间,样本内年化收益,样本外年化收益,推进效率,样本外净损益,样本外最大回撤数值,样本外最大回撤比率,胜率,样本内目标分数,样本外目标分数,样本内最优参数\n");
            double rate = 0.0;
            for(int i = 0;i < ans.size();i++){
                // if(i != 0){
                //     printf("%.2lf,%.2lf\n",ans[i].second.rate_of_return , ans[i-1].second.rate_of_return );
                //     rate = ans[i].second.rate_of_return / ans[i-1].second.rate_of_return;
                // }
                //修改推进效率算法，样本外收益除以样本内收益
                rate = ans[i].second.tot_rate_of_return / ans[i].first.tot_rate_of_return;
                fprintf(fp ,"%s,%s,%s,%s,%.2lf%%,%.2lf%%,%.2lf%%,%.3lf,%.2lf,%.2lf%%,%.2lf%%,%lf,%lf,%s\n",
                    ans[i].first.start_date.c_str() , ans[i].first.end_date.c_str() , ans[i].second.start_date.c_str() , ans[i].second.end_date.c_str() ,
                    ans[i].first.tot_rate_of_return * 100.0 , ans[i].second.tot_rate_of_return * 100.0 , rate * 100.0 , ans[i].second.tot_total_income, ans[i].second.tot_max_windrawl_number ,
                    ans[i].second.tot_max_windrawl_rate * 100.0, ans[i].second.tot_win_rate * 100.0 , ans[i].first.compareValue() , ans[i].second.compareValue() , ans[i].first.getCanshuSetStr().c_str());
            }
            fclose(fp);
            reportFileLists.push_back(push_on_filename);
        }else{
            printf("cannot write file: %s\n",push_on_filename.c_str());
        }

    }
}


#endif
