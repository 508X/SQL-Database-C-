#ifndef DATABASE_QUEUE_H
#define DATABASE_QUEUE_H
#include <iostream>
#include <vector>
#include "../List/list.h"
#include "../Node/node.h"
using namespace std;


template<class ITEM_TYPE>
class Queue
{
public:
    class Iterator{
    public:
        Iterator()                                                              {
            _iter = List<ITEM_TYPE>::Iterator();
        }
        Iterator(node<ITEM_TYPE>* s)                                            {
            _iter = typename List<ITEM_TYPE>::Iterator(s);
        }
        operator bool()                                                         {
            if(*(this->_iter)!=nullptr){return true;}
            else{return false;}
        }
        ITEM_TYPE&      operator  *  ()                                         {
            return *_iter;
        }
        ITEM_TYPE*      operator  -> ()                                         {
            return &*_iter;
        }
        Iterator&       operator  ++ ()                                         {
            _iter++;
            return *this;
        }
        Iterator&       operator  -- ()                                         {
            _iter--;
            return *this;
        }
        friend Iterator operator  ++ (Iterator& it, int unused)                 {
            Iterator hold = it;
            ++it;
            return hold;
        }
        friend Iterator operator  -- (Iterator& it, int unused)                 {
            Iterator hold = it;
            --it;
            return hold;
        }
        friend bool     operator  != (const Iterator&left, const Iterator&right){
            if(left._iter==right._iter){return false;}
            else{return true;}
        }
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

    /* Default Queue constructor
     */
    Queue();
    Queue(const Queue<ITEM_TYPE>& copyme);
    Queue<ITEM_TYPE>& operator =(const Queue& me);
    Queue(const vector<ITEM_TYPE>& me);
    void clear();
    ITEM_TYPE pop();
    ITEM_TYPE push(ITEM_TYPE me);
    bool empty();
    ITEM_TYPE top();
    Iterator QBegin();
    Iterator QEnd();
    Iterator QLast();
    void Printer();
    void PrinterEQ();
    void qtestf();

private:
    List<ITEM_TYPE> _list;
    typename List<ITEM_TYPE>::Iterator _iter;
    typename List<ITEM_TYPE>::Iterator _top;
    typename List<ITEM_TYPE>::Iterator _end;
};

template<class ITEM_TYPE>
Queue<ITEM_TYPE>::Queue()
        :_list()
{
    _top = _list.itBegin();
    _end = _list.itLastNode();
}

template<class ITEM_TYPE>
Queue<ITEM_TYPE>::Queue(const Queue<ITEM_TYPE>& me){
    _list = me._list;
    _top = _list.itBegin();
    _end = _list.itLastNode();
}

template<class ITEM_TYPE>
Queue<ITEM_TYPE>& Queue<ITEM_TYPE>::operator =(const Queue& me){
    if(this==&me){return *this;}
    _list = me._list;
    _top = _list.itBegin();
    _end = _list.itLastNode();
    return *this;
}

template <class ITEM_TYPE>
Queue<ITEM_TYPE>::Queue(const vector<ITEM_TYPE>& me)
        :_list()
{
    _top = _list.itBegin();
    _end = _list.itLastNode();
    for(int i=0; i<me.size(); i++){
        push(me[i]);
    }
}

template<class ITEM_TYPE>
typename Queue<ITEM_TYPE>::Iterator Queue<ITEM_TYPE>::QBegin(){
    return _list.LBegin();
}

template<class ITEM_TYPE>
typename Queue<ITEM_TYPE>::Iterator Queue<ITEM_TYPE>::QEnd(){
    return _list.LEnd();
}

template<class ITEM_TYPE>
typename Queue<ITEM_TYPE>::Iterator Queue<ITEM_TYPE>::QLast(){
    return _list.LLastNode();
}

template<class ITEM_TYPE>
void Queue<ITEM_TYPE>::clear(){
    while(!empty()){
        pop();
    }
}

template<class ITEM_TYPE>
ITEM_TYPE Queue<ITEM_TYPE>::push(ITEM_TYPE me){

    if(empty()){
        _end = _list.itInsertHead(me);
    }
    else{
        _list.itInsertAfter(me, _end);
        _end++;
    }

    _top = _list.itBegin();

    return me;

}

template<class ITEM_TYPE>
ITEM_TYPE Queue<ITEM_TYPE>::pop(){
    ITEM_TYPE hold = _list.itDelete(_top);
    _top = _list.itBegin();
    return hold;
}

template<class ITEM_TYPE>
bool Queue<ITEM_TYPE>::empty(){
    if(_list.LBegin()==_list.LEnd()){
        return true;
    }
    else{
        return false;
    }
}

template<class ITEM_TYPE>
ITEM_TYPE Queue<ITEM_TYPE>::top(){
    return *_top;
}

template<class ITEM_TYPE>
void Queue<ITEM_TYPE>::Printer(){
    Queue<ITEM_TYPE>::Iterator QE = QEnd();
    if(QBegin()!=QEnd()){
        cout<<"|";
    }
    for(Queue<ITEM_TYPE>::Iterator QI(QBegin()); QI!=QE; ++QI){
        cout<<*QI;
        cout<<"|";
    }
}

template<class ITEM_TYPE>
void Queue<ITEM_TYPE>::PrinterEQ(){
    cout<<"E: "<<*_end<<endl;
    cout<<"T: "<<*_top<<endl;
    if(QBegin()!=QEnd()){
    }
    for(Queue<ITEM_TYPE>::Iterator QI(QBegin()); QI!=QEnd(); ++QI){
        cout<<" "<<*QI;
    }
}

#endif //DATABASE_QUEUE_H
