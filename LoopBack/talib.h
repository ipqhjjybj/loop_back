

#ifndef _talib
#define _talib

#include <vector>
#include <string>
#include <sstream>
using namespace std;

//转换出正确的格式
string replaceDate(const string &d){
    return d.substr(0,4) + "-" + d.substr(5,2) + "-" + d.substr(8,2);
}
//转换出日期斜杠的格式
string replaceDateXiegang(const string &d){
    return d.substr(0,4) + "/" + d.substr(5,2) + "/" + d.substr(8,2);
}
//int转string类型
string int2str(const int &int_temp)
{
        stringstream stream;
        stream<<int_temp;
        string s(stream.str());
        if(s.size() < 2) return string("0") + s;
        else return s;
}

//int转string类型
string float2str(const double &float_temp)
{
        stringstream stream;
        stream<<float_temp;
        string s(stream.str());
        return s;
}
// //商品周期和交易日最后一根bar的时间
// pair<int,string> Bar_end_time( string now_period){
//     int BarInterval;//商品数据的周期数值
//     string bar_interval;
//     int BarType;//商品数据的周期类型：1：秒线sec，2：分钟线min；3：小时线hour；4:日线day
//     string end_time_day;//商品交易日最后一根bar的时间

//     if(now_period[0] =='s')
//     {
//         BarType = 1;
//     }
//     if(now_period[0] =='m')
//     {
//         BarType = 2;
//     }
//     if(now_period[0] =='h')
//     {
//         BarType = 3;
//     }
//     if(now_period[0] =='d')
//     {
//         BarType = 4;
//     }

//     if(3 == BarType)
//     {
//         if (now_period.size()>5)
//         {
//             bar_interval = now_period.substr(4,2);
//         }
//         else
//         {
//             bar_interval = now_period[4];
//         }
//     }
//     else
//     {
//         if (now_period.size()>4)
//         {
//             bar_interval = now_period.substr(3,2);
//         }
//         else
//         {
//             bar_interval = now_period[3];
//         }
//     }
//     BarInterval = atoi(bar_interval.c_str());

//     int sec1;
//     sec1 = 60 - BarInterval;
//     if (3 == BarType)
//     {
//         sec1 = 15- BarInterval;
//     }
//     string sec2;
//     sec2 = int2str(sec1);
//     string str1;
//     string str2;
//     if (1 == BarType)
//     {
//         str1 = "14:59:";
//         end_time_day = str1 + sec2;
//     }
//     if (2 == BarType)
//     {
//         str1 = "14:";
//         str2 = ":00";
//         end_time_day = str1 + sec2 + str2;
//     }
//     if (3 == BarType)
//     {
//         str1 = ":00:00";
//         end_time_day = sec2 + str1;
//     }
//     return make_pair(BarType , end_time_day);
// }
/*
 * y = ax + b
 * 一元一次项拟合公式
 */
pair<double,double> polyfit(const vector<double> vecy , double div_base = 1.0){
    double ta = 0.0 , tb = 0.0 ;
    int n = vecy.size();
    double sum_xy = 0.0 , sum_x = 0.0 , sum_y = 0.0 , sum_x2 = 0.0;
    for(int i = 0;i < n;i++){
        double __y = vecy[i] / div_base;
        sum_xy += i * __y;
        sum_x += i;
        sum_y += __y;
        sum_x2 += i*i;
    }
    ta = (n * sum_xy -  (sum_x * sum_y *1.0 )) / ( n * sum_x2 - sum_x * sum_x * 1.0);
    tb = sum_y * 1.0 / n - ta * sum_x / n;
    return make_pair(ta,tb);
}

//字符串分割函数
vector<string> split(string oris,string pattern)
{
    string::size_type pos;
    vector<string> result;
    oris+=pattern;//扩展字符串以方便操作
    int size=oris.size();

    for(int i=0; i<size; i++){
        pos=oris.find(pattern,i);
        if(pos<size)
        {
            string s=oris.substr(i,pos-i);
            result.push_back(s);
            i=pos+pattern.size()-1;
        }
    }
    return result;
}
//从一行中获得所需的参数
vector<double> getParams(const string &oris , const string &split_str = ","){
    vector<double> ret; ret.clear();
    vector<string> ret_str = split(oris , split_str ); //vector<string> ret_str = split(oris , ",");
    for(int i = 0;i < ret_str.size();i++){
        ret.push_back( atof(ret_str[i].c_str()));
    }
    return ret;
}
/**
 * 月份周期推迟 几个月这样
 * 返回对应的日期
 ***/
