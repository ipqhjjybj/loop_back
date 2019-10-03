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


from future import *


'''
通过数据库下载最新的 config配置信息
1、包含手续费
2、牛熊表
'''
class configProduce():
	__conf_storage  = os.path.dirname(os.path.realpath(__file__)) + "/conf/"

	@staticmethod
	def makeNormCode(CODE):
		if len(CODE) == 1:
			CODE = CODE + "9"
		CODE = CODE + "888"
		return CODE

	@staticmethod
	def niuxiongNumber(name):
		try: 
			name = name.decode('gbk')
			number = 0
			if '牛市'.decode('utf-8') in name:
				number = 1
			elif '熊市'.decode('utf-8') in name:
				number = 2
			elif '盘整'.decode('utf-8') in name:
				number = 3
			elif '循环'.decode('utf-8') in name:
				number = 4
			return number
		except Exception,ex:
			print name
			return 0

	def loadCashFee(self):
		filter_arr = ["fu888"]
		conf_path = configProduce.__conf_storage + "fee.txt"
		db = Future.getOracleFutureClientInstance()
		sqll = "select ABBR,CONTRACT_MULTIPLIER,MIN_MOVE,CASH_DEPOSIT,FEE_TYPE,PER_MONEY,PER_MONEY_YESTER from cash_fee"
		db.execute(sqll)
		rs = db.fetchall()
		f = open(conf_path , "w")
		for (ABBR,CONTRACT_MULTIPLIER,MIN_MOVE,CASH_DEPOSIT,FEE_TYPE,PER_MONEY,PER_MONEY_YESTER) in rs:
			ABBR = configProduce.makeNormCode(ABBR)
			if ABBR in filter_arr:
				continue
			arr = [ABBR,CONTRACT_MULTIPLIER,MIN_MOVE,CASH_DEPOSIT,FEE_TYPE,PER_MONEY,PER_MONEY_YESTER]
			arr = [str(x) for x in arr]
			line = ' '.join(arr)
			f.write(line + "\n")
		f.close()


	def insertCashFee(self):
		conf_path = configProduce.__conf_storage + "fee.txt"
		ret = []
		f = open(conf_path , "r")
		for line in f:
			arr = line.strip().split(' ')
			ret.append(arr)
		f.close()

		db = Future.getOracleFutureClientInstance()
		sqll = "truncate table cash_fee"
		db.execute(sqll)
		sqll = "insert into cash_fee(ABBR,CONTRACT_MULTIPLIER,MIN_MOVE,CASH_DEPOSIT,FEE_TYPE,PER_MONEY,PER_MONEY_YESTER) values(:1,:2,:3,:4,:5,:6,:7)"
		db.insert(ret , sqll)
		db.commit()		

	
	def loadNiuXiong(self):
		conf_path = configProduce.__conf_storage + "niuxiong.txt"
		db = Future.getOracleFutureClientInstance()
		sqll = "select CODE,START_DATE,END_DATE,STATE from niuxiongstate"
		db.execute(sqll)
		rs = db.fetchall()
		f = open(conf_path , "w")
		for (CODE,START_DATE,END_DATE,STATE) in rs:
			CODE = configProduce.makeNormCode(CODE)
			STATE = configProduce.niuxiongNumber(STATE)
			arr = [CODE,START_DATE,END_DATE,STATE]
			arr = [str(x) for x in arr]
			line = ' '.join(arr)
			f.write(line + "\n")
		f.close()

	def insertNiuXiong(self):
		conf_path = configProduce.__conf_storage + "niuxiong.txt"
		ret = []
		f = open(conf_path , "r")
		for line in f:
			CODE,START_DATE,END_DATE,STATE = line.strip().split(' ')
			STATE = int(STATE)
			if STATE == 1:
				STATE = u"牛市"
			elif STATE == 2:
				STATE = u"熊市"
			elif STATE == 3:
				STATE = u"盘整"
			elif STATE == 4:
				STATE = u"循环"
			arr = [CODE,START_DATE,END_DATE,STATE]
			ret.append(arr)
		f.close()

		db = Future.getOracleFutureClientInstance()
		sqll = "truncate table niuxiongstate"
		db.execute(sqll)
		sqll = "insert into niuxiongstate(CODE,START_DATE,END_DATE,STATE) values(:1 , :2, :3 , :4)"
		db.insert(ret , sqll)
		db.commit()	


# 抓取以及上传的代码
if __name__ == '__main__':
	#print os.path.dirname(os.path.realpath(__file__))
	#print os.path.abspath(__file__)
	#fu = futureUploader()
	#fu.uploadNiuXiongState()

	cp = configProduce()
	# cp.loadCashFee()
	# cp.loadNiuXiong()

	cp.insertCashFee()
	cp.insertNiuXiong()

	#print "1"

