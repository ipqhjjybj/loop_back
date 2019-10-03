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
#include <cstring>

#include "DataProduce.h"

using namespace std;


void testLoadLines(){
	vector<__dataLine> u_lines;
	u_lines.clear();
	loadLines(u_lines , "rb" , "zhuli" , "min1" , "2017/01/01" , "2017/05/01");
	int sz = u_lines.size();
	for(int i = 0;i < sz;i++){
		string tst = u_lines[i].printOut();
		printf("%s\n",tst.c_str());
	}
}


void testMakeSpreadData(){
	vector<__dataLine> to_res; 
	vector<__zhuli_data> toCombineDataStruct; toCombineDataStruct.clear();
	toCombineDataStruct.push_back( __zhuli_data("rb", "zhuli" , "min1") );
	toCombineDataStruct.push_back( __zhuli_data("cu", "zhuli" , "min1") );
	toCombineDataStruct.push_back( __zhuli_data("ag", "zhuli" , "min1") );

	makeSpreadData( to_res ,toCombineDataStruct , "./data/spread/dataProduceOut.csv", "2017/01/01" , "2017/05/01");
}

int main(){

	// testLoadLines();
	testMakeSpreadData();

	return 0;
}
