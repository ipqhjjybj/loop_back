/**
 * @copyright JiuXi
 * @author ipqhjjybj
 **/
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <vector>
#include <cmath>

/*
double minMovement = 0.2;
double legalTradePrice(double price, double volume ){
    double eps = 1e-6;
    const int jingdu = 10000;
    int v = (int)(price*jingdu + 0.9) , k = minMovement * jingdu ;
    if(v % k == 0){
        return price;
    }else{
        //printf("22 v=%d k=%d\n",v,k);
        return 1.0 * ( v / k * k / jingdu + minMovement);
    }
}
*/

using namespace std;
#include "talib.h"

vector<double> dst;
int m = 10; int n = 14;
vector<double> closePrice , highPrice, lowPrice;
int test1(){
    dst.clear(); dst.reserve(100);
    highPrice.clear();
    highPrice.push_back(3777);
    highPrice.push_back(3776);
    highPrice.push_back(3776);
    highPrice.push_back(3779);
    highPrice.push_back(3788);
    highPrice.push_back(3787);
    highPrice.push_back(3787);
    highPrice.push_back(3785);
    highPrice.push_back(3784);
    highPrice.push_back(3784);
    highPrice.push_back(3784);
    highPrice.push_back(3783);
    lowPrice.clear();
    lowPrice.push_back(3774);
    lowPrice.push_back(3774);
    lowPrice.push_back(3774);
    lowPrice.push_back(3775);
    lowPrice.push_back(3778);
    lowPrice.push_back(3783);
    lowPrice.push_back(3784);
    lowPrice.push_back(3781);
    lowPrice.push_back(3781);
    lowPrice.push_back(3781);
    lowPrice.push_back(3782);
    lowPrice.push_back(3780);
    closePrice.clear();
    closePrice.push_back(3774);
    closePrice.push_back(3775);
    closePrice.push_back(3775);
    closePrice.push_back(3779);
    closePrice.push_back(3786);
    closePrice.push_back(3784);
    closePrice.push_back(3784);
    closePrice.push_back(3781);
    closePrice.push_back(3782);
    closePrice.push_back(3783);
    closePrice.push_back(3782);
    closePrice.push_back(3781);
    closePrice.push_back(3778);
    closePrice.push_back(3781);

    Lowest(dst , closePrice , 3 , n);
    for(int i = 0;i < n;i++){
        printf("i=%d %lf %lf\n",i , closePrice[i] , dst[i]);
    }
    //Summation(dst , closePrice , m , n);
    //StandardDev(dst , closePrice , m , n , 1);
    // AvgTrueRange(dst, highPrice,lowPrice ,closePrice , m , n);
    // for(int i = 0;i < n;i++){
    //     printf("i=%d %lf %lf\n",i , closePrice[i], dst[i] );
    // }
    //printf("%lf\n",legalTradePrice(9.01 , 1.0));
    return 0;
}

/**
 * test macd
20170720 0.14 14735 0 0 0
20170720 0.21 14655 -10.549451 -3.014129 -7.535322
20170720 0.22 14595 -24.373868 -9.116911 -15.256957
20170721 0.09 14630 -27.042522 -14.238514 -12.804007
20170721 0.1 14630 -27.466762 -18.018014 -9.448748
20170721 0.11 14625 -27.175181 -20.634347 -6.540834
20170721 0.13 14785 -4.705537 -16.083258 11.377722
20170721 0.14 14715 -0.148915 -11.530589 11.381674
20170721 0.21 14620 -9.915839 -11.069232 1.153393
20170721 0.22 14550 -24.613834 -14.939118 -9.674716
20170724 0.09 14390 -53.514211 -25.960573 -27.553638
20170724 0.1 14495 -54.783046 -34.195565 -20.58748
20170724 0.11 14440 -60.394625 -41.681011 -18.713614
20170724 0.13 14480 -55.856804 -45.731238 -10.125567
20170724 0.14 14300 -74.395185 -53.920937 -20.474248
20170724 0.21 14365 -73.758155 -59.588713 -14.169441
20170724 0.22 14440 -60.24091 -59.775055 -0.465854
20170725 0.09 14510 -40.192425 -54.180018 13.987593
20170725 0.1 14620 -11.803014 -42.072303 30.269288
20170725 0.11 14635 7.852257 -27.808143 35.660399
20170725 0.13 14580 12.133914 -16.396126 28.530041
20170725 0.14 14530 7.594964 -9.541529 17.136493
20170725 0.21 14745 32.869446 2.575892 30.293554
20170725 0.22 14670 36.810312 12.357155 24.453157
 **/
int test2(){
    char s1[100] , s2[100] ;
    double close , macd  , avg , diff;
    vector<double> closeArray , macdArray , avgArray , diffArray;
    closeArray.clear() , macdArray.clear() , avgArray.clear() , diffArray.clear();
    int i = 0;
    int n = 15 ;
    while(i++ < 20){
        scanf("%s %s %lf %lf %lf %lf",s1,s2,&close, &macd, &avg , &diff);
        closeArray.push_back(close);
    }
    macdArray.reserve(30); avgArray.reserve(30); diffArray.reserve(30);
    MACD( macdArray , avgArray , diffArray , closeArray , 6 , 12 , 6 , i);
    for(int i = 0;i < 20;i++){
        printf("%lf %lf %lf %lf\n", closeArray[i] , macdArray[i] , avgArray[i] , diffArray[i]);
    }
}

