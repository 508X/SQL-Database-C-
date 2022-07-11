#ifndef DATABASE_RECORD_H
#define DATABASE_RECORD_H
#include <iostream>
#include <iomanip>
using namespace std;

template <typename T>
struct record
{
    int _key;
    T _value;
    record(int k=0, T v = T());
    friend bool operator ==(const record<T>& left, const record<T>& right){
        return left._key == right._key;
    }
    friend ostream& operator <<(ostream& outs, const record<T>& print_me){
        outs<<setw(4)<<setfill('0')<<print_me._key<<":"<<setw(4)<<print_me._value<<setfill('0');
        return outs;
    }
    friend istream& operator >>(istream&  ins, record<T>& insert_in_me){
        char rand;
        ins>>insert_in_me._key>>rand>>insert_in_me._value;
        return ins;
    }
};

template <typename T>
record<T>::record(int k, T v):_key(k), _value(v){}

void search( const int a[ ], size_t first, size_t size, int target, bool& found, size_t& location);

/*
 Precondition: The array segment starting at a[first] and containing size
 elements is sorted from smallest to largest.
 Postcondition: The array segment starting at a[first] and containing size
 elements has been searched for the target. If the target was present,
 then found is true, and location is set so that target == a[location].
 Otherwise found is set to false.
*/
void search( const int a[ ], size_t first, size_t size, int target, bool& found, size_t& location){
    size_t middle;
    if (size == 0)
        found = false;
    else{
        middle = first + size/2;
        if (target == a[middle]){
            location = middle; found = true;
        }
        else if (target < a[middle]){
            // The target is less than a[middle],
            //so search before the middle.
            search(a, first, size/2, target, found, location);
        }
        else{
            // The target must be greater than a[middle],
            //so search after the middle.
            search(a, middle+1, (size-1)/2, target, found, location);
        }
    }

}
#endif //DATABASE_RECORD_H
