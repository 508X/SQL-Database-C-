#ifndef DATABASE_MMAP_H
#define DATABASE_MMAP_H

#include <vector>
#include "../bplus/bplustree.h"
#include <iostream>

using namespace std;

template <typename K, typename V>
struct MPair{
    K key;
    vector<V> value_list;

    MPair(const K& k=K()):key(k), value_list(){}
    MPair(const K& k, const V& v) :key(k), value_list(){
        value_list.push_back(v);
    }
    MPair(const K& k, const vector<V>& vlist) :key(k), value_list(vlist){}
    MPair& operator =(const MPair<K, V>& rhs){
        if(this==&rhs){return *this;}
        value_list = rhs.value_list;
        key = rhs.key;
        return *this;
    }
    friend ostream& operator <<(ostream& outs, const MPair<K, V>& print_me){
        outs<<print_me.key;
        outs<<print_me.value_list;
        return outs;
    }
    friend bool operator ==(const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return lhs.key==rhs.key;
    }
    friend bool operator < (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return lhs.key < rhs.key;
    }
    friend bool operator <= (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return lhs.key <= rhs.key;
    }
    friend bool operator > (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return lhs.key > rhs.key;
    }
    friend MPair<K, V> operator + (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        if(lhs==rhs){
            MPair<K, V> Ret;
            Ret.value_list += rhs.value_list;
            Ret.value_list += lhs.value_list;
            Ret.key = lhs.key;
            return Ret;
        }
        assert("Different Keys"=="");
        return MPair<K,V>(K(), V());
    }
    friend MPair<K, V> operator +=(const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        MPair<K, V> retme = lhs+rhs;
        return retme;
    }


};


template <typename K, typename V>
class MMap
{
public:
    typedef bplustree<MPair<K, V> > map_base;
    typedef MPair<K, V> MP;
    class Iterator{
    public:
        friend class MMap;
        Iterator(typename map_base::Iterator it)
                :_it(it)
        {

        }
        Iterator operator ++(int unused){
            return (++_it);
        }
        Iterator operator ++(){
            return (_it++);
        }
        MPair<K, V> operator *(){
            return (*_it);
        }
        bool is_null(){
            return _it.is_null();
        }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs){
            return lhs._it==rhs._it;
        }
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs){
            return lhs._it!=rhs._it;
        }

    private:
        typename map_base::Iterator _it;
    };

    MMap():mmap(true){}

    Iterator begin()                    {
        return mmap.begin();
    }
    Iterator end()                      {
        return mmap.end();
    }
    Iterator find(const K& key)         {
        return mmap.find(MP(key));
    }
    Iterator lower_bound(const K& key)  {
        return mmap.lower_bound(MP(key));
    }
    Iterator upper_bound(const K& key)  {
        return mmap.upper_bound(MP(key));
    }



//  Capacity
    int size() const{return mmap.size();}
    bool empty() const{return mmap.empty();}

//  Element Access
    /*
     * returns a reference to the vector
     * associated witht the key
     *
     * in the const version of the operator
     * the underlying const get function will assert
     * that the key is in the tree
     *
     * in the nonconst version
     * the underlying get function will add a new
     * node with the key and an empty vector
     *
     * NOTE: accessing the non-existent element of an empty
     * array crashes the program, so if you need to print or
     * access values of a key, make sure the key exists first
     *
    */
    const vector<V>& operator[](const K& key) const{

        return mmap.get(key).value_list;
    }
    vector<V>& operator[](const K& key){
        return mmap.get(key).value_list;
    }

//  Modifiers
    void insert(const K& k, const V& v){
        //if->insert contains key already,
        //call braket of key, then pushback
        //new value into vector
        mmap.insert(MPair<K,V>(k,v));
    }
    void erase(const K& key){mmap.remove(MPair<K,V>(key));}
    void clear(){mmap.clear_tree();}

//  Operations:
    bool contains(const K& key){
        return mmap.contains(key);
    }

    vector<V> &get(const K& key){
        return mmap.get(key).value_list;
    }

    bool is_valid(){return mmap.is_valid();}
    friend ostream& operator<<(ostream& outs, const MMap<K, V>& print_me){
        outs<<print_me.mmap<<endl;
        return outs;
    }


private:
    bplustree<MPair<K, V> > mmap;
};

#endif //DATABASE_MMAP_H
