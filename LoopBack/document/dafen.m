function result=dafen(data,cszj,pp)
    data1=time(data);
    time_jiaoyi=data1(1,4);
    res(:,1:3)=data1(:,1:3);
    result=[];
    [row,col]=size(res);
    xx=[];
    yy=[];
    for i=1:row
        res(i,4)=res(i,3)/cszj;%单笔收益率
        res(i,5)=sum(res(1:i,3));%累计净利润
        res(i,6)=res(i,5)/cszj;%累计收益率
        res(i,7)=(res(i,5)-max(res(1:i,5)))/(max(res(1:i,5))+cszj);%回撤率
        if i==1
            res(i,8)=0;%累计盈利次数
            res(i,9)=0;%累计亏损
        else
            res(i,8)=res(i-1,8);
            res(i,9)=res(i-1,9);
        end
        if (res(i,3)>0)
            res(i,8)=res(i,8)+1;
        else
            res(i,9)=res(i,9)+res(i,3);
        end
        res(i,10)=skewness(res(1:i,4));%偏度
        res(i,11)=kurtosis(res(1:i,4));%峰度
        res(i,12)=abs(res(i,5)/res(i,9));%盈利因子
        res(i,13)=power(min(0,res(i,4)-0.03/250),2);
        
        xx(i,1)=i;
        yy(i,1)=res(i,6)*100;
    end

    p=polyfit(xx,yy,1);
    aa=p(1);
    bb=p(2);
    ees=0;
    for i=1:row
        fit=aa*xx(i,1)+bb;
        ees=ees+power((yy(i,1)-fit),2);
    end
    ees=sqrt(ees/row);



    result(1,1)=(res(row,5)/cszj)*(250/time_jiaoyi);%年华收益率
    result(1,2)=result(1,1)*100*0.25;
    result(2,1)=min(res(:,7));%最大回撤率
    result(2,2)=result(2,1)*100*0.25;
    result(3,1)=res(row,8)/row;%胜率
    result(3,2)=result(3,1)*50*0.1;
    %result(4,1)=-1*(res(row,10)+res(row,11)-3)/2;%偏度*峰度
    result(4,1)=aa*100-ees-abs(bb/yy(row,1)*100);%线性回归
    result(4,2)=result(4,1)*0.1;
    result(5,1)=res(row,12);%盈利因子
    result(5,2)=result(5,1)*10*0.1;
    result(6,1)=(result(1,1)-0.03)/sqrt((sum(res(:,13))/(row-1))*250);%索提诺
    result(6,2)=result(6,1)*5*0.2;
    result(7,1)=result(1,2)+result(2,2)+result(3,2)+result(4,2)+result(5,2)+result(6,2);%排名打分
    result(8,1)=aa*100;
    result(9,1)=bb/yy(row,1)*100;
    result(10,1)=ees;


    plot(xx,yy,'o',xx,polyval(p,xx));
    legend(num2str(result(7,1)));
    saveas(gcf,strcat(num2str(pp),'.jpg'));

    save('res.mat','res');
    save('result.mat','result');
end