#include "../include/Action.h"
#include "../include/Volunteer.h"
#include "../include/Customer.h"
#include <iostream>
extern WareHouse *backup;

BaseAction::BaseAction() : errorMsg(""), status(ActionStatus::ERROR) {}

BaseAction::BaseAction(const BaseAction &other) : errorMsg(other.errorMsg), status(other.status) {}

ActionStatus BaseAction::getStatus() const
{
    return status;
}

void BaseAction::complete()
{
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg)
{
    status = ActionStatus::ERROR;
    this->errorMsg = errorMsg;
}

string BaseAction::getErrorMsg() const
{
    return "Error: " + errorMsg;
}

string BaseAction::statusToString() const
{
    if (status == ActionStatus::COMPLETED)
    {
        return "COMPLETED";
    }
    else
    {
        return "ERROR";
    }
}

SimulateStep::SimulateStep(int numOfSteps) : numOfSteps(numOfSteps) {}

SimulateStep::SimulateStep(const SimulateStep &other) : BaseAction(other), numOfSteps(other.numOfSteps) {}

void SimulateStep::act(WareHouse &wareHouse)
{
    for (int i = 0; i < numOfSteps; i++)
    {
        vector<Order *> &pendingOrders = wareHouse.getPendingOrders();
        vector<Order *> &inProcessOrders = wareHouse.getInProcessOrders();
        vector<Order *> &completedOrders = wareHouse.getCompletedOrders();
        vector<Volunteer *> &volunteers = wareHouse.getVolunteers();

        // Stage 1: assign pending orders to volunteers
        for (auto it = pendingOrders.begin(); it != pendingOrders.end();)
        {
            Order *order = *it;
            if (order->getStatus() == OrderStatus::PENDING)
            {
                bool orderAssigned = false; // Track if the order is assigned to any volunteer
                for (Volunteer *v : volunteers)
                {
                    if ((v->getVolunteerType() == "Collector" || v->getVolunteerType() == "LimitedCollector") &&
                        v->canTakeOrder(*order))
                    {
                        v->acceptOrder(*order);
                        order->setStatus(OrderStatus::COLLECTING);
                        order->setCollectorId(v->getId());
                        it = pendingOrders.erase(it); // erase returns the iterator to the next element
                        inProcessOrders.push_back(order);
                        orderAssigned = true;
                        break;
                    }
                }

                if (!orderAssigned)
                {
                    // The order was not assigned to any collector, so move on to the next order
                    ++it;
                }
            }
            else if (order->getStatus() == OrderStatus::COLLECTING)
            {
                bool orderAssigned = false; // Track if the order is assigned to any driver
                for (Volunteer *v : volunteers)
                {
                    if ((v->getVolunteerType() == "Driver" || v->getVolunteerType() == "LimitedDriver") &&
                        v->canTakeOrder(*order))
                    {
                        v->acceptOrder(*order);
                        order->setStatus(OrderStatus::DELIVERING);
                        order->setDriverId(v->getId());
                        it = pendingOrders.erase(it); // erase returns the iterator to the next element
                        inProcessOrders.push_back(order);
                        orderAssigned = true;
                        break;
                    }
                }

                if (!orderAssigned)
                {
                    // The order was not assigned to any driver, so move on to the next order
                    ++it;
                }
            }
        }

        // Stage 2 : make 1 step for all busy volunteers
        for (Volunteer *v : volunteers)
        {
            if (v->isBusy())
            {
                Order &order = wareHouse.getOrder(v->getActiveOrderId());
                v->step();
                if (v->isBusy() == false)
                { // volunteer finished his task
                    if (v->hasOrdersLeft() == false)
                    { // volunteer reached his max orders
                        wareHouse.removeVolunteer(v->getId());
                    }
                    if (order.getStatus() == OrderStatus::COLLECTING) // move to pendingList
                    {
                        pendingOrders.push_back(&order);
                    }
                    else if (order.getStatus() == OrderStatus::DELIVERING) // move to completedList
                    {
                        order.setStatus(OrderStatus::COMPLETED);
                        completedOrders.push_back(&order);
                    }
                    wareHouse.eraseOrderFromInProcess(order.getId());
                }
            }
        }
    }

    complete();
    wareHouse.addAction(this);
}

