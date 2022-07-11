#ifndef DATABASE_NODE_H
#define DATABASE_NODE_H
#include <iostream>
#include <cassert>
using namespace std;

template <typename ITEM_TYPE>
struct node
{
public:
    node();
    node(ITEM_TYPE item);
    friend ostream& operator <<(ostream& outs, const node<ITEM_TYPE> &printMe){
        outs<<printMe._item;
        return outs;
    }
    ITEM_TYPE _item;
    node* next;
    node* prior;
};
template <typename ITEM_TYPE>
void        PrintList(node<ITEM_TYPE>* head);

template <typename ITEM_TYPE>
void        PrintList_backwards(node<ITEM_TYPE> *head);

template <typename ITEM_TYPE>
node<ITEM_TYPE>* SearchList(node<ITEM_TYPE>* head, ITEM_TYPE key);

template <typename ITEM_TYPE>
node<ITEM_TYPE>* InsertHead(node<ITEM_TYPE> *&head, ITEM_TYPE insertThis);

template <typename ITEM_TYPE>
node<ITEM_TYPE>* InsertAfter(node<ITEM_TYPE>*& head, node<ITEM_TYPE> *afterThis, ITEM_TYPE insertThis);

template <typename ITEM_TYPE>
node<ITEM_TYPE>* InsertBefore(node<ITEM_TYPE>*& head, node<ITEM_TYPE>* beforeThis, ITEM_TYPE insertThis);

template <typename ITEM_TYPE>
node<ITEM_TYPE>* PreviousNode(node<ITEM_TYPE>* head, node<ITEM_TYPE>* prevToThis);

template <typename ITEM_TYPE>
ITEM_TYPE   DeleteNode(node<ITEM_TYPE>*&head, node<ITEM_TYPE>* deleteThis);

template <typename ITEM_TYPE>
node<ITEM_TYPE>* CopyList(node<ITEM_TYPE>* head);

template <typename ITEM_TYPE>
void        ClearList(node<ITEM_TYPE>*& head);

template <typename ITEM_TYPE>
ITEM_TYPE&  At(node<ITEM_TYPE>* head, int pos);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
//      Sorted List Routines. order: 0: ascending, order: other: descending
//                              Assume a Sorted List
//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename ITEM_TYPE>
node<ITEM_TYPE>* InsertSorted(node<ITEM_TYPE>* &head, ITEM_TYPE item, bool ascending=true);

template <typename ITEM_TYPE>
node<ITEM_TYPE>* InsertSorted_and_add(node<ITEM_TYPE>* &head, ITEM_TYPE item, bool ascending=true);

template <typename ITEM_TYPE>
node<ITEM_TYPE>* WhereThisGoes(node<ITEM_TYPE>* head, ITEM_TYPE item, bool ascending=true);

template <typename ITEM_TYPE>
node<ITEM_TYPE>* LastNode(node<ITEM_TYPE>* head);

/*
 *-------------------------------------------------------------------------------------------------------------
*/
template <typename ITEM_TYPE>
node<ITEM_TYPE>::node(){
    _item = ITEM_TYPE();
}

template <typename ITEM_TYPE>
node<ITEM_TYPE>::node(ITEM_TYPE item){
    _item = item;
}

template <typename ITEM_TYPE>
void PrintList(node<ITEM_TYPE>* head){
    node<ITEM_TYPE>* walker = head;
    cout<<"H->";
    while(walker!=nullptr){
        cout<<*walker;
        walker = walker->next;
    }
    cout<<"|||"<<endl;
}

template<typename ITEM_TYPE>
bool node_empty(node<ITEM_TYPE>* head){
    if(head==nullptr){return true;}
    else{return false;}
}

template <typename ITEM_TYPE>
node<ITEM_TYPE>* SearchList(node<ITEM_TYPE>* head, ITEM_TYPE key){
    node<ITEM_TYPE>* walker = head;
    if(walker==nullptr){return NULL;}
    while(walker!=nullptr){
        if(walker->_item==key){return walker;}
        walker = walker->next;
    }
    return NULL;
}

