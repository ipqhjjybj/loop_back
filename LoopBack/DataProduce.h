#ifndef _spread_template
#define _spread_template

#include <cstring>
#include "load_config.h"

struct __dataLine{
	string symbol;
    string datetime;
    string date;
    string time;
    double open;
    double high;
    double low;
    double close;
    double volume;

    string mergeSymbol;
    string mergeInfo;

    __dataLine( const string &_mergeSymbol , const string &_mergeInfo , const string &_datetime){
    	this->mergeSymbol = _mergeSymbol ;  this->mergeInfo = _mergeInfo;
    	this->datetime = _datetime;
    	this->symbol = "";
        this->date = ""; this->time = "";
        this->open = 0.0;  this->high = 0.0; this->low = 0.0; this->close = 0.0; this->volume = 0.0;
    }

    __dataLine( string _symbol , const string _datetime, const string _date, const string _time , double _open,double _high, double _low,double _close,double _volume){
        this->symbol = _symbol;
        this->datetime = _datetime;  this->date = _date; this->time = _time;
        this->open = _open;  this->high = _high; this->low = _low; this->close = _close; this->volume = _volume;

        this->mergeSymbol = _symbol;
        this->mergeInfo = this->makeInfo();
    }
    __dataLine(string _symbol ,const string _date, const string _time, double _open,double _high, double _low,double _close,double _volume){
        this->symbol = _symbol;
        this->datetime = _date + " " + _time;   this->date = _date;   this->time = _time;
        this->open = _open;  this->high = _high; this->low = _low; this->close = _close; this->volume = _volume;
        this->mergeSymbol = _symbol;
        this->mergeInfo = this->makeInfo();
    }
    string makeInfo(){
    	char ss[2000];
        sprintf(ss , "%lf:%lf:%lf:%lf:%lf",open,high,low,close,volume);
        return string(ss);
    }
    string printOut(){
        char ss[2000];
        sprintf(ss , "%s,%s %s,%lf,%lf,%lf,%lf,%lf",symbol.c_str(),date.c_str(),time.c_str(),open,high,low,close,volume);
        return string(ss);
    }
    string printInfo(){
    	return this->mergeSymbol + "," +  datetime + "," + this->mergeInfo;
    }
};

// zhuli_data
struct __zhuli_data{
	string code;
	string zhuliType;
	string period;
	__zhuli_data(const string &_code , const string &_zhuliType ,  const string &_period){
		this->code = _code;   this->zhuliType = _zhuliType;  this->period = _period;
	}
};

// 加载某日的数据
void loadLines( vector<__dataLine>& ulines , const char *code , const char *_zhuliType , const char *period  , const char *start_date , const char *end_date){
	char tdate[50],tdatetime[100],ttime[50];
	double open,high,low,close,volume,openInterest;

	string path(__future_yongan_lianxu_path);

	if(3 == use_yongan_data){
		path = string(__future_yongan_lianxu_path);
	}
	string filename = path + period + "/" + string(code) + string(_zhuliType) + "_888_" + string(period) + ".csv";
	ulines.clear();
	FILE *fp = fopen(filename.c_str() , "r");
	while( fscanf(fp,"%s %8s,%lf,%lf,%lf,%lf,%lf,%lf", tdate,ttime, &open,&high,&low,&close,&volume,&openInterest) != EOF){
        if(strcmp(start_date , tdate) <= 0 && strcmp(tdate , end_date) <= 0){
            sprintf(tdatetime, "%s %s",tdate,ttime);
            ulines.push_back(__dataLine(code , tdatetime , tdate , ttime , open , high , low , close , volume ) );
        }
    }
	fclose(fp);
}
// 复制 dataLines  数据
void copyFileDataLines( vector<__dataLine> &desc , const vector<__dataLine> &ori){
	desc.clear();
	int sz = ori.size();
	for(int i = 0;i < sz;i++){
		desc.push_back(ori[i]);
	}
}
// 合并两个文件
void mergeTwoFileLines( vector<__dataLine> &desc , const vector<__dataLine> &ori){
	vector<__dataLine> tmp_desc;  tmp_desc.clear();
	int sz_desc = desc.size() , sz_ori = ori.size();
	int jd = 0 , jo = 0;
	while( jd < sz_desc && jo < sz_ori){
		if( desc[jd].datetime == ori[jo].datetime){
			// 这两行是同一分钟，数据合并在一起
			string new_symbol = desc[jd].mergeSymbol + " " + ori[jo].mergeSymbol;
			string new_info = desc[jd].mergeInfo + "," + ori[jo].mergeInfo;
			string ori_datetime = desc[jd].datetime;
			tmp_desc.push_back( __dataLine(new_symbol , new_info , ori_datetime));
			jd++ , jo++;
		}else if(desc[jd].datetime < ori[jo].datetime){
			// 这两行不是同一分钟，jd计数+1
			jd++;
		}else{
			// desc[jd].datetime > ori[jo].datetime
			// 这两行不是同一分钟，jo计数+1
			jo++;
		}
	}
	desc.clear();
	copyFileDataLines(desc , tmp_desc);
}

// 制作价差文件数据
void makeSpreadData(vector<__dataLine> &to_res ,const vector<__zhuli_data> &toCombineDataStruct , const string &outFileName , const char * start_date , const char * end_date){
	int sz_tocombine = toCombineDataStruct.size();
	if( sz_tocombine > 0){
		to_res.clear();
		vector<__dataLine> to_tmp;  to_tmp.clear();
		loadLines(to_res , toCombineDataStruct[0].code.c_str() , toCombineDataStruct[0].zhuliType.c_str() , toCombineDataStruct[0].period.c_str() , start_date , end_date);
		for(int i = 1;i < sz_tocombine ;i++){
			loadLines(to_tmp , toCombineDataStruct[i].code.c_str() , toCombineDataStruct[0].zhuliType.c_str() , toCombineDataStruct[0].period.c_str() , start_date , end_date);
			mergeTwoFileLines(to_res , to_tmp);
		}
		FILE *fp = fopen( outFileName.c_str() , "w");
		int sz_res = to_res.size();
		for(int i = 0;i < sz_res ;i++){
			fprintf(fp , "%s\n", to_res[i].printInfo().c_str());
		}
		fclose(fp);
	}
}


// 解析单行的数据
void parseSingleLine( const struct __dataLine & combData,  vector<double> &openVec , vector<double> &highVec , vector<double> &lowVec , vector<double> &closeVec , vector<double> &volVec , string &tmp_datetime , string &date , string &time){
	string line = combData.mergeSymbol + "," + combData.mergeInfo;
	vector<string> spl_1 = split(line , ",");
	vector<string> sp_datetime ;
	int sz_spl = spl_1.size();
	openVec.clear();  highVec.clear(); lowVec.clear(); closeVec.clear(); volVec.clear();
	tmp_datetime = combData.datetime;

	sp_datetime = split( tmp_datetime , " ");
	date = sp_datetime[0];
	time = sp_datetime[1];
	for(int i = 1;i < sz_spl;i++){
		vector<string> sp_2 = split(spl_1[i] , ":");
		openVec.push_back(atof(sp_2[0].c_str()));
		highVec.push_back(atof(sp_2[1].c_str()));
		lowVec.push_back(atof(sp_2[2].c_str()));
		closeVec.push_back(atof(sp_2[3].c_str()));
		volVec.push_back(atof(sp_2[4].c_str()));

	}
}

#endif
