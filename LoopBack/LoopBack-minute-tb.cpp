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
#include <fstream>
#include <iostream>

#include "talib.h"
#include "TradingResult.h"


#include "strategy/AligatorStrategy.h"
#include "strategy/LivermoreStrategy.h"
#include "strategy/LivermoreStrategy2.h"
#include "strategy/LivermoreStrategyTest.h"
#include "strategy/LivermoreStrategyTest2.h"
#include "strategy/LivermoreStrategyTest3.h"
#include "strategy/bollingerBotStrategy.h"
#include "strategy/kingKeltnerStrategy.h"
#include "strategy/DualThrustStrategy.h"
#include "strategy/bollingerBreakerStrategy.h"
#include "strategy/ShuangjunxianStrategy.h"
//#include "strategy/SclwStrategy.h"
#include "strategy/Sclw_JdzsStrategy.h"
#include "strategy/TwoEMAStrategy.h"
#include "strategy/BollingerAverageStrategy.h"
#include "strategy/MACDHaiTrunStrategy.h"
#include "strategy/AtrRsiStrategy.h"
#include "strategy/Sclw_MACD_JdzsStrategy.h"
#include "strategy/PeriodDualthrust.h"
#include "strategy/PeriodBollingerBandit.h"
#include "strategy/PeriodTSKingKeltner.h"

#include "load_config.h"
#include "ctaStrategyTemplate.h"
#include "report.h"
#include "engine.h"

#include "InvestmentProtfolio.h"

// 中文测试
using namespace std;