string timeDelayMonths( const string &d , int add_month){
    char flag = d[4];
    int year = atoi(d.substr(0,4).c_str());
    int month = atoi(d.substr(5,2).c_str());
    int day  = atoi(d.substr(8,2).c_str());
    month += add_month;
    while( month < 1){
        year -= 1;
        month += 12;
    }
    while( month > 12){
        year += 1;
        month -= 12;
    }
    return int2str(year) + flag + int2str(month) + flag + int2str(day);
}
//计算Summation
void Summation(vector<double> &dstArr , const vector<double> &closeArray , const int m, const int n){
    dstArr.clear(); dstArr.reserve(n+10);
    if(n > 0){
        dstArr[0] = closeArray[0] * m;
    }
    for(int i = 1;i < n;i++){
        if(i < m){
            dstArr[i] = closeArray[i] + dstArr[i-1] - closeArray[0];
        }else{
            dstArr[i] = closeArray[i] + dstArr[i-1] - closeArray[i - m];
        }
    }
}

// void SummationFC(vector<double> &dstArr , const vector<double> &closeArray , const int m, const int n)
//求数组最大值
void Highest( vector<double> &dstArr , const vector<double> &closeArray , const int m, const int n){
    dstArr.clear(); dstArr.reserve(n+10);
    for(int i = 0;i < n;i++){
        dstArr[i] = closeArray[i];
        for( int j = i - 1;j > i - m && j >= 0; j--){
            dstArr[i] = max(dstArr[i] , closeArray[j]);
        }
    }
}
//求数组最小值
void Lowest( vector<double> &dstArr , const vector<double> &closeArray , const int m , const int n){
    dstArr.clear(); dstArr.reserve(n+10);
    for(int i = 0;i < n;i++){
        dstArr[i] = closeArray[i];
        for(int j = i - 1; j > i - m && j >= 0; j--){
            dstArr[i] = min(dstArr[i] , closeArray[j]);
        }
    }
}
//求平均值
void Average(vector<double> &dstArr , const vector<double> &closeArray , const int m , const int n){
    vector<double> tmpSum; tmpSum.reserve(n+10); tmpSum.clear();
    Summation(tmpSum , closeArray , m , n);
    dstArr.clear(); dstArr.reserve(n+10);
    for(int i = 0;i < n;i++){
        dstArr[i] = tmpSum[i] * 1.0 / m;
    }
}
// 求估计方差
void VariancePS(vector<double> &dstArr , const vector<double> &closeArray , const int m ,const int n , int dataType = 1){
    int _divisor = m - 1;
    vector<double> _mean; _mean.reserve(n + 10); _mean.clear();
    dstArr.clear(); dstArr.reserve(n+10);
    if( 1 == dataType ){
        _divisor = m;
    }
    if(_divisor > 0){
        Average( _mean , closeArray , m, n);
        dstArr[0] = 0.0;
        for(int i = 1;i < n;i++){
            double tmp = 0.0;
            for(int j = 0;j < m;j++){
                tmp += pow( _mean[i] - closeArray[max(0,i-j)] , 2);
            }
            dstArr[i] = tmp;
        }
        for(int i = 0;i < n;i++){
            dstArr[i] = dstArr[i] * 1.0 / m;
        }
    }else{
        for(int i = 0;i < n;i++){
            dstArr[i] = 0.0;
        }
    }
}
// 求标准差
void StandardDev(vector<double> &dstArr , const vector<double> &closeArray ,  const int m ,const int n , int dataType = 1){
    vector<double> _varpsValue; _varpsValue.reserve(n + 10); _varpsValue.clear();
    dstArr.clear(); dstArr.reserve(n+10);
    VariancePS(_varpsValue, closeArray , m, n , dataType);
    for(int i = 0;i < n;i++){
        if( _varpsValue[i] > 0){
            dstArr[i] = sqrt(_varpsValue[i]);
        }else{
            dstArr[i] = 0.0;
        }
    }
}
//计算xAverage
void xAverage(vector<double> &dstArr , const vector<double> &closeArray ,const int m, const int n){
    dstArr.clear(); dstArr.reserve(n+10);
    for(int i = 0;i < n;i++){
        if(i == 0)  dstArr[i] = closeArray[i];
        else dstArr[i] = dstArr[i-1] + 2.0 * (closeArray[i] -  dstArr[i-1]) / (m + 1) ;
    }
}
//计算MACD
void MACD(vector<double> &MACDValue , vector<double> &AvgMACD , vector<double> &MACDDiff, const vector<double> &closeArray ,
        const int FastLength , const int SlowLength , const int MACDLength , const int n){
    vector<double> t_aveFast, t_aveSlow;
    t_aveFast.clear();  t_aveSlow.clear();
    t_aveFast.reserve(n + 10); t_aveSlow.reserve(n + 10);
    xAverage(t_aveFast, closeArray , FastLength , n );
    xAverage(t_aveSlow, closeArray , SlowLength , n );

    MACDValue.clear();  MACDValue.reserve(n+10);
    for(int i = 0;i < n;i++){
        MACDValue.push_back( t_aveFast[i] - t_aveSlow[i]);
    }
    xAverage( AvgMACD , MACDValue, MACDLength , n);
    MACDDiff.clear(); MACDDiff.reserve(n+10);
    for(int i = 0;i < n;i++){
        MACDDiff.push_back( MACDValue[i] - AvgMACD[i]);
    }
}
//计算KDJ
void KDJ(vector<double> &K_arr, vector<double> &D_arr, vector<double> &J_arr, const vector<double> &highArray ,const vector<double> &lowArray, const vector<double> &closeArray ,
 const int Length , const int SlowLength , const int SmoothLength , const int n){
    vector<double> highestValue , lowestValue , sumHLValue , sumCLValue , dValue , hlMinus , clMinus;
    highestValue.clear();  lowestValue.clear() ;  sumHLValue.clear() ; sumCLValue.clear() ; dValue.clear(); hlMinus.clear() ; clMinus.clear();
    highestValue.reserve(n);  lowestValue.reserve(n);   sumHLValue.reserve(n); sumCLValue.reserve(n) ; dValue.reserve(n); hlMinus.reserve(n); clMinus.reserve(n);

    Highest(highestValue , highArray , Length , n);
    Lowest(lowestValue , lowArray , Length , n);
    for(int i = 0;i < n;i++){
        hlMinus.push_back( highestValue[i] - lowestValue[i]);
        clMinus.push_back( closeArray[i] - lowestValue[i]);
    }
    Summation(sumHLValue , hlMinus , SlowLength , n);
    Summation(sumCLValue , clMinus , SlowLength , n);
    K_arr.clear() ; D_arr.clear(); J_arr.clear();
    K_arr.reserve(n+10); D_arr.reserve(n+10); J_arr.reserve(n+10);
    for(int i = 0;i < n;i++){
        if( fabs(sumHLValue[i]) > 0.000001){
            K_arr.push_back(sumCLValue[i] * 100.0 / sumHLValue[i] );
        }else{
            K_arr.push_back( 0.0);
        }
    }
    Average(D_arr , K_arr , SmoothLength , n);
    for(int i = 0;i < n;i++){
        J_arr.push_back( 3 * K_arr[i] - 2 * D_arr[i]);
    }
}