template <typename ITEM_TYPE>
node<ITEM_TYPE>* InsertHead(node<ITEM_TYPE>*& head, ITEM_TYPE insertThis){
    node<ITEM_TYPE>* temp = new node<ITEM_TYPE>(insertThis);

    if(node_empty(head)){
        temp->prior = nullptr;
        temp->next = head;
        head = temp;
    }
    else{
        temp->prior = nullptr;
        temp->next = head;
        head = temp;
        node<ITEM_TYPE>* temp2 = temp->next;
        temp2->prior=temp;
    }
    return head;
}

template <typename ITEM_TYPE>
node<ITEM_TYPE>* InsertAfter(node<ITEM_TYPE>*& head, node<ITEM_TYPE>* afterThis, ITEM_TYPE insertThis){
    /* Connect temp next to next node
     * connect this node to temp
     * return
     */
    node<ITEM_TYPE>* temp = new node<ITEM_TYPE>(insertThis);

    if(!node_empty(head)){
        temp->next = afterThis->next;
        temp->prior = afterThis;
        afterThis->next = temp;
        node<ITEM_TYPE>* temp2 = temp->next;
        //3.
        if(temp2!=nullptr){
            temp2->prior = temp;
        }
    }
    else{
        InsertHead(head,insertThis);
    }

    return temp;
}

template <typename ITEM_TYPE>
node<ITEM_TYPE>* InsertBefore(node<ITEM_TYPE>*& head, node<ITEM_TYPE>* beforeThis, ITEM_TYPE insertThis){
    node<ITEM_TYPE>* walker = PreviousNode<ITEM_TYPE>(head, beforeThis);
    walker = InsertAfter<ITEM_TYPE>(head, walker, insertThis);
    return walker;
}

template <typename ITEM_TYPE>
node<ITEM_TYPE>* PreviousNode(node<ITEM_TYPE>* head, node<ITEM_TYPE>* prevToThis){
    if(head == prevToThis || head == nullptr){return NULL;}

    if(prevToThis==nullptr){
        node<ITEM_TYPE>* walker = head;
        while(walker!=nullptr){
            if(walker->next==prevToThis){return walker;}
            walker=walker->next;
        }
        if(walker==nullptr){assert("Node not within the linked list"!="");}
    }
    else{
        return prevToThis->prior;
    }

    return nullptr;
}

template <typename ITEM_TYPE>
ITEM_TYPE DeleteNode(node<ITEM_TYPE>*&head, node<ITEM_TYPE>* deleteThis){
    /* A lot of parameters, basically wont delete on empty array
     * wont delete the nullptr
     * and only deletes existing elements
     */
    if(deleteThis==nullptr){assert("Deletion Node is the null pointer!"=="");}
    if(head==nullptr){assert("Empty List"=="");}

    ITEM_TYPE TMP = deleteThis->_item;

    node<ITEM_TYPE>* temp2 = deleteThis->next;
    node<ITEM_TYPE>* temp1 = deleteThis->prior;

    if(deleteThis == head){
        if(temp2!=nullptr){
            head=temp2;
            temp2->prior=temp1;
        }
        else{head=nullptr;}
    }
    else{
        if(temp2!=nullptr){
            temp2->prior=temp1;
            temp1->next=temp2;
        }
        else{temp1->next=nullptr;}
    }
    delete deleteThis;
    return TMP;
}

template <typename ITEM_TYPE>
node<ITEM_TYPE>* CopyList(node<ITEM_TYPE>* head){
    /* Makes a new head,
     * and COPIES chain elements
     * into the new linked list
     */

    node<ITEM_TYPE>* thead = new node<ITEM_TYPE>;
    node<ITEM_TYPE>* theadwalk = thead;

    if(head==nullptr){thead==nullptr;return thead;}

    thead->_item=head->_item;
    thead->next = nullptr;
    node<ITEM_TYPE>* walker = head->next;
    while(walker!=NULL){
        theadwalk = InsertAfter(thead, theadwalk, walker->_item);
        walker = walker->next;
    }
    return thead;
}

