#include<iostream>
#include<omp.h>
#include<cstdlib>
#include<ctime>

using namespace std;

// Sequential Bubble Sort
void seqBubbleSort(int arr[], int n){
    for(int i=0; i<n-1; i++){
        for(int j=0; j<n-1-i; j++){
            if(arr[j+1]<arr[j]){
                swap(arr[j+1], arr[j]);
            }
        }
    }
}

// Parallel Bubble Sort
void parBubbleSort(int arr[], int n){
    for(int i=0; i<n-1; i++){
        // even (koi bhi pehle odd ya even dal sakte)
        #pragma omp parallel for
        for(int j=0; j<n-1; j+=2){
            if(arr[j+1]<arr[j]){
                swap(arr[j+1], arr[j]);
            }
        }
        // odd
        #pragma omp parallel for
        for(int j=1; j<n-1; j+=2){
            if(arr[j+1]<arr[j]){
                swap(arr[j+1], arr[j]);
            }
        }

    }
}


void printArray(int arr[], int n){
    cout<<"Array: "<<endl;
    for(int i=0; i<n; i++){
        cout<<arr[i]<<" ";
    }
    cout<<endl;
}

void merge(int arr[], int si, int mid, int ei){
    // int temp[] = new int[ei-si+1];   // error
    int* temp = new int[ei-si+1];
    int i = si;
    int j = mid+1;
    int k = 0;

    while(i<=mid && j<=ei){
        if(arr[i]<arr[j]){
            temp[k] = arr[i];
            k++;
            i++;
        }
        else{
            temp[k] = arr[j];
            k++;
            j++;
        }
    }

    // left part
    while(i <= mid){
        temp[k] = arr[i];
        k++;
        i++;
    }

    // right part
    while(j <= ei){
        temp[k] = arr[j];
        k++;
        j++;
    }

    // copy temp into original array
    for(int m=0, i=si; m<k; m++, i++){
        arr[i] = temp[m];
    }

}

// Merge Sort
void mergeSort(int arr[], int si, int ei){
    // Base case -> Single Element
    if(si>=ei){
        return;
    }

    int mid = (si+ei)/2;

    // left
    mergeSort(arr, si, mid);
    // right
    mergeSort(arr, mid+1, ei);

    merge(arr, si, mid, ei);

}



// Parallel Merge Sort
void parMergeSort(int arr[], int si, int ei, int depth){
    // Base case -> Single Element
    if(si>=ei){
        return;
    }

    int mid = (si+ei)/2;
    // 
    if(depth<=2){
        #pragma omp parallel sections 
        {
            #pragma omp section
            parMergeSort(arr, si, mid, depth+1);
            
            #pragma omp section
            parMergeSort(arr, mid+1, ei, depth+1);
            
        }
    }
    else{
        mergeSort(arr, si, mid);
        mergeSort(arr, mid+1, ei);
    }

    // left
    // mergeSort(arr, si, mid);
    // right
    // mergeSort(arr, mid+1, ei);

    merge(arr, si, mid, ei);

}


// Dummy Array
void createArr(int arr[], int n){
    for(int i=0; i<n; i++){
        arr[i] = rand() % 1000;
    }
}

// copy array
void copyArr(int arr[], int arr1[], int n){
    for(int i=0; i<n; i++){
        arr1[i] = arr[i];
    }
}

int main(){
    cout<<"Parallel and Sequential Bubble and Merge Sort pri eo: "<<endl;
    // int arr[] = {12, 6, 8, 4, 15, 1};
    // int n = 6;

    srand(time(0));
    const int n = 1000;
    int arr1[n], arr2[n], arr3[n], arr4[n];
    createArr(arr1, n);

    copyArr(arr1, arr2, n);
    copyArr(arr1, arr3, n);
    copyArr(arr1, arr4, n);

    cout<<"Bubble Sort res : "<<endl;

    double start = omp_get_wtime();
    seqBubbleSort(arr1, n);
    double end = omp_get_wtime();
    cout<<"Bubble sort (Sequential): "<<end-start<<"Sec"<<endl;
    
    start = omp_get_wtime();
    parBubbleSort(arr2, n);
    end = omp_get_wtime();
    cout<<"Bubble sort (Parallel): "<<end-start<<"Sec"<<endl;
    
    
    cout<<"merge sort res : "<<endl;

    start = omp_get_wtime();
    mergeSort(arr3, 0, n-1);
    end = omp_get_wtime();
    cout<<"Merge sort (Sequential): "<<end-start<<"Sec"<<endl;
    
    start = omp_get_wtime();
    parMergeSort(arr4, 0, n-1, 0);
    end = omp_get_wtime();
    cout<<"Merge sort (Parallel): "<<end-start<<"Sec"<<endl;
    
    cout<<"Arr1: "<<endl;
    printArray(arr1, n);
    
    cout<<"Arr2: "<<endl;
    printArray(arr2, n);
    
    cout<<"Arr3: "<<endl;
    printArray(arr3, n);
    
    cout<<"Arr4: "<<endl;
    printArray(arr4, n);
    



}





