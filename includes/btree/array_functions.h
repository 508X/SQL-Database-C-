#ifndef DATABASE_ARRAY_FUNCTIONS_H
#define DATABASE_ARRAY_FUNCTIONS_H
#include <cassert>
#include <iostream>
#include <vector>
using namespace std;


template<typename T>
void clear_array(T a[], int& size);
template<typename T>
void shift_right(T a[], int& size, int mark);
template<typename T>
void  shift_left(T a[], int& num_used, int mark);
template<typename T>
void remove_item(T a[], int& num_used, int mark);
template<typename T>
void append_array(T dest[], int& dest_num_used, T src[], int src_num_used);

template <typename T>
bool is_asc(T data[], int n);
template <typename T>
T maximal(const T& a, const T& b);  //return the larger of the two items
template <typename T>
void swap(T& a, T& b);  //swap the two items
template <typename T>
int index_of_maximal(T data[ ], int n);     //return index of the largest item in data
template <typename T>
void ordered_insert(T data[ ], int& n, T entry);    //insert entry into the sorted array data with length n
template <typename T>
int first_ge(const T data[], int n, const T& entry);   //return the first element in data that is not less than entry
template <typename T>
void attach_item(T data[ ], int& n, const T& entry);    //append entry to the right of data
template <typename T>
void insert_item(T data[ ], int i, int& n, const T& entry);    //insert entry at index i in data
template <typename T>
void detach_item(T data[ ], int& n, T& entry);  //remove the last element in data and place it in entry
template <typename T>
void delete_item(T data[ ], int i, int& n, T& entry);   //delete item at index i and place it in entry
template <typename T>
void merge(T data1[ ], int& n1, T data2[], int& n2);   //append data2 to the right of data1
template <typename T>
void split(T data1[ ], int& n1, T data2[], int& n2);   //move n/2 elements from the right of data1 and move to data2
template <typename T>
void copy_array(T dest[], const T src[], int& dest_size, int src_size); //copy src[] into dest[]
template <typename T>
void print_array(const T data[], int n, int pos = 0);  //print array data
template <typename T>
bool is_gt(const T data[], int n, const T& item);       //item > all data[i]
template <typename T>
bool is_le(const T data[], int n, const T& item);       //item <= all data[i]
template <typename T>
void swap_arr(T& a, T& b);

template <typename T>
ostream& operator <<(ostream& outs, const vector<T>& list); //print vector list
template <typename T>
vector<T>& operator +=(vector<T>& list, const T& addme); //list.push_back addme
template <typename T>
vector<T>& operator +=(vector<T>& lhs, const vector<T>& rhs);
template <typename T>
vector<T> unionize(const vector<T>& U, const vector<T>& V);
template <typename T>
vector<T> intersect(const vector<T>& U, const vector<T>& V);


template <typename T>
ostream& operator <<(ostream& outs, const vector<T>& list){
    outs<<"{";
    for(int i=0; i<list.size(); i++){

        outs<<list[i];
        if(i+1<list.size()){
            outs<<", ";
        }
    }
    outs<<"}";
    return outs;
}

template <typename T>
vector<T>& operator +=(vector<T>& list, const T& addme){
    list.push_back(addme);
    return list;
}

template <typename T>
vector<T>& operator +=(vector<T>& lhs, const vector<T>& rhs){
    for(int i=0; i<rhs.size(); i++){
        lhs+=rhs[i];
    }
    return lhs;
}

template <typename T>
vector<T> unionize(const vector<T>& U, const vector<T>& V){
    vector<T> res;
    int i=0; int j=0;
    int sumsize = U.size()+V.size();
    while(i+j<sumsize){
        if(j<V.size() && i<U.size() && U[i]<V[j]){
            res+=U[i];
            i++;
        }
        else if(j<V.size() && i<U.size() && V[j]<U[i]){
            res+=V[j];
            j++;
        }
        else if(j<V.size() && i<U.size() && V[j]==U[i]){
            res+=U[i];
            i++;
            j++;
        }
        else if(j==V.size() && i<U.size()){
            while(i<U.size()){
                res+=U[i];
                i++;
            }
        }
        else if(j<V.size() && i==U.size()){
            while(j<V.size()){
                res+=V[j];
                j++;
            }
        }
    }
    return res;
}

template <typename T>
vector<T> intersect(const vector<T>& U, const vector<T>& V){
    vector<T> res;
    int i=0; int j=0;
    while(i<U.size() && j<V.size()){

        if(U[i]==V[j]){
            res+=U[i];
            i++;
            j++;
        }
        else if(U[i]<V[j]){
            i++;
        }
        else if(U[i]>V[j]){
            j++;
        }

    }
    return res;
}

