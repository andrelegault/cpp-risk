#include <Border.hpp>

#include <iostream>

Border::Border(MapNode* n1, MapNode* n2) {
    this->n1 = n1;
    this->n2 = n2;
}

Border::Border(Border* border) {
    MapNode n1(border->n1);
    MapNode n2(border->n2);

    this->n1 = &n1;
    this->n2 = &n2;
}

Border::~Border() {
    if (this->n1) this->n1->remove(this);
    if (this->n2) this->n2->remove(this);
}

ostream& operator<<(ostream& stream, const Border* border) {
    stream << border->n1 << " <-> " << border->n2;

    return stream;
}

MapNode* Border::getOther(MapNode* n) {
    if (n == this->n1) return this->n2;
    else return this->n1;
}