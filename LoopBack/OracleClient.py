# -*- coding: utf-8 -*-
#
# Copyright 2017  Inc
# @author ipqhjjybj 
import sys
import os
import six
import numpy as np
import pandas as pd
import tushare as ts

import cx_Oracle

class Oracle():

    conn = None
    stmt = None

    def __init__(self,name="stock",pwd="123456",conn="192.168.1.1"):
        connstr = '%s/%s@%s:1521/orcl' if "." in conn else '%s/%s@%s'
        try:
            self.conn = cx_Oracle.connect(connstr % (name,pwd,conn))
            self.stmt = self.conn.cursor()
        except Exception, e:
            print str(e).strip()
            if "ORA-12170" in str(e).strip():
                self.__init__(name,pwd,ip)

    def __enter__(self):
        return self

    def __exit__(self,type,value,trace):
        if self.stmt:
            self.stmt.close()
        if self.conn:
            self.conn.close()
        return True

    def insert(self,dataset,sql):
        try:
            self.stmt.prepare(sql)
            self.stmt.executemany(None,dataset)
            self.conn.commit()
            return True
        except Exception, e:
            print str(e).strip()
            return False

    def cuid(self,sql):
        try:
            self.stmt.execute(sql)
            self.conn.commit()
            return True
        except Exception,e:
            print str(e).strip()
            return False

    def select(self,sql):
        try:
            self.stmt.execute(sql)
            return self.stmt.fetchall()
        except Exception, e:
            print str(e).strip()
            return False

    def prc(self,name,params=[]):
        try:
            return self.stmt.callproc(name,params)
        except Exception, e:
            print str(e).strip()
            return False

    def vars(self,num):
        values = ""
        for i in range(num):
            if i == num-1:
                values += ":%s" % str(i+1)
            else:
                values += ":%s," % str(i+1)
        return values

    def execute(self, sqll):
        self.stmt.execute(sqll)

    def fetchall(self):
        return self.stmt.fetchall()

    def commit(self):
        self.conn.commit()