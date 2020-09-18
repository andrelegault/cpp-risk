#include <iostream>

#include <Border.h>

template <typename T>
ostream& operator<<(ostream& stream, const Border<T>* border) {
    stream << border->n1 << " <-> " << border->n2;

    return stream;
}

template <typename T>
Border<T>::Border(T* n1, T* n2) {
    this->n1 = n1;
    this->n2 = n2;
}

template <typename T>
Border<T>::Border(Border<T>* border) {
    this->o1 = &T(border->o1);
    this->o2 = &T(border->o2);
}

template <typename T>
Border<T>::~Border() {
    if(this->o1) this->o1->removeBorder(this);
    if(this->o2) this->o2->removeBorder(this);
}

template <typename T>
T* Border<T>::getOther(T* o) {
    if(o == this->o1) return this->o2;
    else return this->o1; 
}