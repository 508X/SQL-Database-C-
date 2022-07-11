#ifndef DATABASE_LIST_H
#define DATABASE_LIST_H
#include <iostream>
#include "../Node/node.h"

using namespace std;

template <class ITEM_TYPE>
class List
{
public:
    class Iterator
    {
    public:
        friend class List;

        /* Iterator Default constructor
         *
         *
         */
        Iterator()                                                              {
            _ptr=nullptr;
        }
        /* Constructor with Node Pointer
         *
         *
         */
        Iterator(node<ITEM_TYPE>* p)                                            {
            _ptr=p;
        }
        /* Operator Bool
         * If Ptr is null return true else false
         */
        operator bool()                                         {
            if(_ptr!=nullptr){return true;}
            else{return false;}
        }
        /* Operator *
         * Arg:none
         * Ret: Item of ptr
         */
        ITEM_TYPE&      operator  *  ()                                         {
            return _ptr->_item;
        }
        /* Operator ->
         * Returns Location of item
         *
         */
        ITEM_TYPE*      operator  -> ()                                         {
            return &(_ptr->_item);
        }
        /* is null
         * is _ptr is null return true
         *
         */
        bool            is_null()                                               {
            if(_ptr==nullptr){return true;}
            else{return false;}
        }
        /* Operator !=
         * If pointers are not pointing at the same thing return true
         *
         */
        friend bool     operator  != (const Iterator&left, const Iterator&right){
            if(left==right){return false;}
            else{return true;}
        }
        /* Operator ==
         * if pointers are pointing at the same thing return true
         *
         */
        friend bool     operator  == (const Iterator&left, const Iterator&right){

            if((left._ptr==nullptr && right._ptr==nullptr)){
                //if both are null return true
                return true;
            }
            else if( (left._ptr==nullptr && right._ptr!=nullptr) ||
                     (left._ptr==nullptr && right._ptr!=nullptr) ){
                //if one is null and the other isnt return false
                return false;
            }
            else if (left._ptr==right._ptr && left._ptr->_item==right._ptr->_item){
                return true;
            }
        }
        /* Operator iterator++
         * Increments the iterator
         *
         */
        Iterator&       operator  ++ ()                                         {

            //++it;
            //advance iterator to the next position.
            //return *this
            if(_ptr!=nullptr){
                _ptr = _ptr->next;
                return *this;
            }
        }
        /* operator ++iterator
         * increments the iterator and returns previous value
         *
         */
        friend Iterator operator  ++ (Iterator& it, int unused)                 {
            //it++
            //hold on to this position.
            //advance me to the next position.
            //return the position you were holding
            Iterator hold = it;
            ++it;
            return hold;

        }

    private:
        node<ITEM_TYPE>* _ptr;
    };

    List();
    ~List();
    List(const List<ITEM_TYPE> &copyThis);
    List<ITEM_TYPE>& operator =(const List& RHS);

    ITEM_TYPE   LDelete        (node<ITEM_TYPE>* iMarker);
    Iterator    itInsertHead   (ITEM_TYPE i);
    Iterator    itInsertAfter  (ITEM_TYPE i, Iterator iMarker);
    Iterator    itInsertBefore (ITEM_TYPE i, Iterator iMarker);
    Iterator    itInsertSorted (ITEM_TYPE i);

    ITEM_TYPE   itDelete       (List<ITEM_TYPE>::Iterator iMarker);
    void        itPrint        () const;
    Iterator    itSearch       (const ITEM_TYPE &key);
    Iterator    itPrev         (Iterator iMarker);
    ITEM_TYPE&  operator     [](int index);
    Iterator    itBegin        () const;
    Iterator    itEnd          () const;
    Iterator    itLastNode     () const;
    node<ITEM_TYPE>*    LInsertHead    (ITEM_TYPE i);
    node<ITEM_TYPE>*    LInsertAfter   (ITEM_TYPE i, node<ITEM_TYPE>* iMarker);
    node<ITEM_TYPE>*    LInsertBefore  (ITEM_TYPE i, node<ITEM_TYPE>* iMarker);
    node<ITEM_TYPE>*    LInsertSorted  (ITEM_TYPE i);
    node<ITEM_TYPE>*    LSearch        (const ITEM_TYPE &key);
    node<ITEM_TYPE>*    LPrev          (node<ITEM_TYPE>* iMarker);
    node<ITEM_TYPE>*    LBegin         () const;
    node<ITEM_TYPE>*    LEnd           () const;
    node<ITEM_TYPE>*    LLastNode      () const;
    template <class U>
    friend ostream& operator << (ostream& outs, const List<U>& l);

private:


    node<ITEM_TYPE>* head;
};
template <class ITEM_TYPE>
List<ITEM_TYPE>::List(){
    head = nullptr;
}
template <class ITEM_TYPE>
List<ITEM_TYPE>::~List(){
    ClearList(head);
}
template <class ITEM_TYPE>
List<ITEM_TYPE>::List(const List<ITEM_TYPE> &copyThis){
    copyThis = CopyList(head);
}
template <class ITEM_TYPE>
List<ITEM_TYPE>& List<ITEM_TYPE>::operator =(const List& RHS){
    if(this==&RHS){return *this;}

    ClearList(head);
    head = CopyList(RHS.head);
    return *this;
}

