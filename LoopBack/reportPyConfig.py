# -*- coding: utf-8 -*-
#
# Copyright 2017  Inc
# @author ipqhjjybj 

config_backtesting_combination_report = '''
INSERT INTO `yii2advanced`.`combination_backtesting_report`
(`id`,
`algorithm`,
`firstline`,
`contents`)
VALUES
'''

config_bascktesting_assessment_report2 = '''
INSERT INTO `yii2advanced`.`bascktesting_assessment_report`
(`id`,`algorithm`,
`canshu_sets`,
`symbol`,
`period`,`start_time`,`end_time`,
`create_date`,`total_tradedates`,`total_trade_weeks`,`ini_account_money`,`rate_of_return`,
`profit_factor`,`win_rate`,`max_windrawal_rate`,`sortinoRatio`,
`targetFunctionRankScore`,`targetFunctionPromScore`,`net_profits`,`net_loss`,`total_open_num`,
`win_cover_num`,`loss_cover_num`,`win_week_num`,
`loss_week_num`,`ave_cover_income`,
`ave_cover_win`,`ave_cover_loss`,`max_single_win`,`max_single_loss`,`max_continue_win_num`,`max_continue_win`,
`max_continue_loss_num`,`max_continue_loss`,`max_windrawl_number`,`max_windraw_start_date`,
`max_windraw_end_date`,`windrawl_safe_rate`,`total_transcaction_fee`,`ave_transcation_fee`,
`transcation_cover_rate`,`stdandard`,`sharpe`)
VALUES
'''

config_bascktesting_assessment_report3 = '''
INSERT INTO `yii2advanced`.`push_on_bascktesting_report`
(`id`,`algorithm`,
`canshu_sets`,
`symbol`,
`period`,`start_time`,`end_time`,
`create_date`,`total_tradedates`,`total_trade_weeks`,`ini_account_money`,`rate_of_return`,
`profit_factor`,`win_rate`,`max_windrawal_rate`,`sortinoRatio`,
`targetFunctionRankScore`,`targetFunctionPromScore`,`net_profits`,`net_loss`,`total_open_num`,
`win_cover_num`,`loss_cover_num`,`win_week_num`,
`loss_week_num`,`ave_cover_income`,
`ave_cover_win`,`ave_cover_loss`,`max_single_win`,`max_single_loss`,`max_continue_win_num`,`max_continue_win`,
`max_continue_loss_num`,`max_continue_loss`,`max_windrawl_number`,`max_windraw_start_date`,
`max_windraw_end_date`,`windrawl_safe_rate`,`total_transcaction_fee`,`ave_transcation_fee`,
`transcation_cover_rate`,`stdandard`,`sharpe`)
VALUES
'''

config_pushon_report = '''
INSERT INTO `yii2advanced`.`pushon_report`
(`id`,
`algorithm`,
`canshu_sets`,
`symbol`,
`period`,
`start_date`,
`end_date`,
`create_date`,
`ybn_months`,
`ybw_months`,
`tuijin_months`,
`contents`)
VALUES
'''

config_core_index_report = '''
INSERT INTO `yii2advanced`.`core_index_report`
(`id`,
`algorithm`,
`canshu_sets`,
`symbol`,
`period`,
`ybn_start_date`,
`ybn_end_date`,
`ybw_start_date`,
`ybw_end_date`,
`create_date`,
`contents`)
VALUES
'''

config_distribute_best_report = '''
INSERT INTO `yii2advanced`.`distribute_best_report`
(`id`,
`algorithm`,
`canshu_sets`,
`symbol`,
`period`,
`start_date`,
`end_date`,
`create_date`,
`contents`)
VALUES
''';

config_niuxiong_state_report = '''
INSERT INTO `yii2advanced`.`niuxiong_state_report`
(`id`,
`algorithm`,
`canshu_sets`,
`base_symbol`,
`base_period`,
`create_date`,
`contents`)
VALUES
'''

config_other_optimal_report = '''
INSERT INTO `yii2advanced`.`other_optimal_parameter_report`
(`id`,
`algorithm`,
`canshu_sets`,
`base_symbol`,
`base_period`,
`start_date`,
`end_date`,
`create_date`,
`contents`)
VALUES
'''

config_prom_diff_future_report = '''
INSERT INTO `yii2advanced`.`prom_diff_future_report`
(`id`,
`algorithm`,
`canshu_sets`,
`base_symbol`,
`base_period`,
`start_date`,
`end_date`,
`create_date`,
`contents`)
VALUES
'''

config_statbility_report = '''
INSERT INTO `yii2advanced`.`stability_report`
(`id`,
`algorithm`,
`canshu_sets`,
`symbol`,
`period`,
`start_date`,
`end_date`,
`create_date`,
`contents`)
VALUES
'''

config_upload_file_dict = '''
INSERT INTO `yii2advanced`.`strategyreportdict`
(`id`,
`strategy_line`,
`report_dict_line`,
`create_date`)
VALUES
'''

config_upload_best_figure = '''
INSERT INTO `yii2advanced`.`tradingresultfigure`
(`id`,
`algorithm`,
`canshu_sets`,
`symbol`,
`period`,
`start_time`,
`end_time`,
`create_date`,
`contents`)
VALUES
'''