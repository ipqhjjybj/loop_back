void loopForce_debug(){
     vector<string> filenames = getFileName(global_code , global_timePeriod);
    AligatorStrategy obj = AligatorStrategy();
    obj.loadFiles(filenames , global_start_date , global_end_date , global_timePeriod);

    obj.init(global_code);
    vector<struct TradingResultNode> ans;
    ans.clear();
    priority_queue<struct TradingResultNode> Q;
    struct TradingResultNode t1 ;
    int flag1 = 1;
    int flag2 = 0;

    int baoben_n_beg = 1;
    int baobenlv_l_beg = 1;
    int baobenlv_s_beg = 1;
    int fuyinglv_l_beg = 1;
    int fuyinglv_s_beg = 1;
    int per_fy_beg = 1;
    int fuying_b_beg = 1;
    int kk = 0;

    FILE *fp = fopen("debug_loop.log.csv" , "w");

    vector<double> tmp; tmp.clear();
    tmp.push_back( 3.000000);
    tmp.push_back( 4.000000);
    tmp.push_back(30.000000);
    tmp.push_back(2.000000);
    tmp.push_back(2.000000);
    tmp.push_back(8.000000);
    tmp.push_back(6.000000);
    tmp.push_back(11.000000);
    tmp.push_back(1.000000);
    tmp.push_back(3.000000);
    tmp.push_back(0.000000);
    tmp.push_back(5.000000);
    tmp.push_back(20.000000);
    tmp.push_back(20.000000);
    tmp.push_back(1.000000);
    tmp.push_back(20.000000);
    tmp.push_back(20.000000);
    tmp.push_back(0.300000);
    tmp.push_back(6.000000);

    obj.setCanshuFromVec(tmp);
    obj.run();
    struct TradingResultNode t2 = obj.getTradingResult(global_code , global_timePeriod ,global_start_date , global_end_date);

    fclose(fp);
    while(!Q.empty()){
        ans.push_back(Q.top());
        Q.pop();
    }
    reverse(ans.begin() , ans.end());
    debug_report(ans , "debug" ,"debug!");
    if(ans.size() > 0){
        string output_start_date = replaceDate(string(global_start_date));
        string output_end_date   = replaceDate(string(global_end_date));
        //先找到最优解 净利润最高
        struct TradingResultNode *best = &(ans[0]);
        struct TradingResultNode *best_sharpe = &(ans[0]);
        for(int i = 1; i < ans.size(); i++){
            if(ans[i].targetFunctionPromScore > best->targetFunctionPromScore){
                best = &(ans[i]);
            }
            if(ans[i].sharpe > best_sharpe->sharpe){
                best_sharpe = &(ans[i]);
            }
        }
        _config.setBestResult( best->canshuSet);
        if(1 == 1){
            vector<TradingResultNode *> vTradeNodes; vTradeNodes.clear();
            vTradeNodes.push_back(best);
            vTradeNodes.push_back(best_sharpe);

            string fp_filename = string(__report_dir_path) + obj.getStrategyName()+"/" + global_code + "-" + global_timePeriod + "-" + output_start_date + "-" + output_end_date + "--backtesting_assessment_report.csv";
            char firstLine[400];
            sprintf(firstLine,"%s,%s,%s--%s\nLabel,目标函数1(净利润),目标函数2(夏普)",global_code, global_timePeriod , output_start_date.c_str() , output_end_date.c_str());
            backtesting_assessment_report(vTradeNodes , fp_filename.c_str() , firstLine);
        }
    }
}
void loopForce2(){
    // this->CF = arr[0]; this->CM = arr[1]; this->CS = arr[2]; this->d_CF = arr[3] ; this->d_CM = arr[4];  this->d_CS = arr[5];
    // this->N_up = arr[6]; this->N_down = arr[7]; this->zhisunlv_l = arr[8];  this->zhisunlv_s = arr[9];

    //vecLoopArr.clear();
    vector<string> filenames = getFileName(global_code , global_timePeriod);
    AligatorStrategy obj = AligatorStrategy();
    obj.loadFiles(filenames , global_start_date , global_end_date , global_timePeriod);


    vector<struct TradingResultNode> ans;
    ans.clear();
    priority_queue<struct TradingResultNode> Q;
    struct TradingResultNode t1 ;
    int flag1 = 1;
    int flag2 = 0;

    int baoben_n_beg = 1;
    int baobenlv_l_beg = 1;
    int baobenlv_s_beg = 1;
    int fuyinglv_l_beg = 1;
    int fuyinglv_s_beg = 1;
    int per_fy_beg = 1;
    int fuying_b_beg = 1;
    int kk = 0;

   // FILE *fp = fopen("debug_loop.log.csv" , "w");

    for(int CF = 3; CF < 8;CF += 2){
        for(int CM = CF + 1;CM < 16; CM += 3){
            for(int CS = CM +1  ;CS < 31;CS += 5){
                for(int d_CF = 2; d_CF < 5; d_CF += 2){
                    for(int d_CM = d_CF; d_CM < 8; d_CM += 2){
                        for(int d_CS = d_CM; d_CS < 13; d_CS += 3){
                            for(int N_up = 1; N_up < 20; N_up += 5){
                                for(int N_down = 1;N_down < 20; N_down += 5){
                                    for(int zhisunlv_l = 1; zhisunlv_l < 11; zhisunlv_l += 2){
                                        for(int zhisunlv_s = 1; zhisunlv_s < 11; zhisunlv_s += 2){
                                            for (int baoben_f = 0; baoben_f<3 ; baoben_f +=1){
                                                baoben_n_beg = 1;
                                                baobenlv_l_beg = 5;
                                                baobenlv_s_beg = 5;
                                                if (0 == baoben_f)
                                                {
                                                    baoben_n_beg = 5;
                                                    baobenlv_l_beg = 20;
                                                    baobenlv_s_beg = 20;
                                                }
                                                if (1 == baoben_f)
                                                {
                                                    baoben_n_beg = 5;
                                                }

                                                for (int baoben_n = baoben_n_beg; baoben_n<6; baoben_n +=2){
                                                    for (int baobenlv_l = baobenlv_l_beg; baobenlv_l<21; baobenlv_l +=5){
                                                        for (int baobenlv_s = baobenlv_s_beg; baobenlv_s<21; baobenlv_s +=5){
                                                            for(int zhisun_f = 1; zhisun_f<4; zhisun_f +=1){
                                                                fuyinglv_l_beg = 5;
                                                                fuyinglv_s_beg = 5;
                                                                per_fy_beg = -3;
                                                                fuying_b_beg = 2;
                                                                if (1 == zhisun_f)
                                                                {
                                                                    fuyinglv_l_beg = 20;
                                                                    fuyinglv_s_beg = 20;
                                                                    per_fy_beg = 3;
                                                                    fuying_b_beg = 6;
                                                                }
                                                                if ( 2 == zhisun_f)
                                                                {
                                                                    fuying_b_beg = 6;
                                                                }
                                                                for (int fuyinglv_l= fuyinglv_l_beg; fuyinglv_l<21; fuyinglv_l +=5){
                                                                    for (int fuyinglv_s = fuyinglv_s_beg; fuyinglv_s<21; fuyinglv_s +=5){
                                                                        for (int per_fy = per_fy_beg; per_fy<4; per_fy +=1){
                                                                            for (int fuying_b = fuying_b_beg; fuying_b<7; fuying_b +=2){


                                                                                vector<double> tmp; tmp.clear();
                                                                                tmp.push_back( CF * 1.0);
                                                                                tmp.push_back( CM * 1.0);
                                                                                tmp.push_back( CS * 1.0);
                                                                                tmp.push_back( d_CF * 1.0);
                                                                                tmp.push_back( d_CM * 1.0);
                                                                                tmp.push_back( d_CS * 1.0);

                                                                                tmp.push_back( N_up * 1.0);
                                                                                tmp.push_back( N_down * 1.0);

                                                                                tmp.push_back( zhisunlv_l * 1.0);
                                                                                tmp.push_back( zhisunlv_s * 1.0);

                                                                                tmp.push_back( baoben_f * 1.0);
                                                                                tmp.push_back( baoben_n * 1.0);
                                                                                tmp.push_back( baobenlv_l * 1.0);
                                                                                tmp.push_back( baobenlv_s * 1.0);

                                                                                tmp.push_back( zhisun_f * 1.0);
                                                                                tmp.push_back( fuyinglv_l * 1.0);
                                                                                tmp.push_back( fuyinglv_s* 1.0);
                                                                                tmp.push_back( per_fy * 0.1);
                                                                                tmp.push_back( fuying_b * 1.0);

                                                                                kk = kk + 1;


                                                                               /* for(int ui = 0;ui < tmp.size();ui++){
                                                                                    if(ui==0){
                                                                                        fprintf(fp , "%lf",tmp[ui]);
                                                                                    }else{
                                                                                        fprintf(fp , ",%lf",tmp[ui]);
                                                                                    }
                                                                                }fprintf(fp,"\n");
                                                                                fflush(fp);
                                                                                */

                                                                                obj.init(global_code);
                                                                                obj.setCanshuFromVec(tmp);
                                                                                obj.run();
                                                                                struct TradingResultNode t2 = obj.getTradingResult(global_code , global_timePeriod ,global_start_date , global_end_date);
                                                                                if( flag1 == 1 || t2 < t1){
                                                                                    t1 = t2;
                                                                                    flag1 = 0;
                                                                                    flag2 = 1;
                                                                                }
                                                                                if( flag2 == 1){
                                                                                    for(int i = 0;i < t1.canshuSet.size();i++){
                                                                                        if(i == t1.canshuSet.size()-1){
                                                                                            printf("%lf \n %lf\n %d\n", t1.canshuSet[i] , t1.compareValue() , kk);
                                                                                        }else{
                                                                                            printf("%lf ",t1.canshuSet[i]);
                                                                                        }
                                                                                    }
                                                                                    Q.push(t1);
                                                                                    if(Q.size() > 100){
                                                                                        Q.pop();
                                                                                    }
                                                                                    flag2 = 0;
                                                                                }



                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    while(!Q.empty()){
        ans.push_back(Q.top());
        Q.pop();
    }
    reverse(ans.begin() , ans.end());
    debug_report(ans , "debug" ,"debug!");
    if(ans.size() > 0){
        string output_start_date = replaceDate(string(global_start_date));
        string output_end_date   = replaceDate(string(global_end_date));
        //先找到最优解 净利润最高
        struct TradingResultNode *best = &(ans[0]);
        struct TradingResultNode *best_sharpe = &(ans[0]);
        for(int i = 1; i < ans.size(); i++){
            if(ans[i].targetFunctionPromScore > best->targetFunctionPromScore){
                best = &(ans[i]);
            }
            if(ans[i].sharpe > best_sharpe->sharpe){
                best_sharpe = &(ans[i]);
            }
        }
        _config.setBestResult( best->canshuSet);
        if(1 == 1){
            vector<TradingResultNode *> vTradeNodes; vTradeNodes.clear();
            vTradeNodes.push_back(best);
            vTradeNodes.push_back(best_sharpe);

            string fp_filename = string(__report_dir_path) + obj.getStrategyName()+"/" + global_code + "-" + global_timePeriod + "-" + output_start_date + "-" + output_end_date + "--backtesting_assessment_report.csv";
            char firstLine[400];
            sprintf(firstLine,"%s,%s,%s--%s\nLabel,目标函数1(净利润),目标函数2(夏普)",global_code, global_timePeriod , output_start_date.c_str() , output_end_date.c_str());
            backtesting_assessment_report(vTradeNodes , fp_filename.c_str() , firstLine);
        }
    }
}
