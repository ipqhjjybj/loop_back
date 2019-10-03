#include "talib.h"
#ifndef _load_config
#define _load_config


struct canshuNode{
    double l,r,bufa;
    canshuNode(const double _l,const double _r, const double _bufa){
        this->l = _l ; this->r = _r; this->bufa = _bufa;
    }
};
struct feeNode{
    char code[30];
    double csize,minMove,cash_deposit,perMoney , pj_money , pz_money;
    int feeType;
    feeNode(const char *_code,double _csize,double _minMove,double _cash_deposit,double _feeType,double _perMoney , double _pjMoney , double _pzMoney){
        strcpy(code,_code);
        this->csize = _csize; this->minMove = _minMove; this->cash_deposit = _cash_deposit;
        this->feeType = _feeType; this->perMoney = _perMoney;
        this->pj_money = _pjMoney ; this->pz_money = _pzMoney;
    }
};
struct niuxiongNode{
    char code[30], start_date[30],end_date[30];
    int state; // 0表示unknow , 1表示牛市, 2表示熊市, 3表示盘整 , 4表示循环
    niuxiongNode(const char *_code,const char *_start_date,const char *_end_date , int _state){
        strcpy(code , _code); strcpy(start_date,_start_date); strcpy(end_date,_end_date);
        this->state = _state;
    }
};
struct pinzhongNode{
    char code[30] , data_start_date[30] , our_start_date[30];
    pinzhongNode(const char *_code,const char *_data_start_date , const char *_our_start_date){
        strcpy(code,_code); strcpy(data_start_date,_data_start_date); strcpy(our_start_date , _our_start_date);
    }
};

static int noLegalAns = 0;                      // 一个是否有解得标记
static vector<string> reportFileLists;          //保存每次生成的 报告清单
static int use_yongan_data = 1;                 //是否设置成主力  0-->tb_data , 1-->永安数据 , 2-->stock_data  3-->stock_plate_data
static int is_zuhehuice = 0;                    //是否开启组合回测
static const int g_canshu_num_max = 20;
int g_canshu_yueshu[g_canshu_num_max][g_canshu_num_max] = {0};
static double __const_ini_money = 20000;         // 账户初始资金
static const double NoRiseReturn = 3.0;         // 账户无风险利率

static const string EngineBpSearchAlgorithm = "Bp";
static const string EngineForceAlgorithm   = "Force";
static const string EngineLiziqunAlgorithm = "liziqun";
static const string EngineRandomAlgorithm = "Random";

static const string *g_choose_searchAlgorithm = &EngineForceAlgorithm;

static const int rm_max_cycle = 1;
static const int rm_max_NumParticle = 1;
static const int rm_tmp_save_result_num = 1;
//static const int rm_total_cycle = 10;

static const int bp_max_cycle = 100;
static const int bp_max_NumParticle = 10000;

static const int lzq_NumParticle = 1000;
static const int lzq_max_cycle = 100;
static const double lzq_C1 = 1.5;
static const double lzq_C2 = 1.5;

static vector<double> __bestCanshuVector;       // 存储最优参数， 主要用于 效率推进分析
static const double eps = 1e-5;                 // 全局性的数学精度
static const char *__report_dir_path = "./report/";
//static const char *__future_yongan_lianxu_path = "//192.168.0.118/futureData/yonganData2/";          //永安连续的数据
//static const char *__future_yongan_change_date_path = "//192.168.0.118/futureData/yonganData2/changedate/";     //永安换期的数据
static const char *__future_yongan_lianxu_path = "//192.168.0.118/futureData/merge_yongan_shzq/";          //永安+上海中期 连续的数据
static const char *__future_yongan_change_date_path = "//192.168.0.118/futureData/merge_yongan_shzq/changedate/";     //永安+上海中期 换期的数据

