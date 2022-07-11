
#ifndef DATABASE_BPLUSTREE_H
#define DATABASE_BPLUSTREE_H

#include <iostream>
#include <cstdlib>
#include <vector>
#include <cassert>
#include <cstring>
#include <iomanip>
#include "../btree/array_functions.h"

using namespace std;

template<typename T>
class bplustree{
public:
    class Iterator{
    public:
        friend class BPlusTree;
        Iterator(bplustree<T>* _it=NULL, int _key_ptr = 0):it(_it), key_ptr(_key_ptr){}
        Iterator& operator =(const Iterator& other){
            it = other.it;
            key_ptr = other.key_ptr;
        }
        T operator *(){
            assert(key_ptr<it->data_count);
            return it->data[key_ptr];
        }
        Iterator operator++(int un_used){
            bplustree<T>* temp = it;
            int holdme = key_ptr;
            key_ptr++;
            if(key_ptr>=it->data_count){
                key_ptr=0;
                it = it->next;
            }
            return Iterator(temp, holdme);
        }
        Iterator operator++(){
//                cout<<"Doing ++"<<endl;

            key_ptr++;

            if(key_ptr>=it->data_count){
//                    cout<<"Resetting keyptr and moving to next node"<<endl;
                key_ptr=0;
                it = it->next;
            }

//                cout<<"K: "<<key_ptr<<endl;
//                cout<<*it<<endl;
//                cout<<"Returning ++"<<endl;
            return Iterator(it, key_ptr);
        }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs){
            if(lhs.it==rhs.it && lhs.key_ptr==rhs.key_ptr){
                return true;
            }
            return false;
        }
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs){
            if(lhs.it==rhs.it && lhs.key_ptr==rhs.key_ptr){
                return false;
            }
            return true;
        }
        void print_Iterator(){
            cout<<endl<<"______________"<<endl;
            cout<<"|KEY: "<<key_ptr<<endl;;
            cout<<"|VAL: "<<it->data[key_ptr]<<endl;
            cout<<"______________"<<endl;

        }
        bool is_null(){
            return it==NULL;
        }

    private:
        bplustree<T>* it;
        int key_ptr;
    };

    typedef Iterator Iter;

    Iterator begin(){
        return get_smallest_node();
    }
    Iterator end(){
        return nullptr;
    }
    Iterator lower_bound(const T& key){
        int i = first_ge(data, data_count, key);
        bool found = i<data_count && data[i]==key;

        if(is_leaf()){
            if((found) or (!found && i<data_count)){
                return Iterator(this, i);
            }
            return Iterator(this->next, 0);
        }
        else{
            if(found){
                return subset[i+1]->lower_bound(key);
            }
            return subset[i]->lower_bound(key);
        }
    }
    Iterator upper_bound(const T& key){

        int i = first_ge(data, data_count, key);
        bool found = i<data_count && data[i]==key;

        if(is_leaf()){
            if(i<data_count && !found){
                //while were in data where key should be
                //and it isnt there
                return Iter(this, i);
            }
            else if(found && i+1<data_count){
                //if we found it and still have a next in data
                return Iter(this, i+1);
            }
            else{
                return Iter(this->next, 0);
            }
        }
        else{
            if(found){
                //look at bottom right
                return subset[i+1]->upper_bound(key);
            }
            else{
                //just look at indexed data location
                return subset[i]->upper_bound(key);
            }
        }

    }
    Iterator find(const T& key){
        //return iterator to this key, null if not there
        int i = first_ge(data, data_count, key);
        bool found = i<data_count && data[i]==key;

        if(is_leaf()){
            if(found){
                return Iterator(this, i);
            }
            else{
                return NULL;
            }
        }
        else{
            if(found){
                return subset[i+1]->find(key);

            }
            else{
                return subset[i]->find(key);
            }
        }
    }

    bplustree(bool dups = false);
    //big three:
    bplustree(const bplustree<T>& other);
    ~bplustree();
    bplustree<T>& operator =(const bplustree<T>& RHS);
    void insert(const T& entry);                //insert entry into the tree
    void remove(const T& entry);                //remove entry from the tree
    void clear_tree();                          //clear this object (delete all nodes etc.)

    //copy other into this object

    bool contains(const T& entry);              //true if entry can be found in the array
    T& get(const T& entry);                     //return a reference to entry in the tree
    T& get(const T& entry) const;
    T& get_existing(const T& entry);
    T* _find(const T& entry);                    //return a pointer to this key. NULL if not there.


    int  size() const;                           //count the number of elements in the tree
    bool empty() const;                         //true if the tree is empty
    bool is_valid();
    void print_tree(int level = 0, ostream &outs=cout) const; //print a readable version of the tree
    friend ostream& operator<<(ostream& outs, const bplustree<T>& print_me){
        print_me.print_tree(0, outs);
        return outs;
    }

