#include<iostream>
#include<omp.h>
// for random number
#include<cstdlib>
#include<ctime>

using namespace std;

// Sequential Linear Regression
void seq_lr(int x[], int y[], int n){
    int sumxy=0;
    int x2=0;
    float m=0;
    float c=0;
    int sumx=0;
    int sumy=0;
    for(int i=0; i<n; i++){
        sumxy += x[i]*y[i];
        sumx += x[i];
        sumy += y[i];
        x2 += x[i]*x[i];
    } 
    m = float( ((n*sumxy) - (sumx*sumy)) )/ ((n*x2) - ((sumx)*(sumx)) );
    c = ( (sumy) - (m*sumx)) /  n;

    cout<<"Equation of line in Sequential: "<<"y = "<<m<<"x "<<"+ "<<c<<endl;
}

// Parallel Linear Regression
void par_lr(int x[], int y[], int n){
    int sumxy=0;
    int x2=0;
    float m=0;
    float c=0;
    int sumx=0;
    int sumy=0;
    #pragma omp parallel for reduction(+:sumxy, sumx, sumy, x2)
    for(int i=0; i<n; i++){
        sumxy += x[i]*y[i];
        sumx += x[i];
        sumy += y[i];
        x2 += x[i]*x[i];
    } 
    m = float( ((n*sumxy) - (sumx*sumy)) )/ ((n*x2) - ((sumx)*(sumx)) );
    c = ( (sumy) - (m*sumx)) /  n;

    cout<<"Equation of line in Parallel: "<<"y = "<<m<<"x "<<"+ "<<c<<endl;
}

void printArr(int arr[], int n){
    for(int i=0; i<n; i++){
        cout<<arr[i]<<" ";
    }
    cout<<endl;
}


int main(){
    cout<<"Application of HPC for AI/ML domain : "<<endl;
    // int x[] = {1, 2, 3, 4, 5};
    // int y[] = {2, 4, 5, 4, 5};
    // int x[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    // int y[] = {2, 8, 6, 8, 9, 15, 16, 20, 25, 30};

    const int n =10;
    int x[n]; 
    int y[n]; 

    srand(time(0));
    for(int i=0; i<n; i++){
        x[i] = rand()%100;
        y[i] = (rand()%100)+2;
    }
    for(int i=0; i<n; i++){
        y[i] = x[i]+2;
    }

    cout<<"Print Arr1:"<<endl;
    printArr(x, n);
    cout<<"Print Arr2:"<<endl;
    printArr(y, n);


    double start = omp_get_wtime();
    seq_lr(x, y, 10);
    double end = omp_get_wtime();
    cout<<"Execution time in Sequential: "<<end-start<<"sec"<<endl;
    
    cout<<endl;

    start = omp_get_wtime();
    par_lr(x, y, 10);
    end = omp_get_wtime();
    cout<<"Execution time in Parallel: "<<end-start<<"sec"<<endl;

}


