#include "../include/Action.h"
#include <iostream>
#include "Action.h"

BaseAction::BaseAction() : status(ActionStatus::ERROR) {}

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
    //std::cout << "Error: " << errorMsg << std::endl;
}

string BaseAction::getErrorMsg() const
{
    return errorMsg;
}

SimulateStep::SimulateStep(int numOfSteps) : numOfSteps(numOfSteps) {}

void SimulateStep::act(WareHouse &wareHouse)
{
    for (int i = 0; i < numOfSteps; ++i)
    {
        wareHouse.simulateStep();
    }
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

void AddOrder::act(WareHouse &wareHouse)
{
    wareHouse.addOrderByCustomer(customerId);
    complete();
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
}

AddCustomer *AddCustomer::clone() const
{
    return new AddCustomer(*this);
}

string AddCustomer::toString() const
{
    string customerTypeStr;
    if (customerType == CustomerType::Soldier)
    {
        customerTypeStr = "soldier";
    }
    else
    {
        customerTypeStr = "civilian";
    }
    return "customer " + customerName + " " + customerTypeStr + " " + std::to_string(distance) + " " + std::to_string(maxOrders);
}

PrintOrderStatus::PrintOrderStatus(int id) : orderId(id) {}

void PrintOrderStatus::act(WareHouse &wareHouse)
{
    wareHouse.printOrderStatus(orderId);
    complete();
}

PrintOrderStatus *PrintOrderStatus::clone() const
{
    return new PrintOrderStatus(*this);
}

string PrintOrderStatus::toString() const
{
    return "orderStatus " + std::to_string(orderId);
}

PrintCustomerStatus::PrintCustomerStatus(int customerId) : customerId(customerId) {}

void PrintCustomerStatus::act(WareHouse &wareHouse)
{
    wareHouse.printCustomerStatus(customerId);
    complete();
}

PrintCustomerStatus *PrintCustomerStatus::clone() const
{
    return new PrintCustomerStatus(*this);
}

string PrintCustomerStatus::toString() const
{
    return "customerStatus " + std::to_string(customerId);
}

PrintVolunteerStatus::PrintVolunteerStatus(int id) : VolunteerId(id) {}

void PrintVolunteerStatus::act(WareHouse &wareHouse)
{
    wareHouse.printVolunteerStatus(VolunteerId);
    complete();
}

PrintVolunteerStatus *PrintVolunteerStatus::clone() const
{
    return new PrintVolunteerStatus(*this);
}

string PrintVolunteerStatus::toString() const
{
    return "volunteerStatus " + std::to_string(VolunteerId);
}

PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(WareHouse &wareHouse)
{
    wareHouse.printActionsLogs();
    complete();
}

PrintActionsLog *PrintActionsLog::clone() const
{
    return new PrintActionsLog(*this);
}

string PrintActionsLog::toString() const
{
    return "log";
}

Close::Close() {}

void Close::act(WareHouse &wareHouse)
{
    wareHouse.close();
    complete();
}

string Close::toString() const
{
    return "close";
}

BackupWareHouse::BackupWareHouse() {}

void BackupWareHouse::act(WareHouse &wareHouse)
{
    wareHouse.backupWarehouse();
    complete();
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

void RestoreWareHouse::act(WareHouse &wareHouse)
{
    wareHouse.restoreWarehouse();
    complete();
}

RestoreWareHouse *RestoreWareHouse::clone() const
{
    return new RestoreWareHouse(*this);
}

string RestoreWareHouse::toString() const
{
    return "restore";
}