static const char *__future_yongan_wenhua_path = "//192.168.0.118/futureData/wenhuaData/";         //永安文华的数据
static const char *__future_yongan_wenhua_plate_path = "//192.168.0.118/futureData/wenhuaPlateData/";         //永安文华板块的数据

static const char *__stock_single_dir_path = "//192.168.0.118/stockData/";
static const char *__stock_plate_dir_path = "//192.168.0.118/stockPlateData/";

static const char *__future_data_dir_path = "//192.168.0.118/futureData/";
//static const char *__future_data_dir_path = "D:/futureData/";

static const char *__rqalpha_all_code_path = "//192.168.0.118/stockData/rqalpha_codes.txt";
static const char *__stock_all_plate_path = "//192.168.0.118/stockPlateData/all_industry_code.txt";

static const char *__config_range_search_path = "./conf/canshu_range.txt";
static const char *__config_fee_path = "./conf/fee.txt";
static const char *__config_niuxiong_path = "./conf/niuxiong.txt";
static const char *__config_pinzhong_conf_path = "./conf/pinzhong_riqi.txt";
static const char *__config_canshu_global_path = "./conf/best_canshu.txt";
static const char *__config_canshu_yueshu_path = "./conf/yueshu_conf.txt";
static const char *__output_fileList_path = "./output/fileLists.csv";

static const char *__logPrintFilePath = "./log/log.csv";

static int allResultNum = 0;
static int spread_allResultNum = 0; 
static const int MaxSaveResultNum = 100;                // 保存的最大数目解

static const int PushResultMinOpenNum = 0;              // 保存最优解时，过滤的最小开仓天数

static char global_code[40]       = "rb";
static char global_timePeriod[40] = "min30";
static char global_start_date[40] = "2009/03/27";
static char global_end_date[40] = "2016/12/31";

static double solve_1_param1   = 0.7;
static double solve_2_param2   = 0.2;

static char ybn_start_date[50] = "2010/03/27" ;
static char ybn_end_date[50]   = "2015/03/27";
static char ybw_start_date[50] = "2015/03/28";
static char ybw_end_date[50]   = "2050/03/27";

static const int tuijinAnalyseMonthNum = 6;             //推进分析时，固定的推进月份
static const int tuijin_ybn_months = 42;                //推进分析时，样本内月数
static const int tuijin_ybw_months = 6;                 //推进分析时，样本外月份

/**
 * 获得文件名的位置
 两个位置:
 1、采取tb数据还是永安数据
 2、永安数据的换期合约地址
 ***/
vector<string> getFileName( const char * code , const char * period ){
    vector<string> vec; vec.clear();
    if(3 == use_yongan_data){           // 表示跑的是 股票的板块指数
        string path(__stock_plate_dir_path);
        vec.push_back( path + period + "/" + string(code) + ".csv");
    }else if(2 == use_yongan_data){     // 表示跑的是股票 ！！
        string path(__stock_single_dir_path);
        vec.push_back( path + period + "/" + string(code) + ".csv");
    }else if(1 == use_yongan_data){
        string path(__future_yongan_lianxu_path);

        vec.push_back( path +  period + "/" + string(code) + "zhuli_888_" + string(period) + ".csv");
        vec.push_back( path +  period + "/" + string(code) + "qianzhuli_888_" + string(period) + ".csv");
        vec.push_back( path +  period + "/" + string(code) + "cizhuli_888_" + string(period) + ".csv");
        vec.push_back( path +  period + "/" + string(code) + "cicizhuli_888_" + string(period) + ".csv");

        vec.push_back( string(__future_yongan_change_date_path) + string(code) + "/zhuli_change_date.csv");
        vec.push_back( string(__future_yongan_change_date_path) + string(code) + "/qianzhuli_change_date.csv");
        vec.push_back( string(__future_yongan_change_date_path) + string(code) + "/cizhuli_change_date.csv");
        vec.push_back( string(__future_yongan_change_date_path) + string(code) + "/cizhuli_change_date.csv");
    }else{
        string path(__future_data_dir_path);
        string new_code(code);
        if(new_code.size() < 2){
            new_code = new_code + "9888";
        }else{
            new_code = new_code + "888";
        }
        vec.push_back(path + period + "/" + string(new_code) + "_" + string(period) + ".csv");
    }
    return vec;
}

