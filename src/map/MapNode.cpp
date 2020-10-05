#include <MapNode.hpp>

#include <algorithm>

MapNode::MapNode(string name) {
    this->name = name;
}

// TODO: Deep clone.
MapNode::MapNode(MapNode* mapNode) {
    this->name = mapNode->name;
    this->borders = mapNode->borders;
}

MapNode::~MapNode() {
    while (!this->borders.empty()) delete this->borders.back();
}

string MapNode::getName() {
    return this->name;
}

void MapNode::connect(Border* border) {
    this->borders.push_back(border);
}

void MapNode::remove(Border* border) {
    auto it = find(this->borders.begin(), this->borders.end(), border);

    if (it != this->borders.end()) this->borders.erase(it);
}

void MapNode::connect(MapNode* node) {
    Border* border = new Border(this, node);
    this->connect(border);
    node->connect(border);
}