private:

    static const int MINIMUM = 1;
    static const int MAXIMUM = 2*MINIMUM;

    bool dups_ok;                       //true if duplicate keys may be inserted
    int data_count;                     //number of data elements
    T data[MAXIMUM+1];                  //holds the keys
    int child_count;                    //number of children
    bplustree* subset[MAXIMUM+2];       //subtrees

    bool is_leaf() const{return child_count==0;}
    void copy_tree(const bplustree<T>& other, bplustree<T>*& passthrough);
    bool loose_insert(const T& entry);  //allows MAXIMUM+1 data elements in the root
    void fix_excess(int i);             //fix excess of data elements in child i
    bool loose_remove(const T& entry);  //allows MINIMUM-1 data elements in the root
    void get_smallest(T& entry);        //entry := leftmost leaf
    void get_biggest(T& entry);         //entry := rightmost leaf
    void remove_biggest(T& entry);      //remove the biggest child of this tree->entry
    void rotate_left(int i);            //transfer one element LEFT from child i
    void rotate_right(int i);           //transfer one element RIGHT from child i
    void transfer_left(int i);
    void transfer_right(int i);
    T&   get_smallest();
    bplustree<T>* fix_shortage(int i);  //fix shortage of data elements in child i
    bplustree<T>* get_smallest_node();
    bplustree<T>* get_largest_node();
    bplustree<T>* merge_with_next_subset(int i); //merge subset i with subset i+1
    bplustree<T>* next;
    int _size;

};


template<typename T>
bplustree<T>::bplustree(bool dups){
    next = nullptr;
    data_count = 0;
    child_count = 0;
    _size = 0;
    dups_ok = dups;
    for(int i=0; i<MAXIMUM+1; i++){
        data[i] = T();
    }
    for(int i=0; i<MAXIMUM+2; i++){
//        subset[i]->child_count = 0;

        subset[i] = nullptr;
    }

}
template<typename T>
bplustree<T>::bplustree(const bplustree<T>& other){




    dups_ok = other.dups_ok;
    bplustree<T>* temp = nullptr;
    copy_tree(other, temp);


}
template<typename T>
bplustree<T>::~bplustree(){
    clear_tree();
}
template<typename T>
bplustree<T>& bplustree<T>::operator =(const bplustree<T>& RHS){
    if(this==&RHS){return *this;}
    dups_ok = RHS.dups_ok;
    clear_tree();
    bplustree<T>* pass = nullptr;
    copy_tree(RHS, pass);
    return *this;
}

