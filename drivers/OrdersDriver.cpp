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
    Deploy deploy;
    Advance advance;
    Advance advOther = advance;

    ordersList.addOrder(&deploy);
    ordersList.addOrder(&advance);

    OrdersList ordersList2 = OrdersList(ordersList);
    assert(ordersList.getLength() == 2);
    ordersList.remove(&deploy);
    assert(ordersList.getLength() == 1);
    ordersList.remove(&advance);
    assert(ordersList.getLength() == 0);
    return 0;
}