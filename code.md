## Phase 1: Nouns->Classes
From the story, I clearly see these nouns:

- Vehicle
- ParkingLotSystem
- EntranceGate
- ParkingSpot
- ParkingTicket
- ExitGate
- PricingService
- PaymentService

1) Vehicle (A vehicle arrives at the parking lot.)
```cpp
class Vehicle {
private:
    string number;

public:
    Vehicle(string number) : number(number) {}

    string getNumber() const {
        return number;
    }
};
```

2) ParkingLotSystem (The parking lot system manages parking spots and tickets.)
```cpp
class ParkingLotSystem {
    // will fill later
};
```
3) EntranceGate (The entrance gate issues parking tickets.)
```cpp
class EntranceGate {
public:
    // entry-related behavior will go here
};
```
4) ParkingSpot (The vehicle parks in an assigned spot.)
>A parking spot just stores state: free or occupied.
It should NOT make decisions.
```cpp
class ParkingSpot {
private:
    int id;
    bool isFree;

public:
    ParkingSpot(int id) : id(id), isFree(true) {}

    bool available() const {
        return isFree;
    }

    void markOccupied() {
        isFree = false;
    }

    void release() {
        isFree = true;
    }

    int getId() const {
        return id;
    }
};
```
5) ParkingTicket (The parking ticket records entry time and spot.)
```cpp
#include <ctime>

class ParkingTicket {
private:
    Vehicle* vehicle;
    ParkingSpot* spot;
    time_t entryTime;

public:
    ParkingTicket(Vehicle* v, ParkingSpot* s)
        : vehicle(v), spot(s), entryTime(time(nullptr)) {}

    Vehicle* getVehicle() const {
        return vehicle;
    }

    ParkingSpot* getSpot() const {
        return spot;
    }

    time_t getEntryTime() const {
        return entryTime;
    }
};
```
6) ExitGate, PricingService, PaymentService
```cpp
class ExitGate {};
class PricingService {};
class PaymentService {};
```
## Phase 2: Identify Design Patterns
Now I’ll walk through the sequence diagram top to bottom and connect these classes.

**Step 1: Singleton — One Central System Controls the Flow**

First, I connect all the main actions to one system object.
```cpp
class ParkingLotSystem {
private:
    static ParkingLotSystem* instance;

    ParkingLotSystem() {}   // private constructor

public:
    static ParkingLotSystem* getInstance() {
        if (!instance)
            instance = new ParkingLotSystem();
        return instance;
    }

    // entry & exit are system responsibilities
    ParkingTicket* handleEntry(Vehicle* vehicle);
    void handleExit(ParkingTicket* ticket);
};

```
**Step 2: Factory — Creating Flow-Driven Objects in One Place**
