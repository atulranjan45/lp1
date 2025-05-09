#include<iostream>
#include<omp.h>

// for random number
#include<cstdlib>
#include<ctime>

using namespace std;


// Sequential Minimum
void seq_min(int arr[], int n){
    int min=arr[0];
    for(int i=0; i<n; i++){
        if(arr[i]<min){
            min=arr[i];
        }
    }
    cout<<"Minimum element in Sequential: "<<min<<endl;
}

// Parallel Minimum
void par_min(int arr[], int n){
    int min=arr[0];
    #pragma omp parallel for reduction(min:min)
    for(int i=0; i<n; i++){
        if(arr[i]<min){
            min=arr[i];
        }
    }
    cout<<"Minimum element in Parallel: "<<min<<endl;
}

// Sequential Maximum
void seq_max(int arr[], int n){
    int max=arr[0];
    for(int i=0; i<n; i++){
        if(arr[i]>max){
            max=arr[i];
        }
    }
    cout<<"Maximum element in Sequential: "<<max<<endl;
}

// Parallel Maximum
void par_max(int arr[], int n){
    int max_num=arr[0];
    #pragma omp parallel for reduction(max:max_num)
    for(int i=0; i<n; i++){
        if(arr[i]>max_num){
            max_num=arr[i];
        }
    }
    cout<<"Maximum element in Parallel: "<<max_num<<endl;
}


// Average
int avg(int sum, int n){
    int avg = sum/n;
    return avg;
}


// Sequential Sum
void seq_sum(int arr[], int n){
    int sum=0;
    for(int i=0; i<n; i++){
        sum += arr[i];
    }
    cout<<"Sequential Sum: "<<sum<<endl;  
    int avg_num = avg(sum, n);
    cout<<"Sequential Avg: "<<avg_num<<endl;  

}

// Parallel Sum
void par_sum(int arr[], int n){
    int sum_num=0;
    #pragma omp parallel for reduction(+:sum_num) 
    for(int i=0; i<n; i++){
        sum_num += arr[i];
    }
    cout<<"Parallel Sum: "<<sum_num<<endl;
    int avg_num = avg(sum_num, n);
    cout<<"Parallel Avg: "<<avg_num<<endl;  

}



int main(){
    cout<<"Min, Max, Sum, Avg using Parallel Reduction"<<endl;

    // int arr[] = {12, 4, 8, 18, 5};
    // int n=5;

    int n;
    cout<<"Enter size of array : ";
    cin>>n;
    // imp
    int* arr = new int[n];

    srand(time(0));
    for(int i=0; i<n; i++){
        arr[i] = rand()%10000;
    }

    // print array
    for(int i=0; i<n; i++){
        cout<<arr[i]<<" ";
    }
    cout<<endl;


    double start = omp_get_wtime();
    seq_min(arr, n);
    seq_max(arr, n);
    seq_sum(arr, n);
    double end = omp_get_wtime();
    cout<<"Execution time in sequential process : "<<end-start<<" sec"<<endl;
    
    cout<<endl;
    
    start=omp_get_wtime();
    par_min(arr, n);
    par_max(arr, n);
    par_sum(arr, n);
    end=omp_get_wtime();
    cout<<"Execution time in parallel process : "<<end-start<<" sec"<<endl;
    cout<<endl;



    // seq_min(arr, n);
    // par_min(arr, n);

    // cout<<endl;
    // seq_max(arr, n);
    // par_max(arr, n);

    // cout<<endl;
    // seq_sum(arr, n);
    // cout<<endl;
    // par_sum(arr, n);

}