template<typename T>
void clear_array(T a[], int& size){
    for(int i=0; i<size; i++){
        a[i] = T();
    }
    size=0;
}

template<typename T>
void shift_right(T a[], int& size, int mark){
    size++;
    for(int i = size-1; i>mark; i--){
        a[i]=a[i-1];
    }
}

template<typename T>
void  shift_left(T a[], int& num_used, int mark){
    for(int i = mark; i<num_used; i++){
        a[i] = a[i+1];
    }
    a[num_used-1]=T();
    num_used--;
}

template<typename T>
void remove_item(T a[], int& num_used, int mark){
    shift_left(a, num_used, mark);
}

template<typename T>
void append_array(T dest[], int& dest_num_used, T src[], int src_num_used){
    int temp = dest_num_used;
    dest_num_used+=src_num_used;
    for(int i = 0; i<src_num_used; i++){
        dest[temp+i] = src[i];
    }
}

template<typename T>
void append_array_half(T dest[], int& dest_num_used, T src[], int src_num_used){
    dest_num_used = 0;
    dest_num_used+=src_num_used/2;
    for(int i = 0; i<src_num_used/2; i++){
        dest[i] = src[i+src_num_used/2];
    }
}

template <typename T>
T maximal(const T& a, const T& b){
    if(a>b){
        return a;
    }
    else{
        return b;
    }
}

template <typename T>
void swap(T& a, T& b){
    T temp = a;
    a = b;
    b = temp;
}

template <typename T>
void swap_arr(T& a, T& b){
    T temp = a;
    a = b;
    b = temp;
}

template <typename T>
int index_of_maximal(T data[], int n){
    T max = 0;
    for(int i=0; i<n; i++){
        if(data[i]>max){
            max = i;
        }
    }
    return max;
}

template <typename T>
void ordered_insert(T data[], int& n, T entry){
    int place = first_ge(data, n, entry);
    shift_right(data, n, place);
    data[place] = entry;
}

template <typename T>
int first_ge(const T data[], int n, const T& entry){
    /* array is a sorted list
    return the index of the first item
    that is not less than item
    if item is larger than the last element,
    return size */

    for(int i=0; i<n; i++){
        if(data[i]>entry || data[i]==entry){
            return i;
        }
    }
    return n;
}

template <typename T>
void attach_item(T data[], int& n, const T& entry){
    n++;
    data[n-1]=entry;
}

template <typename T>
void insert_item(T data[], int i, int& n, const T& entry){
    shift_right(data, n, i);
    data[i] = entry;

}

template <typename T>
void detach_item(T data[], int& n, T& entry){
    if(n==0){
        entry = T();
        return;
    }
    entry = data[n-1];
    shift_left(data, n, n);
}

template <typename T>
void delete_item(T data[], int i, int& n, T& entry){

    assert(i<n);
    assert(i>=0);
    assert(n>=0);

    entry = data[i];
    remove_item(data, n, i);
}

template <typename T>
void merge(T data1[], int& n1, T data2[], int& n2){
    append_array(data1, n1, data2, n2);
}

template <typename T>
void split(T data1[], int& n1, T data2[], int& n2){
    n2=0;

    if(n1%2==0){/* if even */
        for(int i=n1/2, j=0; i<n1; i++, j++){
            n2++;
            data2[j] = data1[i];
        }
        n1/=2;
    }
    else{
        for(int i=n1/2 + 1, j=0; i<n1; i++, j++){
            n2++;
            data2[j] = data1[i];
        }
        n1 = n1/2+1;
    }

}

template <typename T>
void copy_array(T dest[], const T src[], int& dest_size, int src_size){
    dest_size = src_size;
    for(int i=0; i<dest_size; i++){
        dest[i] = src[i];
    }
}

template <typename T>
void print_array(const T data[], int n, int pos){
    assert(pos<=n);
    cout<<"{";
    for(int i=pos; i<n; i++){
        if(i!=pos)
            cout<<",";
        cout<<data[i];
    }
    cout<<"}";
}

template <typename T>
bool is_gt(const T data[], int n, const T& item){
    for(int i=0; i<n; i++){
        if(item<=data[i]){
            return false;
        }
    }
    return true;
}

template <typename T>
bool is_le(const T data[], int n, const T& item){
    for(int i=0; i<n; i++){
        if(item>data[i]){
            return false;
        }
    }
    return true;
}

template <typename T>
bool is_asc(T data[], int n){
    if(n==1){
        return true;}
    for(int i=0; i<n-1; i++){
        if(data[i]>data[i+1]){
            return false;
        }
    }
    return true;


}

#endif //DATABASE_ARRAY_FUNCTIONS_H
