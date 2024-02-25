#include "../include/Order.h"

Order::Order(int id, int customerId, int distance)
    : id(id), customerId(customerId), distance(distance), status(OrderStatus::PENDING), collectorId(NO_VOLUNTEER), driverId(NO_VOLUNTEER) {}

int Order::getId() const
{
    return id;
}

int Order::getCustomerId() const
{
    return customerId;
}

void Order::setStatus(OrderStatus status)
{
    this->status = status;
}

void Order::setCollectorId(int collectorId)
{
    this->collectorId = collectorId;
}

void Order::setDriverId(int driverId)
{
    this->driverId = driverId;
}

int Order::getCollectorId() const
{
    return collectorId;
}

int Order::getDriverId() const
{
    return driverId;
}

OrderStatus Order::getStatus() const
{
    return status;
}
int Order::getDistance() const
{
    return distance;
}

const string Order::getStatusString() const
{
    string statusStr;
    switch (status)
    {
    case OrderStatus::PENDING:
        statusStr = "Pending";
        break;
    case OrderStatus::COLLECTING:
        statusStr = "Collecting";
        break;
    case OrderStatus::DELIVERING:
        statusStr = "Delivering";
        break;
    case OrderStatus::COMPLETED:
        statusStr = "Completed";
        break;
    }
    return statusStr;
}

const string Order::toString() const
{
    string statusStr = getStatusString();
    string collectorStr = (collectorId != NO_VOLUNTEER) ? std::to_string(collectorId) : "None";
    string driverStr = (driverId != NO_VOLUNTEER) ? std::to_string(driverId) : "None";

    return "Order id: " + std::to_string(id) + "\n" +
           "OrderStatus: " + statusStr + "\n" +
           "Customer id: " + std::to_string(customerId) + "\n" +
           "Collector: " + collectorStr + "\n" +
           "Driver: " + driverStr + "\n";
}

Order *Order::clone() const
{
    return new Order(*this);
}