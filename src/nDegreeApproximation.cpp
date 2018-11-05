//
// Created by yagi on 18/10/03.
//

#include "nDegreeApproximation.h"
using namespace std;

void nDegreeApproximation::sai()
{
    int i,j,k;
    int n = this->_n;
    cv::Mat A = cv::Mat::zeros(n, n+1, CV_64F);

    //値の代入
    for(i=0; i<n; i++)
    {
        for(j=0; j<n; j++)
        {
            for(k=0; k < _S; k++)
            {
                A.at<double>(i,j) += pow(this->_data[k].x,i+j);
            }
        }
    }

    for(i=0;i<n;i++)
    {
        for(k=0;k<_S;k++)
        {
            A.at<double>(i,n)+=pow(this->_data[k].x,i)*this->_data[k].y;
        }
    }

    gauss(A);

}

void nDegreeApproximation::gauss(cv::Mat a){
    int i, j, k, pivot;
    float p, ai, amax;

    /*係数の出力*/
    printf("係数の出力\n");
    for(i=0;i<this->_N;i++)
    {
        for(j=0;j<this->_N+1;j++) {
            printf("%10.2f", a.at<double>(i, j));
        }
        printf("\n");
    }
    printf("\n");

    /*前進消去*/
    printf("前進消去\n");
    for(k=0;k<this->_N-1;k++)
    {
        amax=fabs((double)a.at<double>(k, k));
        pivot=k;
        for(i=k+1;i<this->_N;i++)
        {
            ai=fabs((double)a.at<double>(i, k));
            if(ai>amax)
            {
                amax=ai;
                pivot=i;
            }
            for(j=0;j<=this->_N;j++)
            {
                ai=a.at<double>(k, j);
                a.at<double>(k, j)=a.at<double>(pivot, j);
                a.at<double>(pivot, j)=ai;
            }

            for(i=0;i<this->_N;i++)
            {
                for(j=0;j<this->_N+1;j++)
                    printf("%10.2f",a.at<double>(i, j));
                printf("\n");
            }
            printf("\n");
        }

        for(i=k+1;i<this->_N;i++)
        {
            p=a.at<double>(i, k)/a.at<double>(k, k);
            for(j=0;j<this->_N+1;j++)
                a.at<double>(i, j)=a.at<double>(i, j)-p*a.at<double>(k, j);
        }

        /*計算過程の表示*/
        for(i=0;i<this->_N;i++)
        {
            for(j=0;j<this->_N+1;j++)
                printf("%10.2f",a.at<double>(i, j));
            printf("\n");
        }
        printf("\n");
    }

    /*後退代入*/
    a.at<double>(_N-1, _N)=a.at<double>(_N-1, _N)/a.at<double>(_N-1, _N);
    for(i=this->_N-2;i>=0;i--)
    {
        for(j=i+1;j<this->_N;j++)
            a.at<double>(i, _N)=a.at<double>(i, _N)-a.at<double>(i, j)*a.at<double>(j, _N);
        a.at<double>(i, _N)=a.at<double>(i, _N)/a.at<double>(i, i);
    }

    /*解の表示*/
    printf("解の表示\n");
    for(i=0;i<this->_N;i++)
    {
        printf("a[%d]=%f",this->_n-1-i,a.at<double>(i, _N));
        printf("\n");
    }
}