template <typename ITEM_TYPE>
void ClearList(node<ITEM_TYPE>*& head){

    //Keeps linking head to the next node, and deleting the
    //interrim node that was unchained

    node<ITEM_TYPE>* walker = LastNode(head);
    node<ITEM_TYPE>* w2 = walker;
    while(walker!=head){
        w2 = walker;
        walker = walker->prior;

        DeleteNode(head, w2);
    }
    head = nullptr;
}

template <typename ITEM_TYPE>
ITEM_TYPE& At(node<ITEM_TYPE>* head, int pos){

    //Returns the index of the position in the node chain
    //If its an invalid position, will assert

    node<ITEM_TYPE>* walker = head;
    for(int i=0; i<pos && walker!=nullptr;i++){

        walker=walker->next;
    }
    if(walker==nullptr){assert("Invalid Position"=="");}
    else{return walker->_item;}
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
//      Sorted List Routines. order: 0: ascending, order: other: descending
//                              Assume a Sorted List
//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename ITEM_TYPE>
node<ITEM_TYPE>* InsertSorted(node<ITEM_TYPE>* &head, ITEM_TYPE item, bool ascending){
    //Utilizes WTG for proper placement and places after it
    node<ITEM_TYPE>* walker = WhereThisGoes(head, item, ascending);


    if(ascending){
        if(walker==NULL){
            return InsertHead(head,item);
        }
        else{
            return InsertAfter(head,walker,item);
        }
        return head;
    }
    else{
        if(walker==NULL){
            return InsertHead(head,item);
        }
        else{
            return InsertBefore(head,walker,item);
        }
    }
}
template <typename ITEM_TYPE>
node<ITEM_TYPE>* InsertSorted_and_add(node<ITEM_TYPE>* &head, ITEM_TYPE item, bool ascending){
    /* Inserts to head in an empty array
     * Inserts after on a non matching sorted array
     * Inserts INTO if MATCHING
     */
    node<ITEM_TYPE>* walker = SearchList(head, item);
    if(walker==NULL){
        return InsertSorted(head,item, ascending);
    }
    if(SearchList(head,item)==NULL){
        walker = InsertSorted(head,item,ascending);
        return walker;
    }
    else{
        walker->_item+=item; return walker;
    }
}
template <typename ITEM_TYPE>
node<ITEM_TYPE>* WhereThisGoes(node<ITEM_TYPE>* head, ITEM_TYPE item, bool ascending){
    /* Comparison statements
     * decide the index for the item to be inserted
     * always inserting in a position that it belongs
     *
     * same concept for ascending and descending
     */
    if(ascending){
        if(head == nullptr){
            return NULL;
        }
        if(item<head->_item){
            return NULL;
        }
        node<ITEM_TYPE>* walker = head;
        node<ITEM_TYPE>* walker2 = walker->next;

        while(walker!=nullptr){
            if(walker2!=nullptr){
                if(item<walker2->_item || item==walker2->_item){
                    return walker;
                }
            }

            else if(walker2==nullptr){
                return walker;
            }

            walker=walker->next;
            walker2=walker2->next;
        }
    }
    else{
        if(head == nullptr){
            return NULL;
        }
        if(item<head->_item){
            return head;
        }
        node<ITEM_TYPE>* walker = head;
        node<ITEM_TYPE>* walker2 = walker->next;

        while(walker!=nullptr){


            if(walker2!=nullptr){
                if(item>walker2->_item || item==walker2->_item){
                    return walker;
                }
            }

            else if(walker2==nullptr){
                return walker;
            }

            walker=walker->next;
            walker2=walker2->next;
        }
    }
}

template <typename ITEM_TYPE>
node<ITEM_TYPE>* LastNode(node<ITEM_TYPE>* head){

    node<ITEM_TYPE>* walker = nullptr;

    if(head==nullptr){
        return NULL;}

    return PreviousNode(head, walker);

}
#endif //DATABASE_NODE_H