std::string SimulateStep::toString() const
{
    return "simulateStep " + std::to_string(numOfSteps) + " " + statusToString();
}

SimulateStep *SimulateStep::clone() const
{
    return new SimulateStep(*this);
}

AddOrder::AddOrder(int id) : customerId(id) {}

AddOrder::AddOrder(const AddOrder &other) : BaseAction(other), customerId(other.customerId) {}

void AddOrder::act(WareHouse &wareHouse)
{
    Customer &cust = wareHouse.getCustomer(customerId);
    if (cust.getId() >= 0 && cust.canMakeOrder())
    {
        int orderCounter = wareHouse.getOrderCounter();
        wareHouse.addOrder(new Order(orderCounter, customerId, cust.getCustomerDistance()));
        cust.addOrder(orderCounter);
        complete();
    }
    else
    {
        error("Cannot place this order");
        std::cout << "Error: Cannot place this order" << std::endl;
    }
    wareHouse.addAction(this);
}

AddOrder *AddOrder::clone() const
{
    return new AddOrder(*this);
}

string AddOrder::toString() const
{
    return "order " + std::to_string(customerId) + " " + statusToString();
}

AddCustomer::AddCustomer(const string &customerName, const string &customerType, int distance, int maxOrders)
    : customerName(customerName), customerType(getCustomerType(customerType)), distance(distance), maxOrders(maxOrders) {}

AddCustomer::AddCustomer(const AddCustomer &other)
    : BaseAction(other), customerName(other.customerName), customerType(other.customerType), distance(other.distance), maxOrders(other.maxOrders) {}

CustomerType AddCustomer::getCustomerType(const string &customerType)
{
    if (customerType == "soldier")
    {
        return CustomerType::Soldier;
    }
    else
    {
        return CustomerType::Civilian;
    }
}

void AddCustomer::act(WareHouse &wareHouse)
{
    if (customerType == CustomerType::Soldier)
    {
        wareHouse.addCustomer(customerName, "soldier", distance, maxOrders);
    }
    else
    {
        wareHouse.addCustomer(customerName, "civilian", distance, maxOrders);
    }
    complete();
    wareHouse.addAction(this);
}

AddCustomer *AddCustomer::clone() const
{
    return new AddCustomer(*this);
}

string AddCustomer::toString() const
{
    return "add customer " + customerName + " " + statusToString();
}

PrintOrderStatus::PrintOrderStatus(int id) : orderId(id) {}

PrintOrderStatus::PrintOrderStatus(const PrintOrderStatus &other) : BaseAction(other), orderId(other.orderId) {}

void PrintOrderStatus::act(WareHouse &wareHouse)
{
    Order &order = wareHouse.getOrder(orderId);
    if (&order != &wareHouse.emptyOrder)
    {
        std::cout << order.toString() << std::endl;
        complete();
    }
    else
    {
        error("Order doesn't exist");
        std::cout << "Error: Order doesn't exist" << std::endl;
    }
    wareHouse.addAction(this);
}

PrintOrderStatus *PrintOrderStatus::clone() const
{
    return new PrintOrderStatus(*this);
}

string PrintOrderStatus::toString() const
{
    return "orderStatus " + std::to_string(orderId) + " " + statusToString();
}

PrintCustomerStatus::PrintCustomerStatus(int customerId) : customerId(customerId) {}

PrintCustomerStatus::PrintCustomerStatus(const PrintCustomerStatus &other) : BaseAction(other), customerId(other.customerId) {}

void PrintCustomerStatus::act(WareHouse &wareHouse)
{
    Customer &customer = wareHouse.getCustomer(customerId);
    if (&customer != &wareHouse.emptyCustomer)
    {
        string str = "CustomerID: " + std::to_string(customer.getId()) + "\n";
        for (int orderId : customer.getOrdersIds())
        {
            Order &order = wareHouse.getOrder(orderId);
            str += "OrderID: " + std::to_string(orderId) + "\n";
            str += "OrderStatus: " + order.getStatusString() + "\n";
        }
        int numOrdersLeft = customer.getMaxOrders() - customer.getNumOrders();
        str += "numOrdersLeft: " + std::to_string(numOrdersLeft) + "\n";
        std::cout << str << std::endl;
        complete();
    }
    else
    {
        error("Customer doesn't exist");
        std::cout << "Error: Customer doesn't exist" << std::endl;
    }
    wareHouse.addAction(this);
}

