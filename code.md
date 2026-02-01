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
What does the system need to remember?
- To assign a spot → I need all spots
- To calculate fee → I need pricing service
- To take payment → I need payment service

```cpp
#include <vector>

class ParkingLotSystem {
private:
    vector<ParkingSpot*> spots;
    PricingService* pricingService;
    PaymentService* paymentService;

public:
    ParkingLotSystem(PricingService* pricing,
                     PaymentService* payment)
        : pricingService(pricing),
          paymentService(payment) {}

    ParkingTicket* handleEntry(Vehicle* vehicle);
    void handleExit(ParkingTicket* ticket);

    void addSpot(ParkingSpot* spot) {
        spots.push_back(spot);
    }
};
```
Now update the gates so they talk to the same system object.
```cpp
class EntranceGate {
private:
    ParkingLotSystem* system;

public:
    EntranceGate(ParkingLotSystem* system)
        : system(system) {}

    ParkingTicket* enter(Vehicle* vehicle) {
        return system->handleEntry(vehicle);
    }
};

class ExitGate {
private:
    ParkingLotSystem* system;

public:
    ExitGate(ParkingLotSystem* system)
        : system(system) {}

    void exit(ParkingTicket* ticket) {
        system->handleExit(ticket);
    }
};
```

**Step 2: Factory — Creating Flow-Driven Objects in One Place**

A ParkingTicket is created only when a vehicle enters.
So we don’t want:

- new ParkingTicket() scattered everywhere
- Vehicle or spot creating tickets

Here:
- ParkingLotSystem decides when to create a ticket
- EntranceGate handles issuing the ticket

```cpp
class ParkingLotSystem {
private:
    vector<ParkingSpot*> spots;
    PricingService* pricingService;
    PaymentService* paymentService;

    // Factory-style creation
    ParkingTicket* createTicket(Vehicle* vehicle, ParkingSpot* spot) {
        return new ParkingTicket(vehicle, spot);
    }

public:
    ParkingLotSystem(PricingService* pricing,
                     PaymentService* payment)
        : pricingService(pricing),
          paymentService(payment) {}

    void addSpot(ParkingSpot* spot) {
        spots.push_back(spot);
    }

    ParkingTicket* handleEntry(Vehicle* vehicle) {
        for (auto spot : spots) {
            if (spot->available()) {
                spot->markOccupied();
                return createTicket(vehicle, spot);
            }
        }
        return nullptr; // parking full
    }

    void handleExit(ParkingTicket* ticket);
};
```
**Step 3: Strategy — Rules Are Separate from the Flow**

From the sequence diagram:

- System → PricingService
- System → PaymentService


So pricing and payment logic should live outside the system.

```cpp
class PricingService {
public:
    int calculateFee(time_t entryTime, time_t exitTime) {
        int hours = difftime(exitTime, entryTime) / 3600;
        if (hours < 1) hours = 1;
        return hours * 50; // hourly pricing
    }
};
```
```cpp
class PaymentService {
public:
    bool processPayment(int amount) {
        // assume cash payment
        return true;
    }
};
```
Now complete the exit flow in ParkingLotSystem:
```cpp
void ParkingLotSystem::handleExit(ParkingTicket* ticket) {
    int fee = pricingService->calculateFee(
        ticket->getEntryTime(),
        time(nullptr)
    );

    if (paymentService->processPayment(fee)) {
        ticket->getSpot()->release();
    }
}
```
Phase 3: main() Function to Tie It All Together
```cpp
int main() {
    PricingService pricingService;
    PaymentService paymentService;

    ParkingLotSystem system(&pricingService, &paymentService);

    system.addSpot(new ParkingSpot(1));
    system.addSpot(new ParkingSpot(2));

    EntranceGate entrance(&system);
    ExitGate exitGate(&system);

    Vehicle car("KA-01-1234");

    ParkingTicket* ticket = entrance.enter(&car);

    if (ticket) {
        exitGate.exit(ticket);
    }

    return 0;
}
```



# PYTHON IMPLEMENTATION

```python
from abc import ABC, abstractmethod
from enum import Enum
import time
```
# Vehicle Class
````python
class Vehicle:
    def __init__(self, number, vehicle_type):
        self._number = number
        self._type = vehicle_type

    def get_type(self):
        return self._type

````
`````python
class VehicleType(Enum):
    BIKE = 1
    CAR = 2
    TRUCK = 3


class SpotType(Enum):
    COMPACT = 1
    REGULAR = 2
    LARGE = 3
`````

>💡 **Enum**  
>Use enum when the options are **fixed** and will not change.
It only tells what type it is, no logic inside.


# ParkingSpot Class
```python
class ParkingSpot:
    def __init__(self, spot_id, spot_type):
        self._id = spot_id
        self._type = spot_type
        self._occupied = False

    def is_available(self):
        return not self._occupied

    def occupy(self):
        self._occupied = True

    def free(self):
        self._occupied = False

    def get_type(self):
        return self._type
```

