# Parking Lot Low lewel Design

![alt text](ParkingManagment.gif)

## Problem Statement

Design a Parking Lot system that can handle different types of vehicles, assign parking spots, generate tickets, and calculate parking fees.

---
LLD is not about jumping straight into classes and code. As humans, we cannot think of all the requirements at once, and that is completely fine. That is why asking the right questions is an important part of Low Level Design. These questions help us understand what the system should do before we decide how to design it. Instead of assuming everything upfront, we start with the basics and correct our understanding step by step as the design evolves.

---
> ## **Interviewer ↔ Candidate Clarification**
>
> **Candidate:** What types of vehicles will the parking lot support?  
> **Interviewer:** It will support three types of vehicles: Bike, Car, and Truck.  
>
> **Candidate:** Okay. So will there be different parking spots for different vehicle types?  
> **Interviewer:** Yes. Each vehicle type will have its own parking spot category: Compact, Regular, and Large.  
>
> **Candidate:** When a vehicle enters the parking lot, is the ticket generated at entry or at exit?  
> **Interviewer:** The ticket is generated at entry.  
>
> **Candidate:** Okay. So ticket generation happens at entry, and payment is done at exit, right?  
> **Interviewer:** Yes, that is correct.  
>
> **Candidate:** What pricing model should we follow? Hourly or flat rate?  
> **Interviewer:** We will use hourly pricing.  
>
> **Candidate:** Got it. That means we need to track the time from entry to exit. So the ticket should store the entry time, which we can later use to calculate the parking duration.  
> **Interviewer:** Yes.  
>
> **Candidate:** What payment methods should we support for now?  
> **Interviewer:** For now, cash payment is sufficient.  
>
> **Candidate:** One more question. What happens if the parking lot is already full? We should not issue a ticket in that case, right?  
> **Interviewer:** Yes. If there is no available parking spot, the system should not generate a ticket and should deny entry.  

---


After the discussion, the candidate writes down only what is confirmed.

![alt text](image-3.png)
---
## Story Time
Before jumping into diagrams or code, we write a simple story describing how the system works in real life.

While writing this story, we follow one simple rule:

+ Nouns represent classes
+ Verbs (action words) represent methods

This story helps us understand the complete flow from entry to exit without thinking about implementation details.


![alt text](image-1.png)


From this story, we can clearly see what actions are performed by the customer and what responsibilities are handled by the system. We will use these observations directly when identifying actors and defining use cases in the next section.

---
## Use Case Diagram


As we saw in the story, the customer performs actions such as entering the parking lot, receiving a ticket, paying the parking fee, and exiting the system. These actions naturally translate into customer use cases.

In most low-level designs, we usually identify two primary actors:

+ Customer, who uses the system

+ Admin, who manages and configures the system

Both of these actors interact with the Parking Lot system, which handles tasks like ticket generation, spot assignment, fee calculation, and payment processing. Using this understanding, we can now model the system behavior using a use case diagram.

---

## Actors and Their Use Cases

Based on the discussion, we identify the following actors and their interactions with the Parking Lot system.

---

### 1. Customer

**Description:** The person who uses the parking lot as an end user.

**Customer Use Cases:**
- Enter parking lot
- Receive parking ticket
- Pay parking fee
- Exit parking lot

---

### 2. Admin

**Description:** Responsible for managing and configuring the parking lot.

**Admin Use Cases:**
- Manage parking spots
- Manage pricing rules

---

### 3. System

**Description:** Represents the automated behavior handled without manual intervention.

**System Use Cases:**
- Generate parking ticket
- Assign parking spot
- Calculate parking fee
- Process payment

---

![alt text](<ParkingLot.drawio (4).png>)

