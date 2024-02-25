#include "../include/WareHouse.h"
#include "../include/Customer.h"
#include "../include/Volunteer.h"
#include "../include/Order.h"
#include "../include/Customer.h"
#include "../include/Action.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

extern WareHouse *backup;

Order WareHouse::emptyOrder(-1, -1, -1);
SoldierCustomer WareHouse::emptyCustomer(-1, "", -1, -1);
CollectorVolunteer WareHouse::emptyVolunteer(-1, "", -1);

WareHouse::WareHouse(const string &configFilePath) // constructor
    : isOpen(true), customerCounter(0), volunteerCounter(0), orderCounter(0), configFilePath(configFilePath)
{
    readFirstInput();
}

WareHouse::WareHouse(const WareHouse &other) // copy constructor
    : isOpen(other.isOpen), customerCounter(other.customerCounter), volunteerCounter(other.volunteerCounter), orderCounter(other.orderCounter)
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

WareHouse::WareHouse(WareHouse &&other) : // move constructor
                                          isOpen(other.isOpen),
                                          actionsLog(std::move(other.actionsLog)),
                                          volunteers(std::move(other.volunteers)),
                                          pendingOrders(std::move(other.pendingOrders)),
                                          inProcessOrders(std::move(other.inProcessOrders)),
                                          completedOrders(std::move(other.completedOrders)),
                                          customers(std::move(other.customers)),
                                          configFilePath(other.configFilePath),
                                          customerCounter(other.customerCounter),
                                          volunteerCounter(other.volunteerCounter),
                                          orderCounter(other.orderCounter)
{
}

