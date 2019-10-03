
 

學習粒子群最佳化(PSO)程式設計

陳慶瀚

E-mail : pierre@isu.edu.tw

 

　

 

粒子群最佳化(Particle Swarm Optimization, PSO)是一種以族群動力學為基礎的最佳化方法，它的概念來自社會行為的模擬。在一個社會化的族群中，個體的行為不但會受到個體過去經驗和認知的影響，同時也會受到整體社會行為影響。在PSO每個粒子在搜尋空間中個自擁有其速度，並且根據自我過去經驗與群體行為進行機率事的搜尋策略調整，PSO相似於傳統式遺傳演算法突變的機制，但和典型的突變不同的地方在於它並非完全隨機的，從很多的研究結果顯示PSO可以快速的在問題空間找出最佳解，對於動態系統提供了一個高度適應性的最佳化方法。其演算法描述如下：

 

步驟1、 問題的形式化。將D個參數最佳化的問題視為一個D維的求解空間，定義一個具有個粒子的族群，每個粒子的位置及速率均為D維的向量。

步驟2、初始化。以隨機方式設定每個粒子的位置及其速率。

步驟3、將粒子的位置代入求解問題評估函數以求得每個粒子的評估值。

步驟 4、每一個粒子的評估值與該粒子所經歷的最佳評估值比較，假若新的評估值比粒子的最佳評估值佳，則以新的位置及評估值取代粒子的最佳解位置及評估值。

步驟 5、每一個粒子的最佳評估值與群體的最佳評估值比較，假若粒子的最佳評估值比群體的最佳評估值佳，則以粒子的最佳解位置及評估值取代所有粒子的最佳解位置及評估值。

步驟 6、使用下列式子改變個體的速率並移動粒子位置：

     

                   

其中為粒子的位置,為粒子的速度，為粒子最佳解位置，為所有粒子的最佳解位置，，為-1至1的亂數值，

步驟 7、重複步驟3~步驟6，直到群體的最佳評估值符合我們的需求。

　
　

　

最精簡的PSO程式碼範例
 
#include <fstream.h>

#include <stdlib.h>

#include"randgen.h"

#define Dim 2  // Dimension of problem's search space

#define NumParticle 40  // Number of particles in population

#define max_cycle 100  // Maximum iteration cycle

#define C1 1.5

#define C2 1.5

 

void main()

{

// Variable Declaration

  float xLo[Dim],xHi[Dim];    // Lower bound and upper bound for each dimension

  float v[Dim][NumParticle],  //current velocity of particle

        x[Dim][NumParticle],  //current position of particle

        individual_optimal_fit[NumParticle], //individual optimal fitness

        individual_optimal_x[Dim][NumParticle], // individual optimal position

        social_optimal_fit,   // social optimal fitness     

        social_optimal_x[Dim]; // social optimal position

  fRan32 rand(123456789); // random number generator

 

// Initialization

  xLo[0]=-5;xHi[0]=5;xLo[1]=-5;xHi[1]=5;

  for(int i=0;i<Dim;i++)for(int j=0;j<NumParticle;j++)

  {

    x[i][j]=individual_optimal_x[i][j]=rand.Next(xLo[i],xHi[i]);

    v[i][j]=rand.Next(xLo[i]/20.,xHi[i]/20.);     

  }  

  for(int j=0;j<NumParticle;j++)individual_optimal_fit[j]=x[0][j]*x[0][j]+x[1][j]*x[1][j];

  social_optimal_fit=individual_optimal_fit[0];

  

// Optimization

  float fitness;

  for(int cycle=0;cycle<max_cycle;cycle++)

  {

    for(int j=0;j<NumParticle;j++)// Evaluate each particle

    {

      fitness=x[0][j]*x[0][j]+x[1][j]*x[1][j]; // evaluation function

      if(fitness>individual_optimal_fit[j])// Replace the individual optimal position

      {

        individual_optimal_fit[j]=fitness;

        individual_optimal_x[0][j]=x[0][j];

        individual_optimal_x[1][j]=x[1][j];    

      }      

    }

    for(int j=0;j<NumParticle;j++)// Replace the social optimal position

    {

       if(individual_optimal_fit[j]>social_optimal_fit)

       {

          social_optimal_fit=individual_optimal_fit[j];

          social_optimal_x[0]=individual_optimal_x[0][j];

          social_optimal_x[1]=individual_optimal_x[1][j];          

       }     

    }

    for(int i=0;i<Dim;i++)for(int j=0;j<NumParticle;j++)// Modify the velocity and position of particle

    {

       v[i][j]=v[i][j]+C1*rand.Next(-1.,1.)*(individual_optimal_x[i][j]-x[i][j])

              +C2*rand.Next(-1.,1.)*(social_optimal_x[i]-x[i][j]);

       x[i][j]=x[i][j]+v[i][j];

       if(x[i][j]>xHi[i])x[i][j]=xHi[i];

       if(x[i][j]<xLo[i])x[i][j]=xLo[i];

    }

  }

  ofstream out("result.txt");

  cout<<social_optimal_x[0]<<"\t"<<social_optimal_x[1]<<"\t"<<social_optimal_fit<<endl;

  out<<social_optimal_x[0]<<"\t"<<social_optimal_x[1]<<"\t"<<social_optimal_fit<<endl;

  system("PAUSE");   

}