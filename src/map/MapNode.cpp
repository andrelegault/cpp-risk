#include <MapNode.h>

template <typename T>
MapNode<T>::MapNode(string name) {
    this->name = name;
}

template <typename T>
MapNode<T>::~MapNode() {
    for(auto border : this->borders) {
        delete border;
    }

    if(this->parent) this->parent.remove(this);
}

template <typename T>
void MapNode<T>::add(Border<MapNode<T>>* border) {
    this->borders.push_back(border);
}

template <typename T>
void MapNode<T>::remove(Border<MapNode<T>>* border) {
    auto it = find(this->borders.begin(), this->borders.end(), border);

    if(it != this->borders.end()) this->borders.erase(it);
}

template <typename T>
void MapNode<T>::set(T* parent) {
    this->parent = parent;
}

template <typename T>
void MapNode<T>::connect(MapNode<T>* node) {
    auto border = &Border<MapNode<T>>(this, node);
    this->add(border);
    node->add(border);
}