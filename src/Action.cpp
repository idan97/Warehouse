#include "../include/Action.h"
#include "../include/Volunteer.h"
#include "../include/Customer.h"
#include <iostream>
extern WareHouse *backup;

BaseAction::BaseAction() : status(ActionStatus::ERROR), errorMsg("") {}

BaseAction::BaseAction(const BaseAction &other) : status(other.status), errorMsg(other.errorMsg) {}

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

SimulateStep::SimulateStep(const SimulateStep &other) : numOfSteps(other.numOfSteps), BaseAction(other) {}

void SimulateStep::act(WareHouse &wareHouse)
{
    // Stage 1: Go through pendingOrders and hand them over to the next operation
    for (auto &order : wareHouse.getPendingOrders())
    {
        if (order.getStatus() == OrderStatus::PENDING)
        {
            // Assign orders to collectors and drivers based on their status and availability
            Volunteer *volunteer = nullptr;
            for (Volunteer *v : wareHouse.getVolunteers())
            {
                if (v->getStatus() == VolunteerStatus::AVAILABLE)
                {
                    if (v->getType() == VolunteerType::COLLECTOR)
                    {
                        volunteer = v;
                        break;
                    }
                    else if (v->getType() == VolunteerType::DRIVER)
                    {
                        if (volunteer == nullptr)
                        {
                            volunteer = v;
                        }
                        else if (volunteer->getType() == VolunteerType::COLLECTOR)
                        {
                            volunteer = v;
                            break;
                        }
                    }
                }
            }

            // Your logic to associate orders with volunteers goes here
            // Use wareHouse.getCollectorVolunteer(), wareHouse.getDriverVolunteer(), etc.

            if (volunteer != nullptr)
            {
                order.setStatus(OrderStatus::COLLECTING); // Update order status
                volunteer->acceptOrder(*order);           // Assign the order to the volunteer
                wareHouse.moveOrderToInProcess(*order);   // Move order to inProcessOrders
            }
        }
    }

    // Stage 2: Perform a step in the simulation
    for (Volunteer *volunteer : wareHouse.getVolunteers())
    {
        volunteer->step(); // Decrease timeLeft for collectors
    }

    // Stage 3: Check if volunteers have finished their job
    wareHouse.checkVolunteersAndPushOrders();

    // Stage 4: Delete volunteers that reached maxOrders limit
    wareHouse.deleteVolunteersWithMaxOrders();

    complete();
}

std::string SimulateStep::toString() const
{
    return "simulateStep " + std::to_string(numOfSteps);
}

SimulateStep *SimulateStep::clone() const
{
    return new SimulateStep(*this);
}

AddOrder::AddOrder(int id) : customerId(id) {}

AddOrder::AddOrder(const AddOrder &other) : customerId(other.customerId), BaseAction(other) {}

void AddOrder::act(WareHouse &wareHouse)
{
    Customer &cust = wareHouse.getCustomer(customerId);
    if (cust.getId() >= 0 && cust.canMakeOrder())
    {
        int orderCounter = wareHouse.getOrderCounter();
        wareHouse.addOrder(new Order(orderCounter, customerId, cust.getCustomerDistance()););
        cust.addOrder(orderCounter);
        complete();
    }
    else
    {
        error("Cannot place this order");
        std::cout << "Cannot place this order" << std::endl;
    }
    wareHouse.addAction(this);
}

AddOrder *AddOrder::clone() const
{
    return new AddOrder(*this);
}

string AddOrder::toString() const
{
    return "order " + std::to_string(customerId);
}

AddCustomer::AddCustomer(const string &customerName, const string &customerType, int distance, int maxOrders)
    : customerName(customerName), distance(distance), maxOrders(maxOrders), customerType(getCustomerType(customerType))
{
}

AddCustomer::AddCustomer(const AddCustomer &other)
    : customerName(other.customerName), distance(other.distance), maxOrders(other.maxOrders),
      customerType(other.customerType), BaseAction(other) {}

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
    wareHouse.addCustomer(customerName, customerType, distance, maxOrders);
    complete();
    warehouse.addAction(this);
}

AddCustomer *AddCustomer::clone() const
{
    return new AddCustomer(*this);
}

string AddCustomer::toString() const
{
    return "add customer " + customerName;
}

PrintOrderStatus::PrintOrderStatus(int id) : orderId(id) {}

PrintOrderStatus::PrintOrderStatus(const PrintOrderStatus &other) : orderId(other.orderId), BaseAction(other) {}

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
        std::cout << "Order doesn't exist" << std::endl;
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

PrintCustomerStatus::PrintCustomerStatus(const PrintCustomerStatus &other) : customerId(other.customerId), BaseAction(other) {}

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
        std::cout << "Customer doesn't exist" << std::endl;
    }
    wareHouse.addAction(this);
}

PrintCustomerStatus *PrintCustomerStatus::clone() const
{
    return new PrintCustomerStatus(*this);
}

string PrintCustomerStatus::toString() const
{
    return "customerStatus " + std::to_string(customerId);
}

PrintVolunteerStatus::PrintVolunteerStatus(int id) : volunteerId(id) {}

PrintVolunteerStatus::PrintVolunteerStatus(const PrintVolunteerStatus &other) : volunteerId(other.volunteerId), BaseAction(other) {}

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
        std::cout << "Volunteer doesn't exist" << std::endl;
    }
    wareHouse.addAction(this);
}

PrintVolunteerStatus *PrintVolunteerStatus::clone() const
{
    return new PrintVolunteerStatus(*this);
}

string PrintVolunteerStatus::toString() const
{
    return "volunteerStatus " + std::to_string(volunteerId);
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
    return "log" + statusToString();
}

Close::Close() {}

Close::Close(const Close &other) : BaseAction(other) {}

void Close::act(WareHouse &wareHouse)
{
    wareHouse.printAndDeleteAllOrders();
    wareHouse.setOpenStatus(false);
    wareHouse.clearVolunteersAndCustomers(); // Free all memory
    complete();
    // exit(0); // Finish the program
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
    return "backup";
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
        std::cout << "No backup available" << std::endl;
    }
    wareHouse.addAction(this);
}

RestoreWareHouse *RestoreWareHouse::clone() const
{
    return new RestoreWareHouse(*this);
}

string RestoreWareHouse::toString() const
{
    return "restore";
};
