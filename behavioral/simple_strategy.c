/*
 * SIMPLE STRATEGY PATTERN
 * 
 * Core Concept: Define a family of algorithms, make them interchangeable
 * The algorithm can vary independently from the clients that use it
 * 
 * Real-world analogy:
 * - Different ways to get to work: Car, Bus, Bike, Walk
 * - Same goal (get to work), different strategies
 * - You can switch strategies based on weather, time, mood, etc.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// =============================================================================
// STEP 1: Define what a Strategy looks like
// =============================================================================

typedef struct Strategy Strategy;
struct Strategy {
    char name[30];
    void (*execute)(Strategy* self, char* destination);
    int (*get_time)(Strategy* self);     // minutes to destination
    float (*get_cost)(Strategy* self);   // cost in dollars
};

// =============================================================================
// STEP 2: Concrete Strategies (different ways to travel)
// =============================================================================

// Strategy 1: Drive by Car
void car_execute(Strategy* self, char* destination) {
    printf("🚗 Driving to %s\n", destination);
    printf("   - Starting engine, checking GPS\n");
    printf("   - Taking the highway route\n");
    printf("   - Parking at destination\n");
}

int car_get_time(Strategy* self) { return 15; }  // 15 minutes
float car_get_cost(Strategy* self) { return 3.50; }  // Gas + parking

Strategy car_strategy = {
    .name = "Car",
    .execute = car_execute,
    .get_time = car_get_time,
    .get_cost = car_get_cost
};

// Strategy 2: Take the Bus
void bus_execute(Strategy* self, char* destination) {
    printf("🚌 Taking bus to %s\n", destination);
    printf("   - Walking to bus stop\n");
    printf("   - Waiting for Route 42\n");
    printf("   - Riding for 8 stops\n");
    printf("   - Walking from bus stop to destination\n");
}

int bus_get_time(Strategy* self) { return 35; }  // 35 minutes
float bus_get_cost(Strategy* self) { return 2.25; }  // Bus fare

Strategy bus_strategy = {
    .name = "Bus",
    .execute = bus_execute,
    .get_time = bus_get_time,
    .get_cost = bus_get_cost
};

// Strategy 3: Ride a Bike
void bike_execute(Strategy* self, char* destination) {
    printf("🚴 Biking to %s\n", destination);
    printf("   - Getting bike from garage\n");
    printf("   - Taking the bike lane route\n");
    printf("   - Locking bike at destination\n");
}

int bike_get_time(Strategy* self) { return 25; }  // 25 minutes
float bike_get_cost(Strategy* self) { return 0.0; }  // Free!

Strategy bike_strategy = {
    .name = "Bike",
    .execute = bike_execute,
    .get_time = bike_get_time,
    .get_cost = bike_get_cost
};

// Strategy 4: Walk
void walk_execute(Strategy* self, char* destination) {
    printf("🚶 Walking to %s\n", destination);
    printf("   - Putting on comfortable shoes\n");
    printf("   - Taking the scenic route through the park\n");
    printf("   - Enjoying the fresh air\n");
}

int walk_get_time(Strategy* self) { return 50; }  // 50 minutes
float walk_get_cost(Strategy* self) { return 0.0; }  // Free and healthy!

Strategy walk_strategy = {
    .name = "Walk",
    .execute = walk_execute,
    .get_time = walk_get_time,
    .get_cost = walk_get_cost
};

// =============================================================================
// STEP 3: Context - Person who needs to travel
// =============================================================================

typedef struct {
    char name[50];
    Strategy* travel_strategy;  // Current strategy
    float budget;
    int time_available;  // minutes
} Person;

Person* create_person(char* name, float budget, int time_available) {
    Person* person = malloc(sizeof(Person));
    strcpy(person->name, name);
    person->budget = budget;
    person->time_available = time_available;
    person->travel_strategy = &walk_strategy;  // Default strategy
    return person;
}

void set_travel_strategy(Person* person, Strategy* strategy) {
    person->travel_strategy = strategy;
    printf("👤 %s switched to: %s strategy\n", person->name, strategy->name);
}

void travel_to(Person* person, char* destination) {
    Strategy* strategy = person->travel_strategy;
    
    printf("\n🎯 %s wants to go to %s\n", person->name, destination);
    printf("💰 Budget: $%.2f, ⏰ Time available: %d minutes\n", 
           person->budget, person->time_available);
    
    printf("\nUsing %s strategy:\n", strategy->name);
    printf("⏱️  Estimated time: %d minutes\n", strategy->get_time(strategy));
    printf("💵 Estimated cost: $%.2f\n", strategy->get_cost(strategy));
    
    // Check if strategy fits constraints
    if (strategy->get_cost(strategy) <= person->budget && 
        strategy->get_time(strategy) <= person->time_available) {
        printf("✅ Strategy works! Let's go:\n");
        strategy->execute(strategy, destination);
    } else {
        printf("❌ Strategy doesn't work! Need to choose a different one.\n");
        if (strategy->get_cost(strategy) > person->budget) {
            printf("   💸 Too expensive (need $%.2f more)\n", 
                   strategy->get_cost(strategy) - person->budget);
        }
        if (strategy->get_time(strategy) > person->time_available) {
            printf("   ⏰ Too slow (need %d more minutes)\n", 
                   strategy->get_time(strategy) - person->time_available);
        }
    }
}

// =============================================================================
// STEP 4: Smart strategy selector
// =============================================================================

Strategy* find_best_strategy(Person* person) {
    Strategy* strategies[] = {&car_strategy, &bus_strategy, &bike_strategy, &walk_strategy};
    Strategy* best = NULL;
    int best_time = 9999;
    
    printf("\n🔍 Finding best strategy for %s (Budget: $%.2f, Time: %d min):\n", 
           person->name, person->budget, person->time_available);
    
    for (int i = 0; i < 4; i++) {
        Strategy* s = strategies[i];
        printf("   %s: %d min, $%.2f", s->name, s->get_time(s), s->get_cost(s));
        
        if (s->get_cost(s) <= person->budget && s->get_time(s) <= person->time_available) {
            printf(" ✅");
            if (s->get_time(s) < best_time) {
                best = s;
                best_time = s->get_time(s);
            }
        } else {
            printf(" ❌");
        }
        printf("\n");
    }
    
    if (best) {
        printf("🏆 Best choice: %s (%d minutes)\n", best->name, best_time);
    } else {
        printf("😅 No strategy fits! Using walk as fallback.\n");
        best = &walk_strategy;
    }
    
    return best;
}

// =============================================================================
// DEMO: Show how strategies can be switched dynamically
// =============================================================================

int main() {
    printf("=== SIMPLE STRATEGY PATTERN ===\n");
    printf("Travel Planning Example\n\n");
    
    // Create a person
    Person* alice = create_person("Alice", 5.00, 30);
    
    printf("👤 Meet %s! She needs to get to work.\n\n", alice->name);
    
    printf("=== Scenario 1: Normal Day ===\n");
    alice->budget = 5.00;
    alice->time_available = 30;
    
    Strategy* best = find_best_strategy(alice);
    set_travel_strategy(alice, best);
    travel_to(alice, "Downtown Office");
    
    printf("\n=== Scenario 2: Running Late! ===\n");
    alice->budget = 10.00;
    alice->time_available = 20;  // Very tight schedule
    
    best = find_best_strategy(alice);
    set_travel_strategy(alice, best);
    travel_to(alice, "Important Meeting");
    
    printf("\n=== Scenario 3: Broke Student ===\n");
    alice->budget = 0.50;  // Almost no money
    alice->time_available = 60;
    
    best = find_best_strategy(alice);
    set_travel_strategy(alice, best);
    travel_to(alice, "University");
    
    printf("\n=== Scenario 4: Nice Weather Weekend ===\n");
    alice->budget = 20.00;
    alice->time_available = 90;  // Plenty of time
    
    printf("👤 Alice thinks: 'What a beautiful day! Let me enjoy it.'\n");
    set_travel_strategy(alice, &bike_strategy);  // Manual choice
    travel_to(alice, "Park");
    
    printf("\n✨ Strategy Pattern Benefits:\n");
    printf("   • Same person, different travel strategies\n");
    printf("   • Can switch strategies at runtime\n");
    printf("   • Easy to add new travel methods (Uber, Scooter, etc.)\n");
    printf("   • Each strategy encapsulates its own algorithm\n");
    printf("   • Client code doesn't need to know strategy details\n");
    
    printf("\n🎯 Key Insight:\n");
    printf("   The Person doesn't care HOW they travel.\n");
    printf("   They just call travel_to() and the current strategy handles it.\n");
    printf("   This makes it easy to adapt to changing conditions!\n");
    
    // Cleanup
    free(alice);
    
    return 0;
}