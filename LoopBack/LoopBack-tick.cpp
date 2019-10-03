#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <vector>
using namespace std;

/**
 * bar lines
 */
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
 * TB file input
 * 2009/03/27 09:00:00,3543,3663,3543,3663,7546,7862
 **/
vector<Bar> loadFile( char * filename){
    FILE *fp = NULL;
    vector<Bar> vbar; vbar.clear();
    char date[40] , time[40];
    double open,high,low,close;
    double volume,openInterest;
    if(fp = fopen(filename, "r")){
        while(fscanf(fp,"%s %5s,%lf,%lf,%lf,%lf,%lf,%lf", date,time, &open,&high,&low,&close,&volume,&openInterest) != EOF){
            //printf("%s %lf\n",time,open);
            vbar.push_back( Bar(date,time, open,high,low,close,volume,openInterest));
        }
        fclose(fp);
    }else{
        printf("open file %s Error!" , filename);
    }
    return vbar;
}

const int BufferSize = 50;
double closeArray[BufferSize + 10];
double openArray[BufferSize + 10];
double highArray[BufferSize + 10];
double lowArray[BufferSize + 10];

int CF = 5 , CM = 8 , CS = 13 , d_CF = 3 , d_CM = 5 , d_CS = 8;
int lots = 1;
int N_up = 10 , N_down = 10;
int zhisunlv_l = 10 , zhisunlv_s = 10;
int bufferCount = 0;

vector<int> orderList;

double lips_N[BufferSize+10] , teeth_N[BufferSize+10] , croco_N[BufferSize+10];
double lips[BufferSize+10] , teeth[BufferSize+10] , croco[BufferSize+10];

int zuiArray[BufferSize+10];
void cancelOrder(int orderID){} // 取消某个Order的编号
void onBar(const Bar &bar){
    //deal with orderList
    //for orderID in self.orderList
    for(int i = 0;i < orderList.size();i++)
        cancelOrder(orderList[i]);
    for(int i = BufferSize - 2; i >= 0;i--){
        openArray[i]   = openArray[i+1];
        closeArray[i]  = closeArray[i+1];
        highArray[i]   = highArray[i+1];
        lowArray[i]    = lowArray[i+1];

        lips_N[i]      = lips_N[i+1];
        teeth_N[i]     = teeth_N[i+1];
        croco_N[i]     = croco_N[i+1];

        zuiArray[i]    = zuiArray[i+1];
    }
    openArray[BufferSize-1]  = bar.open;
    closeArray[BufferSize-1] = bar.close;
    highArray[BufferSize-1]  = bar.high;
    lowArray[BufferSize-1]   = bar.low;

    if(bufferCount == 0){
        lips[BufferSize-1]    = bar.close;
        teeth_N[BufferSize-1] = bar.close;
        croco_N[BufferSize-1] = bar.close;
    }else{
        lips_N[BufferSize-1]  = self.lips_N[BufferSize-2] + 2.0 * (bar.close - lips_N[BufferSize-2]) / (CF + 1 );
        teeth_N[BufferSize-1] = self.teeth_N[BufferSize-2] + 2.0 * (bar.close - teeth_N[BufferSize-2]) / (CM + 1);
        croco_N[BufferSize-1] = self.croco_N[BufferSize-2] + 2.0 * (bar.close - croco_N[BufferSize-2]) / (CS + 1);
    }
    bufferCount += 1;
    if(bufferCount < BufferSize){
        return;
    }
    int zui_value = 0;
    if
}
void LoopBack(const vector<Bar> &vb ,const string start_date,const string end_date){
    for(int i = 0;i < vb.size() && i < 20; i++){
        Bar a = vb[i];
        //cout << a.printOut() << endl;
        //printf("qq %s %d\n",a.datetime,start_date.compare(a.datetime));
        //printf("ss %s %d\n",a.datetime,end_date.compare(a.datetime));
        if( start_date.compare(a.datetime) <= 0 && end_date.compare(a.datetime) >= 0){
                cout << a.printOut() << endl;
                onBar(a);
        }
    }
}
int main(){
    vector<Bar> vb = loadFile("rb888_30_minutes.csv");
    /*
    for(int i = 0;i < vb.size() && i < 10; i++){
        Bar a = vb[i];
        cout << a.printOut() << endl;

    }
    */
    LoopBack(vb , "2009/03/27" , "2050/03/31"); // 2050/03/31 will never be reached !! Take tare of it .
    return 0;
}
