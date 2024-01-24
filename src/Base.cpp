#include "Base.h"

BaseAction::BaseAction() : status(ActionStatus::COMPLETED) {}

ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::complete() {
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg) {
    status = ActionStatus::ERROR;
    this->errorMsg = errorMsg;
}

string BaseAction::getErrorMsg() const {
    return errorMsg;
}

SimulateStep::SimulateStep(int numOfSteps) : numOfSteps(numOfSteps) {}

void SimulateStep::act(WareHouse &wareHouse) {
    // TODO: Implement act() for SimulateStep
}

string SimulateStep::toString() const {
    // TODO: Implement toString() for SimulateStep
    return "";
}

SimulateStep *SimulateStep::clone() const {
    // TODO: Implement clone() for SimulateStep
    return nullptr;
}

Order::Order(int id) : customerId(id) {}

void Order::act(WareHouse &wareHouse) {
    // TODO: Implement act() for Order
}

Order *Order::clone() const {
    // TODO: Implement clone() for Order
    return nullptr;
}

string Order::toString() const {
    // TODO: Implement toString() for Order
    return "";
}

AddCustomer::AddCustomer(string customerName, string customerType, int distance, int maxOrders)
    : customerName(customerName), distance(distance), maxOrders(maxOrders) {
    if (customerType == "Soldier") {
        this->customerType = CustomerType::Soldier;
    } else {
        this->customerType = CustomerType::Civilian;
    }
}

void AddCustomer::act(WareHouse &wareHouse) {
    // TODO: Implement act() for AddCustomer
}

AddCustomer *AddCustomer::clone() const {
    // TODO: Implement clone() for AddCustomer
    return nullptr;
}

string AddCustomer::toString() const {
    // TODO: Implement toString() for AddCustomer
    return "";
}

PrintOrderStatus::PrintOrderStatus(int id) : orderId(id) {}

void PrintOrderStatus::act(WareHouse &wareHouse) {
    // TODO: Implement act() for PrintOrderStatus
}

PrintOrderStatus *PrintOrderStatus::clone() const {
    // TODO: Implement clone() for PrintOrderStatus
    return nullptr;
}

string PrintOrderStatus::toString() const {
    // TODO: Implement toString() for PrintOrderStatus
    return "";
}

PrintCustomerStatus::PrintCustomerStatus(int customerId) : customerId(customerId) {}

void PrintCustomerStatus::act(WareHouse &wareHouse) {
    // TODO: Implement act() for PrintCustomerStatus
}

PrintCustomerStatus *PrintCustomerStatus::clone() const {
    // TODO: Implement clone() for PrintCustomerStatus
    return nullptr;
}

string PrintCustomerStatus::toString() const {
    // TODO: Implement toString() for PrintCustomerStatus
    return "";
}

PrintVolunteerStatus::PrintVolunteerStatus(int id) : VolunteerId(id) {}

void PrintVolunteerStatus::act(WareHouse &wareHouse) {
    // TODO: Implement act() for PrintVolunteerStatus
}

PrintVolunteerStatus *PrintVolunteerStatus::clone() const {
    // TODO: Implement clone() for PrintVolunteerStatus
    return nullptr;
}

string PrintVolunteerStatus::toString() const {
    // TODO: Implement toString() for PrintVolunteerStatus
    return "";
}

PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(WareHouse &wareHouse) {
    // TODO: Implement act() for PrintActionsLog
}

PrintActionsLog *PrintActionsLog::clone() const {
    // TODO: Implement clone() for PrintActionsLog
    return nullptr;
}

string PrintActionsLog::toString() const {
    // TODO: Implement toString() for PrintActionsLog
    return "";
}

Close::Close() {}

void Close::act(WareHouse &wareHouse) {
    // TODO: Implement act() for Close
}

string Close::toString() const {
    // TODO: Implement toString() for Close
    return "";
}

BackupWareHouse::BackupWareHouse() {}

void BackupWareHouse::act(WareHouse &wareHouse) {
    // TODO: Implement act() for BackupWareHouse
}

BackupWareHouse *BackupWareHouse::clone() const {
    // TODO: Implement clone() for BackupWareHouse
    return nullptr;
}

string BackupWareHouse::toString() const {
    // TODO: Implement toString() for BackupWareHouse
    return "";
}

RestoreWareHouse::RestoreWareHouse() {}

void RestoreWareHouse::act(WareHouse &wareHouse) {
    // TODO: Implement act() for RestoreWareHouse
}

RestoreWareHouse *RestoreWareHouse::clone() const {
    // TODO: Implement clone() for RestoreWareHouse
    return nullptr;
}

string RestoreWareHouse::toString() const {
    // TODO: Implement toString() for RestoreWareHouse
    return "";
}
