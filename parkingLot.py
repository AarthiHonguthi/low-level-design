from abc import ABC, abstractmethod
from enum import Enum
import time


# ---------- FIXED TYPES ----------

class VehicleType(Enum):
    BIKE = 1
    CAR = 2
    TRUCK = 3


class SpotType(Enum):
    COMPACT = 1
    REGULAR = 2
    LARGE = 3


# ---------- BASIC ENTITIES ----------

class Vehicle:
    # Represents a vehicle entering the parking lot
    def __init__(self, number, vehicle_type):
        self._number = number
        self._type = vehicle_type

    def get_type(self):
        return self._type


class ParkingSpot:
    # Represents a single parking spot
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


class ParkingTicket:
    # Created when vehicle enters, used till exit
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


# ---------- STRATEGIES ----------

class PricingStrategy(ABC):
    # Pricing rule interface
    @abstractmethod
    def calculate_fee(self, entry_time, exit_time):
        pass


class HourlyPricing(PricingStrategy):
    # Hourly pricing logic
    def calculate_fee(self, entry_time, exit_time):
        hours = (exit_time - entry_time) / 3600
        return max(1, hours) * 50


class PaymentStrategy(ABC):
    # Payment interface
    @abstractmethod
    def pay(self, amount):
        pass


class CashPayment(PaymentStrategy):
    # Cash payment implementation
    def pay(self, amount):
        print(f"Cash payment received: ₹{amount}")
        return True


# ---------- PARKING LOT (SINGLETON) ----------

class ParkingLot:
    # Central system managing spots and tickets
    _instance = None

    def __init__(self):
        if ParkingLot._instance:
            raise Exception("Use get_instance()")

        self._spots = []
        self._tickets = {}
        self._ticket_counter = 1
        self._pricing_strategy = HourlyPricing()

    @staticmethod
    def get_instance():
        if not ParkingLot._instance:
            ParkingLot._instance = ParkingLot()
        return ParkingLot._instance

    def add_spot(self, spot):
        self._spots.append(spot)

    def _find_spot(self, vehicle_type):
        # Match vehicle type with correct spot type
        mapping = {
            VehicleType.BIKE: SpotType.COMPACT,
            VehicleType.CAR: SpotType.REGULAR,
            VehicleType.TRUCK: SpotType.LARGE
        }

        for spot in self._spots:
            if spot.get_type() == mapping[vehicle_type] and spot.is_available():
                return spot
        return None

    def park_vehicle(self, vehicle):
        # Entry flow
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

    def exit_vehicle(self, ticket_id, payment_strategy):
        # Exit flow
        if ticket_id not in self._tickets:
            print("Invalid Ticket")
            return

        ticket = self._tickets[ticket_id]
        fee = self._pricing_strategy.calculate_fee(
            ticket.get_entry_time(), time.time()
        )

        if payment_strategy.pay(fee):
            ticket.get_spot().free()
            del self._tickets[ticket_id]
            print("Exit successful")


# ---------- GATES ----------

class EntranceGate:
    # Entry gate
    def enter(self, vehicle):
        return ParkingLot.get_instance().park_vehicle(vehicle)


class ExitGate:
    # Exit gate
    def exit(self, ticket_id):
        ParkingLot.get_instance().exit_vehicle(ticket_id, CashPayment())


# ---------- MAIN ----------

if __name__ == "__main__":
    parking_lot = ParkingLot.get_instance()

    parking_lot.add_spot(ParkingSpot(1, SpotType.COMPACT))
    parking_lot.add_spot(ParkingSpot(2, SpotType.REGULAR))
    parking_lot.add_spot(ParkingSpot(3, SpotType.LARGE))

    vehicle = Vehicle("KA01AB1234", VehicleType.CAR)

    entrance = EntranceGate()
    ticket = entrance.enter(vehicle)

    if ticket:
        exit_gate = ExitGate()
        exit_gate.exit(ticket.get_ticket_id())
