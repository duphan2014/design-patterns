/*
 * DECORATOR PATTERN
 * 
 * Problem: Add behavior to objects dynamically without altering their structure
 * 
 * Use Cases:
 * - Adding features to UI components
 * - Stream processing (compression, encryption)
 * - Middleware in web frameworks
 * - Game character abilities
 * 
 * Pros:
 * - More flexible than inheritance
 * - Add/remove responsibilities at runtime
 * - Combine multiple decorators
 * 
 * Cons:
 * - Can result in many small objects
 * - Harder to debug wrapped objects
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Component interface
typedef struct Coffee Coffee;
struct Coffee {
    char description[256];
    double cost;
    double (*get_cost)(Coffee* self);
    void (*get_description)(Coffee* self, char* result);
};

// Concrete Component: Basic Coffee
typedef struct {
    Coffee base;
} BasicCoffee;

double basic_coffee_cost(Coffee* self) {
    return self->cost;
}

void basic_coffee_description(Coffee* self, char* result) {
    strcpy(result, self->description);
}

Coffee* create_basic_coffee() {
    BasicCoffee* coffee = (BasicCoffee*)malloc(sizeof(BasicCoffee));
    strcpy(coffee->base.description, "Basic Coffee");
    coffee->base.cost = 2.00;
    coffee->base.get_cost = basic_coffee_cost;
    coffee->base.get_description = basic_coffee_description;
    return (Coffee*)coffee;
}

// Decorator Base
typedef struct {
    Coffee base;
    Coffee* wrapped_coffee;
} CoffeeDecorator;

// Concrete Decorator: Milk
typedef struct {
    CoffeeDecorator decorator;
} MilkDecorator;

double milk_cost(Coffee* self) {
    MilkDecorator* milk = (MilkDecorator*)self;
    return milk->decorator.wrapped_coffee->get_cost(milk->decorator.wrapped_coffee) + 0.50;
}

void milk_description(Coffee* self, char* result) {
    MilkDecorator* milk = (MilkDecorator*)self;
    char base_desc[256];
    milk->decorator.wrapped_coffee->get_description(milk->decorator.wrapped_coffee, base_desc);
    sprintf(result, "%s + Milk", base_desc);
}

Coffee* add_milk(Coffee* coffee) {
    MilkDecorator* milk = (MilkDecorator*)malloc(sizeof(MilkDecorator));
    milk->decorator.wrapped_coffee = coffee;
    milk->decorator.base.get_cost = milk_cost;
    milk->decorator.base.get_description = milk_description;
    return (Coffee*)milk;
}

// Concrete Decorator: Sugar
typedef struct {
    CoffeeDecorator decorator;
} SugarDecorator;

double sugar_cost(Coffee* self) {
    SugarDecorator* sugar = (SugarDecorator*)self;
    return sugar->decorator.wrapped_coffee->get_cost(sugar->decorator.wrapped_coffee) + 0.25;
}

void sugar_description(Coffee* self, char* result) {
    SugarDecorator* sugar = (SugarDecorator*)self;
    char base_desc[256];
    sugar->decorator.wrapped_coffee->get_description(sugar->decorator.wrapped_coffee, base_desc);
    sprintf(result, "%s + Sugar", base_desc);
}

Coffee* add_sugar(Coffee* coffee) {
    SugarDecorator* sugar = (SugarDecorator*)malloc(sizeof(SugarDecorator));
    sugar->decorator.wrapped_coffee = coffee;
    sugar->decorator.base.get_cost = sugar_cost;
    sugar->decorator.base.get_description = sugar_description;
    return (Coffee*)sugar;
}

// Concrete Decorator: Whipped Cream
typedef struct {
    CoffeeDecorator decorator;
} WhippedCreamDecorator;

double whipped_cream_cost(Coffee* self) {
    WhippedCreamDecorator* cream = (WhippedCreamDecorator*)self;
    return cream->decorator.wrapped_coffee->get_cost(cream->decorator.wrapped_coffee) + 0.75;
}

void whipped_cream_description(Coffee* self, char* result) {
    WhippedCreamDecorator* cream = (WhippedCreamDecorator*)self;
    char base_desc[256];
    cream->decorator.wrapped_coffee->get_description(cream->decorator.wrapped_coffee, base_desc);
    sprintf(result, "%s + Whipped Cream", base_desc);
}

Coffee* add_whipped_cream(Coffee* coffee) {
    WhippedCreamDecorator* cream = (WhippedCreamDecorator*)malloc(sizeof(WhippedCreamDecorator));
    cream->decorator.wrapped_coffee = coffee;
    cream->decorator.base.get_cost = whipped_cream_cost;
    cream->decorator.base.get_description = whipped_cream_description;
    return (Coffee*)cream;
}

// Concrete Decorator: Vanilla Syrup
typedef struct {
    CoffeeDecorator decorator;
} VanillaSyrupDecorator;

double vanilla_syrup_cost(Coffee* self) {
    VanillaSyrupDecorator* syrup = (VanillaSyrupDecorator*)self;
    return syrup->decorator.wrapped_coffee->get_cost(syrup->decorator.wrapped_coffee) + 0.60;
}

void vanilla_syrup_description(Coffee* self, char* result) {
    VanillaSyrupDecorator* syrup = (VanillaSyrupDecorator*)self;
    char base_desc[256];
    syrup->decorator.wrapped_coffee->get_description(syrup->decorator.wrapped_coffee, base_desc);
    sprintf(result, "%s + Vanilla Syrup", base_desc);
}

Coffee* add_vanilla_syrup(Coffee* coffee) {
    VanillaSyrupDecorator* syrup = (VanillaSyrupDecorator*)malloc(sizeof(VanillaSyrupDecorator));
    syrup->decorator.wrapped_coffee = coffee;
    syrup->decorator.base.get_cost = vanilla_syrup_cost;
    syrup->decorator.base.get_description = vanilla_syrup_description;
    return (Coffee*)syrup;
}

// Utility function to print coffee details
void print_coffee_order(Coffee* coffee) {
    char description[512];
    coffee->get_description(coffee, description);
    printf("Order: %s\n", description);
    printf("Cost: $%.2f\n", coffee->get_cost(coffee));
    printf("------------------------\n");
}

// Example usage
int main() {
    printf("=== DECORATOR PATTERN EXAMPLE ===\n\n");
    printf("Coffee Shop Order System\n\n");
    
    // Order 1: Basic coffee
    printf("Order 1:\n");
    Coffee* coffee1 = create_basic_coffee();
    print_coffee_order(coffee1);
    
    // Order 2: Coffee with milk
    printf("Order 2:\n");
    Coffee* coffee2 = create_basic_coffee();
    coffee2 = add_milk(coffee2);
    print_coffee_order(coffee2);
    
    // Order 3: Coffee with milk and sugar
    printf("Order 3:\n");
    Coffee* coffee3 = create_basic_coffee();
    coffee3 = add_milk(coffee3);
    coffee3 = add_sugar(coffee3);
    print_coffee_order(coffee3);
    
    // Order 4: Fully loaded coffee
    printf("Order 4:\n");
    Coffee* coffee4 = create_basic_coffee();
    coffee4 = add_milk(coffee4);
    coffee4 = add_sugar(coffee4);
    coffee4 = add_whipped_cream(coffee4);
    coffee4 = add_vanilla_syrup(coffee4);
    print_coffee_order(coffee4);
    
    // Order 5: Multiple decorators of same type
    printf("Order 5 (Double Sugar):\n");
    Coffee* coffee5 = create_basic_coffee();
    coffee5 = add_sugar(coffee5);
    coffee5 = add_sugar(coffee5);  // Double sugar!
    coffee5 = add_milk(coffee5);
    print_coffee_order(coffee5);
    
    printf("Demonstrating flexibility of decorator pattern:\n");
    printf("- Can add any combination of decorators\n");
    printf("- Can add multiple instances of same decorator\n");
    printf("- Cost and description automatically calculated\n");
    
    // Note: In a real implementation, you'd need proper cleanup
    // This example focuses on demonstrating the pattern
    
    return 0;
}