PrintCustomerStatus *PrintCustomerStatus::clone() const
{
    return new PrintCustomerStatus(*this);
}

string PrintCustomerStatus::toString() const
{
    return "customerStatus " + std::to_string(customerId) + " " + statusToString();
}

PrintVolunteerStatus::PrintVolunteerStatus(int id) : volunteerId(id) {}

PrintVolunteerStatus::PrintVolunteerStatus(const PrintVolunteerStatus &other) : BaseAction(other), volunteerId(other.volunteerId) {}

void PrintVolunteerStatus::act(WareHouse &wareHouse)
{
    Volunteer &volunteer = wareHouse.getVolunteer(volunteerId);
    if (&volunteer != &wareHouse.emptyVolunteer)
    {
        std::cout << volunteer.toString() << std::endl;
        complete();
    }
    else
    {
        error("Volunteer doesn't exist");
        std::cout << "Error: Volunteer doesn't exist" << std::endl;
    }
    wareHouse.addAction(this);
}

PrintVolunteerStatus *PrintVolunteerStatus::clone() const
{
    return new PrintVolunteerStatus(*this);
}

string PrintVolunteerStatus::toString() const
{
    return "volunteerStatus " + std::to_string(volunteerId) + " " + statusToString();
}

PrintActionsLog::PrintActionsLog() {}

PrintActionsLog::PrintActionsLog(const PrintActionsLog &other) : BaseAction(other) {}

void PrintActionsLog::act(WareHouse &wareHouse)
{
    std::vector<BaseAction *> actions = wareHouse.getActions();
    for (BaseAction *action : actions)
    {
        std::cout << action->toString() << std::endl;
    }
    complete();
    wareHouse.addAction(this);
}

PrintActionsLog *PrintActionsLog::clone() const
{
    return new PrintActionsLog(*this);
}

string PrintActionsLog::toString() const
{
    return "log " + statusToString();
}

Close::Close() {}

Close::Close(const Close &other) : BaseAction(other) {}

void Close::act(WareHouse &wareHouse)
{
    wareHouse.printAllOrders();
    wareHouse.addAction(this);
    wareHouse.clearWareHouse(); // Free all memory
    wareHouse.setOpenStatus(false);
}

Close *Close::clone() const
{
    return new Close(*this);
}

string Close::toString() const
{
    return "close";
}

BackupWareHouse::BackupWareHouse() {}

BackupWareHouse::BackupWareHouse(const BackupWareHouse &other) : BaseAction(other) {}

void BackupWareHouse::act(WareHouse &wareHouse)
{
    // Create a new WareHouse object and copy the state of wareHouse into it
    if (backup != nullptr)
    {
        delete backup;
        backup = nullptr;
    }

    backup = new WareHouse(wareHouse);
    complete();
    wareHouse.addAction(this);
}

BackupWareHouse *BackupWareHouse::clone() const
{
    return new BackupWareHouse(*this);
}

string BackupWareHouse::toString() const
{
    return "backup " + statusToString();
}

RestoreWareHouse::RestoreWareHouse() {}

RestoreWareHouse::RestoreWareHouse(const RestoreWareHouse &other) : BaseAction(other) {}

void RestoreWareHouse::act(WareHouse &wareHouse)
{
    if (backup != nullptr)
    {
        // Copy the state of backup into wareHouse
        wareHouse = *backup;
        complete();
    }
    else
    {
        error("No backup available");
        std::cout << "Error: No backup available" << std::endl;
    }
    wareHouse.addAction(this);
}

RestoreWareHouse *RestoreWareHouse::clone() const
{
    return new RestoreWareHouse(*this);
}

string RestoreWareHouse::toString() const
{
    return "restore " + statusToString();
};
