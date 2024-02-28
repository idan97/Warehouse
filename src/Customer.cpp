#include <string>
#include <vector>
#include "../include/Customer.h"
using std::string;
using std::vector;

Customer::Customer(int id, const string &name, int locationDistance, int maxOrders)
    : id(id), name(name), locationDistance(locationDistance), maxOrders(maxOrders) ,ordersId() {}

Customer::Customer(const Customer &other)
    : id(other.getId()), name(other.getName()), locationDistance(other.getCustomerDistance()), maxOrders(other.getMaxOrders()), ordersId(other.getOrdersIds()) {}

const string &Customer::getName() const
{
    return name;
}

int Customer::getId() const
{
    return id;
}

int Customer::getCustomerDistance() const
{
    return locationDistance;
}

int Customer::getMaxOrders() const
{
    return maxOrders;
}

int Customer::getNumOrders() const
{
    return ordersId.size();
}

bool Customer::canMakeOrder() const
{
    return getNumOrders() < maxOrders;
}

const vector<int> &Customer::getOrdersIds() const
{
    return ordersId;
}

int Customer::addOrder(int orderId)
{
    if (canMakeOrder())
    {
        ordersId.push_back(orderId);
        return orderId;
    }
    return -1;
}

SoldierCustomer::SoldierCustomer(int id, const string &name, int locationDistance, int maxOrders)
    : Customer(id, name, locationDistance, maxOrders) {}

SoldierCustomer::SoldierCustomer(const SoldierCustomer &other)
    : Customer(other) {}

SoldierCustomer *SoldierCustomer::clone() const
{
    return new SoldierCustomer(*this);
}

CivilianCustomer::CivilianCustomer(int id, const string &name, int locationDistance, int maxOrders)
    : Customer(id, name, locationDistance, maxOrders) {}

CivilianCustomer::CivilianCustomer(const CivilianCustomer &other)
    : Customer(other) {}

CivilianCustomer *CivilianCustomer::clone() const
{
    return new CivilianCustomer(*this);
}
