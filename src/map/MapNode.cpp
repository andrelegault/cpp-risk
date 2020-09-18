#include <MapNode.hpp>

#include <algorithm>

MapNode::MapNode(string name) {
    this->name = name;
}

MapNode::MapNode(MapNode* mapNode) {
    this->name = mapNode->name;
}

MapNode::~MapNode() {
    for(auto border : this->borders) {
        delete border;
    }
}

string MapNode::getName() { return this->name; }

void MapNode::add(Border* border) {
    this->borders.push_back(border);
}


void MapNode::remove(Border* border) {
    auto it = find(this->borders.begin(), this->borders.end(), border);

    if(it != this->borders.end()) this->borders.erase(it);
}

void MapNode::connect(MapNode* node) {
    Border border(this, node);
    this->add(&border);
    node->add(&border);
}