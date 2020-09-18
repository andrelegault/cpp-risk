#include <MapNode.h>
#include <algorithm>

MapNode::MapNode(string name) {
    this->name = name;
}

MapNode::~MapNode() {
    for(auto border : this->borders) {
        delete border;
    }
}


void MapNode::add(Border<MapNode>* border) {
    this->borders.push_back(border);
}


void MapNode::remove(Border<MapNode>* border) {
    auto it = find(this->borders.begin(), this->borders.end(), border);

    if(it != this->borders.end()) this->borders.erase(it);
}

void MapNode::connect(MapNode* node) {
    Border<MapNode> border = Border<MapNode>(this, node);
    this->add(&border);
    node->add(&border);
}