>🪝 **Hook 1**  
ParkingSpot only stores state.  
No decisions here.

# ParkingTicket Class
```python

class ParkingTicket:
    def __init__(self, ticket_id, vehicle, spot):
        self._ticket_id = ticket_id
        self._vehicle = vehicle
        self._spot = spot
        self._entry_time = time.time()

    def get_entry_time(self):
        return self._entry_time

    def get_spot(self):
        return self._spot

    def get_ticket_id(self):
        return self._ticket_id
```


# Pricing Strategy Classes
```python
class PricingStrategy(ABC):
    @abstractmethod
    def calculate_fee(self, entry_time, exit_time):
        pass
class HourlyPricing(PricingStrategy):
    def calculate_fee(self, entry_time, exit_time):
        hours = (exit_time - entry_time) / 3600
        if hours < 1:
            hours = 1
        return hours * 50
```

>💡 **Abstract Class**  
Use abstract class when **logic can change**, but method name stays same.
It tells what work must be done, not how.

>🪝 **Hook 2**  
Logic can change, method name should not.  
Strategy pattern.

# Payment Strategy Classes
```python
class PaymentService(ABC):
    @abstractmethod
    def pay(self, amount):
        pass
class CashPayment(PaymentService):
    def pay(self, amount):
        print(f"Cash payment received: ₹{amount}")
        return True
```

# ParkingLotSystem Class
```python
class ParkingLotSystem:
    _instance = None


    def __init__(self):
        if ParkingLotSystem._instance:
            raise Exception("Use get_instance()")

        self._spots = []
        self._tickets = {}
        self._ticket_counter = 1
        self._pricing_strategy = HourlyPricing()

    @staticmethod
    def get_instance():
        if not ParkingLotSystem._instance:
            ParkingLotSystem._instance = ParkingLotSystem()
        return ParkingLotSystem._instance
```
>🪝 **Hook 3**  
>Only one parking lot object should exist, otherwise data will mismatch.  
Singleton avoids confusion.

## Spot Assignment Logic
```python
    def _find_spot(self, vehicle_type):
        mapping = {
            VehicleType.BIKE: SpotType.COMPACT,
            VehicleType.CAR: SpotType.REGULAR,
            VehicleType.TRUCK: SpotType.LARGE
        }

        for spot in self._spots:
            if spot.get_type() == mapping[vehicle_type] and spot.is_available():
                return spot
        return None
```
```python
    def add_spot(self, spot):
        self._spots.append(spot)

    def park_vehicle(self, vehicle):
        spot = self._find_spot(vehicle.get_type())
        if not spot:
            print("Parking Full")
            return None

        spot.occupy()
        ticket = ParkingTicket(self._ticket_counter, vehicle, spot)
        self._tickets[self._ticket_counter] = ticket
        print(f"Ticket generated: {self._ticket_counter}")
        self._ticket_counter += 1
        return ticket
```
## Exit Flow
```python
    def exit_vehicle(self, ticket_id, payment_service):
        if ticket_id not in self._tickets:
            print("Invalid Ticket")
            return

        ticket = self._tickets[ticket_id]
        fee = self._pricing_strategy.calculate_fee(
            ticket.get_entry_time(), time.time()
        )

        if payment_service.pay(fee):
            ticket.get_spot().free()
            del self._tickets[ticket_id]
            print("Exit successful")

```
## Gates
```python
class EntranceGate:
    def enter(self, vehicle):
        return ParkingLotSystem.get_instance().park_vehicle(vehicle)


class ExitGate:
    def exit(self, ticket_id):
        ParkingLotSystem.get_instance().exit_vehicle(ticket_id, CashPayment())
```
>🪝 **Hook 4**  
**Why Gates?**  
Real parking lots have many entry and exit points.
Gates act as a common entry point to the system.   
If there are no gates, every entry point will directly call ParkingLotSystem.
Each entry may add its own checks and logic.
Same logic gets written again and again in different places.
When rules change, we must update code in many places.
This increases bugs and makes the system hard to maintain.

```python
if __name__ == "__main__":
    parking_lot = ParkingLotSystem.get_instance()

    parking_lot.add_spot(ParkingSpot(1, SpotType.COMPACT))
    parking_lot.add_spot(ParkingSpot(2, SpotType.REGULAR))
    parking_lot.add_spot(ParkingSpot(3, SpotType.LARGE))

    vehicle = Vehicle("KA01AB1234", VehicleType.CAR)

    entrance = EntranceGate()
    ticket = entrance.enter(vehicle)

    if ticket:
        exit_gate = ExitGate()
        exit_gate.exit(ticket.get_ticket_id())
```