//计算RSI
void RSI(vector<double> &RSIValue , const vector<double> &closeArray , const int length , const int n){
    double netChgAvg , totChgAvg , preNetChgAvg , preTotChgAvg;
    double SF , Change , ChgRatio;
    preNetChgAvg = 0.0;
    preTotChgAvg = 0.0;
    RSIValue.clear(); RSIValue.reserve(n + 10);
    for(int i = 0;i < n;i++){
        if(i <= length - 1){
            netChgAvg = (closeArray[i] - closeArray[max(i-length,0)] ) / length;
            double sum = 0.0;
            for(int j = 0;j < length;j++){
                sum += abs( closeArray[ max(i - j , 0)] - closeArray[max(i-j-1 , 0)]);
            }
            sum /= length;
            totChgAvg = sum;
        }else{
            SF = 1.0 / length;
            Change = closeArray[i] - closeArray[max(i-1 , 0)];
            netChgAvg = preNetChgAvg + SF * ( Change - preNetChgAvg);
            totChgAvg = preTotChgAvg + SF * ( abs(Change) - preTotChgAvg);
        }
        if( abs(totChgAvg) > 0.000001){
            ChgRatio = netChgAvg * 1.0 / totChgAvg;
        }else{
            ChgRatio = 0;
        }
        RSIValue.push_back( 50 * ( ChgRatio + 1));

        preNetChgAvg = netChgAvg;
        preTotChgAvg = totChgAvg;

    }

}

