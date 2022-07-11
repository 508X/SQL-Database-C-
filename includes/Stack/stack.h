#ifndef DATABASE_STACK_H
#define DATABASE_STACK_H
#include "../Node/node.h"
#include "../List/list.h"
#include <iostream>


using namespace std;
template<class ITEM_TYPE>

class Stack{

public:
    class Iterator{
    public:
        /* Iterator Constructor
         *
         */
        Iterator()                                                              {
            _iter = List<ITEM_TYPE>::Iterator();
        }
        /* iterator (NODE)
         * Constructos an iterator observing a node
         */
        Iterator(node<ITEM_TYPE>* s)                                            {
            _iter = typename List<ITEM_TYPE>::Iterator(s);
        }
        /* operator bool
         * returns true is this->List Iterator is oberving NULL
         */
        operator bool()                                         {
            if(*(this->_iter)!=nullptr){return true;}
            else{return false;}
        }
        /* Operator *
         * Returns the Item the iterator is observing
         */
        ITEM_TYPE&      operator  *  ()                                         {
            return *_iter;
        }
        /* Operator ->
         * Returns the location of the item the iterator is observing
         */
        ITEM_TYPE*      operator  -> ()                                         {
            return &*_iter;
        }
        /* Operator ++
         * Increments the Iterator to the next position in the list
         */
        Iterator&       operator  ++ ()                                         {

            _iter++;
            return *this;

        }
        /* Operator ++ 2Arg
         * Increments the iterator but returns previous value
         */
        friend Iterator operator  ++ (Iterator& it, int unused)                 {

            Iterator hold = it;
            ++it;
            return hold;

        }
        /* Operator !=
         * If the iterators are NOT in the same position, obsering the same items: RET True
         */
        friend bool     operator  != (const Iterator&left, const Iterator&right){
            if(left._iter==right._iter){return false;}
            else{return true;}
        }
        /* Operator ==
         * If the iterators are in the same position, obsering the same items: RET True
         */
        friend bool     operator  == (const Iterator&left, const Iterator&right){
            if(left._iter==right._iter){
                return true;
            }
            else{
                return false;
            }

        }

    private:
        typename List<ITEM_TYPE>::Iterator _iter;
    };

    Stack();
    Stack operator =(const Stack<ITEM_TYPE>& me);
    Stack(const Stack<ITEM_TYPE>& copyme);
    ITEM_TYPE pop();
    ITEM_TYPE push(const ITEM_TYPE& me);
    bool empty();
    int size();
    ITEM_TYPE top();
    Iterator STKBegin();
    Iterator STKEnd();
    Iterator STKLastNode();
    void Printer();

private:
    List<ITEM_TYPE> _list;
    typename List<ITEM_TYPE>::Iterator _top;
    int _size;
};

template<class ITEM_TYPE>
typename Stack<ITEM_TYPE>::Iterator Stack<ITEM_TYPE>::STKBegin(){
    return _list.LBegin();
}

template<class ITEM_TYPE>
typename Stack<ITEM_TYPE>::Iterator Stack<ITEM_TYPE>::STKEnd(){
    return _list.LEnd();
}

template<class ITEM_TYPE>
typename Stack<ITEM_TYPE>::Iterator Stack<ITEM_TYPE>::STKLastNode(){
    return _list.LLastNode();
}

template<class ITEM_TYPE>
Stack<ITEM_TYPE>::Stack() :_list(), _size(0)
{
    _top = _list.itBegin();
}

template<class ITEM_TYPE>
Stack<ITEM_TYPE> Stack<ITEM_TYPE>::operator =(const Stack<ITEM_TYPE>& me){
    if(this==&me){return *this;}
    _list = me._list;
    _size = me._size;
    _top=_list.itBegin();
    return *this;
}

template<class ITEM_TYPE>
Stack<ITEM_TYPE>::Stack(const Stack<ITEM_TYPE>& copyme){
    _list = copyme._list;
    _size = copyme._size;
    _top = _list.itBegin();
}

template<class ITEM_TYPE>
ITEM_TYPE Stack<ITEM_TYPE>::push(const ITEM_TYPE& me){
    _list.itInsertHead(me);
    _top = _list.itBegin();
    _size++;
    return me;
}

template<class ITEM_TYPE>
ITEM_TYPE Stack<ITEM_TYPE>::pop(){
    ITEM_TYPE hold = _list.itDelete(_top);
    _top = _list.itBegin();
    if(_size>0){
        _size--;
    }
    return hold;
}

template<class ITEM_TYPE>
bool Stack<ITEM_TYPE>::empty(){
    if(_size==0){
        return true;
    }
    else{
        return false;
    }
}

template<class ITEM_TYPE>
ITEM_TYPE Stack<ITEM_TYPE>::top(){
    return *_top;
}

template <class ITEM_TYPE>
int Stack<ITEM_TYPE>::size(){
    return _size;
}

template<class ITEM_TYPE>
void Stack<ITEM_TYPE>::Printer(){
    cout<<"[>>]";
    if(STKBegin()!=STKEnd()){
        cout<<"|";
    }
    for(Stack<ITEM_TYPE>::Iterator SI(STKBegin()); SI!=STKEnd(); ++SI){
        Stack<ITEM_TYPE >::Iterator GI=SI;
        cout<<*SI<<"|";
    }
    cout<<"[<<]";
}

#endif //DATABASE_STACK_H
