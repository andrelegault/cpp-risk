#include <iostream>
#include <Order.hpp>
#include <assert.h>

using std::cout;
using std::endl;

/**
 * Creates a list of orders.
 * Demonstrates that the order/orderlist objects indeed have the mentioned features.
 */
int main() {
    OrdersList ordersList;
    Deploy* deploy = new Deploy;
    Advance* advance = new Advance;
    Bomb* bomb = new Bomb;

    ordersList.addOrder(bomb);
    ordersList.addOrder(deploy);
    ordersList.addOrder(advance);

    ordersList.move(bomb, advance);
    assert(advance == ordersList.getAtIndex(0));
    assert(deploy == ordersList.getAtIndex(1));
    assert(bomb == ordersList.getAtIndex(2));

    // OrdersList ordersList2 = OrdersList(ordersList);
    assert(ordersList.getLength() == 3);
    ordersList.remove(deploy);
    assert(ordersList.getLength() == 2);
    ordersList.remove(bomb);
    assert(ordersList.getLength() == 1);
    ordersList.remove(advance);
    assert(ordersList.getLength() == 0);
    return 0;
}