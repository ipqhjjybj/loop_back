#include <cstdio>
#include <cmath>
// 将报单进行合法化， 需要按照最小一跳进行处理
double legalTradePrice(double price, double volume , double minMovement){
    int bei = 0;
    bei = price/minMovement+0.0000001;
    double price_new = 0.0;
    price_new = bei * 1.0 * minMovement;
    if(volume > 0 && fabs(price_new-price)> 0.000001)
    {
        price_new = price_new + minMovement;
    }
    return price_new;
}
double legalTradePrice2(double price, double volume , double minMovement){
    const int jingdu = 1000;
    if(volume > 0){
        int v = (int)(price*jingdu + 0.9) , k = minMovement * jingdu ;
        if(v % k == 0){
            return price;
        }else{
            return 1.0 * ( v / k * k / ( 1.0 * jingdu) + minMovement);
        }
    }else{
        int v = (int)(price*jingdu) , k = (int)(minMovement * jingdu) ;
        return 1.0 * (v / k * k / (1.0 * jingdu)) ;
    }
}

int main(){
	printf("%lf\n",legalTradePrice(9.2 , -1 , 0.001));
    return 0;
}
