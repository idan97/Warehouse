#include <fstream>
#include <sstream>
#include "../include/WareHouse.h"
#include "../include/Customer.h"
#include "../include/Volunteer.h"
#include "../include/Order.h"
#include "../include/Customer.h"
#include "../include/Action.h"
#include <iostream>
extern WareHouse *backup;

Order WareHouse::emptyOrder(-1, -1, -1);
SoldierCustomer WareHouse::emptyCustomer(-1, "", -1, -1);
CollectorVolunteer WareHouse::emptyVolunteer(-1, "", -1);

WareHouse::WareHouse(const string &configFilePath) : isOpen(true), customerCounter(0), volunteerCounter(0), orderCounter(0)
{
    ifstream configFile(configFilePath);
    string line;

    while (getline(configFile, line))
    {
        istringstream iss(line);
        string type, name, specificType;
        iss >> type >> name >> specificType;

        if (type == "customer")
        {
            int distance, maxOrders;
            iss >> distance >> maxOrders;
            if (specificType == "soldier")
                customers.push_back(new SoldierCustomer(customerCounter, name, distance, maxOrders));
            else if (specificType == "civilian")
                customers.push_back(new CivilianCustomer(customerCounter, name, distance, maxOrders));
            customerCounter++;
        }
        else if (type == "volunteer")
        {
            if (specificType == "collector")
            {
                int cooldown;
                iss >> cooldown;
                volunteers.push_back(new CollectorVolunteer(volunteerCounter, name, cooldown));
            }
            else if (specificType == "limited_collector")
            {
                int cooldown, maxOrders;
                iss >> cooldown >> maxOrders;
                volunteers.push_back(new LimitedCollectorVolunteer(volunteerCounter, name, cooldown, maxOrders));
            }
            else if (specificType == "driver")
            {
                int maxDistance, distancePerStep;
                iss >> maxDistance >> distancePerStep;
                volunteers.push_back(new DriverVolunteer(volunteerCounter, name, maxDistance, distancePerStep));
            }
            else if (specificType == "limited_driver")
            {
                int maxDistance, distancePerStep, maxOrders;
                iss >> maxDistance >> distancePerStep >> maxOrders;
                volunteers.push_back(new LimitedDriverVolunteer(volunteerCounter, name, maxDistance, distancePerStep, maxOrders));
            }
            volunteerCounter++;
        }
    }
}

WareHouse::WareHouse(const WareHouse &other) : isOpen(other.isOpen), customerCounter(other.customerCounter), volunteerCounter(other.volunteerCounter), orderCounter(other.orderCounter)
{
    for (auto &customer : other.customers)
    {
        customers.push_back(customer->clone());
    }
    for (auto &volunteer : other.volunteers)
    {
        volunteers.push_back(volunteer->clone());
    }
    for (auto &order : other.pendingOrders)
    {
        pendingOrders.push_back(order->clone());
    }
    for (auto &order : other.inProcessOrders)
    {
        inProcessOrders.push_back(order->clone());
    }
    for (auto &order : other.completedOrders)
    {
        completedOrders.push_back(order->clone());
    }
    for (auto &action : other.actionsLog)
    {
        actionsLog.push_back(action->clone());
    }
}

void WareHouse::start()
{
    if (isOpen)
    {
        cout << "Warehouse is open" << endl;
    }
    else
    {
        cout << "Warehouse is closed" << endl;
    }
}

const vector<BaseAction *> &WareHouse::getActions() const
{
    return actionsLog;
}

void WareHouse::addOrder(Order *order)
{
    pendingOrders.push_back(order);
    orderCounter++;
}

void WareHouse::addAction(BaseAction *action)
{
    actionsLog.push_back(action);
}

void WareHouse::addCustomer(string customerName, CustomerType customerType, int distance, int maxOrders)
{
    if (customerType == CustomerType::Soldier)
    {
        customers.push_back(new SoldierCustomer(customerCounter, customerName, distance, maxOrders));
    }
    else
    {
        customers.push_back(new CivilianCustomer(customerCounter, customerName, distance, maxOrders));
    }
    customerCounter++;
}

Customer &WareHouse::getCustomer(int customerId) const
{
    for (const auto &customer : customers)
    {
        if (customer->getId() == customerId)
        {
            return *customer;
        }
    }
    // Return a default customer if not found
    return *customers[0];
}

Volunteer &WareHouse::getVolunteer(int volunteerId) const
{
    for (const auto &volunteer : volunteers)
    {
        if (volunteer->getId() == volunteerId)
        {
            return *volunteer;
        }
    }
    // Return a default volunteer if not found
    return *volunteers[0];
}

Order &WareHouse::getOrder(int orderId) const
{
    for (const auto &order : pendingOrders)
    {
        if (order->getId() == orderId)
        {
            return *order;
        }
    }
    for (const auto &order : inProcessOrders)
    {
        if (order->getId() == orderId)
        {
            return *order;
        }
    }
    for (const auto &order : completedOrders)
    {
        if (order->getId() == orderId)
        {
            return *order;
        }
    }
    return emptyOrder;
}

void WareHouse::close()
{
    isOpen = false;
}

void WareHouse::open()
{
    isOpen = true;
}

int WareHouse::getOrderCounter() const
{
    return orderCounter;
}
int WareHouse::getCustomerCounter() const
{
    return customerCounter;
}
int WareHouse::getVolunteerCounter() const
{
    return volunteerCounter;
}
void WareHouse::setOpenStatus(bool status)
{
    isOpen = status;
}

void WareHouse::printAndDeleteAllOrders()
{
    for (auto &order : pendingOrders)
    {
        std::cout << buildOrderInfo(*order) << std::endl;
        delete order;
    }
    for (auto &order : inProcessOrders)
    {
        std::cout << buildOrderInfo(*order) << std::endl;
        delete order;
    }
    for (auto &order : completedOrders)
    {
std::cout << buildOrderInfo(*order) << std::endl;
        delete order;
    }
    pendingOrders.clear();
    inProcessOrders.clear();
    completedOrders.clear();
}

string WareHouse::buildOrderInfo(const Order &order) const
{
    string orderInfo = "OrderID: " + std::to_string(order.getId()) +
                       " , CustomerID: " + std::to_string(order.getCustomerId()) +
                       ", OrderStatus: " + order.getStatusString();
    return orderInfo;
}

void WareHouse::clearVolunteersAndCustomers()
{
    for (auto &volunteer : volunteers)
    {
        delete volunteer;
    }
    for (auto &customer : customers)
    {
        delete customer;
    }
    volunteers.clear();
    customers.clear();
}

vector<Order *> WareHouse::getPendingOrders() const
{
    return pendingOrders;
}   
vector<Order *> WareHouse::getInProcessOrders() const
{
    return inProcessOrders;
}   
vector<Volunteer *> WareHouse::getVolunteers() const
{
    return volunteers;
}