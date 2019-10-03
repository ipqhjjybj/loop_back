# -*- coding: utf-8 -*-
#
# Copyright 2017  Inc
# @author ipqhjjybj 

'''
上传期货的数据
'''
import sys
import os
import six
import numpy as np
import pandas as pd

sys.path.append("..")


from config import *
from connect import *
from reportPyConfig import *


upload_date = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")

# class spreadReportUploader():
# 	'''
# 	上次详细回测报告
# 	'''
# 	@staticmethod
# 	def uploadSpreadBacktestingAssessmentReport(filePath , strategyName):
# 		try:
			
# 		except Exception,ex:
# 			print ex

class reportUploader():
	'''
	删除某一天之前的交易报告
	'''
	@staticmethod
	def deleteAllReportBeforeDay( use_date , clear_table_arr = ["`bascktesting_assessment_report`","`pushon_report`","`core_index_report`","`distribute_best_report`","`niuxiong_state_report`","`other_optimal_parameter_report`","`prom_diff_future_report`","`stability_report`","`strategyreportdict`"]):
		db = Stock.getDB('mysql')
		for table_name in clear_table_arr:
			sqll = "delete from %s where `create_date` <= '%s'" % (table_name , use_date)
			db.execute(sqll)
		db.commit()


	'''
	上传交易回测报告
	'''
	@staticmethod
	def uploadBacktestingAssessmentReport( filePath , strategyName ):
		# 2009/03/27---2017/03/27,2009/03/27---2017/03/27
		try:
			f = open(filePath , "r")
			firstLine = ""
			flag = 0
			ret = []
			i = 0
			for line in f:
				i = i + 1
				line = line.strip()
				if flag == 0 :
					flag = 1
					firstLine = line
					continue
				if i < 4:
					continue

				line = line.replace('1.$', '0.0')
				line = line.replace('-1.#J', '0.0')
				x = line.strip().split(',')
				if '---' in x:
					continue
				x = x[1]
				if '%' in x:
					x = x[:-1]
				if '---' in x :
					(withdraw_startdate , withdraw_enddate) = x.strip().split('---')
					ret.append(withdraw_startdate)
					ret.append(withdraw_enddate)
				else: 
					ret.append(x)
			
			f.close()
			# rb,min15,2009/03/27--2016/07/27,3 4 5 1 1 1 1 1 1 1 10 10 10 0 10 10 1 10 1
			(code , period , time_during , setValues) = firstLine.strip().split(',') 
			arr = time_during.strip().split('--')
			start_date , end_date = arr
			ret = [strategyName , setValues , code , period , start_date , end_date , upload_date ] + ret

			ret = [str(x) for x in ret]
			fuck = [x for x in ret if '%' in x]
			ret = ["'" + x + "'" for x in ret]
			line = ','.join(ret)
			sqll = config_bascktesting_assessment_report2 + " (NULL," + line + ")"

			db = Stock.getDB('mysql')
			db.execute(sqll)
			db.commit()

			n_id = db.lastRowID()
			return n_id
		except Exception,ex:
			print ex
			print "Error in %s %s " % (filePath , strategyName)
			return 0
	'''
	上传组合回测报告
	'''
	@staticmethod
	def uploadCombinationAssessmentReport( filePath , strategyName):
		try:
			f = open(filePath , "r")
			firstLine = ""
			i = 0
			content_arr = []
			for line in f:
				line = line.strip()
				i = i + 1
				if i == 1:
					firstLine = line
				if i < 2:
					continue
				content_arr.append(line)
			f.close()

			contents = '\n'.join(content_arr)
			ret = [strategyName , firstLine , contents]
			ret = [str(x) for x in ret]
			ret = ["'" + x + "'" for x in ret]
			line = ','.join(ret)
			sqll = config_backtesting_combination_report + " (NULL," + line + ")"
			#print sqll
			db = Stock.getDB('mysql')
			db.execute(sqll)
			db.commit()

			n_id = db.lastRowID()
			return n_id
		except Exception,ex:
			print ex
			print "Error in %s %s " % (filePath , strategyName)
			return 0
	'''
	上传交易回测报告
	'''
	@staticmethod
	def uploadPushOnBacktestingReport( filePath , strategyName ):
		# 2009/03/27---2017/03/27,2009/03/27---2017/03/27
		try:
			f = open(filePath , "r")
			firstLine = ""
			flag = 0
			ret = []
			i = 0
			for line in f:
				i = i + 1
				line = line.strip()
				if flag == 0 :
					flag = 1
					firstLine = line
					continue
				if i < 4:
					continue
				# line = line.replace('-1.#J', '0.0')
				# line = line.replace('1.#QO', '0.0')
				x = line.strip().split(',')
				if '---' in x:
					continue
				x = x[1]
				if '%' in x:
					x = x[:-1]
				if '---' in x :
					(withdraw_startdate , withdraw_enddate) = x.strip().split('---')
					ret.append(withdraw_startdate)
					ret.append(withdraw_enddate)
				else: 
					ret.append(x)
			
			f.close()
			# rb,min15,2009/03/27--2016/07/27,3 4 5 1 1 1 1 1 1 1 10 10 10 0 10 10 1 10 1
			(code , period , time_during , setValues) = firstLine.strip().split(',') 
			arr = time_during.strip().split('--')
			start_date , end_date = arr
			ret = [strategyName , setValues , code , period , start_date , end_date , upload_date ] + ret

			ret = [str(x) for x in ret]
			fuck = [x for x in ret if '%' in x]
			ret = ["'" + x + "'" for x in ret]
			line = ','.join(ret)
			sqll = config_bascktesting_assessment_report3 + " (NULL," + line + ")"

			db = Stock.getDB('mysql')
			db.execute(sqll)
			db.commit()

			n_id = db.lastRowID()
			return n_id
		except Exception,ex:
			print ex
			print "Error in %s %s " % (filePath , strategyName)
			return 0


	'''
	上传推进报告
	'''
	@staticmethod
	def uploadPushOnReport( filePath , strategyName):
		try:
			f = open(filePath , "r")
			firstLine = ""
			i = 0
			content_arr = [] 
			for line in f:
				line = line.strip()
				i = i + 1
				if i == 1:
					firstLine = line
				if i < 4:
					continue
				# line = line.replace('-1.#J', '0.0')
				# line = line.replace('1.#QO', '0.0')
				content_arr.append(line)
			f.close()
			contents = '\n'.join(content_arr)
			(code , period , time_during , setValues , ybn_months, ybw_months , tuijin_months ) = firstLine.strip().split(',') 
			arr = time_during.strip().split('--')
			start_date , end_date = arr
			ret = [strategyName , setValues , code , period , start_date , end_date , upload_date , ybn_months, ybw_months , tuijin_months , contents]
			ret = ["'" + x + "'" for x in ret]
			line = ','.join(ret)
			sqll = config_pushon_report + " (NULL," + line + ")"
			db = Stock.getDB('mysql')
			db.execute(sqll)
			db.commit()

			n_id = db.lastRowID()
			return n_id
		except Exception,ex:
			print ex
			print "Error in %s %s " % (filePath , strategyName)
			return 0

	'''
	core_index_Report
	'''
	@staticmethod
	def uploadCoreIndexReport( filePath , strategyName):
		try:
			f = open(filePath , "r")
			firstLine = ""
			i = 0
			content_arr = []
			for line in f:
				i = i + 1
				line = line.strip().decode('utf-8')
				if i == 1:
					firstLine = line
					continue
				if i < 4 :
					continue
				# line = line.replace('-1.#J', '0.0')
				# line = line.replace('1.#QO', '0.0')
				content_arr.append(line)
			f.close()
			(code , period , time_during1 , time_during2 , setValues  ) = firstLine.strip().split(',')
			(ybn_start_date , ybn_end_date) = time_during1.split('--')
			(ybw_start_date , ybw_end_date) = time_during2.split('--')
			contents = '\n'.join(content_arr)
			#contents = ""
			ret = [strategyName , setValues , code , period , ybn_start_date , ybn_end_date , ybw_start_date , ybw_end_date , upload_date , contents]
			ret = ["'" + x + "'" for x in ret]
			line = ','.join(ret)
			sqll = config_core_index_report + " (NULL," + line + ")"
			#print sqll

			db = Stock.getDB('mysql')
			db.execute(sqll)
			db.commit()

			n_id = db.lastRowID()
			return n_id
		except Exception,ex:
			print ex
			print "Error in %s %s " % (filePath , strategyName)
			return 0

	'''
	distribute_best_report
	'''
	@staticmethod
	def uploadDistributeBestReport( filePath , strategyName):
		try:
			f = open(filePath , "r")
			firstLine = ""
			i = 0
			content_arr = []
			for line in f:
				i = i + 1
				line = line.strip().decode('utf-8')
				if i == 1:
					firstLine = line
					continue
				if i < 3:
					continue
				# line = line.replace('-1.#J', '0.0')
				# line = line.replace('1.#QO', '0.0')
				content_arr.append(line)
			f.close()
			(code , period , time_during1 , setValues) = firstLine.strip().split(',')
			(start_date , end_date) = time_during1.split('--')
			contents = '\n'.join(content_arr)
			ret = [strategyName , setValues , code , period , start_date , end_date , upload_date , contents]
			ret = ["'" + x + "'" for x in ret]
			line = ','.join(ret)
			sqll = config_distribute_best_report + " (NULL," + line + ")"
			db = Stock.getDB('mysql')
			db.execute(sqll)
			db.commit()

			n_id = db.lastRowID()
			return n_id
		except Exception,ex:
			print ex
			print "Error in %s %s " % (filePath , strategyName)
			return 0

	'''
	牛熊阶段报告
	'''
	@staticmethod
	def uploadNiuxiong_state_report( filePath , strategyName):
		try:
			f = open(filePath , "r")
			firstLine = ""
			i = 0
			content_arr = []
			for line in f:
				i = i + 1
				line = line.strip().decode('utf-8')
				if i == 1:
					firstLine = line
				if i < 3:
					continue
				content_arr.append(line)
			f.close()
			(code , period  , setValues) = firstLine.strip().split(',')
			contents = '\n'.join(content_arr)
			ret = [strategyName , setValues , code , period , upload_date ,contents]
			ret = ["'" + x + "'" for x in ret]
			line = ','.join(ret)
			sqll = config_niuxiong_state_report + " (NULL," + line + ")"
			db = Stock.getDB('mysql')
			db.execute(sqll)
			db.commit()

			n_id = db.lastRowID()
			return n_id
		except Exception,ex:
			print ex
			print "Error in %s %s " % (filePath , strategyName)
			return 0

	'''
	其他可选参数报告
	'''
	@staticmethod
	def uploadOtherOptiomalParameterReport( filePath , strategyName):
		try:
			f = open(filePath , "r")
			firstLine = ""
			i = 0
			content_arr = []
			for line in f:
				i = i + 1
				line = line.strip().decode('utf-8')
				if i == 1:
					firstLine = line
				if i < 3:
					continue
				content_arr.append(line)
			f.close()
			(code , period , time_during1 , setValues) = firstLine.strip().split(',')
			(start_date , end_date) = time_during1.split('--')
			contents = '\n'.join(content_arr)
			ret = [strategyName , setValues , code , period , start_date , end_date , upload_date , contents]
			ret = ["'" + x + "'" for x in ret]
			line = ','.join(ret)
			sqll = config_other_optimal_report + " (NULL," + line + ")"
			db = Stock.getDB('mysql')
			db.execute(sqll)
			db.commit()

			n_id = db.lastRowID()
			return n_id
		except Exception,ex:
			print ex
			print "Error in %s %s " % (filePath , strategyName)
			return 0

	'''
	prom报告
	'''
	@staticmethod
	def uploadPromDiffFutureReport(filePath , strategyName):
		try:
			f = open(filePath , "r")
			firstLine = ""
			i = 0
			content_arr = []
			for line in f:
				i = i + 1
				line = line.strip().decode('utf-8')
				if i == 1:
					firstLine = line
				if i  < 4:
					continue
				content_arr.append(line)
			f.close()
			(code , period , time_during1 , setValues) = firstLine.strip().split(',')
			(start_date , end_date) = time_during1.split('--')
			contents = '\n'.join(content_arr)
			ret = [strategyName , setValues , code , period , start_date , end_date , upload_date , contents]
			ret = ["'" + x + "'" for x in ret]
			line = ','.join(ret)
			sqll = config_prom_diff_future_report + " (NULL," + line + ")"
			db = Stock.getDB('mysql')
			db.execute(sqll)
			db.commit()

			n_id = db.lastRowID()
			return n_id
		except Exception,ex:
			print ex
			print "Error in %s %s " % (filePath , strategyName)
			return 0

	'''
	上次交易交易收益报告
	'''
	@staticmethod
	def uploadTradingResultFigure(filePath , strategyName):
		try:
			f = open(filePath , "r")
			firstLine = ""
			flag = 0
			ret = []
			i = 0
			content_arr = []
			for line in f:
				i = i + 1
				line = line.strip()
				if flag == 0:
					flag = 1
					firstLine = line
					continue
				if i < 2:
					continue
				content_arr.append(line)
			f.close()
			try:
				(code , period , time_during1 , setValues) = firstLine.strip().split(',')
				(start_date , end_date) = time_during1.split('--')
			except Exception,ex:
				(code , period , time_during1 , setValues) = ["","","",""]
				(start_date , end_date) = ["",""] 

			contents = '\n'.join(content_arr)
			ret = [strategyName , setValues , code , period , start_date , end_date , upload_date , contents]
			ret = ["'" + x + "'" for x in ret]
			line = ','.join(ret)
			sqll = config_upload_best_figure + " (NULL," + line + ")"
			db = Stock.getDB('mysql')
			db.execute(sqll)
			db.commit()

			n_id = db.lastRowID()
			return n_id
		except Exception,ex:
			print ex
			print "Error in %s %s " % (filePath , strategyName)
			return 0


	'''
	stability report
	'''
	@staticmethod
	def uploadStability_report(filePath , strategyName):
		try:
			f = open(filePath , "r")
			firstLine = ""
			i = 0
			content_arr = []
			for line in f:
				i = i + 1
				line = line.strip().decode('utf-8')
				if i == 1:
					firstLine = line
				if i < 4:
					continue
				content_arr.append(line)
			f.close()
			(code , period , time_during1 , setValues) = firstLine.strip().split(',')
			(start_date , end_date) = time_during1.split('--')
			contents = '\n'.join(content_arr)
			ret = [strategyName , setValues , code , period , start_date , end_date , upload_date , contents]
			ret = ["'" + x + "'" for x in ret]
			line = ','.join(ret)
			sqll = config_statbility_report + " (NULL," + line + ")"
			db = Stock.getDB('mysql')
			db.execute(sqll)
			db.commit()

			n_id = db.lastRowID()
			return n_id
		except Exception,ex:
			print ex
			print "Error in %s %s " % (filePath , strategyName)
			return 0

	@staticmethod
	def uploadFileLists( fileListPath = "./output/fileLists.csv"):
		f = open(fileListPath , "r") 
		r = reportUploader()
		ret_id = []
		strategyName_list = []
		upload_name = ""
		i = 0
		for line in f:
			i = i + 1
			line = line.strip()
			if i < 2:
				upload_name = line
				continue
			tmp_arr = line.split('/')
			strategyName = tmp_arr[2]
			strategyName_list.append(strategyName)
			if os.path.exists(line):
				if "backtesting_assessment_report" in line:
					n_id = r.uploadBacktestingAssessmentReport(line, strategyName)
					ret_id.append( "backtesting_assessment_report:" + str(n_id))
					print "upload backtesting_assessment_report succes!"
				if "push_backtesting_report" in line:
					n_id = r.uploadPushOnBacktestingReport(line, strategyName)
					ret_id.append( "push_backtesting_report:" + str(n_id))
					print "upload push_backtesting_report succes!"
				if "PushOnReport" in line:
					n_id = r.uploadPushOnReport(line, strategyName)
					ret_id.append( "PushOnReport:" + str(n_id))
					print "upload PushOnReport succes!"
				if "Core_index" in line:
					n_id = r.uploadCoreIndexReport(line, strategyName)
					ret_id.append( "Core_index:" + str(n_id))
					print "upload Core_index succes!"
				if "distribute-best" in line:
					n_id = r.uploadDistributeBestReport(line, strategyName)
					ret_id.append( "distribute-best:" + str(n_id))
					print "upload distribute-best succes!"
				if "niuxiong_state" in line:
					n_id = r.uploadNiuxiong_state_report(line, strategyName)
					ret_id.append( "niuxiong_state:" + str(n_id))
					print "upload niuxiong_state succes!"
				if "Other_optimal_parameter_sets" in line:
					n_id = r.uploadOtherOptiomalParameterReport(line, strategyName)
					ret_id.append( "Other_optimal_parameter_sets:" + str(n_id))
					print "upload Other_optimal_parameter_sets succes!"
				if "PROM_of_each_future_in_diff_period" in line:
					n_id = r.uploadPromDiffFutureReport(line, strategyName)
					ret_id.append( "PROM_of_each_future_in_diff_period:" + str(n_id))
					print "upload PROM_of_each_future_in_diff_period succes!"
				if "stability" in line:
					n_id = r.uploadStability_report(line, strategyName)
					ret_id.append( "stability:" + str(n_id))
					print "upload stability succes!"
				if "best-figure" in line:
					n_id = r.uploadTradingResultFigure(line , strategyName)
					ret_id.append( "best-figure:" + str(n_id))
					print "upload best-figure succes!"
				if "combination-report" in line:
					n_id = r.uploadCombinationAssessmentReport(line , strategyName)
					ret_id.append( "combination-report:" + str(n_id))
					print "upload combination-report success!"
		f.close()

		id_new_line = ','.join(ret_id)
		strategyName_list = list(set(strategyName_list))
		strategyName_list.sort()
		strategy_line = ','.join(strategyName_list)
		ret = [strategy_line, id_new_line , upload_date + ":" + upload_name]
		ret = ["'" + x + "'" for x in ret]
		line = ','.join(ret)
		sqll = config_upload_file_dict + " (NULL," + line + ")"
		db = Stock.getDB('mysql')
		db.execute(sqll)
		db.commit()


