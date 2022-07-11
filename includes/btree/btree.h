#ifndef DATABASE_BTREE_H
#define DATABASE_BTREE_H
#include <iostream>
#include <cstdlib>
#include <vector>
#include <cassert>
#include <iomanip>
#include "array_functions.h"

using namespace std;

template<typename T>
class BTree{
public:
    BTree(bool dups = false);
    BTree(const BTree<T>& other);
    ~BTree();
    BTree<T>& operator =(const BTree<T>& RHS);

    void insert(const T& entry);                //insert entry into the tree
    void remove(const T& entry);                //remove entry from the tree
    void clear_tree();                          //clear this object (delete all nodes etc.)
    void copy_tree(const BTree<T>& other);      //copy other into this object
    bool contains(const T& entry);              //true if entry can be found in the array

    T& get(const T& entry);                     //return a reference to entry in the tree
    T& get(const T& entry) const;
    T& get_existing(const T& entry);
    T* find(const T& entry);                    //return a pointer to this key. NULL if not there.

    int size() const;                           //count the number of elements in the tree
    bool empty() const;                         //true if the tree is empty
    bool is_valid();
    void print_tree(int level = 0, ostream &outs=cout) const; //print a readable version of the tree
    friend ostream& operator<<(ostream& outs, const BTree<T>& print_me){
        print_me.print_tree(0, outs);
        return outs;
    }

//private:

    static const int MINIMUM = 1;
    static const int MAXIMUM = 2*MINIMUM;

    bool dups_ok;                       //true if duplicate keys may be inserted
    int data_count;                     //number of data elements
    T data[MAXIMUM+1];                  //holds the keys
    int child_count;                    //number of children
    BTree* subset[MAXIMUM+2];           //subtrees

    bool is_leaf() const                //true if this is a leaf node
    {return child_count==0;}

    //insert element functions
    bool loose_insert(const T& entry);  //allows MAXIMUM+1 data elements in the root

    void fix_excess(int i);             //fix excess of data elements in child i

    //remove element functions:
    bool loose_remove(const T& entry);  //allows MINIMUM-1 data elements in the root
    void fix_shortage(int i);           //fix shortage of data elements in child i

    void remove_biggest(T& entry);      //remove the biggest child of this tree->entry
    void rotate_left(int i);            //transfer one element LEFT from child i
    void rotate_right(int i);           //transfer one element RIGHT from child i
    void merge_with_next_subset(int i); //merge subset i with subset i+1

    int _size;

};

template<typename T>
BTree<T>::BTree(bool dups){

    data_count = 0;
    child_count = 0;
    _size = 0;
    dups_ok = dups;
    for(int i=0; i<MAXIMUM+1; i++){
        data[i] = T();
    }
    for(int i=0; i<MAXIMUM+2; i++){
        subset[i] = nullptr;
    }

}
template<typename T>
BTree<T>::BTree(const BTree<T>& other){
    dups_ok = other.dups_ok;
    copy_tree(other);
}
template<typename T>
BTree<T>::~BTree(){
    clear_tree();
}
template<typename T>
BTree<T>& BTree<T>::operator =(const BTree<T>& RHS){
    if(this==&RHS){return *this;}
    dups_ok = RHS.dups_ok;
    copy_tree(RHS);
    return *this;
}


