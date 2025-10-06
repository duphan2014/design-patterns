/*
 * SIMPLE TEMPLATE METHOD PATTERN
 * 
 * Core Concept: Define the skeleton of an algorithm, let subclasses fill in the details
 * The overall structure stays the same, but specific steps can be customized
 * 
 * Real-world analogy:
 * - Making different types of coffee: Espresso, Latte, Cappuccino
 * - All follow same basic steps: grind → brew → add extras → serve
 * - But each type customizes the "add extras" step differently
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// =============================================================================
// STEP 1: Define the Template (base coffee maker)
// =============================================================================

typedef struct CoffeeMaker CoffeeMaker;
struct CoffeeMaker {
    char name[30];
    
    // Template method - defines the algorithm structure
    void (*make_coffee)(CoffeeMaker* self);
    
    // Steps that are the same for everyone
    void (*grind_beans)(CoffeeMaker* self);
    void (*brew_coffee)(CoffeeMaker* self);
    void (*serve)(CoffeeMaker* self);
    
    // Step that varies - subclasses customize this
    void (*add_extras)(CoffeeMaker* self);
};

// =============================================================================
// STEP 2: Common steps (same for all coffee types)
// =============================================================================

void grind_beans(CoffeeMaker* self) {
    printf("☕ Grinding fresh coffee beans...\n");
    printf("   *whirr whirr* - Perfect grind achieved!\n");
}

void brew_coffee(CoffeeMaker* self) {
    printf("☕ Brewing coffee with hot water...\n");
    printf("   *bubble bubble* - Rich aroma filling the air!\n");
}

void serve(CoffeeMaker* self) {
    printf("☕ Serving your %s\n", self->name);
    printf("   ☕ Enjoy your delicious coffee!\n");
}

// =============================================================================
// STEP 3: The Template Method (defines the algorithm skeleton)
// =============================================================================

void make_coffee_template(CoffeeMaker* self) {
    printf("\n=== Making %s ===\n", self->name);
    
    // This is the template - the algorithm structure that never changes
    printf("Step 1: ");
    self->grind_beans(self);
    
    printf("Step 2: ");
    self->brew_coffee(self);
    
    printf("Step 3: ");
    self->add_extras(self);    // This is where customization happens!
    
    printf("Step 4: ");
    self->serve(self);
    
    printf("✅ %s is ready!\n", self->name);
}

// =============================================================================
// STEP 4: Concrete implementations (different coffee types)
// =============================================================================

// Espresso - just pure coffee, no extras
void espresso_add_extras(CoffeeMaker* self) {
    printf("Adding extras for Espresso...\n");
    printf("   → No extras needed - pure coffee perfection!\n");
}

CoffeeMaker* create_espresso_maker() {
    CoffeeMaker* maker = malloc(sizeof(CoffeeMaker));
    strcpy(maker->name, "Espresso");
    maker->make_coffee = make_coffee_template;
    maker->grind_beans = grind_beans;
    maker->brew_coffee = brew_coffee;
    maker->add_extras = espresso_add_extras;  // Custom step
    maker->serve = serve;
    return maker;
}

// Latte - coffee with steamed milk
void latte_add_extras(CoffeeMaker* self) {
    printf("Adding extras for Latte...\n");
    printf("   → Steaming milk to 150°F\n");
    printf("   → Adding steamed milk to coffee\n");
    printf("   → Creating beautiful latte art 🎨\n");
}

CoffeeMaker* create_latte_maker() {
    CoffeeMaker* maker = malloc(sizeof(CoffeeMaker));
    strcpy(maker->name, "Latte");
    maker->make_coffee = make_coffee_template;
    maker->grind_beans = grind_beans;
    maker->brew_coffee = brew_coffee;
    maker->add_extras = latte_add_extras;     // Custom step
    maker->serve = serve;
    return maker;
}

// Cappuccino - coffee with milk foam
void cappuccino_add_extras(CoffeeMaker* self) {
    printf("Adding extras for Cappuccino...\n");
    printf("   → Steaming milk with extra foam\n");
    printf("   → Adding equal parts coffee, steamed milk, and foam\n");
    printf("   → Dusting with cinnamon ✨\n");
}

CoffeeMaker* create_cappuccino_maker() {
    CoffeeMaker* maker = malloc(sizeof(CoffeeMaker));
    strcpy(maker->name, "Cappuccino");
    maker->make_coffee = make_coffee_template;
    maker->grind_beans = grind_beans;
    maker->brew_coffee = brew_coffee;
    maker->add_extras = cappuccino_add_extras; // Custom step
    maker->serve = serve;
    return maker;
}

// Mocha - coffee with chocolate
void mocha_add_extras(CoffeeMaker* self) {
    printf("Adding extras for Mocha...\n");
    printf("   → Adding rich chocolate syrup 🍫\n");
    printf("   → Steaming milk\n");
    printf("   → Topping with whipped cream\n");
    printf("   → Drizzling more chocolate on top!\n");
}

CoffeeMaker* create_mocha_maker() {
    CoffeeMaker* maker = malloc(sizeof(CoffeeMaker));
    strcpy(maker->name, "Mocha");
    maker->make_coffee = make_coffee_template;
    maker->grind_beans = grind_beans;
    maker->brew_coffee = brew_coffee;
    maker->add_extras = mocha_add_extras;     // Custom step
    maker->serve = serve;
    return maker;
}

// =============================================================================
// STEP 5: Coffee shop simulation
// =============================================================================

void destroy_coffee_maker(CoffeeMaker* maker) {
    free(maker);
}

typedef struct {
    CoffeeMaker* makers[10];
    int count;
} CoffeeShop;

void add_coffee_maker(CoffeeShop* shop, CoffeeMaker* maker) {
    shop->makers[shop->count] = maker;
    shop->count++;
}

void process_orders(CoffeeShop* shop) {
    printf("☕ Welcome to Simple Coffee Shop!\n");
    printf("📋 Processing today's orders...\n");
    
    for (int i = 0; i < shop->count; i++) {
        shop->makers[i]->make_coffee(shop->makers[i]);
        printf("\n");
    }
}

// =============================================================================
// DEMO: Show how template method works
// =============================================================================

int main() {
    printf("=== SIMPLE TEMPLATE METHOD PATTERN ===\n");
    printf("Coffee Shop Example\n");
    
    // Create different coffee makers
    CoffeeMaker* espresso = create_espresso_maker();
    CoffeeMaker* latte = create_latte_maker();
    CoffeeMaker* cappuccino = create_cappuccino_maker();
    CoffeeMaker* mocha = create_mocha_maker();
    
    // Set up coffee shop
    CoffeeShop shop = {.count = 0};
    add_coffee_maker(&shop, espresso);
    add_coffee_maker(&shop, latte);
    add_coffee_maker(&shop, cappuccino);
    add_coffee_maker(&shop, mocha);
    
    // Process orders
    process_orders(&shop);
    
    printf("🎯 Template Method Pattern Analysis:\n\n");
    
    printf("📋 Algorithm Structure (Template):\n");
    printf("   1. Grind beans    ← Same for everyone\n");
    printf("   2. Brew coffee    ← Same for everyone\n");
    printf("   3. Add extras     ← CUSTOMIZABLE! Different for each type\n");
    printf("   4. Serve          ← Same for everyone\n\n");
    
    printf("✨ Pattern Benefits:\n");
    printf("   • Algorithm structure is consistent and reusable\n");
    printf("   • Common steps avoid code duplication\n");
    printf("   • Easy to add new coffee types (just implement add_extras)\n");
    printf("   • Template enforces the correct order of operations\n");
    printf("   • Subclasses can't accidentally break the main algorithm\n\n");
    
    printf("🔍 Key Insight:\n");
    printf("   All coffee makers follow the SAME 4-step process.\n");
    printf("   Only step 3 (add_extras) changes between different coffee types.\n");
    printf("   This prevents code duplication while allowing customization!\n\n");
    
    printf("💡 Real-world uses:\n");
    printf("   • Cooking recipes (same steps, different ingredients)\n");
    printf("   • Data processing pipelines (same flow, different transforms)\n");
    printf("   • Game AI (same decision process, different strategies)\n");
    printf("   • Web frameworks (same request handling, different business logic)\n");
    
    // Cleanup
    destroy_coffee_maker(espresso);
    destroy_coffee_maker(latte);
    destroy_coffee_maker(cappuccino);
    destroy_coffee_maker(mocha);
    
    return 0;
}