/**
 * 获得永安数据的位置
 **/
class Config{
public:
    vector<struct feeNode> _feeNodes;
    vector<struct niuxiongNode> _niuxiongNodes;
    vector<struct pinzhongNode> _pinzhongNodes;

    vector<pair<int,int> > _canshuYueshu;

    vector<string> _all_rqalpha_codes;
    vector<string> _all_plate_codes;

    FILE *logFile;
    /*
    B06
    B09
    B11
    C27
     */
    vector<string> getAllStockPlateCodes(){
        vector<string> tmpStr;
        char ts[40];
        FILE *fp;
        tmpStr.clear();
        if( (fp = fopen(__stock_all_plate_path , "r")) != NULL){
            while(fscanf(fp , "%s" , ts) != EOF){
                tmpStr.push_back( ts);
            }
        }else{
            printf("open file %s Error!" , __rqalpha_all_code_path);
        }
        return tmpStr;
    }
    /**
     *
    000005.XSHE
    000027.XSHE
    000028.XSHE
    000029.XSHE
    000033.XSHE
    000037.XSHE
    000038.XSHE
    000039.XSHE
     **/
    vector<string> getAllRqalpha_codes(){
        vector<string> tmpStr;
        char ts[40];
        FILE *fp;
        tmpStr.clear();
        if( (fp = fopen(__rqalpha_all_code_path , "r")) != NULL){
            while(fscanf(fp , "%s" , ts) != EOF){
                tmpStr.push_back( ts);
            }
        }else{
            printf("open file %s Error!" , __rqalpha_all_code_path);
        }
        return tmpStr;
    }

    /**
     *   code , size , minmovement ,
     input:
     a9888 10 1 11 1 2.7
     c9888 10 1 11 1 1.62
     cs888 10 1 11 1 2.02
     i9888 100 0.5 15 2 0.022
     */
    vector<struct feeNode> loadConfigFee(){
        char ts[30];
        double _csize,_minMove,_cash_deposit,_perMoney , _pjMoney , _pzMoney;
        int _feeType;
        vector<struct feeNode > vFee; vFee.clear();
        FILE *fp;
        if( (fp = fopen(__config_fee_path , "r")) != NULL ){
            while(fscanf(fp,"%s %lf %lf %lf %d %lf %lf",ts,&_csize,&_minMove,&_cash_deposit,&_feeType,&_pjMoney,&_pzMoney) != EOF){
                _perMoney = max(_pjMoney , _pzMoney);
                string tcode(ts);
                if(tcode.find("9888") != tcode.npos){
                    tcode = tcode.substr(0,1);
                }
                if(tcode.find("888") != tcode.npos){
                    tcode = tcode.substr(0,2);
                }
                _feeType = 3 - _feeType;
                vFee.push_back( feeNode(tcode.c_str() , _csize, _minMove , _cash_deposit , _feeType , _perMoney , _pjMoney , _pzMoney ) );
            }
            fclose(fp);
        }else{
            printf("open file %s Error!" , __config_fee_path);
        }
        return vFee;
    }

