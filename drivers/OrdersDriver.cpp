#include <iostream>
#include <assert.h>
#include <Order.hpp>
#include <Player.hpp>

using std::cout;
using std::endl;

/**
 * Creates a list of orders.
 * Demonstrates that the order/orderlist objects indeed have the mentioned features.
 */
int main() {
    OrdersList orders;
    Deploy* deploy = new Deploy(nullptr, nullptr);
    Advance* advance = new Advance(nullptr, nullptr, nullptr);
    Bomb* bomb = new Bomb(nullptr, nullptr);
    Blockade* blockade = new Blockade(nullptr, nullptr);
    Airlift* airlift = new Airlift(nullptr, nullptr, nullptr, nullptr);
    Negotiate* negotiate = new Negotiate(nullptr);


    // Testing Order::execute
    assert(deploy->validate() == true);
    assert(advance->validate() == true);
    assert(bomb->validate() == true);
    assert(blockade->validate() == true);
    assert(airlift->validate() == true);
    assert(negotiate->validate() == true);

    // Testing OrdersList::addOrder
    orders.addOrder(bomb);
    orders.addOrder(deploy);
    orders.addOrder(advance);
    orders.addOrder(blockade);
    orders.addOrder(airlift);
    orders.addOrder(negotiate);
    assert(bomb == orders.getAtIndex(0));
    assert(deploy == orders.getAtIndex(1));
    assert(advance == orders.getAtIndex(2));

    // Testing the OrdersList copy constructor.
    OrdersList orders2(orders);
    Player mockPlayer;

    orders.getAtIndex(0)->execute();
    orders2.getAtIndex(0)->execute();

    orders.getAtIndex(5)->execute();
    orders2.getAtIndex(5)->execute();

    // Testing OrdersList::move
    orders.move(bomb, advance);
    assert(advance == orders.getAtIndex(0));
    assert(deploy == orders.getAtIndex(1));
    assert(bomb == orders.getAtIndex(2));

    // Testing OrdersList::remove
    assert(orders.getLength() == 6);
    orders.remove(deploy);
    assert(orders.getLength() == 5);
    orders.remove(bomb);
    assert(orders.getLength() == 4);
    orders.remove(advance);
    assert(orders.getLength() == 3);

    assert(orders2.getLength() == 6);

    return 0;
}