template<typename T>
void    bplustree<T>::insert(const T& entry){
//    cout<<"Inserting"<<endl;
    bool inserted = loose_insert(entry);
    if(inserted){
        _size++;
    }
//    cout<<"Insert:: Inserted "<<entry<<" ::"<<endl<<endl;
    if(data_count>MAXIMUM){

        bplustree<T>* shallow_copy = new bplustree<T>;

        copy_array(shallow_copy->data, data, shallow_copy->data_count, data_count);

        copy_array(shallow_copy->subset, subset, shallow_copy->child_count, child_count);

        clear_array(subset, child_count);

        clear_array(data, data_count);

        insert_item(subset, 0, child_count, shallow_copy);

        fix_excess(0);

    }


}
template<typename T>
void    bplustree<T>::remove(const T& entry){

    //Loose_remove the entry from this tree.
    if(loose_remove(entry))
        _size--;
    cout<<"SIZE AF: "<<_size<<endl;

    //point a temporary pointer (shrink_ptr) and point it to this root's only subset
    if(_size>0 && data_count<MINIMUM){
        bplustree<T>* shrink_ptr = subset[0];
        //copy all the data and subsets of this subset into the root (through shrink_ptr)
        copy_array(data, shrink_ptr->data, data_count, shrink_ptr->data_count);
        copy_array(subset, shrink_ptr->subset, child_count, shrink_ptr->child_count);
        //now, the root contains all the data and poiners of it's old child.
        //now, simply delete shrink_ptr (blank out child), and the tree has shrunk by one level.
        shrink_ptr->data_count = 0;
        shrink_ptr->child_count = 0;

        delete shrink_ptr;

    }
    //Note, the root node of the tree will always be the same, it's the child node we delete
}
template<typename T>
void    bplustree<T>::clear_tree(){

    /*
      in main:

      BTree bt;
      bt.clear_tree(); [][][]
                      [][][][]

      items are static so there isnt anything to do
      _____________________________________________

      Now lets say you have a btree object that has

                         [][][]
                        [][][][]
                     /     |      \
                    /      |       \
                   /       |        \
              [][][]     [][][]     [][][]
             [][][][]   [][][][]   [][][][]
            V V V V V  V V V V V   V V V V V


            1. data_count = 0;

            2. delete all V's first (children of subsets, recursively)
               subset[i]->clear_tree();

            3. set all subset[i] to null of root

            4. child_count = 0;


    */
    data_count = 0;
    if(!is_leaf()){
        for(int i=0; i<child_count; i++){
            subset[i]->clear_tree();
            delete subset[i];
            subset[i] = nullptr;
        }
    }
    data_count = 0;
    child_count = 0;
    _size = 0;

}
template<typename T>
void    bplustree<T>::copy_tree(const bplustree<T>& other, bplustree<T>*& passthrough){


    if(other.is_leaf()){

        if(passthrough!=nullptr)
            passthrough->next = this;

        passthrough = this;
    }
    else{
        next = nullptr;
    }

    copy_array(data, other.data, data_count, other.data_count);
    _size = other.size();
    child_count=0;

    for(int i=0; i<other.child_count; i++){
        attach_item(subset, child_count, new bplustree(other.dups_ok));
        subset[i]->copy_tree(*other.subset[i], passthrough);
    }

}
template<typename T>
bool    bplustree<T>::contains(const T& entry){
    if(_find(entry)!=NULL){return true;}
    else{return false;}
}
template<typename T>
T&      bplustree<T>::get(const T& entry){

    //If entry is not in the tree, CRASH
    //assert(contains(entry));
    if(!contains(entry))
        insert(entry);

    return get_existing(entry);
}
template<typename T>
T&      bplustree<T>::get(const T& entry) const{

    //If entry is not in the tree, CRASH
    //assert(contains(entry));
    if(!contains(entry))
        insert(entry);

    return get_existing(entry);
}
template <typename T>
T&      bplustree<T>::get_existing(const T& entry){
    T* temp = _find(entry);
    assert(temp!=NULL);
    return *temp;
}
template<typename T>
bplustree<T>* bplustree<T>::get_smallest_node(){

    if(!is_leaf()){
        return subset[0]->get_smallest_node();
    }
    else{
        return this;
    }

}
template<typename T>
bplustree<T>* bplustree<T>::get_largest_node(){
    if(!is_leaf()){
        return subset[child_count-1]->get_largest_node();
    }
    else{
        return this;
    }
}
template<typename T>
void    bplustree<T>::get_smallest(T& entry){

    bplustree<T>* temp = get_smallest_node();
    cout<<"SMALLEST NODE"<<endl;
    cout<<temp->data[0]<<endl;
    entry = (temp->data[0]);

}
template<typename T>
T&    bplustree<T>::get_smallest(){

    bplustree<T>* temp = get_smallest_node();
    return (temp->data[0]);

}
template<typename T>
void    bplustree<T>::get_biggest(T& entry){
    assert(data_count>0);
    if(!is_leaf()){
        subset[child_count-1]->get_biggest(entry);
    }
    else{
        entry = &data[data_count-1];
    }
}
template<typename T>
T*      bplustree<T>::_find(const T& entry){

    int i = first_ge(data, data_count, entry);
    bool found = data[i]==entry;
    if(found){
        if(is_leaf()){
            return &data[i];
        }
        else{
            return subset[i+1]->_find(entry);
        }

    }
    else {
        if(is_leaf()){
            return NULL;
        }
        else{
            return subset[i]->_find(entry);
        }
    }
}
template<typename T>
int     bplustree<T>::size() const{
    return _size;
}
template<typename T>
bool    bplustree<T>::empty() const{
    if(_size==0){
        return true;
    }
    else{
        return false;
    }
}
template<typename T>
void    bplustree<T>::print_tree(int level, ostream &outs) const{

    /*
    set INDENT
    for loop from end until i>=0
        if its a leaf then print right child i+1
        print setw level and then data endl
    if it isnt a leaf subset 0 print tree
    */
    int INDENT = 4;
    for(int i=data_count-1; i>=0; i--){
        if(!is_leaf()){
            subset[i+1]->print_tree(level+INDENT);
        }
        outs
                <<"["<<child_count<<"]"
                <<setw(level+INDENT)
                <<""<<"["<<data[i]<<"]";
        if(next!=NULL and i == data_count-1){
            outs<<" -> "<<next->data[0];
        }
        outs<<endl;
    }
    if(!is_leaf()){
        subset[0]->print_tree(level+INDENT);
    }
}
template<typename T>
bool    bplustree<T>::loose_insert(const T& entry){

    int i = first_ge(data, data_count, entry);
    bool found = (i<data_count && data[i] == entry);
    bool inserted = false;
    if(found){
        if(is_leaf()){
            if(dups_ok){
                cout<<"Adding dupliate"<<endl;
                data[i] = data[i]+entry;
                return true;
            }
            return false;
        }
        else{
            subset[i+1]->loose_insert(entry);
            fix_excess(i);
        }
    }
    else{
        if(is_leaf()){
            insert_item(data, i, data_count, entry);
            return true;
        }
        else{
            inserted = subset[i]->loose_insert(entry);
            fix_excess(i);
        }
    }
    return inserted;

}
template<typename T>
void    bplustree<T>::fix_excess(int i){
    //this node's child i has one too many items: 3 steps:
    //1. add a new subset at location i+1 of this node
    //2. split subset[i] (both the subset array and the data array) and move half into
    // subset[i+1] (this is the subset we created in step 1.)
    //3. detach the last data item of subset[i] and bring it and insert it into this node's data[]
    // //Note that this last step may cause this node to have too many items. This is OK. This will be
    // dealt with at the higher recursive level. (my parent will fix it!)
    if(subset[i]->data_count>MAXIMUM){
        //split and move half to i+1
        bplustree<T>* temp = new bplustree<T>(dups_ok);
        insert_item(subset, i+1, child_count, temp);
        split(subset[i]->data, subset[i]->data_count, temp->data, temp->data_count);
        split(subset[i]->subset, subset[i]->child_count, temp->subset, temp->child_count);
        T item_tmp;
        detach_item(subset[i]->data, subset[i]->data_count, item_tmp);
        insert_item(data, i, data_count, item_tmp);
        if(subset[i]->is_leaf()){

            insert_item(subset[i+1]->data, 0, subset[i+1]->data_count, item_tmp);

            bplustree<T>* nextptr = subset[i]->next;
            subset[i]->next = subset[i+1];
            subset[i+1]->next = nextptr;
        }

    }

}
template<typename T>
bool    bplustree<T>::loose_remove(const T& entry){

    /* four cases:
           leaves:
                a. not found: there is nothing to do
                b. found    : just remove the target
           non leaf:
                c. not found: subset[i]->loose_remove, fix_shortage(i)
                d. found    : subset[i+1]->loose_remove, fix_shortage(i+1) [...]
                    (No More remove_biggest)

             |   !found               |   found                 |
       ------|------------------------|-------------------------|-------
       leaf  |  a: nothing            | b: delete               |
             |     to do              |    target               |
       ------|------------------------|-------------------------|-------
       !leaf | c:                     | d: B_PLUS_TREE          |
             |  [i]->  loose_remove   |   [i+1]-> loose_remove  |
             |  fix_shortage(i)       | fix_shortage(i+1) [...] |
       ------|------------------------|-------------------------|-------


     */

    int i = first_ge(data, data_count, entry);
    bool found = (data[i]==entry);
    bool removed = false;
    T found_entry;
    if(found){
        if(is_leaf()){
            T temp_item;
            delete_item(data, i, data_count, temp_item);
            removed = true;
            //if shortage
            /*
             * data[i] is the same as the item that we have deleted.
             * so, now, it needs to be replaced by the current smallest key
             *      in subset[i+1]
             * Problem: After fix_shortage, data[i] could have moved to a different index(?)
             *      or it could have sunk down to a lower level as a result of merge
             *      we need to find this item and replace it.
             *
             *      Before fix_shortage: we cannot replace data[i] with subset[i+1]->smallest
             *      before fix_excess because this smallest key could be the very entry
             *      we are removing. So, we'd be replacing data[i] with entry. and this will
             *      leave the deleted key in the inner nodes.
             *
             *      After fix_shortage: We have no way of knowing where data[i] key (which is
             *      same as entry) will end up. It could move down one level by the time
             *      fix_shortage returns.
             *
             *      Therefore it seems like we have no choice but to search for it in
             *      data[ ] AND subset[i]->data[ ]
             * Current Solution: Kind of messy:
             *      After fix_shortage(i+1):
             *      Search for entry in data[ ] and if found, replace it with smallest.
             *      otherwise, search for it in subset[i]->data[ ]
             *          and replace it. that's where the key (entry) will end up after
             *          fix order returns (credit: Jiaruy Li)
             *
             */
        }
        else{
            assert(i<child_count-1);
            removed = subset[i+1]->loose_remove(entry);
            if(subset[i+1]->data_count<MINIMUM){
                bplustree<T>* temptree = fix_shortage(i+1);

                int a = first_ge(data, data_count, entry);
                bool foundindata = a<data_count && data[a]==entry;

                if(foundindata){
                    temptree->get_smallest(data[a]);
                }
                else{

                    int b = first_ge(temptree->data, temptree->data_count, entry);
                    bool foundintemp = b<temptree->data_count && temptree->data[b] == entry;

                    if(foundintemp){
                        temptree->subset[b+1]->get_smallest(temptree->data[b]);
                    }
                    else{
                        return removed;
                    }
                }
            }
            else{
                subset[i+1]->get_smallest(data[i]);
                removed = true;
            }
        }
    }
    else{
        if(is_leaf()){
            removed = false;
        }
        else{
            removed = subset[i]->loose_remove(entry);
            if(subset[i]->data_count<MINIMUM){
                fix_shortage(i);
            }
        }
    }

    return removed;

}
template <typename T>
bplustree<T>* bplustree<T>::fix_shortage(int i){
    /*
     * fix shortage in subtree i:
     * if child i+1 has more than MINIMUM,
     *          (borrow from right) transfer / rotate left(i+1)
     * elif child i-1 has more than MINIMUM,
     *          (borrow from left) transfer /rotate right(i-1)
     * elif there is a left sibling,
     *          merge with prev child: merge(i-1)
     * else
     *          merge with next (right) child: merge(i)
     *
     *
     * returns a pointer to the fixed_subset
     */
    print_array(subset[i]->data, subset[i]->data_count);cout<<endl;
    cout<<"Fix Shortage"<<endl;
    //borrow from right subtree if you can
    if(i+1<child_count && subset[i+1]->data_count>MINIMUM){
        cout<<"Transferring Left"<<endl;
        transfer_left(i+1);
    }
    else if(i-1>=0 && subset[i-1]->data_count>MINIMUM){
        cout<<"Transferring Right"<<endl;
        transfer_right(i-1);
    }
    else if(i>0 && subset[i-1]!=nullptr){
        cout<<"Merge with next subset[i-1]"<<endl;
        return merge_with_next_subset(i-1);
    }
    else{
        cout<<"Merge with next subset[i]"<<endl;
        return merge_with_next_subset(i);
    }

    return subset[i];
}

