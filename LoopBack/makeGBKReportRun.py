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

def dealWith(filename , parent , dest_path):
	lines = []
	f = open( os.path.join(parent,filename) , "r")
	for line in f:
		line = line.strip().decode('utf-8')
		lines.append(line)
	f.close()
	f = open( os.path.join(dest_path , filename) , "w")
	for line in lines:
		f.write( (line + "\n").encode('gbk'))
	f.close()

def makeGbkReport(dir_path = "./report" , dest_path="./gbk_report"):
	for parent,dirnames,filenames in os.walk(dir_path):
		for filename in filenames:                        #输出文件信息
			dealWith(filename , parent , dest_path)
			#print "the full name of the file is:" + os.path.join(parent,filename) #输出文件路径信息

# 抓取以及上传的代码
if __name__ == '__main__':
	makeGbkReport()