#include <Border.hpp>

#include <iostream>

Border::Border(MapNode* n1, MapNode* n2) {
    this->n1 = n1;
    this->n2 = n2;
}

Border::Border(Border* border) {
    this->n1 = border->n1;
    this->n2 = border->n2;
}

Border::~Border() {
    if (this->n1) this->n1->remove(this);
    if (this->n2) this->n2->remove(this);
}

ostream& operator<<(ostream& stream, const Border* border) {
    stream << border->n1 << " <-> " << border->n2;

    return stream;
}

void Border::operator=(const Border* border) {
    this->~Border();
    this->n1 = border->n1;
    this->n2 = border->n2;
}

MapNode* Border::getOther(MapNode* n) {
    if (n == this->n1) return this->n2;
    else return this->n1;
}

bool Border::has(MapNode* node) {
    return node == this->n1 || node == this->n2;
}