int test3(){
    double high, low, close , t1 ,t2 ,t3;
    vector<double> highArray , lowArray , closeArray , k_arr , d_arr , j_arr;
    highArray.clear() ; lowArray.clear(); closeArray.clear();
    k_arr.clear(); d_arr.clear(); j_arr.clear();
    int i = 0;
    while( i++ < 20){
        scanf("%lf %lf %lf %lf %lf %lf", &high , &low , &close , &t1 , &t2 , &t3);
        highArray.push_back(high);
        lowArray.push_back(low);
        closeArray.push_back(close);
    }
    k_arr.reserve(30); d_arr.reserve(30); j_arr.reserve(30) ;
    KDJ(k_arr , d_arr , j_arr , highArray , lowArray , closeArray , 14 , 3 , 3 , i);
    for(int i = 0;i < 20;i++){
        printf("%lf %lf %lf\n", k_arr[i] , d_arr[i] , j_arr[i]);
    }
}

/**
 * 测试 K 线连接
1 1 1 1
2 2 2 2
3 3 3 3
1 1 1 1
1 1 1 1
1 2 3 4
5 6 7 8
 */
int test4(){
    vector<double> openArray , highArray , lowArray , closeArray;
    vector<double> newOpenArray , newHighArray , newLowArray , newCloseArray;
    vector<int> barMapper;
    double open, high,low, close;
    int useBar = 4;
    int n = 0;
    while( scanf("%lf %lf %lf %lf",&open,&high,&low,&close) != EOF){
        n++;
        openArray.push_back(open); highArray.push_back(high); lowArray.push_back(low); closeArray.push_back(close);
    }
    combineK_line( barMapper , newOpenArray , newHighArray , newLowArray , newCloseArray , openArray, highArray , lowArray, closeArray , useBar , n);
    for(int i = 0;i < n;i++){
        printf("%d\n", barMapper[i]);
    }
    for(int i = 0;i < newOpenArray.size();i++){
        printf("%d %lf %lf %lf %lf\n", barMapper[i] , newOpenArray[i] , newHighArray[i] , newLowArray[i] , newCloseArray[i]);
    }
}


/**
 *
 *
 测试 Force Index
 **/
int test5(){
    vector<double> closeArray , volArray;
    vector<double> dstArr;
    double close, vol;
    int n = 0;
    while( scanf("%lf %lf",&close , &vol) != EOF){
        closeArray.push_back(close);
        volArray.push_back(vol);
        n++;
    }
    ForceIndex( dstArr , closeArray , volArray , 2 , n);
    for(int i = 0;i < n;i++){
        printf("%lf\n", dstArr[i]);
    }
    return 0;
}

/**
 * 测试 日数据合成代码
 ***/
int test6(){
    char tdate[50],ttime[50] ,tdatetime[100];
    double open,high,low,close,volume,openInterest;
    vector<double> openArray , highArray , lowArray , closeArray;
    vector<string> g_time;
    vector<double> newOpenArray , newHighArray , newLowArray , newCloseArray;
    vector<int> barMapper;

    int n = 0;
    while( scanf("%s %8s,%lf,%lf,%lf,%lf,%lf,%lf", tdate,ttime, &open,&high,&low,&close,&volume,&openInterest) != EOF){
        g_time.push_back(ttime);
        openArray.push_back(open); highArray.push_back(high);
        lowArray.push_back(low);  closeArray.push_back(close);
        n++;
    }

    combineK_dayLine( barMapper , newOpenArray , newHighArray , newLowArray , newCloseArray , openArray, highArray , lowArray, closeArray , g_time , n);

    for(int i = 0;i < n;i++){
        printf("%d\n", barMapper[i]);
    }
    for(int i = 0;i < newOpenArray.size();i++){
        printf("%d %lf %lf %lf %lf\n", barMapper[i] , newOpenArray[i] , newHighArray[i] , newLowArray[i] , newCloseArray[i]);
    }

    return 0;
}

int test7(){
    int n = 0;
    char tdate[50] , ttime[50] ;
    vector<double> closeArray , rsiArray;
    double close, NetChgAvg , TotChgAvg , ChgRatio , RSIValue;
    closeArray.clear();
    while( scanf("%s %s %lf %lf %lf %lf %lf", tdate , ttime , &close, &NetChgAvg , &TotChgAvg , &ChgRatio , &RSIValue) != EOF){
        closeArray.push_back( close);
        n++;
    }
    RSI( rsiArray , closeArray , 14 , n);
    for(int i = 0;i < rsiArray.size() ; i++){
        printf("%lf, %lf\n",closeArray[i] , rsiArray[i]);
    }
}

int main(){
    //freopen("D:/out_file/Text_Combin_K_line_in_0.txt", "r", stdin);
    freopen("D:/out_file/Text112.txt" , "r" , stdin);
    //test2();
    //test3();
    //test4();
    //test5();
    //test6();
    test7();
    return 0;
}