template<typename T>
void    bplustree<T>::remove_biggest(T& entry){
    /*
     Keep looking in the last subtree (recursive)
     until you get to a leaf.
     Then, detach the last (biggest) data item
     after the recursive call, fix shortage.
    */
    if(is_leaf()){

        detach_item(data, data_count, entry);
    }
    else{
        subset[child_count-1]->remove_biggest(entry);
        if(subset[child_count-1]->data_count<MINIMUM){
            fix_shortage(child_count-1);
        }
    }
}

template<typename T>
bplustree<T>* bplustree<T>::merge_with_next_subset(int i){

    /*
    * ----------------------------------------------------------------------
    *  Merge subset[i] with subset [i+1] REMOVE data[i];
    *  non leaf: same as BTree
    *  leaves  : delete but do not bring down data[i]
    * ----------------------------------------------------------------------
    *
    *   1. remove data[i] from this object //done
    *   2. if not a leaf, append it to child[i]->data: //done
    *   3. Move all data items from subset[i+1]->data to right of subset[i]->data //done
    *   4. Move all subset pointers from subset[i+1]->subset to //done
    *          right of subset[i]->subset
    *   5. delete subset[i+1] (store in a temp ptr)
    *   6. if a leaf, point subset[i]->next to temp_ptr->next
    *   6. delete temp ptr
    */

    assert(i<child_count-1);

    T temp_item;
    delete_item(data, i, data_count, temp_item);


    if(!subset[i]->is_leaf()){
        attach_item(subset[i]->data, subset[i]->data_count, temp_item);
    }

    merge(subset[i]->data, subset[i]->data_count, subset[i+1]->data, subset[i+1]->data_count);
    merge(subset[i]->subset, subset[i]->child_count, subset[i+1]->subset, subset[i+1]->child_count);

    bplustree<T>* tempptr;
    delete_item(subset, i+1, child_count, tempptr);

    if(subset[i]->is_leaf()){
        subset[i]->next = tempptr->next;
    }

    tempptr->data_count=0;
    tempptr->child_count=0;

    delete tempptr;

    return subset[i];
}
template <typename T>
void    bplustree<T>::transfer_right(int i){
    /*
     * --------------------------------------------------------------
     * transfer_left: from subset[i] to subset[i-1]
     *      non-leaf: same as BTree
     *      leaves  : rotate and leave and update data[i-1] with subset[i]->data[0]
     *                  leftmost key after you borrowed goes to replace data[i-1]
     * --------------------------------------------------------------
     *
     * non-leaf keys: BTree's rotate_left
     * ---------------
     *
     * (0 < i < child_count) and (subset[i]->data_count > MINIMUM)
     * subset[i-1] has only MINIMUM - 1 entries.
     *
     * item transfers from child[i] to child [i-1]
     *
     * FIRST item of subset[i]->data moves up to data to replace data[i-1],
     * data[i-1] moves down to the RIGHT of subset[i-1]->data
     *
     *  i = 1:
     *              [50  100]
     *  [  ]         [65 75]       ....
     *   [a]      [b]  [c]  [d]
     *
     *  65 move up to replace 50 (data[i])
     *  65's child (its child 0) moves over to be the future child of 50
     *  50 moves down to the right of subset[i]->data
     *
     *               [65 100]
     *   [50]         [ 75 ]       ....
     * [a]  [b]      [c]  [d]
     *
     * ****This does not belong here:
     * last item of subset[i-1]->data moves up to data to replace data[i],
     * data[i] moves down to the RIGHT of subset[i]->data
     *
     * leaf keys:
     * -------------
     *  fix_shortage(0) calls trnasfer_left(1) so, i = 1
     *          [7 | 10]
     *        /    |     \
     * [( )]<=[(7) | 8]  [10]
     *
     * 1. transfer subset[i(1)]->data[0] to end of subset[i-1]
     *        [7 | 10]
     *      /    |     \
     *   [7]    [8]   [10]
     *
     * 2. update parent of the transfered subset: data[i-1 (0)] = subset[i(1)]->data[0]
     *
     *        [(8) | 10]
     *      /      |     \
     *   [7]     [(8)]   [10]
     *
     *
     *
     */
    if(i<child_count-1 && subset[i]->data_count>MINIMUM){
        if(!subset[i]->is_leaf()){
            T temp_movedn_toright = data[i];
            T temp_moveup_todata;

            detach_item(subset[i]->data, subset[i]->data_count, temp_moveup_todata);

            data[i] = temp_moveup_todata;

            attach_item(subset[i+1]->data, subset[i+1]->data_count, temp_movedn_toright);
//            attach_item(subset[i-1]->data, subset[i-1]->data_count, temp_moveup);

            bplustree<T>* tempptr = nullptr;
            detach_item(subset[i]->subset, subset[i]->child_count, tempptr);
            insert_item(subset[i+1]->subset, 0, subset[i+1]->child_count, tempptr);
        }
        else{

            T tempitem;
            detach_item(subset[i]->data, subset[i]->data_count, tempitem);
            ordered_insert(subset[i+1]->data, subset[i+1]->data_count, tempitem);

            data[i] = subset[i+1]->data[0];

        }

    }

}
template <typename T>
void    bplustree<T>::transfer_left(int i){

    /*
     * --------------------------------------------------------------
     * transfer_left: from subset[i] to subset[i-1]
     *      non-leaf: same as BTree
     *      leaves  : rotate and leave and update data[i-1] with subset[i]->data[0]
     *                  leftmost key after you borrowed goes to replace data[i-1]
     * --------------------------------------------------------------
     *
     * non-leaf keys: BTree's rotate_left
     * ---------------
     *
     *
     * (0 < i < child_count) and (subset[i]->data_count > MINIMUM)
     * subset[i-1] has only MINIMUM - 1 entries.
     *
     * item transfers from child[i] to child [i-1]
     *
     * FIRST item of subset[i]->data moves up to data to replace data[i-1],
     * data[i-1] moves down to the RIGHT of subset[i-1]->data
     *
     *  i = 1:
     *              [50  100]
     *  [  ]         [65 75]       ....
     *   [a]      [b]  [c]  [d]
     *
     *  65 move up to replace 50 (data[i])
     *  65's child (its child 0) moves over to be the future child of 50
     *  50 moves down to the right of subset[i]->data
     *
     *               [65 100]
     *   [50]         [ 75 ]       ....
     * [a]  [b]      [c]  [d]
     *
     * ****This does not belong here:
     * last item of subset[i-1]->data moves up to data to replace data[i],
     * data[i] moves down to the RIGHT of subset[i]->data
     *
     * leaf keys:
     * -------------
     *  fix_shortage(0) calls trnasfer_left(1) so, i = 1
     *          [7 | 10]
     *        /    |     \
     * [( )]<=[(7) | 8]  [10]
     *
     * 1. transfer subset[i(1)]->data[0] to end of subset[i-1]
     *        [7 | 10]
     *      /    |     \
     *   [7]    [8]   [10]
     *
     * 2. update parent of the transfered subset: data[i-1 (0)] = subset[i(1)]->data[0]
     *
     *        [(8) | 10]
     *      /      |     \
     *   [7]     [(8)]   [10]
     *
     *        //if subset is a leaf change data[i] with subset[i+1] data[i]

     *
     */

    if(i>0 and i<child_count and subset[i]->data_count>MINIMUM){
        if(!subset[i]->is_leaf()){
            //non-leaf: same as BTree

            T temp_movedn_toleft = data[i-1];
            T temp_moveup_todata;

            delete_item(subset[i]->data, 0, subset[i]->data_count, temp_moveup_todata);

            data[i-1] = temp_moveup_todata;

            ordered_insert(subset[i-1]->data, subset[i-1]->data_count, temp_movedn_toleft);
//            attach_item(subset[i-1]->data, subset[i-1]->data_count, temp_moveup);

            bplustree<T>* tempptr;
            delete_item(subset[i]->subset, 0, subset[i]->child_count, tempptr);
            attach_item(subset[i-1]->subset, subset[i-1]->child_count, tempptr);


        }
        else{
            //leaves  : rotate and leave and update data[i-1] with subset[i]->data[0]
            //          leftmost key after you borrowed goes to replace data[i-1]

            T tempitem;
            delete_item(subset[i]->data, 0, subset[i]->data_count, tempitem);
            ordered_insert(subset[i-1]->data, subset[i-1]->data_count, tempitem);

            data[i-1] = subset[i]->data[0];
        }

    }


}
template<typename T>
bool bplustree<T>::is_valid(){

    /*
        //should check that every data[i] < data[i+1]
        if (is_leaf())
            return true;


        //data[data_count-1] must be less than equal to
        //  every subset[child_count-1]->data[ ]

        //every data[i] is greater than every subset[i]->data[ ]

        //B+Tree: Every data[i] is equal to subset[i+1]->smallest

        //Recursively validate every subset[i]

        return true;
    }
    */


    bool ret = true;
    if(!empty()){
        // must have data between MAX and MIN
        if(data_count>MAXIMUM || data_count<MINIMUM){
            return false;
        }
        // If it is not a leaf, it must follow
        // data_count = n, child_count = n+1
        if(!is_leaf() && child_count!=data_count+1){
            return false;
        }
        //should check that every data[i] < data[i+1]
        if(!is_asc(data, data_count)){
            return false;
        }
        //must check if data[i]<subset[i+1]'s items
        //and check if data[i]>subset[i]'s items
        for(int i=0; i<child_count-1; i++){
            if(!is_gt(subset[i]->data, subset[i]->data_count, data[i]) &&
               data[i]!=subset[i+1]->get_smallest()){
                return false;
            }
        }
        //do recursion
        for(int i=0; i<child_count; i++){
            ret = subset[i]->is_valid();
        }
    }


    return ret;
}

#endif //DATABASE_BPLUSTREE_H