# 抓取以及上传的代码
if __name__ == '__main__':
	r = reportUploader()
	#print r.uploadBacktestingAssessmentReport("./report/LivermoreStrategyTest2/rb-min15-2009-03-27-2017-07-27--backtesting_assessment_report.csv" , "LivermoreStrategyTest2")
	#r.uploadBacktestingAssessmentReport("./report/aligatorStrategy/rb-min15-2009-03-27-2016-07-27--backtesting_assessment_report.csv" , "aligatorStrategy")
	#r.uploadPushOnReport("./report/aligatorStrategy/PushOnReport_based_on_rb_min15_2009-03-27_2016-07-27_36_06.csv" ,"aligatorStrategy")
	#r.uploadCoreIndexReport("./report/aligatorStrategy/rb-all_period-2010-03-27-2015-03-27-2015-03-28-2050-03-27-Core_index_of_in-sample_and_out-of-sample_in_diff_period.csv" , "aligatorStrategy")
	#r.uploadDistributeBestReport("./report/aligatorStrategy/rb-min15-2009-03-27-2016-07-27--distribute-best.csv" , "aligatorStrategy")
	#r.uploadNiuxiong_state_report("./report/aligatorStrategy/capital_of_each_future_in_diff_period_in_diff_niuxiong_state_(Based_on_rb-min15-_best_parameter).csv" , "aligatorStrategy")
	#r.uploadOtherOptiomalParameterReport("./report/aligatorStrategy/rb-min15-2009-03-27-2016-07-27--Other_optimal_parameter_sets.csv","aligatorStrategy")
	#r.uploadPromDiffFutureReport("./report/aligatorStrategy/PROM_of_each_future_in_diff_period_(Based_on_rb-min15-2009-03-27-2016-07-27_best_parameter).csv","aligatorStrategy")
	#r.uploadStability_report("./report/aligatorStrategy/rb-min15-2009-03-27-2016-07-27--stability.csv" , "aligatorStrategy")
	#r.deleteAllReportBeforeDay( upload_date )
	r.uploadFileLists()