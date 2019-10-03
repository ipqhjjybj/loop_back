

#ifndef _example
#define _example

/**
 * 针对这种输入进行操作
    filename
    start_date end_date
    slippage rate perSize minMovementMultiPriceScale
    fee_type
    3 -->(多少组参数要跑)
    CF CM CS d_CF d_CM d_CS N_up N_down zhisunlv_l zhisunlv_s
    CF CM CS d_CF d_CM d_CS N_up N_down zhisunlv_l zhisunlv_s
    CF CM CS d_CF d_CM d_CS N_up N_down zhisunlv_l zhisunlv_s
    End  -->用来结束的东西

    rb888_30_minutes.csv
    2009/03/27 2050/03/31
    1.0 1.29 10.0 1.0
    1
    3
    5 8 13 3 5 8 10 10 10.0 10.0
    5 8 13 3 5 8 10 10 10.0 10.0
    5 8 13 3 5 8 10 10 10.0 10.0
End
void solve0(){
    vector<TradingResultNode> ans;
    char filename[100] , start_date[40] , end_date[40];
    double slippage,rate,perSize,minMovementMultiPriceScale;
    int CF,CM,CS,d_CF,d_CM,d_CS,N_up,N_down;
    int fee_type;
    double zhisunlv_l,zhisunlv_s;

    AligatorStrategy test1 = AligatorStrategy();
    ans.clear();
    while(scanf("%s",filename) != EOF){
        if(strcmp(filename , "End") == 0){
            break;
        }
        scanf("%s %s", start_date , end_date);
        test1.loadFile(filename , start_date , end_date);
        test1.init();
        if(strcmp(filename , "End") == 0){
            break;
        }else{
            scanf("%lf %lf %lf %lf",&slippage,&rate,&perSize,&minMovementMultiPriceScale);
            scanf("%d",&fee_type);
            test1.setSlippageAndRate( slippage, rate, perSize , minMovementMultiPriceScale,  1.0);
            test1.setFeeType(1);  // 手续费的种类
            int tmp_n;
            scanf("%d",&tmp_n);
            while(tmp_n){
                scanf("%d %d %d %d %d %d %d %d %lf %lf",&CF,&CM,&CS,&d_CF,&d_CM,&d_CS,&N_up,&N_down,&zhisunlv_l,&zhisunlv_s);
                test1.setCanshu(CF,CM,CS,d_CF,d_CM,d_CS,N_up,N_down,zhisunlv_l,zhisunlv_s);
                test1.run();
                ans.push_back(test1.getTradingResult(filename , "min30" ,start_date , end_date));
            }
        }
    }
}
**/

#endif