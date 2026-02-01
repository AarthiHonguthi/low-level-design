#include <iostream>
#include <vector>
#include <string>
#include <ctime>
using namespace std;

// ---------- Phase 1: Core Entities ----------

class Vehicle
{
private:
    string number;

public:
    Vehicle(string number) : number(number) {}

    string getNumber() const
    {
        return number;
    }
};

class ParkingSpot
{
private:
    int id;
    bool isFree;

public:
    ParkingSpot(int id) : id(id), isFree(true) {}

    bool available() const
    {
        return isFree;
    }

    void markOccupied()
    {
        isFree = false;
        cout << "[SYSTEM] ParkingSpot " << id << " marked OCCUPIED\n";
    }

    void release()
    {
        isFree = true;
        cout << "[SYSTEM] ParkingSpot " << id << " released and now FREE\n";
    }

    int getId() const
    {
        return id;
    }
};

class ParkingTicket
{
private:
    Vehicle *vehicle;
    ParkingSpot *spot;
    time_t entryTime;

public:
    ParkingTicket(Vehicle *v, ParkingSpot *s)
        : vehicle(v), spot(s), entryTime(time(nullptr))
    {
        cout << "[SYSTEM] Ticket created for Vehicle "
             << v->getNumber()
             << " at Spot " << s->getId() << endl;
    }

    Vehicle *getVehicle() const
    {
        return vehicle;
    }

    ParkingSpot *getSpot() const
    {
        return spot;
    }

    time_t getEntryTime() const
    {
        return entryTime;
    }
};

// ---------- Phase 2: Services ----------

class PricingService
{
public:
    int calculateFee(time_t entryTime, time_t exitTime)
    {
        int hours = difftime(exitTime, entryTime) / 3600;
        if (hours < 1)
            hours = 1;

        int fee = hours * 50;
        cout << "[PRICING] Parking Fee Calculated: ₹" << fee << endl;
        return fee;
    }
};

class PaymentService
{
public:
    bool processPayment(int amount)
    {
        cout << "[PAYMENT] Processing payment of ₹" << amount << endl;
        cout << "[PAYMENT] Payment SUCCESSFUL\n";
        return true;
    }
};

// ---------- Phase 3: Central System ----------

class ParkingLotSystem
{
private:
    vector<ParkingSpot *> spots;
    PricingService *pricingService;
    PaymentService *paymentService;

    ParkingTicket *createTicket(Vehicle *vehicle, ParkingSpot *spot)
    {
        return new ParkingTicket(vehicle, spot);
    }

public:
    ParkingLotSystem(PricingService *pricing,
                     PaymentService *payment)
        : pricingService(pricing),
          paymentService(payment) {}

    void addSpot(ParkingSpot *spot)
    {
        spots.push_back(spot);
        cout << "[SYSTEM] Added ParkingSpot " << spot->getId() << endl;
    }

    ParkingTicket *handleEntry(Vehicle *vehicle)
    {
        cout << "\n[ENTRY] Vehicle " << vehicle->getNumber() << " arrived\n";

        for (auto spot : spots)
        {
            if (spot->available())
            {
                spot->markOccupied();
                return createTicket(vehicle, spot);
            }
        }

        cout << "[ENTRY] Parking Full! No spot available\n";
        return nullptr;
    }

    void handleExit(ParkingTicket *ticket)
    {
        cout << "\n[EXIT] Vehicle "
             << ticket->getVehicle()->getNumber()
             << " is exiting\n";

        int fee = pricingService->calculateFee(
            ticket->getEntryTime(),
            time(nullptr));

        if (paymentService->processPayment(fee))
        {
            ticket->getSpot()->release();
            cout << "[EXIT] Exit completed successfully\n";
        }
    }
};

// ---------- Phase 4: Gates ----------

class EntranceGate
{
private:
    ParkingLotSystem *system;

public:
    EntranceGate(ParkingLotSystem *system)
        : system(system) {}

    ParkingTicket *enter(Vehicle *vehicle)
    {
        cout << "[GATE] EntranceGate triggered\n";
        return system->handleEntry(vehicle);
    }
};

class ExitGate
{
private:
    ParkingLotSystem *system;

public:
    ExitGate(ParkingLotSystem *system)
        : system(system) {}

    void exit(ParkingTicket *ticket)
    {
        cout << "[GATE] ExitGate triggered\n";
        system->handleExit(ticket);
    }
};

// ---------- MAIN ----------

int main()
{
    PricingService pricingService;
    PaymentService paymentService;

    ParkingLotSystem system(&pricingService, &paymentService);

    system.addSpot(new ParkingSpot(1));
    system.addSpot(new ParkingSpot(2));

    EntranceGate entrance(&system);
    ExitGate exitGate(&system);

    Vehicle car("KA-01-1234");

    ParkingTicket *ticket = entrance.enter(&car);

    if (ticket)
    {
        exitGate.exit(ticket);
    }

    return 0;
}