template<class ITEM_TYPE>
typename List<ITEM_TYPE>::Iterator List<ITEM_TYPE>::itInsertHead(ITEM_TYPE i){
    return LInsertHead(i);
}
template<class ITEM_TYPE>
typename List<ITEM_TYPE>::Iterator List<ITEM_TYPE>::itInsertAfter(ITEM_TYPE i, typename List<ITEM_TYPE>::Iterator iMarker){
    return LInsertAfter(i, iMarker._ptr);
}
template<class ITEM_TYPE>
typename List<ITEM_TYPE>::Iterator List<ITEM_TYPE>::itInsertBefore(ITEM_TYPE i, typename List<ITEM_TYPE>::Iterator iMarker){
    return LInsertBefore(i, iMarker._ptr);
}
template<class ITEM_TYPE>
typename List<ITEM_TYPE>::Iterator List<ITEM_TYPE>::itInsertSorted(ITEM_TYPE i){
    return LInsertSorted(i);
}

template<class ITEM_TYPE>
ITEM_TYPE List<ITEM_TYPE>::itDelete(typename List<ITEM_TYPE>::Iterator iMarker){
    return LDelete(iMarker._ptr);
}
template<class ITEM_TYPE>
void     List<ITEM_TYPE>::itPrint() const{
    int i = 0;
    for(List<ITEM_TYPE>::Iterator it=itBegin();it!=itEnd(); it++){
        cout<<"("<<*it<<")";
        i++;
    }
}
template<class ITEM_TYPE>
typename List<ITEM_TYPE>::Iterator List<ITEM_TYPE>::itSearch(const ITEM_TYPE &key){
    return LSearch(key);
}
template<class ITEM_TYPE>
typename List<ITEM_TYPE>::Iterator List<ITEM_TYPE>::itPrev(typename List<ITEM_TYPE>::Iterator iMarker){
    return LPrev(iMarker);
}

template<class ITEM_TYPE>
ITEM_TYPE& List<ITEM_TYPE>::operator[](int index){
    return At(head, index);
}

template<class ITEM_TYPE>
typename List<ITEM_TYPE>::Iterator List<ITEM_TYPE>::itBegin() const{
    return LBegin();
}
template<class ITEM_TYPE>
typename List<ITEM_TYPE>::Iterator List<ITEM_TYPE>::itEnd() const{
    return LEnd();
}
template<class ITEM_TYPE>
typename List<ITEM_TYPE>::Iterator List<ITEM_TYPE>::itLastNode() const{
    return LLastNode();
}
template <class U>
ostream& operator <<(ostream& outs, const List<U>& l){
    l.itPrint();
    return outs;
}

template<class ITEM_TYPE>
node<ITEM_TYPE>* List<ITEM_TYPE>::LInsertHead(ITEM_TYPE i){
    InsertHead<ITEM_TYPE>(head, i);
    return head;
}
template<class ITEM_TYPE>
node<ITEM_TYPE>* List<ITEM_TYPE>::LInsertAfter(ITEM_TYPE i, node<ITEM_TYPE>* iMarker){
    if(iMarker == nullptr){
        return LInsertHead(i);
    }
    else{
        return InsertAfter<ITEM_TYPE>(head, iMarker, i);
    }
}
template<class ITEM_TYPE>
node<ITEM_TYPE>* List<ITEM_TYPE>::LInsertBefore(ITEM_TYPE i, node<ITEM_TYPE>* iMarker){
    if(LPrev(iMarker)==nullptr){
        LInsertHead(i);
    }
    else{
        InsertBefore<ITEM_TYPE>(head, iMarker, i);
    }
    return LPrev(iMarker);
}
template<class ITEM_TYPE>
node<ITEM_TYPE>* List<ITEM_TYPE>::LInsertSorted(ITEM_TYPE i){
    InsertSorted<ITEM_TYPE>(head, i);
    return head;
}
template<class ITEM_TYPE>
ITEM_TYPE        List<ITEM_TYPE>::LDelete(node<ITEM_TYPE>* iMarker){
    if(iMarker!=nullptr){
        return DeleteNode<ITEM_TYPE>(head, iMarker);
    }
    else{
        return ITEM_TYPE();
    }
}
template<class ITEM_TYPE>
node<ITEM_TYPE>* List<ITEM_TYPE>::LSearch(const ITEM_TYPE &key){
    return SearchList<ITEM_TYPE>(head, key);
}
template<class ITEM_TYPE>
node<ITEM_TYPE>* List<ITEM_TYPE>::LPrev(node<ITEM_TYPE>* iMarker){
    return PreviousNode<ITEM_TYPE>(head, iMarker);
}
template<class ITEM_TYPE>
node<ITEM_TYPE>* List<ITEM_TYPE>::LBegin() const{
    return head;
}
template<class ITEM_TYPE>
node<ITEM_TYPE>* List<ITEM_TYPE>::LEnd() const{
    return nullptr;
}
template<class ITEM_TYPE>
node<ITEM_TYPE>* List<ITEM_TYPE>::LLastNode() const{
    return LastNode(head);
}
#endif //DATABASE_LIST_H
