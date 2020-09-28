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

    assert(&advOther != &advance);

    ordersList.orders->push_back(&deploy);
    ordersList.orders->push_back(&advance);
    OrdersList ordersList2 = OrdersList(ordersList);
    assert(ordersList.orders->size() == 2);
    ordersList.remove(&deploy);
    assert(ordersList.orders->size() == 1);
    ordersList.remove(&advance);
    assert(ordersList.orders->size() == 0);
    return 0;
}