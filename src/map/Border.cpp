#include <Border.hpp>

#include <iostream>

Border::Border() : Border(nullptr, nullptr) {}

Border::Border(MapNode* n1, MapNode* n2) {
    this->n1 = n1;
    this->n2 = n2;
}

Border::Border(Border* border) {
    Map* m = new Map(border->getMap());
    Border* borderCopy = m->get(border);

    this->n1 = borderCopy->n1;
    this->n2 = borderCopy->n2;
}

Border::~Border() {
    if (this->n1) this->n1->remove(this);
    if (this->n2) this->n2->remove(this);
}

ostream& operator<<(ostream& stream, const Border* border) {
    stream << border->n1 << " <-> " << border->n2;

    return stream;
}

bool operator== (const Border& b1, const Border& b2) {
    return *b1.n1 == *b2.n1 && *b1.n2 == *b2.n2;
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

Map* Border::getMap() const {
    return this->n1->getMap();
}