template<typename T>
void    BTree<T>::insert(const T& entry){
    bool inserted = loose_insert(entry);
    if(inserted){
        _size++;
    }
    if(data_count>MAXIMUM){
        BTree<T>* shallow_copy = new BTree<T>;
        copy_array(shallow_copy->data, data, shallow_copy->data_count, data_count);
        copy_array(shallow_copy->subset, subset, shallow_copy->child_count, child_count);
        clear_array(subset, child_count);
        clear_array(data, data_count);
        insert_item(subset, 0, child_count, shallow_copy);
        fix_excess(0);
    }
}
template<typename T>
void    BTree<T>::remove(const T& entry){

    //Loose_remove the entry from this tree.
    bool deleted = loose_remove(entry);
    if(deleted){
        _size--;
    }
    //point a temporary pointer (shrink_ptr) and point it to this root's only subset
    if(_size>0 && data_count<MINIMUM){
        BTree<T>* shrink_ptr = subset[0];
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
void    BTree<T>::clear_tree(){

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
void    BTree<T>::copy_tree(const BTree<T>& other){
    /*

    STEP ZER0: Call Clear on *THIS

      1. copy data of counterpart
      2. for every child, create one child
      3. recursively call copy tree on ym child, giving his child as argument

      subset[i]->copy_tree(other child)

    calling on              argument

    [][][]       <----      [btree]
   [][][][]     copy data    V  V  V

   [child]                  [other guys child]

   create a new child then, invoke copy on this child
   with the other guys child as argument
   LAYER BY LAYER

   some ptr  = new object when you build a new one

   make new btree on subset i
   and call copy on other.subset i

    */

    copy_array(data, other.data, data_count, other.data_count);
    _size = other.size();
    for(int i(0); i< other.child_count; i++){

        attach_item(subset, child_count, new BTree(other.dups_ok));
        subset[i]->copy_tree(*other.subset[i]);

    }
}
template<typename T>
bool    BTree<T>::contains(const T& entry){
    if(find(entry)!=NULL){return true;}
    else{return false;}
}
template<typename T>
T&      BTree<T>::get(const T& entry){

    //If entry is not in the tree, CRASH
    if(!contains(entry)){
        insert(entry);
    }
    return get_existing(entry);
}
template<typename T>
T&      BTree<T>::get(const T& entry) const{

    //If entry is not in the tree
    if(!contains(entry))
        insert(entry);
    return get_existing(entry);
}
template <typename T>
T&      BTree<T>::get_existing(const T& entry){
    T* temp = find(entry);
    assert(temp!=NULL);
    return *temp;
}
template<typename T>
T*      BTree<T>::find(const T& entry){

    int i = first_ge(data, data_count, entry);
    bool found = data[i]==entry;
    if(found){
        return &data[i];
    }
    if(is_leaf()){
        return NULL;
    }
    else{
        return subset[i]->find(entry);
    }

}
template<typename T>
int     BTree<T>::size() const{

    return _size;
}
template<typename T>
bool    BTree<T>::empty() const{
    if(_size==0){
        return true;
    }
    else{
        return false;
    }
}
template<typename T>
void    BTree<T>::print_tree(int level, ostream &outs) const{

    /*
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
                <<setw(level+INDENT)

                <<""<<"["<<data[i]<<"]"<<endl;
    }
    if(!is_leaf()){
        subset[0]->print_tree(level+INDENT);
    }
}
template<typename T>
bool    BTree<T>::loose_insert(const T& entry){


    /*
           int i = first_ge(data, data_count, entry);
           bool found = (i<data_count && data[i] == entry);

           three cases:
             a. found: deal with duplicates
             ! found:
             b. leaf : insert entry in data at position i
             c. !leaf: subset[i]->loose_insert(entry)
                       fix_excess(i) if there is a need
                |   found     |   !found        |
          ------|-------------|-----------------|-------
          leaf  |  a. Deal    | b: insert entry |
                |     with    |    at data[i]   |
          ------|  duplicates |-----------------|-------
                |             | d: subset[i]->  |
          !leaf |             |    loose_insert |
                |             |    fix_excess(i)|
          ------|-------------|-----------------|-------
        */
    int i = first_ge(data, data_count, entry);
    bool found = (i<data_count && data[i] == entry);
    bool inserted = false;
    if(found){
        if(dups_ok){
            data[i] += entry;
            return true;
        }
        else{
            data[i]=entry;
            return false;
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
void    BTree<T>::fix_excess(int i){
    //this node's child i has one too many items: 3 steps:
    //1. add a new subset at location i+1 of this node
    //2. split subset[i] (both the subset array and the data array) and move half into
    // subset[i+1] (this is the subset we created in step 1.)
    //3. detach the last data item of subset[i] and bring it and insert it into this node's data[]
    // //Note that this last step may cause this node to have too many items. This is OK. This will be
    // dealt with at the higher recursive level. (my parent will fix it!)

    if(subset[i]->data_count>MAXIMUM){
        BTree<T>* temp = new BTree<T>(dups_ok);

        insert_item(subset, i+1, child_count, temp);
        /* split the subset[i]->data into temp->data */
        split(subset[i]->data, subset[i]->data_count, temp->data, temp->data_count);
        split(subset[i]->subset, subset[i]->child_count, temp->subset, temp->child_count);
        T temp_item;
        detach_item(subset[i]->data, subset[i]->data_count, temp_item);
        int it = first_ge(data, data_count, temp_item);
        insert_item(data, it, data_count, temp_item);
    }
}
template<typename T>
bool    BTree<T>::loose_remove(const T& entry){
    /* four cases:
              a. leaf && not found target: there is nothing to do
              b. leaf && found target: just remove the target
              c. not leaf and not found target: recursive call to loose_remove
              d. not leaf and found: replace target with largest child of subset[i]

                 |   !found    |   found       |
           ------|-------------|---------------|-------
           leaf  |  a: nothing | b: delete     |
                 |     to do   |    target     |
           ------|-------------|---------------|-------
           !leaf | c: loose_   | d: replace    |
                 |    remove   |    w/ biggest |
           ------|-------------|---------------|-------


         */

    int j = first_ge(data, data_count, entry);
    bool found = (data[j]==entry);
    bool removed = true;
    if(found){
        if(is_leaf()){
            T temp_item;
            delete_item(data, j, data_count, temp_item);
            return true;
        }
        else{
            T biggest;
            subset[j]->remove_biggest(biggest);
            data[j] = biggest;
            if(subset[j]->data_count<MINIMUM){
                fix_shortage(j);
            }
            return true;
        }
    }
    else{
        if(is_leaf()){
            return false;
        }
        else{
            removed = subset[j]->loose_remove(entry);
            if(subset[j]->data_count<MINIMUM){
                fix_shortage(j);
            }
        }
    }
    return removed;


}
template<typename T>
void    BTree<T>::fix_shortage(int i){

    /*
     * fix shortage in subtree i:
     * if child i+1 has more than MINIMUM, rotate left
     * elif child i-1 has more than MINIMUM, rotate right
     * elif there is a right child, merge child i with next child
     * else merge child i with left child
     */

    assert(i>=0);
    assert(i<child_count);
    //* if child i+1 has more than MINIMUM, rotate left
    if(i+1<child_count && subset[i+1]->data_count>MINIMUM){
        rotate_left(i+1);
    }
        //* elif child i-1 has more than MINIMUM, rotate right
    else if(i-1>=0 && subset[i-1]->data_count>MINIMUM){
        rotate_right(i-1);
    }
        //* elif there is a right child, merge child i with next child
    else if(i+1<child_count && subset[i+1]!=nullptr){
        merge_with_next_subset(i);
    }
        //* else merge child i with left child
    else if(i>0){
        merge_with_next_subset(i-1);
    }

}
template<typename T>
void    BTree<T>::remove_biggest(T& entry){
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
void    BTree<T>::rotate_left(int i){
    /*
         * (0 < i < child_count) and (subset[i]->data_count > MINIMUM)
         * subset[i-1] has only MINIMUM - 1 entries.
         *
         * item transfers from child[i] to child [i-1]
         *
         * FIRST item of subset[i]->data moves up to data to replace data[i-1],
         * data[i-1] moves down to the RIGHT of subset[i-1]->data
         *
         *  i = 1: Rotating the bigger child the one that is not short
         *              [50 100]
         *  [  ]        [65 75]       ....
         *            [a]  [b]  [c]
         *
         *  65 move up to replace 50 (data[i])
         *  65's child (its child 0) moves over to be the child of 50
         *  50 moves down to the right of subset[i]->data
         *
         *              [65 100]
         *  [50]         [ 75 ]       ....
         *     [a]      [b]  [c]
         *
         *
         *
         * If necessary, shift first subset of subset[i] to end of subset[i-1]
         */

    if(i>0 and i<child_count and subset[i]->data_count>MINIMUM){

        T temp_movedn = data[i-1];
        T temp_moveup;

        delete_item(subset[i]->data, 0, subset[i]->data_count, temp_moveup);
        data[i-1] = temp_movedn;

        attach_item(subset[i-1]->data, subset[i-1]->data_count, temp_moveup);

        if(!subset[i]->is_leaf()){
            BTree<T>* tempptr = nullptr;
            delete_item(subset[i]->subset, 0, subset[0]->child_count, tempptr);
            attach_item(subset[i-1]->subset, subset[i-1]->child_count, tempptr);
        }

    }


}
template<typename T>
void    BTree<T>::rotate_right(int i){

    /*(i < child_count - 1) and (subset[i]->data_count > MINIMUM)
    * subset[i+ 1] has only MINIMUM - 1 entries. assuming only call this when this is true
    *
    * item transfers from child[i] to child [i+1]
    *
    * LAST item of subset[i]->data moves up to data to replace data[i],
    * data[i] moves down to the LEFT of subset[i+1]->data
    *
    * i = 1
    *                     [50 100]
    *      [20 30]        [65 75]          [ ]
    *  [..] [..] [..]   [a] [b] [c]        [..]
    *
    *  75 moves up to replace 100 (data[i])
    *  75's child (its last child) moves over to be the (child 0) child of 100
    *  100 moves down to subset[i]->data
    *
    *                     [50 75]
    *      [20 30]          [65]          [100]
    *  [..] [..] [..]     [a] [b]        [c] [..]
    *
    *
    *
    *
    * If necessary, shift last subset of subset[i] to front of subset[i+1]
    */

    if(i<child_count-1 && subset[i]->data_count>MINIMUM){
        T temp1 = data[i]; //100
        T temp2;
        detach_item(subset[i]->data, subset[i]->data_count, temp2);
        //temp2 75
        data[i] = temp2;
        insert_item(subset[i+1]->data, 0, subset[i+1]->data_count, temp1);


        if(!subset[i]->is_leaf()){
//            cout<<"rotate right -> !leaf"<<endl;
            BTree<T>* tempptr;
            detach_item(subset[i]->subset, subset[i]->child_count, tempptr);
            insert_item(subset[i+1]->subset, 0, subset[i+1]->child_count, tempptr);
        }
    }

}
template<typename T>
void    BTree<T>::merge_with_next_subset(int i){
    /*
     *  Merge subset[i] with subset [i+1] with data[i] in the middle
     *
     *   1. remove data[i] from this object
     *   2. append it to child[i]->data
     *   3. Move all data items from subset[i+1]->data to subset[i]->data
     *   4. Move all subset pointers from subset[i+1]->subset to subset[i]->subset
     *   5. delete subset[i+1] (store in a temp ptr)
     *   6. delete temp ptr
     */

    /* ASSERT HERE! */
    // |-| |-| |-| |-| |-|
    //  0   1   2   3   4   child count = 5, must always be < child_count-1
    assert(i<child_count-1);

    //1. remove data[i] from this object
    //2. append it to child[i]->data

    T temp_item;
    delete_item(data, i, data_count, temp_item);
//    cout<<"Temp Item: "<<temp_item<<endl;
    ordered_insert(subset[i]->data, subset[i]->data_count, temp_item);
//    insert_item(subset[i]->data, i, subset[i]->data_count, temp_item);

    //3. Move all data items from subset[i+1]->data to subset[i]->data
    //4. Move all subset pointers from subset[i+1]->subset to subset[i]->subset
    merge(subset[i]->data, subset[i]->data_count, subset[i+1]->data, subset[i+1]->data_count);
    merge(subset[i]->subset, subset[i]->child_count, subset[i+1]->subset, subset[i+1]->child_count);
    clear_array(subset[i+1]->data, subset[i+1]->data_count);
    clear_array(subset[i+1]->subset, subset[i+1]->child_count);

    //5. delete subset[i+1] (store in a temp ptr)
    BTree<T>* del_temp;
    delete_item(subset, i+1, child_count, del_temp);

    //6. delete temp ptr
    delete del_temp;
}
template<typename T>
bool BTree<T>::is_valid(){

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
        // must check if in order
        if(!is_asc(data, data_count)){
            return false;
        }
        //must check if data[i]<subset[i+1]'s items
        //and check if data[i]>subset[i]'s items
        for(int i=0; i<child_count-1; i++){
            if(!is_le(subset[i+1]->data, subset[i+1]->data_count, data[i]) ||
               !is_gt(subset[i]->data, subset[i]->data_count, data[i])){
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

#endif //DATABASE_BTREE_H