void needSymbol(){
    Sclw_MACD_JdzsStrategy test4 = Sclw_MACD_JdzsStrategy();
    //makePushOnReport(&test4 , "ih" , global_timePeriod , "2015/04/16" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    //makePushOnReport(&test4 , "ru" , global_timePeriod , "2004/01/02" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    makePushOnReport(&test4 , "rb" , global_timePeriod , "2012/10/10" , global_end_date , 30 , tuijin_ybw_months );
    //makePushOnReport(&test4 , "cu" , global_timePeriod , "2004/01/02" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    //makePushOnReport(&test4 , "ni" , global_timePeriod , "2015/03/27" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    //makePushOnReport(&test4 , "oi" , global_timePeriod , "2007/06/08" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    //makePushOnReport(&test4 , "rb" , global_timePeriod , "2009/03/27" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
}
void needSymbol2(){
    Sclw_MACD_JdzsStrategy test4 = Sclw_MACD_JdzsStrategy();
    makePushOnReport(&test4 , "ih" , "min30" , "2015/04/16" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    //makePushOnReport(&test4 , "ru" , "min30" , "2004/01/02" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    makePushOnReport(&test4 , "ag" , "min30" , "2012/05/10" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    makePushOnReport(&test4 , "cu" , "min30" , "2004/01/02" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    makePushOnReport(&test4 , "ni" , "min30" , "2015/03/27" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    makePushOnReport(&test4 , "oi" , "min30" , "2007/06/08" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    makePushOnReport(&test4 , "rb" , "min30" , "2009/03/27" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
}
void pushOnMake_if_ic_ih(){
    reportFileLists.clear();

    Sclw_MACD_JdzsStrategy test4 = Sclw_MACD_JdzsStrategy();

    makePushOnReport(&test4 , "if" , global_timePeriod , "2011/04/16" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    makePushOnReport(&test4 , "ic" , global_timePeriod , "2015/10/16" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    makePushOnReport(&test4 , "ih" , global_timePeriod , "2015/10/16" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );

    _config.outputFileLists("ipqhjjybj, pushOnMake_if_ic_ih test");
    system("uploadReport.bat");
}
void pushOnMake_black(){
    reportFileLists.clear();

    Sclw_MACD_JdzsStrategy test4 = Sclw_MACD_JdzsStrategy();
    makePushOnReport(&test4 , "rb" , global_timePeriod , "2010/03/27" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    makePushOnReport(&test4 , "cu" , global_timePeriod , "2007/06/30" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );

    makePushOnReport(&test4 , "hc" , global_timePeriod , "2015/03/21" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    makePushOnReport(&test4 , "i" , global_timePeriod ,  "2014/10/22" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    makePushOnReport(&test4 , "j" , global_timePeriod ,  "2012/04/15" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    makePushOnReport(&test4 , "jm" , global_timePeriod , "2014/03/22" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    makePushOnReport(&test4 , "zc" , global_timePeriod , "2014/09/26" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );

    _config.outputFileLists("ipqhjjybj, pushOnMake_if_ic_ih test");
    system("uploadReport.bat");
}
void pushOnMake_nong(){
    reportFileLists.clear();

    Sclw_MACD_JdzsStrategy test4 = Sclw_MACD_JdzsStrategy();
    makePushOnReport(&test4 , "a" , global_timePeriod ,  "2007/06/30" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    makePushOnReport(&test4 , "c" , global_timePeriod ,  "2007/06/30" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );

    makePushOnReport(&test4 , "cf" , global_timePeriod , "2007/06/30" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    makePushOnReport(&test4 , "cs" , global_timePeriod , "2015/02/19" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    makePushOnReport(&test4 , "jd" , global_timePeriod , "2014/11/11" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    makePushOnReport(&test4 , "m" , global_timePeriod ,  "2007/06/30" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    makePushOnReport(&test4 , "oi" , global_timePeriod , "2008/06/08" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );

    makePushOnReport(&test4 , "rm" , global_timePeriod , "2013/12/28" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    makePushOnReport(&test4 , "sr" , global_timePeriod , "2007/06/30" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    makePushOnReport(&test4 , "y" , global_timePeriod ,  "2007/06/30" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );

    _config.outputFileLists("ipqhjjybj, pushOnMake_if_ic_ih test");
    system("uploadReport.bat");
}

void pushOnMake_jingshu(){
    reportFileLists.clear();

    Sclw_MACD_JdzsStrategy test4 = Sclw_MACD_JdzsStrategy();
    makePushOnReport(&test4 , "ag" , global_timePeriod , "2013/05/10" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    makePushOnReport(&test4 , "al" , global_timePeriod , "2007/06/30" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    makePushOnReport(&test4 , "au" , global_timePeriod , "2009/01/10" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    makePushOnReport(&test4 , "cu" , global_timePeriod , "2007/06/30" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    makePushOnReport(&test4 , "ni" , global_timePeriod , "2015/09/27" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    makePushOnReport(&test4 , "pb" , global_timePeriod , "2012/03/24" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    makePushOnReport(&test4 , "sn" , global_timePeriod , "2015/09/27" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    makePushOnReport(&test4 , "zn" , global_timePeriod , "2008/03/26" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );

    _config.outputFileLists("ipqhjjybj, pushOnMake_if_ic_ih test");
    system("uploadReport.bat");
}

void pushOnMake_syhg(){
    reportFileLists.clear();
    Sclw_MACD_JdzsStrategy test4 = Sclw_MACD_JdzsStrategy();
    makePushOnReport(&test4 , "bu" , global_timePeriod , "2014/10/22" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    makePushOnReport(&test4 , "fg" , global_timePeriod , "2013/12/03" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    makePushOnReport(&test4 , "l" , global_timePeriod ,  "2008/07/31" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    makePushOnReport(&test4 , "ma" , global_timePeriod , "2012/10/28" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    makePushOnReport(&test4 , "pp" , global_timePeriod , "2015/02/28" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    makePushOnReport(&test4 , "ru" , global_timePeriod , "2007/06/30" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    makePushOnReport(&test4 , "ta" , global_timePeriod , "2007/06/30" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    makePushOnReport(&test4 , "v" , global_timePeriod ,  "2009/05/25" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );

    _config.outputFileLists("ipqhjjybj, pushOnMake_if_ic_ih test");
    system("uploadReport.bat");
}

void pushOnMake_all(){
    reportFileLists.clear();
    PeriodDualthrust test4 = PeriodDualthrust();
    const char use_time_period[] = "min15";
    const char use_end_date[] = "2017/10/16";

    int use_tuijin_ybn_months = 12;
    int use_tuijin_ybw_months = 3;

    makePushOnReport(&test4 , "if" , use_time_period , "2011/04/16" , use_end_date , use_tuijin_ybn_months , use_tuijin_ybw_months );
    // makePushOnReport(&test4 , "ic" , use_time_period , "2015/10/16" , use_end_date , use_tuijin_ybn_months , use_tuijin_ybw_months );
    // makePushOnReport(&test4 , "ih" , use_time_period , "2015/10/16" , use_end_date , use_tuijin_ybn_months , use_tuijin_ybw_months );

    makePushOnReport(&test4 , "a" , use_time_period ,  "2007/06/30" , use_end_date , use_tuijin_ybn_months , use_tuijin_ybw_months );
    makePushOnReport(&test4 , "c" , use_time_period ,  "2007/06/30" , use_end_date , use_tuijin_ybn_months , use_tuijin_ybw_months );
    makePushOnReport(&test4 , "cs" , use_time_period , "2015/02/19" , use_end_date , use_tuijin_ybn_months , use_tuijin_ybw_months );
    makePushOnReport(&test4 , "i" , use_time_period ,  "2014/10/22" , use_end_date , use_tuijin_ybn_months , use_tuijin_ybw_months );
    makePushOnReport(&test4 , "j" , use_time_period ,  "2012/04/15" , use_end_date , use_tuijin_ybn_months , use_tuijin_ybw_months );
    makePushOnReport(&test4 , "jd" , use_time_period , "2014/11/11" , use_end_date , use_tuijin_ybn_months , use_tuijin_ybw_months );
    makePushOnReport(&test4 , "jm" , use_time_period , "2014/03/22" , use_end_date , use_tuijin_ybn_months , use_tuijin_ybw_months );
    makePushOnReport(&test4 , "l" , use_time_period ,  "2008/07/31" , use_end_date , use_tuijin_ybn_months , use_tuijin_ybw_months );
    // makePushOnReport(&test4 , "p" , use_time_period , "2008/10/29" , use_end_date , use_tuijin_ybn_months , use_tuijin_ybw_months );
    makePushOnReport(&test4 , "m" , use_time_period ,  "2007/06/30" , use_end_date , use_tuijin_ybn_months , use_tuijin_ybw_months );
    makePushOnReport(&test4 , "pp" , use_time_period , "2015/02/28" , use_end_date , use_tuijin_ybn_months , use_tuijin_ybw_months );
    makePushOnReport(&test4 , "v" , use_time_period ,  "2009/05/25" , use_end_date , use_tuijin_ybn_months , use_tuijin_ybw_months );
    makePushOnReport(&test4 , "y" , use_time_period ,  "2007/06/30" , use_end_date , use_tuijin_ybn_months , use_tuijin_ybw_months );


    makePushOnReport(&test4 , "ag" , use_time_period , "2013/05/10" , use_end_date , use_tuijin_ybn_months , use_tuijin_ybw_months );
    makePushOnReport(&test4 , "al" , use_time_period , "2007/06/30" , use_end_date , use_tuijin_ybn_months , use_tuijin_ybw_months );
    makePushOnReport(&test4 , "au" , use_time_period , "2009/01/10" , use_end_date , use_tuijin_ybn_months , use_tuijin_ybw_months );
    makePushOnReport(&test4 , "bu" , use_time_period , "2014/10/22" , use_end_date , use_tuijin_ybn_months , use_tuijin_ybw_months );
    makePushOnReport(&test4 , "cu" , use_time_period , "2007/06/30" , use_end_date , use_tuijin_ybn_months , use_tuijin_ybw_months );
    makePushOnReport(&test4 , "hc" , use_time_period , "2015/03/21" , use_end_date , use_tuijin_ybn_months , use_tuijin_ybw_months );
    makePushOnReport(&test4 , "ni" , use_time_period , "2015/09/27" , use_end_date , use_tuijin_ybn_months , use_tuijin_ybw_months );
    makePushOnReport(&test4 , "pb" , use_time_period , "2012/03/24" , use_end_date , use_tuijin_ybn_months , use_tuijin_ybw_months );
    makePushOnReport(&test4 , "rb" , use_time_period , "2010/03/27" , use_end_date , use_tuijin_ybn_months , use_tuijin_ybw_months );
    makePushOnReport(&test4 , "ru" , use_time_period , "2007/06/30" , use_end_date , use_tuijin_ybn_months , use_tuijin_ybw_months );
    makePushOnReport(&test4 , "sn" , use_time_period , "2015/09/27" , use_end_date , use_tuijin_ybn_months , use_tuijin_ybw_months );
    makePushOnReport(&test4 , "zn" , use_time_period , "2008/03/26" , use_end_date , use_tuijin_ybn_months , use_tuijin_ybw_months );
    
    makePushOnReport(&test4 , "cf" , use_time_period , "2007/06/30" , use_end_date , use_tuijin_ybn_months , use_tuijin_ybw_months );
    makePushOnReport(&test4 , "fg" , use_time_period , "2013/12/03" , use_end_date , use_tuijin_ybn_months , use_tuijin_ybw_months );
    makePushOnReport(&test4 , "ma" , use_time_period , "2012/10/28" , use_end_date , use_tuijin_ybn_months , use_tuijin_ybw_months );
    makePushOnReport(&test4 , "oi" , use_time_period , "2008/06/08" , use_end_date , use_tuijin_ybn_months , use_tuijin_ybw_months );
    makePushOnReport(&test4 , "rm" , use_time_period , "2013/12/28" , use_end_date , use_tuijin_ybn_months , use_tuijin_ybw_months );
    // makePushOnReport(&test4 , "rs" , use_time_period , "2013/12/28" , use_end_date , use_tuijin_ybn_months , use_tuijin_ybw_months );
    makePushOnReport(&test4 , "sr" , use_time_period , "2007/06/30" , use_end_date , use_tuijin_ybn_months , use_tuijin_ybw_months );
    makePushOnReport(&test4 , "ta" , use_time_period , "2007/06/30" , use_end_date , use_tuijin_ybn_months , use_tuijin_ybw_months );
    makePushOnReport(&test4 , "zc" , use_time_period , "2014/09/26" , use_end_date , use_tuijin_ybn_months , use_tuijin_ybw_months );
    
}
void testMakeReport(){
    Sclw_MACD_JdzsStrategy test4 = Sclw_MACD_JdzsStrategy();
    //ctaTemplate *obj , const char *code ,const char *timePeriod , const char *start_date , const char *end_date , int ybn_months_1 = 18 , int ybn_months_2 = 18 , int ybw_months = 6 , int use_tuijin_months = 6, int use_total_cycle = 10
    //makePushOnThreePart(&test4 , global_code , global_timePeriod , global_start_date , global_end_date , 18 ,  18 ,  6 ,  6 , 4);
    makePushOnThreePart(&test4 , "rb" , global_timePeriod , "2009/04/19" , global_end_date , 18 ,  18 ,  6 ,  6 , 20);
    makePushOnThreePart(&test4 , "rb" , global_timePeriod , "2009/04/20" , global_end_date , 18 ,  18 ,  6 ,  6 , 20);
    makePushOnThreePart(&test4 , "rb" , global_timePeriod , "2009/04/21" , global_end_date , 18 ,  18 ,  6 ,  6 , 20);

    // makePushOnThreePart(&test4 , "rb" , global_timePeriod , "2009/04/21" , global_end_date , 12 ,  2 ,  2 ,  3 , 8);
    // makePushOnThreePart(&test4 , "rb" , global_timePeriod , "2009/04/22" , global_end_date , 12 ,  2 ,  2 ,  3 , 8);
    // makePushOnThreePart(&test4 , "rb" , global_timePeriod , "2009/04/23" , global_end_date , 12 ,  2 ,  2 ,  3 , 8);
    // makePushOnThreePart(&test4 , "ih" , global_timePeriod , "2015/04/16" , global_end_date , 18 ,  18 ,  6 ,  6 , 4);
    // makePushOnThreePart(&test4 , "ih" , global_timePeriod , "2015/04/16" , global_end_date , 18 ,  18 ,  6 ,  6 , 4);
    // //makePushOnThreePart(&test4 , "ru" , global_timePeriod , "2004/01/02" , global_end_date , 18 ,  18 ,  6 ,  6 , 4 );
    // makePushOnThreePart(&test4 , "ag" , global_timePeriod , "2012/05/10" , global_end_date , 18 ,  18 ,  6 ,  6 , 4 );
    // makePushOnThreePart(&test4 , "cu" , global_timePeriod , "2004/01/02" , global_end_date , 18 ,  18 ,  6 ,  6 , 4 );
    // makePushOnThreePart(&test4 , "ni" , global_timePeriod , "2015/03/27" , global_end_date , 18 ,  18 ,  6 ,  6 , 4 );
    // makePushOnThreePart(&test4 , "oi" , global_timePeriod , "2007/06/08" , global_end_date , 18 ,  18 ,  6 ,  6 , 4 );
    // makePushOnThreePart(&test4 , "rb" , global_timePeriod , "2009/03/27" , global_end_date , 18 ,  18 ,  6 ,  6 , 4);
}

void testRunScore(){
    AligatorStrategy test4 = AligatorStrategy();
    // const char *code , const char *timePeriod , const char *start_date , const char *end_date, int ybn_months_1 , int ybn_months_2 , int ybw_months , int use_total_cycle = 30
    printAllYangben(&test4 , "rb" , global_timePeriod , "2009/04/19" , global_end_date , 18 , 3 , 3 , 4);
}
/**
 *
 ****/

void solve_livermoreTest(){
    is_zuhehuice = 1;
    __const_ini_money = 2000000;
    InvestmentProtfolio investment;

    LivermoreStrategy t1 = LivermoreStrategy();
    LivermoreStrategy t2 = LivermoreStrategy();
    LivermoreStrategy t3 = LivermoreStrategy();
    LivermoreStrategy t4 = LivermoreStrategy();
    LivermoreStrategy t5 = LivermoreStrategy();
    vector<double> tmp;

    tmp.clear();tmp.push_back(0.00847462); tmp.push_back(0.0310975);tmp.push_back(0.0312175); tmp.push_back( 0.0473956); tmp.push_back(200.0); tmp.push_back(200.0);// 0.00847462 0.0310975 0.0312175 0.0473956 200 200
    protfolioNode a = protfolioNode(&t1 , "cu" , "min30" , tmp , __const_ini_money) ;
    investment.addPro(a);
    //0.00946587 0.00402966 0.0501871 0.0187754 200 200
    tmp.clear();tmp.push_back(0.00946587); tmp.push_back(0.00402966);tmp.push_back(0.0501871); tmp.push_back( 0.0187754); tmp.push_back(200.0); tmp.push_back(200.0);
    protfolioNode b = protfolioNode(&t2 , "ih" , "min15" , tmp , __const_ini_money) ;
    investment.addPro(b);

    //RU,min15,0.0396271 0.0219996 0.00528047 0.0416787 200 200
    tmp.clear();tmp.push_back(0.0396271); tmp.push_back(0.0219996);tmp.push_back(0.00528047); tmp.push_back( 0.0416787); tmp.push_back(200.0); tmp.push_back(200.0);
    protfolioNode c = protfolioNode(&t3 , "ru" , "min15" , tmp , __const_ini_money) ;
    investment.addPro(c);
    //AG,min15,0.0329786 0.00524594 0.019681 0.0308595 200 200
    tmp.clear();tmp.push_back(0.0329786); tmp.push_back(0.00524594);tmp.push_back(0.019681); tmp.push_back( 0.0308595); tmp.push_back(200.0); tmp.push_back(200.0);
    protfolioNode d = protfolioNode(&t4 , "ag" , "min15" , tmp , __const_ini_money) ;
    investment.addPro(d);
    //0.0121133 0.0151206 0.00472222 0.0098238 200 200
    tmp.clear();tmp.push_back(0.0121133); tmp.push_back(0.0151206);tmp.push_back(0.00472222); tmp.push_back( 0.0098238); tmp.push_back(200.0); tmp.push_back(200.0);
    protfolioNode e = protfolioNode(&t5 , "ni" , "min15" , tmp , __const_ini_money) ;
    investment.addPro(e);

    investment.loadFile(global_start_date , global_end_date);
    investment.init();
    investment.run(global_start_date , global_end_date);
}
void check(){
    //checkLegalCanshu(__bestCanshuVector);
    vector<double> vec; vec.clear();
    vec.push_back(1.0);
    vec.push_back(2.0);
    pair<double,double> poly = polyfit(vec);
    printf("x,y : %lf,%lf\n",poly.first, poly.second);
}

void solve(){
    //AligatorStrategy test4 = AligatorStrategy();
    //LivermoreStrategyTest test4 = LivermoreStrategyTest();
    //ShuangjunxianStrategy test4 = ShuangjunxianStrategy();
    //LivermoreStrategy test4 = LivermoreStrategy();
    //LivermoreStrategyTest test4 = LivermoreStrategyTest();
    //LivermoreStrategyTest2 test4 = LivermoreStrategyTest2();
    //BollingerBotStrategy test4 = BollingerBotStrategy();
    //kingKeltnerStrategy test4 = kingKeltnerStrategy();
    //DualThrustStrategy test4 = DualThrustStrategy();
    //PeriodDualthrust test4 = PeriodDualthrust();
    Sclw_JdzsStrategy test4 = Sclw_JdzsStrategy();

    backtesting_assessment_report_and_Other_optimal_parameter_sets_report(&test4 , "rb" , "min30" , "2015/12/01" , "2017/12/01" ,solve_1_param1 , solve_2_param2 ,1);
    // backtesting_assessment_report_and_Other_optimal_parameter_sets_report(&test4 , "sn" , "min30" , "2016/12/01" , "2017/12/01" ,solve_1_param1 , solve_2_param2 ,1);
    // backtesting_assessment_report_and_Other_optimal_parameter_sets_report(&test4 , "hc" , "min30" , "2016/12/01" , "2017/12/01" ,solve_1_param1 , solve_2_param2 ,1);
    // backtesting_assessment_report_and_Other_optimal_parameter_sets_report(&test4 , "rb" , "min30" , "2016/12/01" , "2017/12/01" ,solve_1_param1 , solve_2_param2 ,1);
    

    //makeFirstFilterReport(&test4 , "min15" , "2017/06/30" , 4);

    //makePushOnReport(&test4 , "rb" , "min5" , "2009/03/27" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    //makePushOnReport(&test4 , global_code , global_timePeriod , global_start_date , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    //makeBestCanshuUsingOneAlgorithm(&test4 , global_start_date , global_end_date);

    //backtesting_assessment_report_and_Other_optimal_parameter_sets_report(&test4 , "zc" , global_timePeriod , "2014/10/22" , global_end_date ,solve_1_param1 , solve_2_param2 ,1);
    //Core_index_of_in_or_out_of_sample_report(&test4 , global_code , global_timePeriod , ybn_start_date , ybn_end_date, ybw_start_date, ybw_end_date);
    //get_PROM_of_each_future_in_diff_period_report( &test4 , global_code , global_timePeriod , global_start_date , global_end_date);
    //get_niuxiong_every_future_every_period_report( &test4 , global_code , global_timePeriod);
}


void solve2(){
    use_yongan_data = 1;
    //AligatorStrategy test4 = AligatorStrategy();
    //LivermoreStrategyTest test4 = LivermoreStrategyTest();
    //LivermoreStrategy test4 = LivermoreStrategy();
    //LivermoreStrategy test4 = LivermoreStrategy();
    //LivermoreStrategyTest test4 = LivermoreStrategyTest();
    //LivermoreStrategyTest2 test4 = LivermoreStrategyTest2();
    //BollingerBotStrategy test4 = BollingerBotStrategy();
    //kingKeltnerStrategy test4 = kingKeltnerStrategy();
    //DualThrustStrategy test4 = DualThrustStrategy();
    //LivermoreStrategyTest3 test4 = LivermoreStrategyTest3();
    //Sclw_JdzsStrategy test4 = Sclw_JdzsStrategy();
    //TwoEMAStrategy test4 = TwoEMAStrategy();
    //MACDHaiTrunStrategy test4 = MACDHaiTrunStrategy();
    //Sclw_MACD_JdzsStrategy test4 = Sclw_MACD_JdzsStrategy();
    //AtrRsiStrategy test4 = AtrRsiStrategy();
    //PeriodDualthrust test4 = PeriodDualthrust();
    //PeriodBollingerBandit test4 = PeriodBollingerBandit();
    PeriodTSKingKeltner test4 = PeriodTSKingKeltner();

    reportFileLists.clear();
    makeFirstFilterReport(&test4 , "hour1" , "2016/12/30" , 4);
    _config.outputFileLists("ipqhjjybj, PeriodTSKingKeltner FirstFilterReport hour1 run6");
    system("uploadReport.bat");

    reportFileLists.clear();
    makeFirstFilterReport(&test4 , "min30" , "2016/12/30" , 4);
    _config.outputFileLists("ipqhjjybj, PeriodTSKingKeltner FirstFilterReport min30 run6");
    system("uploadReport.bat");

    reportFileLists.clear();
    makeFirstFilterReport(&test4 , "min15", "2016/12/30" , 4);
    _config.outputFileLists("ipqhjjybj, PeriodTSKingKeltner FirstFilterReport min15 run6");
    system("uploadReport.bat");

    reportFileLists.clear();
    makeFirstFilterReport(&test4 , "day", "2016/12/30" , 4);
    _config.outputFileLists("ipqhjjybj, PeriodTSKingKeltner FirstFilterReport day run6");
    system("uploadReport.bat");

    // reportFileLists.clear();
    // makeFirstFilterReport(&test4 , "hour1" , "2017/06/30" , 4);
    // _config.outputFileLists("ipqhjjybj, LivermoreStrategyTest FirstFilterReport hour1 run6");
    // system("uploadReport.bat");

    // reportFileLists.clear();
    // makeFirstFilterReport(&test4 , "min30" , "2017/06/30" , 4);
    // _config.outputFileLists("ipqhjjybj, LivermoreStrategyTest FirstFilterReport min30 run6");
    // system("uploadReport.bat");

    //makePushOnReport(&test4 , "rb" , "min5" , "2009/03/27" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    //makePushOnReport(&test4 , global_code , global_timePeriod , global_start_date , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    //makeBestCanshuUsingOneAlgorithm(&test4 , global_start_date , global_end_date);

    //backtesting_assessment_report_and_Other_optimal_parameter_sets_report(&test4 , global_code , global_timePeriod , global_start_date , global_end_date ,solve_1_param1 , solve_2_param2 ,1);
    //Core_index_of_in_or_out_of_sample_report(&test4 , global_code , global_timePeriod , ybn_start_date , ybn_end_date, ybw_start_date, ybw_end_date);
    //get_PROM_of_each_future_in_diff_period_report( &test4 , global_code , global_timePeriod , global_start_date , global_end_date);
    //get_niuxiong_every_future_every_period_report( &test4 , global_code , global_timePeriod);
}


void solve_algorithm_merge_zuhe(){
    __const_ini_money = 2000000;
    is_zuhehuice = 1;
    InvestmentProtfolio investment;
    investment.clear();

    AligatorStrategy t1 = AligatorStrategy();
    vector<double> now_cansh_cs = getParams("3 7 22 2 5 6 14 5 17.482 17.8372 0 4 19.3958 19.49 1 19.1607 19.2843 2.6204 5"," ");
    protfolioNode cs = protfolioNode(&t1 , "cs" , "min30" , now_cansh_cs , __const_ini_money) ;
    investment.addPro(cs);

    AligatorStrategy t2 = AligatorStrategy();
    vector<double> now_cansh_j = getParams("3 5 9 1 4 9 17 3 9.32035 15.9689 0 4 19.0477 19.1264 1 19.1362 19.5105 2.30146 5"," ");

    protfolioNode j = protfolioNode(&t2 , "j" , "min30" , now_cansh_j , __const_ini_money) ;
    investment.addPro(j);

    investment.loadFile("2015/02/19" , "2017/06/30"); //"2015/02/19" , "2017/06/30"   , 2012/04/15
    investment.init();
    investment.run("2015/02/19" , "2017/06/30");

    investment.exportReport("test" , "backtesting_firstLine");
}

void solve_file_zuhe(){
    reportFileLists.clear();
    makeCombinationReport("./output/combination_aligator_min30_all_4.txt", 1000000.0 ,"2017/06/30","zuhe all 2 ","zuhe all 2");
    //_config.outputFileLists("ipqhjjybj, combination zuhe all");
    // reportFileLists.clear();
    // makeCombinationReport("./output/combination_aligator_min30_all_1.txt", 1000000.0 ,"2017/06/30","zuhe all","zuhe all");
    // _config.outputFileLists("ipqhjjybj, combination zuhe all");
    // system("uploadReport.bat");
    // reportFileLists.clear();
    // makeCombinationReport("./output/combination_aligator_min30_all_2.txt", 1000000.0 ,"2017/06/30","zuhe exclude yellow ","zuhe exclude yellow ");
    // _config.outputFileLists("ipqhjjybj, combination exclude yellow");
    // system("uploadReport.bat");
    // reportFileLists.clear();
    // makeCombinationReport("./output/combination_aligator_min30_all_3.txt", 1000000.0 ,"2017/06/30","exclude yellow green","zuhe exclude yellow green");
    // _config.outputFileLists("ipqhjjybj, exclude yellow green");
    // system("uploadReport.bat");
}


void solve_continue_file_zuhe(){
    // reportFileLists.clear();
    // makeCombinationReport("./output/combination_aligator_min30_all_4.txt", 1000000.0 ,"2017/06/30","zuhe all 2 ","zuhe all 2");

    reportFileLists.clear();
    makeContinueCombinationReport("./output/combination_continue_aligator_min30_all_0.txt" , 1000000.0 , "test" , "zuhe");
}

void test_sb(){
    vector<TradingResultNode *> v_report;
    const char start_date[40] = "2009/01/27"; // 5.30 buy
    const char end_date[40] = "2016/12/31";
    // const char start_date[40] = "2017/01/01";
    // const char end_date[40] = "2017/10/13";
    //const char end_date[40] = "2009/03/28";
    const char symbol[40] = "rb";
    const char period[40] = "min30";
    use_yongan_data = 0;
    PeriodTSKingKeltner test4 = PeriodTSKingKeltner();
    //AtrRsiStrategy test4 = AtrRsiStrategy();
    //Sclw_MACD_JdzsStrategy test4 = Sclw_MACD_JdzsStrategy();
    //MACDHaiTrunStrategy test4 = MACDHaiTrunStrategy();
    //PeriodDualthrust test4 = PeriodDualthrust();
    //PeriodBollingerBandit test4 = PeriodBollingerBandit();

    //BollingerAverageStrategy test4 = BollingerAverageStrategy();
    //TwoEMAStrategy test4 = TwoEMAStrategy();
    //Sclw_JdzsStrategy test4 = Sclw_JdzsStrategy();
    //SclwStrategy test4 = SclwStrategy();
    //ShuangjunxianStrategy test4 = ShuangjunxianStrategy();
    //LivermoreStrategyTest test4 = LivermoreStrategyTest();
    //LivermoreStrategy test4 = LivermoreStrategy();
    //LivermoreStrategyTest3 test4 = LivermoreStrategyTest3();
    //LivermoreStrategy2 test4 = LivermoreStrategy2();
    //ShuangjunxianStrategy test4 = ShuangjunxianStrategy();
    v_report.clear();

    vector<string> filenames = getFileName( symbol , period);
    for(int i = 0;i < filenames.size();i++){
        printf("%s\n",filenames[i].c_str());
    }
    //example 1 : 6 9 10 1 3 8 5 5 1.11829 8.62563 0 4 19.1944 19.1338 1 19.0581 19.5247 2.86898 5
    //6 11 21 3 5 10 6 4 13.6541 18.072 0 4 19.6897 19.217 1 19.628 19.7154 2.33149 5
    //shuangjunxian 15 25 125200 4 2 14 6 4 1"
    //0.026 0.014667 0.02 0 200 200
    //0.03 0.02 0.015 0.01 200 200 10
    //12 26 9 4 6 14 3 3 30 70 15 0.8 10 60
    // sclw_jdzs : 5 2 5 20
    // twoAverage : 5 20
    // BollingerAverageStrategy: 14 1.8
    // MACDHaiTrunStrategy: 12 26 9
    // Sclw_MACD_JDZS : 5 2 12 26 9
    // AtrRsiStrategy : 22 10 5 16 0.8
    // PeriodDualthrust : 1 0.5 0.5 1 1
    // PeriodBollingerBandit : 50 1.25 30 50
    // PeriodTSKingKeltner : 40 20 
    vector<double> now_cansh_j = getParams("40 20"," ");

    test4.loadFiles(filenames , start_date , end_date , period);
    test4.init(symbol);
    test4.setCanshuFromVec(now_cansh_j);
    test4.run();
    struct TradingResultNode tnode = test4.getTradingResult(symbol , period ,start_date , end_date);
    makeFigureCsv(tnode, (string(__report_dir_path) + test4.getStrategyName() + "/figure_trading_result/debug_sb8_" + symbol + "-" + period + "-" + replaceDate(string(start_date)) + "-" + replaceDate(string(end_date)) + "--best-figure.csv").c_str() , "sb\n");
    string tradingResultFile = string(__report_dir_path) + test4.getStrategyName() + "/figure_trading_result/debug_sb8_" + symbol + "-" + period + "-" + replaceDate(string(start_date)) + "-" + replaceDate(string(end_date)) + "--best-trading_result.csv";
    debug_tradingResult( &tnode , tradingResultFile.c_str() ,"sb_line");
    printf( "net_income:%lf\n",tnode.net_profits);
    printf( "net_loss:%lf\n",tnode.net_loss);
    printf( "income:%lf\n",tnode.net_profits + tnode.net_loss);
    printf( "year_rate:%lf\n",tnode.rate_of_return);
    printf( "max_windrawal_rate:%lf\n",tnode.max_windrawal_rate);
    printf( "prom:%lf\n",tnode.targetFunctionPromScore);
    printf( "rankscore:%lf\n",tnode.targetFunctionRankScore);
    printf( "income/withdraw:%lf\n",(tnode.net_profits + tnode.net_loss) / fabs( tnode.max_windrawl_number ) );

    v_report.push_back(&tnode);
    v_report.push_back(&tnode);
    string back_filename = string(__report_dir_path) + test4.getStrategyName() + "/debug_sb8_" + symbol + "-" + period + "-" + replaceDate(string(start_date)) + "-" + replaceDate(string(end_date)) + "--backtesting_assessment_report.csv";
    backtesting_assessment_report(v_report , back_filename.c_str() , "sb");
}

void StockFilter(){
    //Sclw_JdzsStrategy test4 = Sclw_JdzsStrategy();
    //TwoEMAStrategy test4 = TwoEMAStrategy();
    MACDHaiTrunStrategy test4 = MACDHaiTrunStrategy();

    reportFileLists.clear();
    makeFirstFilterStockReport(&test4 , "day" , "2007/01/01", "2016/12/30" , 1);
    //_config.outputFileLists("ipqhjjybj, Sclw_JdzsStrategy FirstFilterReport hour1 run6");
    //system("uploadReport.bat");
}

void solveNiuXiong(){
    Sclw_MACD_JdzsStrategy test4 = Sclw_MACD_JdzsStrategy();
    vector<double> now_cansh_j = getParams("5 2 12 26 9"," ");
    _config.setBestResult(now_cansh_j);
    get_niuxiong_every_future_every_period_report( &test4 , "rb", "hour1");
}

void solve_simpleReport(){
    AtrRsiStrategy test4 = AtrRsiStrategy();
    makeSimpleReport(&test4 , "./simpleNeedReport/atrRsiStrategy.txt" , "./simpleNeedReport/atrRsiStrategy.txt.out");
}

void solve_protfolio_for_one_algorithm(){
    // makePushOnReport(&test4 , "rb" , global_timePeriod , "2010/03/27" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    // makePushOnReport(&test4 , "cu" , global_timePeriod , "2007/06/30" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );

    // makePushOnReport(&test4 , "hc" , global_timePeriod , "2015/03/21" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    // makePushOnReport(&test4 , "i" , global_timePeriod ,  "2014/10/22" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    // makePushOnReport(&test4 , "j" , global_timePeriod ,  "2012/04/15" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    // makePushOnReport(&test4 , "jm" , global_timePeriod , "2014/03/22" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );
    // makePushOnReport(&test4 , "zc" , global_timePeriod , "2014/09/26" , global_end_date , tuijin_ybn_months , tuijin_ybw_months );

    char firstLine[500];
    PeriodDualthrust test4 = PeriodDualthrust();
    vector<struct investment_result_node> investResult;
    vector<string> needCodes; needCodes.clear();
    needCodes.push_back( "rb");
    needCodes.push_back( "cu");
    // needCodes.push_back( "hc");
    needCodes.push_back( "i");
    needCodes.push_back( "j");
    needCodes.push_back( "jm");
    needCodes.push_back( "zc");

    investResult.clear();

    makeProtfolioReport( investResult , &test4, needCodes , "min30" , "2015/01/01" , "2015/12/31" , "firstLine");
    sprintf(firstLine , "./protfolio/%s_protfolio.csv" , test4.getStrategyName().c_str());
    FILE * fp = NULL; 
    if( (fp = fopen(firstLine, "w")) != NULL ){
        for(int i = 0;i < investResult.size();i++ ){
            struct investment_result_node *pp = &investResult[i];
            fprintf(fp , "%s;%lf\n", pp->str_canshu.c_str(),pp->score);
        }
        fclose(fp);
    }
}

int main(){
    srand((unsigned)time(0));
    

    //solve_protfolio_for_one_algorithm();
    //test_sb();

    //pushOnMake_all();
    
    //solve_simpleReport();
    solve();
    //solve2();

    //solveNiuXiong();
    //printf("Hello world!\n");
    //StockFilter();

    //solve_continue_file_zuhe();
    //solve_file_zuhe();
    //solve();

    //solve_algorithm_merge_zuhe();

    //solve_algorithm_merge_zuhe();
    //solve();
    //testRunScore();
    //testMakeReport();
    //needSymbol();
    /*******************************
    //pushOnMake_if_ic_ih();
    //pushOnMake_black();
    //pushOnMake_nong();
    //pushOnMake_jingshu();
    //pushOnMake_syhg();
    *********************************/
    //solve();
    //loopForce_debug();
    //loopForce2();
    //check();
    //solve_protfolio();
    //solve_livermoreTest();

    //_config.outputFileLists("ipqhjjybj, ShuangjunxianStrategy test");
    //system("uploadReport.bat");
    return 0;
}