/**
 * 通过 短周期的，构造长周期的数据
 **/
void combineK_line( vector<int> &barMapper , vector<double> &newOpenArray, vector<double> &newHighArray , vector<double> &newLowArray , vector<double> &newCloseArray ,
                const vector<double> &openArray , const vector<double> &highArray , const vector<double> &lowArray , const vector<double> &closeArray , const int use_bar , const int n){
    barMapper.clear(); newOpenArray.clear();  newHighArray.clear(); newLowArray.clear(); newCloseArray.clear();
    barMapper.reserve(n /use_bar + 10); newOpenArray.reserve(n /use_bar+10); newHighArray.reserve(n /use_bar +10); newLowArray.reserve(n /use_bar+10); newCloseArray.reserve(n /use_bar+10);
    if( n > 0){
        double open = openArray[0];
        double high = highArray[0];
        double low = lowArray[0];
        double close = closeArray[0];

        barMapper.push_back(0);
        for(int i = 1;i < n;i++){
            if( i % use_bar == 0){
                newOpenArray.push_back( open);
                newHighArray.push_back( high);
                newLowArray.push_back( low);
                newCloseArray.push_back( close);

                open = openArray[i];
                high = highArray[i];
                low = lowArray[i];
                close = closeArray[i];
            }else{
                high = max(highArray[i] , high);
                low = min(lowArray[i] , low);
                close = closeArray[i];
            }
            barMapper.push_back( i / use_bar);
        }
        newOpenArray.push_back(open);
        newHighArray.push_back(high);
        newLowArray.push_back(low);
        newCloseArray.push_back(close);
    }
}

/**
 * 通过 短周期的，构造 日级别 的数据
 **/
void combineK_dayLine(vector<int> &barMapper , vector<double> &newOpenArray, vector<double> &newHighArray , vector<double> &newLowArray , vector<double> &newCloseArray ,
                   const vector<double> &openArray , const vector<double> &highArray , const vector<double> &lowArray , const vector<double> &closeArray , const vector<string> &g_time, const int n){
    barMapper.clear(); newOpenArray.clear();  newHighArray.clear(); newLowArray.clear(); newCloseArray.clear();
    barMapper.reserve(n ); newOpenArray.reserve(n ); newHighArray.reserve(n ); newLowArray.reserve(n ); newCloseArray.reserve(n );
    int tt_bar = 0;
    if( n > 0 ){
        double open = openArray[0];
        double high = highArray[0];
        double low = lowArray[0];
        double close = closeArray[0];

        barMapper.push_back(0);
        for(int i = 1; i < n;i++){
            if( (strcmp(g_time[i-1].c_str() , "15:31:00") <= 0 &&  strcmp(g_time[i].c_str() , "15:31:00") >= 0) ||
                (strcmp(g_time[i-1].c_str() , "15:31:00") <= 0 &&  strcmp(g_time[i-1].c_str() , g_time[i].c_str() ) >= 0 ) ){
                newOpenArray.push_back( open);
                newHighArray.push_back( high);
                newLowArray.push_back( low);
                newCloseArray.push_back( close);

                open = openArray[i];
                high = highArray[i];
                low = lowArray[i];
                close = closeArray[i];

                tt_bar = tt_bar += 1;
                barMapper.push_back(tt_bar);
            }else{
                high = max(highArray[i] , high);
                low = min(lowArray[i] , low);
                close = closeArray[i];
                barMapper.push_back( tt_bar );
            }
        }
        newOpenArray.push_back(open);
        newHighArray.push_back(high);
        newLowArray.push_back(low);
        newCloseArray.push_back(close);

        barMapper.push_back(tt_bar);
    }
}

