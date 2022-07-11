#ifndef DATABASE_MAP_H
#define DATABASE_MAP_H
#include "../btree/btree.h"
#include "../bplus/bplustree.h"
#include <iostream>

using namespace std;

template <typename K, typename V>
struct Pair{
    K key;
    V value;
    Pair(const K& k=K(), const V& v=V()):key(k),value(v){}
    Pair(const Pair<K,V>& other){
        key = other.key;
        value = other.value;
    }
    Pair<K,V>&      operator = (const Pair<K,V>& rhs){
        key = rhs.key;
        value = rhs.value;
        return *this;
    }
    friend ostream& operator <<(       ostream& outs, const Pair<K,V>&  me){
        outs<<me.key<<":"<<me.value;
        return outs;
    }
    friend bool     operator ==(const Pair<K,V>& lhs, const Pair<K,V>& rhs){
        return lhs.key==rhs.key;
    }
    friend bool     operator < (const Pair<K,V>& lhs, const Pair<K,V>& rhs){
        return lhs.key<rhs.key;
    }
    friend bool     operator > (const Pair<K,V>& lhs, const Pair<K,V>& rhs){
        return lhs.key>rhs.key;
    }
    friend bool     operator <=(const Pair<K,V>& lhs, const Pair<K,V>& rhs){
        return lhs.key<=rhs.key;
    }
    friend bool     operator >=(const Pair<K,V>& lhs, const Pair<K,V>& rhs){
        return lhs.key>=rhs.key;
    }
    friend Pair<K,V>operator + (const Pair<K,V>& lhs, const Pair<K,V>& rhs){
        if(lhs.key==rhs.key){
            return Pair(lhs.key, lhs.value+rhs.value);
        }
        return lhs;
    }
    friend Pair<K,V>operator +=(const Pair<K,V>& lhs, const Pair<K,V>& rhs){
        return lhs+rhs;
    }
    friend istream& operator >>(        istream& ins,       Pair<K,V>& rhs){
        char temp;
        K key;
        V val;
        ins>>key;
        ins>>temp;
        ins>>val;
        rhs.key=key;
        rhs.value=val;
        return ins;
    }
};

template <typename K, typename V>
class Map
{
public:
    typedef bplustree<Pair<K, V> > map_base;
    typedef Pair<K,V> P;

    class Iterator{
    public:
        friend class Map;
        Iterator(typename map_base::Iterator it):_it(it){}
        Iterator operator ++(int unused){
            return ++_it;
        }
        Iterator operator ++(){
            return _it++;
        }
        P operator *(){
            return (*_it).value;
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

    Map():map(), key_count(){}

    Iterator begin(){
        return map.begin();
    }
    Iterator end(){
        return map.end();
    }
    Iterator find(const K& key){
        //return iterator to this key
        //NULL if not there
        return map.find(P(key));
    }
    Iterator lower_bound(const K& key){
        //returns iterator to the key if found, else
        //it returns iterator to the next key


        //return first that goes  NOT BEFORE
        //key entry or next if does not
        //exist >= entry
        cout<<"Lower Bound: "<<*map.lower_bound(P(key));
        return map.lower_bound(P(key));


    }
    Iterator upper_bound(const K& key){
        // always return iterator to next key

        // return first that goes AFTER KEY
        // exists or not, the next entry > entry
        //
        cout<<"Upper Bound: "<<*map.upper_bound(P(key));
        return map.upper_bound(P(key));


    }


    int size() const{return key_count;}
    bool empty() const{return key_count==0;}

    V& operator[](const K& key){
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
        return map.get(key).value;
    }
    V& at(const K& key){

    }
    const V& at(const K& key) const{

    }

    void insert(const K& k, const V& v){
        map.insert(Pair<K, V>(k,v));
        key_count++;
    }
    void erase(const K& key){map.remove(P(key));}
    void clear(){map.clear_tree();}
    V get(const K& key){
        return map.get(key).value;
    }

    bool contains(const Pair<K, V>& target){
        return map.contains(target);
    }
    bool is_valid(){return map.is_valid();}
    friend ostream& operator<<(ostream& outs, const Map<K, V>& print_me){
        outs<<print_me.map<<endl;
        return outs;
    }

private:
    int key_count;
    bplustree<Pair<K, V> > map;
};


#endif //DATABASE_MAP_H
