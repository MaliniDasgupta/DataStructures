#include "list.hpp"
#include <fstream>

// TODO: Insert List method definitions here.
List::List(){
    head = new Node;
    head->next = nullptr;
    last = head;
    len = 0;
}

List::List(const List& other){
    head = new Node;
    last = head;
    len = 0;
    for(unsigned i=0;i<other.length();i++){
        pushBack(other[i]);
    }

}

List::~List(){
    Node* next;
    Node* curr = head;
    while(curr!=nullptr){
        next = curr->next;
        delete curr;
        len--;
        curr = next;
    }

}

List& List::operator=(const List& other){
    //copy values of other into this list
    Node* next;
    Node* curr = head;
    while(curr!=nullptr){
        next = curr->next;
        delete curr;
        len--;
        curr = next;
    }
    head = new Node;
    for(unsigned i=0;i<other.length();i++){
        pushBack(other[i]);
    }
    return (*this);
}

unsigned List::length() const{
    return len;
}

void List::pushFront(int data){
    if(len == 0){
        head->data = data;
        last = head;
    }
    else{
        Node* newVal = new Node;
        newVal->data = data;
        newVal->next = head;
        head = newVal;
    }
    len++;
}

void List::pushBack(int data){
    if(len == 0){
        head->data = data;
        last = head;
    }
    else{
        Node* newVal = new Node;
        newVal->data = data;
        newVal->next = nullptr;
        last->next = newVal;
        last = newVal;
    }
    len++;
}

void List::operator+=(int data){
    pushBack(data);
}

int& List::operator[](unsigned index){
    if(index>=len){
        throw std::out_of_range{"Index out of bounds"};
    }
    Node* curr = head;
    for(unsigned i=0;i<index;i++){
        curr = curr->next;
    }
    return curr->data;
}

const int& List::operator[] (unsigned index) const{
    if(index>=len){
        throw std::out_of_range{"Index out of bounds"};
    }
    Node* curr = head;
    for(unsigned i=0;i<index;i++){
        curr = curr->next;
    }
    return curr->data;

}

bool List::contains(int data) const{
    Node* cur = head;
    for(unsigned i=0;i<len;i++){
        if(cur->data == data){
            return 1;
        }
        else{
            cur = cur->next;
        }
    }
    return 0;
}   

bool List::remove(int data){
    if(head->data == data){
        Node* prev = head;
        head = head->next;
        delete prev;
        len--;
        return 1;
    }
    Node* cur = head->next;
    Node* prev = head;

    while(cur!=nullptr){
        if(cur->data == data){
            prev->next = cur->next;
            if(prev->next == nullptr){
                last = prev;
            }
            delete cur;
            len--;
            return 1;
        }
        else{
            prev = cur;
            cur = cur->next;
        }
    }
    return 0;

}

void List::operator-=(int data){
   remove(data);
}

std::ostream& operator<<(std::ostream& os, const List& list){
    for(unsigned i=0;i<list.length();i++){
        os << list[i] << ' ';
    }
    os << '\n';
    return os;
}