    /**
     *
     a9888 10 1 11 1 2.7
     c9888 10 1 11 1 1.62
     cs888 10 1 11 1 2.02
     i9888 100 0.5 15 2 0.022
     j9888 100 0.5 14 2 0.0432
     ****/
    vector<struct niuxiongNode > loadNiuxiong(){
        char ts[30],tst[30],ted[30];
        int _state;
        vector<struct niuxiongNode> vNiuxiong; vNiuxiong.clear();
        FILE *fp;
        if(fp = fopen(__config_niuxiong_path, "r")){
            while(fscanf(fp,"%s %s %s %d",ts,tst,ted,&_state) != EOF){
                string tcode(ts);
                if(tcode.find("9888") != tcode.npos){
                    tcode = tcode.substr(0,1);
                }
                if(tcode.find("888") != tcode.npos){
                    tcode = tcode.substr(0,2);
                }
                vNiuxiong.push_back( niuxiongNode(tcode.c_str(),tst,ted,_state));
            }
            fclose(fp);
        }else{
            printf("open file %s Error!" , __config_niuxiong_path);
        }
        return vNiuxiong;
    }
    /**
     *
    if 2010/04/16 2011/04/16
    ic 2015/04/16 2015/10/16
    ih 2015/04/16 2015/10/16
    a 2004/01/02 2007/06/30
    c 2004/09/22 2007/06/30
     **/
    vector<struct pinzhongNode> loadPinzhong(){
        char ts[30],tssrq[30],t_our_riqi[30];
        vector<struct pinzhongNode> vPinzhong; vPinzhong.clear();
        FILE *fp;
        if( fp = fopen(__config_pinzhong_conf_path , "r")){
            while( fscanf(fp , "%s %s %s" , ts , tssrq , t_our_riqi) != EOF){
                string tcode(ts);
                if(tcode.find("9888") != tcode.npos){
                    tcode = tcode.substr(0,1);
                }
                if(tcode.find("888") != tcode.npos){
                    tcode = tcode.substr(0,2);
                }
                printf("pinzhongNode %s,%s,%s\n",tcode.c_str() , tssrq , t_our_riqi);
                vPinzhong.push_back( pinzhongNode(tcode.c_str() , tssrq , t_our_riqi));
            }
        }else{
            printf("open file %s Error!" , __config_pinzhong_conf_path);
        }
        return vPinzhong;
    }
    /**
     *
    5 6 2
    8 10 2
    8 9 1
    3 4 2
    3 4 1
    5 10 2
    5 13 2
    9 13 2
    7 13 2
    7 13 2
     */
    vector<canshuNode> loadCanshuRange(){
        vector<struct canshuNode> vCanshu; vCanshu.clear();
        FILE *fp;
        if((fp = fopen(__config_range_search_path , "r")) != NULL){
            double l,r,bufa;
            while( fscanf(fp,"%lf %lf %lf",&l,&r,&bufa) != EOF){
                printf("%lf %lf %lf\n",l,r,bufa);
                vCanshu.push_back( canshuNode(l,r,bufa));
            }
            fclose(fp);
        }else{
            printf("open file %s Error!" , __config_fee_path);
        }
        return vCanshu;
    }
    void loadCanshuYueShu(){
        // 初始化约束条件
        for(int i = 0;i < g_canshu_num_max;i++){
            for(int j = 0;j < g_canshu_num_max;j++){
                g_canshu_yueshu[i][j] = 0;
            }
        }
        FILE *fp;
        if((fp = fopen( __config_canshu_yueshu_path, "r")) != NULL){
            _canshuYueshu.clear();
            int x1,x2;
            while(fscanf(fp,"%d %d",&x1,&x2) != EOF){
                x1 -= 1 , x2 -=1;
                g_canshu_yueshu[x1][x2] = 1;
                _canshuYueshu.push_back( make_pair(x1,x2));
                printf("%d %d\n",x1,x2);
            }
            fclose(fp);
        }else{
            printf("open file %s Error!" , __config_canshu_yueshu_path);
        }
    }
    /**
     *
    5.000000
    8.000000
    8.000000
    3.000000
    3.000000
    9.000000
    10.000000
    10.000000
    10.000000
    10.000000
     */
    vector<double> loadCanshuConfig(){
        vector<double> vCanshuConfig; vCanshuConfig.clear();
        FILE *fp;
        if( (fp = fopen(__config_canshu_global_path,"r")) != NULL){
            double v;
            while(fscanf(fp,"%lf",&v) != EOF){
                vCanshuConfig.push_back(v);
            }
            fclose(fp);
        }else{
            printf("open file %s Error!" , __config_canshu_global_path);
        }
        return vCanshuConfig;
    }
    //输出到log
    void appendLog(const string &s){
        fprintf(logFile , "%s\n" ,s.c_str());
    }

