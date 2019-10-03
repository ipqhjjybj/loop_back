# -*- coding: utf-8 -*-
#
# Copyright 2017  Inc
# @author ipqhjjybj 

import tushare as ts

from config import *
from OracleClient import *


'''
期货连接类
'''
class Future():
	__oracle_future_db = None
	__cache_future_day = {}
	# 获得Oracle期货的连接客户端
	@staticmethod
	def getOracleFutureClientInstance():
		if Future.__oracle_future_db == None:
			Future.__oracle_future_db = Oracle(OracleFutureUser,OracleFuturePasswd,OracleFutureHost)
		return Future.__oracle_future_db

	# 缓存单品种的数据
	@staticmethod
	def getOneInstrumentDayAll( order_book_id ):
		ret = []
		try:
			ret = Future.__cache_future_day[order_book_id]
		except Exception,ex:
			db  = Future.getOracleFutureClientInstance()
			sqll = "select * from rqalpha_rhq_future where CODE = '%s' order by tradedate" % (order_book_id)
			db.execute(sqll)
			rs = db.fetchall()
			if len(rs) > 0:
				ret = rs
		return ret

	# 获得单品种某区间的数据
	@staticmethod
	def getOneInstrumentDayBetween(order_book_id , start_date , end_date):
		h_ret = Future.getOneInstrumentDayAll(order_book_id)
		ret = [x for x in h_ret if start_date <= x[0] and x[0] <= end_date]
		return ret