//计算TrueRange
void TrueRange(vector<double> &dstArr ,const vector<double> &highArray , const vector<double> &lowArray, const vector<double> &closeArray , const int n){
    dstArr.clear(); dstArr.reserve(n+10);
    if(n > 0){
        dstArr[0] = highArray[0] - lowArray[0];
    }
    for(int i = 1;i < n;i++){
        double TrueHigh = max(highArray[i] , closeArray[i-1]);
        double TrueLow  = min(lowArray[i] , closeArray[i-1]);
        dstArr[i] = TrueHigh - TrueLow;
    }
}
//计算平均真实范围
void AvgTrueRange(vector<double> &dstArr ,const vector<double> &highArray , const vector<double> &lowArray,const vector<double> &closeArray ,const int m, const int n){
    vector<double> trueRangeArray; trueRangeArray.reserve(n + 10); trueRangeArray.clear();
    dstArr.clear(); dstArr.reserve(n+10);
    TrueRange( trueRangeArray , highArray , lowArray , closeArray , n);
    Average(dstArr , trueRangeArray , m , n);
}

//向左平移单位的算法
void LeftMove(vector<double> &dstArr , const vector<double> &oriArray ,const int m, const int n){
    dstArr.clear(); dstArr.reserve(n+10);
    for(int i = 0;i < n;i++){
        dstArr[i] = oriArray[max(0, i-m)];
    }
}

//计算劲道指数 ForceIndex
void ForceIndex( vector<double> &dstArr , const vector<double> &closeArray , const vector<double> &volArr , const int m ,const int n){
    vector<double> tmpJd ; tmpJd.clear(); tmpJd.reserve(n + 10);
    dstArr.clear(); dstArr.reserve(n + 10);
    for(int i = 0;i < n;i++){
        if(0 == i){
            tmpJd[i] = 0;
        }else{
            tmpJd[i] = (closeArray[i] - closeArray[i-1]) * ( volArr[i] / 100000.0);
        }
    }
    xAverage( dstArr , tmpJd , m ,n);
}

// tradeNode 交易节点
struct tradeNode{
    char symbol_code[50];
    char entryTime[50];
    char exitTime[50];

    double entryPrice,exitPrice;
    double volume;

    double turnover;
    double commission;
    double per_slippage;
    double pnl;
    double bzj;

    tradeNode(){
        volume = 0.0;

        turnover = 0.0;
        commission = 0.0;
        per_slippage = 0.0;
        pnl = 0.0;
        bzj = 0.0;
    }

    //重载最小比较符号
    bool operator < (const struct tradeNode & a) const{
        return ( strcmp(this->exitTime , a.exitTime) < 0 );
        //return this->targetFunctionRankScore > a.targetFunctionRankScore;
    }
};


// struct spreadTradeNode{
//     string uCode;
//     string entryTime;
//     string exitTime;

// };

struct Order{

};

struct Bar{
    char datetime[60];
    char date[40];
    char time[40];
    double open;
    double high;
    double low;
    double close;
    double volume;
    double openInterest;
    Bar(const char *_datetime, const char * _date, const char *_time , double _open,double _high, double _low,double _close,double _volume,double _openInterest){
        strcpy(this->datetime , _datetime); strcpy(this->date,_date); strcpy(this->time,_time);
        this->open = _open;  this->high = _high; this->low = _low; this->close = _close;
        this->volume = _volume; this->openInterest = _openInterest;
    }
    Bar(const char *_date, const char *_time, double _open,double _high, double _low,double _close,double _volume,double _openInterest){
        sprintf(this->datetime,"%s %s",_date,_time); strcpy(this->date, _date ); strcpy(this->time,_time);
        this->open = _open;  this->high = _high; this->low = _low; this->close = _close;
        this->volume = _volume; this->openInterest = _openInterest;
    }
    Bar(const char *_datetime, double _open,double _high, double _low,double _close,double _volume,double _openInterest){
        strcpy(this->datetime , _datetime); strncpy(this->date, _datetime  , 10); strcpy(this->time,_datetime + 11);
        this->open = _open;  this->high = _high; this->low = _low; this->close = _close;
        this->volume = _volume; this->openInterest = _openInterest;
    }
    string printOut(){
        char ss[2000];
        sprintf(ss , "%s %s,%lf,%lf,%lf,%lf,%lf,%lf",date,time,open,high,low,close,volume,openInterest);
        //sprintf(ss , "%lf",openInterest);
        return string(ss);
    }
};

