
import datetime,time

OracleStockUser    = 'stock'
OracleStockPasswd  = '123456'
OracleStockHost    = '192.168.0.118'

OracleFundUser     = 'fund'
OracleFundPasswd   = '123456'
OracleFundkHost    = '192.168.0.118'

OracleFutureUser   = 'future'
OracleFuturePasswd = '123456'
OracleFutureHost   = '192.168.0.118'

# now = time.strftime("%Y-%m-%d %H:%M:%S")
# print now

now = datetime.datetime.now()
str_today = now.strftime("%Y-%m-%d")