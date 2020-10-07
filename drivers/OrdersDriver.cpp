#include <iostream>
#include <assert.h>
#include <Order.hpp>

using std::cout;
using std::endl;

/**
 * Creates a list of orders.
 * Demonstrates that the order/orderlist objects indeed have the mentioned features.
 */
int main() {
    OrdersList* ordersList = new OrdersList;
    Deploy* deploy = new Deploy;
    Advance* advance = new Advance;
    Bomb* bomb = new Bomb;
    Blockade* blockade = new Blockade;
    Airlift* airlift = new Airlift;
    Negotiate* negotiate = new Negotiate;

    // Testing Order::execute
    assert(deploy->validate() == true);
    assert(advance->validate() == true);
    assert(bomb->validate() == true);
    assert(blockade->validate() == true);
    assert(airlift->validate() == true);
    assert(negotiate->validate() == true);

    // Testing OrdersList::addOrder
    ordersList->addOrder(bomb);
    ordersList->addOrder(deploy);
    ordersList->addOrder(advance);
    ordersList->addOrder(blockade);
    ordersList->addOrder(airlift);
    ordersList->addOrder(negotiate);
    assert(bomb == ordersList->getAtIndex(0));
    assert(deploy == ordersList->getAtIndex(1));
    assert(advance == ordersList->getAtIndex(2));

    // Testing OrdersList::move
    ordersList->move(bomb, advance);
    assert(advance == ordersList->getAtIndex(0));
    assert(deploy == ordersList->getAtIndex(1));
    assert(bomb == ordersList->getAtIndex(2));

    // Testing OrdersList::remove
    assert(ordersList->getLength() == 6);
    ordersList->remove(deploy);
    assert(ordersList->getLength() == 5);
    ordersList->remove(bomb);
    assert(ordersList->getLength() == 4);
    ordersList->remove(advance);
    assert(ordersList->getLength() == 3);

    delete ordersList;
    ordersList = nullptr;
    return 0;
}