WareHouse &WareHouse::operator=(const WareHouse &other) // copy assignment operator
{
    if (this != &other)
    {
        isOpen = other.isOpen;
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        orderCounter = other.orderCounter;
        clearWareHouse();
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
    return *this;
}

WareHouse &WareHouse::operator=(WareHouse &&other) // move assignment operator
{
    if (this != &other)
    {
        clearWareHouse();
        isOpen = other.isOpen;
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        orderCounter = other.orderCounter;
        actionsLog = std::move(other.actionsLog);
        volunteers = std::move(other.volunteers);
        pendingOrders = std::move(other.pendingOrders);
        inProcessOrders = std::move(other.inProcessOrders);
        completedOrders = std::move(other.completedOrders);
        customers = std::move(other.customers);
    }
    return *this;
}

WareHouse::~WareHouse() // destructor
{
    clearWareHouse();
}

void WareHouse::start()
{
    open();
    std::cout << "Warehouse is open!" << std::endl;
    // readInput();?
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

void WareHouse::printAllOrders()
{
    for (auto &order : pendingOrders)
    {
        cout << buildOrderInfo(*order) << endl;
    }
    for (auto &order : inProcessOrders)
    {
        cout << buildOrderInfo(*order) << endl;
    }
    for (auto &order : completedOrders)
    {
        cout << buildOrderInfo(*order) << endl;
    }
}

string WareHouse::buildOrderInfo(const Order &order) const
{
    string orderInfo = "OrderID: " + std::to_string(order.getId()) +
                       " , CustomerID: " + std::to_string(order.getCustomerId()) +
                       ", OrderStatus: " + order.getStatusString();
    return orderInfo;
}

void WareHouse::clearWareHouse()
{
    for (auto &customer : customers)
    {
        delete customer;
    }
    for (auto &volunteer : volunteers)
    {
        delete volunteer;
    }
    for (auto &order : pendingOrders)
    {
        delete order;
    }
    for (auto &order : inProcessOrders)
    {
        delete order;
    }
    for (auto &order : completedOrders)
    {
        delete order;
    }
    for (auto &action : actionsLog)
    {
        delete action;
    }
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

void WareHouse::readFirstInput()
{
    string line;
    ifstream inputFile(configFilePath);

    if (inputFile.is_open())
    {
        string line;
        while (getline(inputFile, line))
        {
            istringstream iss(line);
            vector<string> words;
            string word;
            while (iss >> word)
            {
                words.push_back(word);
            }

            if (words.size() > 5 && words[0] != "#")
            {
                string personType = words[0];
                string name = words[1];
                string subType = words[2];
                if (personType == "customer")
                {
                    int distance = stoi(words[3]);
                    int maxOrders = stoi(words[4]);
                    if (subType == "civilian")
                    {
                        customers.push_back(new CivilianCustomer(customerCounter, name, distance, maxOrders));
                    }
                    else
                    {
                        customers.push_back(new SoldierCustomer(customerCounter, name, distance, maxOrders));
                    }
                    customerCounter++;
                }
                else if (personType == "volunteer")
                {
                    if (subType.find("collector") != std::string::npos)
                    {
                        int coolDown = stoi(words[3]);
                        if (subType == "limited_collector")
                        {
                            int maxOrders = stoi(words[4]);
                            LimitedCollectorVolunteer *limitedCollector = new LimitedCollectorVolunteer(volunteerCounter, name, coolDown, maxOrders);
                            volunteers.push_back(limitedCollector);
                        }
                        else
                        {
                            CollectorVolunteer *collector = new CollectorVolunteer(volunteerCounter, name, coolDown);
                            volunteers.push_back(collector);
                        }
                    }
                    else if (subType.find("driver") != std::string::npos)
                    {
                        int maxDistance = stoi(words[3]);
                        int distancePerStep = stoi(words[4]);
                        if (subType == "limited_driver")
                        {
                            int maxOrders = stoi(words[5]);
                            LimitedDriverVolunteer *limitedDriver = new LimitedDriverVolunteer(volunteerCounter, name, maxDistance, distancePerStep, maxOrders);
                            volunteers.push_back(limitedDriver);
                        }
                        else
                        {
                            DriverVolunteer *driver = new DriverVolunteer(volunteerCounter, name, maxDistance, distancePerStep);
                            volunteers.push_back(driver);
                        }
                    }
                    volunteerCounter++;
                }
            }
        }
        inputFile.close();
    }
}

void WareHouse::processUserInput()
{
    string input;
    while (isOpen)
    {
        getline(cin, input);
        istringstream iss(input);

        vector<string> words;
        string word;

        while (iss >> word)
        {
            words.push_back(word);
        }
        string actionType = words[0];

        if (actionType == "customer" && words.size() >= 5)
        {
            string customerName = words[1];
            string customerType = words[2];
            int distance = stoi(words[3]);
            int maxOrders = stoi(words[4]);

            AddCustomer *addCust = new AddCustomer(customerName, customerType, distance, maxOrders);
            addCust->act(*this);
        }
        else if (actionType == "step" && words.size() >= 2)
        {
            int numOfStep = stoi(words[1]);
            SimulateStep *addStep = new SimulateStep(numOfStep);
            addStep->act(*this);
        }
        else if (actionType == "order" && words.size() >= 2)
        {
            int customerId = stoi(words[1]);
            AddOrder *addOrder = new AddOrder(customerId);
            addOrder->act(*this);
        }
        else if (actionType == "orderStatus" && words.size() >= 2)
        {
            int orderId = stoi(words[1]);
            PrintOrderStatus *orderStatus = new PrintOrderStatus(orderId);
            orderStatus->act(*this);
        }
        else if (actionType == "customerStatus" && words.size() >= 2)
        {
            int customerId = stoi(words[1]);
            PrintCustomerStatus *customerStatus = new PrintCustomerStatus(customerId);
            customerStatus->act(*this);
        }
        else if (actionType == "volunteerStatus" && words.size() >= 2)
        {
            int volunteerId = stoi(words[1]);
            PrintVolunteerStatus *volunteerStatus = new PrintVolunteerStatus(volunteerId);
            volunteerStatus->act(*this);
        }
        else if (actionType == "log")
        {
            PrintActionsLog *log = new PrintActionsLog();
            log->act(*this);
        }
        else if (actionType == "close")
        {
            Close *close = new Close();
            close->act(*this);
        }
        else if (actionType == "backup")
        {
            BackupWareHouse *backup = new BackupWareHouse();
            backup->act(*this);
        }
        else if (actionType == "restore")
        {
            RestoreWareHouse *restore = new RestoreWareHouse();
            restore->act(*this);
        }
        else
        {
            cout << "error: not a valid command" << endl;
        }
    }
}