    Config(){
        _feeNodes = loadConfigFee();
        _niuxiongNodes = loadNiuxiong();
        _pinzhongNodes = loadPinzhong();
        __bestCanshuVector = loadCanshuConfig();
        loadCanshuYueShu();

        _all_rqalpha_codes = getAllRqalpha_codes();
        _all_plate_codes = getAllStockPlateCodes();

        if( (logFile = fopen(__logPrintFilePath , "w")) != NULL){

        }else{
            printf("canot write to log File!\n");
        }
    }

    // 通过t_code 获得一个 Fee节点
    struct feeNode getCodeFee(const char *t_code){
        for(int i = 0;i < _feeNodes.size(); i++){
            if(strcmp(_feeNodes[i].code , t_code) == 0){
                return _feeNodes[i];
            }
        }
        return feeNode(t_code,0.0,0.0,0.0,0,0.0,0.0,0.0);
    }
    // 通过t_code 获得一个牛熊时期节点
    vector<niuxiongNode> getNiuXiongNode(const char *t_code){
        vector<niuxiongNode > vNode; vNode.clear();
        for(int i = 0;i < _niuxiongNodes.size();i++){
            if(strcmp(_feeNodes[i].code , t_code) == 0){
                vNode.push_back(_niuxiongNodes[i]);
            }
        }
        return vNode;
    };

    // 通过查询手续费的那种，获得所有需要计算的品种
    vector<string> getAllFeeCodes(){
        vector<string> vCodes; vCodes.clear();
        for(int i = 0;i < _feeNodes.size() ; i++){
            vCodes.push_back( string(_feeNodes[i].code));
        }
        return vCodes;
    }
    vector<string> getAllPeriods(){
        vector<string> vPeriods; vPeriods.clear();
        // vPeriods.push_back(string("min5"));
        // vPeriods.push_back(string("min15"));
        vPeriods.push_back(string("min30"));
        vPeriods.push_back(string("hour1"));
        //vPeriods.push_back(string("hour4"));
        return vPeriods;
    }

    vector<double>& getCanshuSetValues(){
        return __bestCanshuVector;
    }

    string getCanshuSetStr(){
        vector<double> vec = this->getCanshuSetValues();
        stringstream stream;
        for(int i = 0;i < vec.size();i++){
            if(i == 0){
                stream << vec[i] ;
            }else{
                stream << " " <<vec[i] ;
            }
        }
        return string(stream.str());
    }

    void setBestResult(const vector<double> & bestResult){
        __bestCanshuVector.clear();
        for(int i = 0;i < bestResult.size();i++){
            __bestCanshuVector.push_back(bestResult[i]);
        }
        if(bestResult.size() > 0){
            FILE *fp = NULL;
            if( (fp = fopen(__config_canshu_global_path , "w")) != NULL){
                for(int i = 0;i < __bestCanshuVector.size();i++){
                    fprintf(fp,"%lf\n", __bestCanshuVector[i]);
                }
                fclose(fp);
            }else{
                printf( "cannot write to __config_canshu_global_path");
            }
        }
    }

    void outputFileLists(const string &firstLine){
        FILE *fp = NULL;
        if( (fp = fopen(__output_fileList_path , "w")) != NULL){
            fprintf(fp , "%s\n", firstLine.c_str());
            for(int i = 0;i < reportFileLists.size();i++){
                fprintf(fp , "%s\n",reportFileLists[i].c_str());
            }
            fclose(fp);
        }else{
            printf("cannot open output file %s\n", __output_fileList_path);
        }
    }
};

Config _config = Config();

#endif // _load_config