/**
 * C++判断今天是星期几
 * 基姆拉尔森计算公式
 * W= (d+2*m+3*(m+1)/5+y+y/4-y/100+y/400+1) mod 7
 * 在公式中d表示日期中的日数，m表示月份数，y表示年数。
 * 注意：在公式中有个与其他公式不同的地方：
 * 把一月和二月看成是上一年的十三月和十四月，例：如果是2004-1-10则换算成：2003-13-10来代入公式计算。
 **/
int CaculateWeekDay(int y, int m, int d)
{
    if(m==1||m==2) //把一月和二月换算成上一年的十三月和是四月
    {
        m+=12;
        y--;
    }
    int Week=(d+2*m+3*(m+1)/5+y+y/4-y/100+y/400)%7;
    return Week;
    //     case 0: PrintfDay(y,m,d); cout << "是星期一" << endl; break;
    //     case 6: PrintfDay(y,m,d); cout << "是星期日" << endl; break;
}
int CaculateWeekDayS(const char * s){
    int y,m,d;
    sscanf(s , "%d/%d/%d" , &y,&m,&d);
    return CaculateWeekDay(y,m,d);
}

// 将报单进行合法化， 需要按照最小一跳进行处理
// double legalTradePrice(double price, double volume , double minMovement){
//     const int jingdu = 1000;
//     if(volume > 0){
//         int v = (int)(price*jingdu + 0.9) , k = minMovement * jingdu ;
//         if(v % k == 0){
//             return price;
//         }else{
//             return 1.0 * ( v / k * k / ( 1.0 * jingdu) + minMovement);
//         }
//     }else{
//         int v = (int)(price*jingdu) , k = (int)(minMovement * jingdu) ;
//         return 1.0 * (v / k * k / (1.0 * jingdu)) ;
//     }
// }
// 将报单进行合法化， 需要按照最小一跳进行处理
double legalTradePrice(double price, double volume , double minMovement){
    int bei = 0;
    bei = price/minMovement+0.0000001;
    double price_new = 0.0;
    price_new = bei * 1.0 * minMovement;
    if(volume >0 && fabs(price_new-price)> 0.000001)
    {
        price_new = price_new + minMovement;
    }
    return price_new;
}

int datenumToHash(int y, int m, int d)
{
    int month1[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    int month2[12] = {31,29,31,30,31,30,31,31,30,31,30,31};
    int year[4] = {366,365,365,365};
    int yy = y-1900;
    int yy_z = yy / 4;
    int yy_yu = yy % 4;
    int yy_day = yy_z * 1461;
    for (int i=0; i<yy_yu ; i++)
    {
        yy_day = yy_day + year[i];
    }

    int mm;
    int mm_day;
    if (0 == y % 4)
    {
        mm=2;
    }
    else
    {
        mm=1;
    }

    if (1==m)
    {
        mm_day = d;
    }
    else
    {
        if(1 == mm)
        {
            mm_day = 0;
            for (int j=0; j<m-1; j++)
            {
                mm_day = mm_day + month1[j];
            }
            mm_day = mm_day + d;
        }
        else
        {
            mm_day = 0;
            for (int j=0; j<m-1; j++)
            {
                mm_day = mm_day + month2[j];
            }
            mm_day = mm_day + d;
        }
    }

    int day = yy_day + mm_day;
    return day;
}
int hashDateNum[100000];
int datenum1( int y,int m ,int d){
    int cc = (y - 2002) * 12 * 31 + m * 31 + d;
    //printf("cc=%d\n",cc);
    if(hashDateNum[cc] > 0){
        return hashDateNum[cc];
    }else{
        int v = datenumToHash(y, m, d);
        hashDateNum[cc] = v;
        return v;
    }
}

int datenum(const char * s){
    int y,m,d;
    sscanf(s , "%d/%d/%d" , &y,&m,&d);
    return datenum1(y,m,d);
}

#endif // _talib
