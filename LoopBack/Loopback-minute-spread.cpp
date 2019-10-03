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
#include <ctime>
#include <cstdlib>

#include "talib.h"

#include "DataProduce.h"
#include "strategy/spreadTestStrategy.h"

#include "spreadStrategyTemplate.h"
#include "TradingResult.h"
#include "report_spread.h"

using namespace std;


void solve(){
	vector<TradingResultNode> resultList ;
	const char start_date[40] = "2016/12/20"; // 5.30 buy
    const char end_date[40] = "2017/01/05";

    const char symbol[40] = "rb";
    const char period[40] = "min30";

    use_yongan_data = 0;
    vector<double> now_cansh_j = getParams("-42600.0"," ");

	SpreadTestStrategy spread = SpreadTestStrategy();

	vector<struct __zhuli_data> zhuliData ; zhuliData.clear();
	zhuliData.push_back( __zhuli_data("rb", "zhuli" , "min1") );
	zhuliData.push_back( __zhuli_data("cu", "zhuli" , "min1") );
	// zhuliData.push_back( __zhuli_data("ni", "zhuli" , "min1") );

	spread.loadFile( zhuliData , start_date , end_date);
	spread.init();

	spread.setCanshuFromVec(now_cansh_j);

	spread.run();

	// printf("debug 1\n");


	struct mergeAllTradingResult mergeResult = spread.getTradingResultList( resultList );

	makeSpredMergeFixReport( mergeResult , "spread_report.csv" , "test");

	// makeSpreadSingleReport( resultList[0] , "test1_report.csv" , "test");

	// printf("End!\n");s
}

void test_MakePushOnReport(){
	reportFileLists.clear();

	vector<TradingResultNode> resultList ;
	const char start_date[40] = "2013/05/10"; // 5.30 buy
    const char end_date[40] = "2017/06/30";

    const char symbol[40] = "rb";
    const char period[40] = "min30";

    use_yongan_data = 0;
    vector<double> now_cansh_j = getParams("-42600.0"," ");

	SpreadTestStrategy spread = SpreadTestStrategy();

	vector<struct __zhuli_data> zhuliData ; zhuliData.clear();
	zhuliData.push_back( __zhuli_data("rb", "zhuli" , "min1") );
	zhuliData.push_back( __zhuli_data("cu", "zhuli" , "min1") );

    // spread_backtesting_assessment_report_running( &spread , zhuliData , start_date , end_date ,  1 );

	spread_makePushOnReport( &spread , zhuliData , start_date , end_date  ,  36 , 6 );

	_config.outputFileLists("ipqhjjybj, spread test strategy ");
    system("uploadReport.bat");
}

int main(){
	srand((unsigned)time(0));

	// solve();
	test_MakePushOnReport();

	return 0;
}
