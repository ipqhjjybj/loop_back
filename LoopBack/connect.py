#coding:utf-8
import socket
import threading
import time
import os
import datetime
from config import *

import pandas as pd
import pandas.io.sql as pandas_sql
import MySQLdb
import logging


MYSQL_DB_USER_NAME="JXquant"
MYSQL_DB_PASS_WORD="JXcapital2016."
# MYSQL_DB_USER_NAME="root"
# MYSQL_DB_PASS_WORD="JXcapital2016"
MYSQL_DB_DATABASE="yii2advanced"
MYSQL_DB_PORT=3306
MYSQL_DB_HOST="106.14.195.159"
# MYSQL_DB_HOST='127.0.0.1'

'''
Common function
'''
class Common:
    @staticmethod
    def getMarket(code):
        if len(code) == 6:
            if code[:2] == "60":
                return "SH"
            if code[:2] in ["00","30"]:
                return "SZ"
        return "unknown"

class Connect_Mysql():
    def __init__(self, userName=MYSQL_DB_USER_NAME,passWord=MYSQL_DB_PASS_WORD,host=MYSQL_DB_HOST,database=MYSQL_DB_DATABASE,port=MYSQL_DB_PORT):
        self.conn = MySQLdb.connect(host=host,user=userName,passwd=passWord,db=database,port=port,charset="utf8")
        self.cur = self.conn.cursor()

    def execute(self,sql):
        self.cur.execute(sql)

    def fetchall(self):
        return self.cur.fetchall()

    def fetchone(self):
        return self.fetchone()

    def commit(self):
        self.conn.commit()

    def close(self):
        self.conn.close()

    def lastRowID(self):
        sqll = "select LAST_INSERT_ID()"
        self.execute(sqll)
        rs = self.fetchall()
        return rs[0][0]

'''
比较强大的股票类
'''
class Stock():


    mysql_data = False
    '''
    通过编号获得哪个数据库的连接
    100 --> oracle110
    '''
    @staticmethod
    def getDB(name):

        if name == "mysql":
            if Stock.mysql_data == False:
                while True:
                    try:
                        Stock.mysql_data = Connect_Mysql()
                        print "connect to mysql successily!"
                        break
                    except Exception,ex:
                        print ex
                        break
            return Stock.mysql_data
        return False

    '''
    关闭数据库连接
    '''
    @staticmethod
    def close():
        if Stock.mysql_data != False:
            Stock.mysql_data.close()
            Stock.mysql_data.close()
            
    '''
    把 2002这种数字转成  002002
    '''
    @staticmethod
    def norm_code(code):
        code = str(code)
        l = 6 - len(code)
        return '0' * l + code 
    '''
    把股票变成标准的股票
    '''
    @staticmethod
    def makeCode(code):
        return code[0:6] + "." + (Stock.get_market(code[0:6])).upper()

    '''
    判断这个stock_code是不是A股
    '''
    @staticmethod
    def check(stock_code):
        try:
            make_code = stock_code[0:6] + "." + (Stock.get_market(stock_code[0:6])).upper()
            if make_code == stock_code:
                return True
            else:
                return False
        except Exception,ex:
            print ex


    '''
    300104这样的,返回 sz , 00开头的sz,60,sh
    获得某个股票的 市场编号
    '''
    @staticmethod
    def get_market(code):
        code = Stock.norm_code(code)
        if len(code) == 6 :
            if code[:2] == "60":
                return "sh"
            if code[:2] == "00" or code[:2] == "30":
                return "sz"
            if code[:2] == "90":
                return "sh"
            if code[:2] == "20":
                return "sz"
        return "unknown"



class BrowserBase(object): 

    def __init__(self):
        socket.setdefaulttimeout(20)

    @staticmethod
    def speak(name,content):
        print '[%s]%s' % (name,content)

    def openurl(self,url):
        """
        打开网页
        """
        cookie_support= urllib2.HTTPCookieProcessor(cookielib.CookieJar())
        self.opener = urllib2.build_opener(cookie_support,urllib2.HTTPHandler)
        urllib2.install_opener(self.opener)
        user_agents = [
                    'Mozilla/5.0 (Windows; U; Windows NT 5.1; it; rv:1.8.1.11) Gecko/20071127 Firefox/2.0.0.11',
                    'Opera/9.25 (Windows NT 5.1; U; en)',
                    'Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 1.1.4322; .NET CLR 2.0.50727)',
                    'Mozilla/5.0 (compatible; Konqueror/3.5; Linux) KHTML/3.5.5 (like Gecko) (Kubuntu)',
                    'Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.8.0.12) Gecko/20070731 Ubuntu/dapper-security Firefox/1.5.0.12',
                    'Lynx/2.8.5rel.1 libwww-FM/2.14 SSL-MM/1.4.1 GNUTLS/1.2.9',
                    "Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.7 (KHTML, like Gecko) Ubuntu/11.04 Chromium/16.0.912.77 Chrome/16.0.912.77 Safari/535.7",
                    "Mozilla/5.0 (X11; Ubuntu; Linux i686; rv:10.0) Gecko/20100101 Firefox/10.0 ",

                    ] 
       
        agent = random.choice(user_agents)
        self.opener.addheaders = [("User-agent",agent),("Accept","*/*"),('Referer','http://www.google.com')]
        try:
            res = self.opener.open(url)
            return res.read()
        except Exception,e:
            # BrowserBase.speak(str(e),url)
            # return self.openurl(url)
            return ""
        return ''
'''
HTML 信息类
'''
class HTML():
    @staticmethod
    def browser_html(url):
        splider = BrowserBase()
        return splider.openurl(url)

    @staticmethod
    def get_html(url,data={}):
        try:
            # print "try request"
            request = urllib2.Request(url)
            request.add_header('User-Agent','Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/44.0.2403.155 Safari/537.36')
            request.add_header('Connection',"keep-alive")
            request.add_header('Cache-Control',"")
            request.add_header("Accept-Encoding","gzip, deflate, sdch")
            request.add_header("Accept","text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8")
            request.add_header("Upgrade-Insecure-Requests","1")
            for key in data.keys():
                request.add_header(key,data[key])
            # print "try response"
            response = urllib2.urlopen(request,timeout=10)
            html = ""
            if response.info().get('Content-Encoding') == 'gzip':
                # print "try gzip read"
                buf = StringIO.StringIO(response.read())
                f = gzip.GzipFile(fileobj=buf)
                html = f.read()
            else:
                # print "try read"
                opener = urllib2.build_opener()
                html = opener.open(request).read()
            # print "finish"
            try:
                html = html.decode('utf-8')
            except Exception,ex:
                try:
                    html = html.decode('gbk')
                except Exception,ex:
                    pass
            return html
        except Exception,ex:
            print "Error in Spider.get_html"
            print ex
            if "IncompletedRead" in ex:
                print "try again spider"
                return HTML.get_html(url)
            if "HTTP Error 500" in str(ex):
                print "try again spider"
                return HTML.get_html(url)
            if "timed out" in ex:
                print "try again spider"
                return HTML.get_html(url)