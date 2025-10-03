/*
 * STATE PATTERN
 * 
 * Problem: Allow an object to alter its behavior when its internal state changes
 * 
 * Use Cases:
 * - State machines
 * - Game character states
 * - Order processing systems
 * - Network connection states
 * 
 * Pros:
 * - Localizes state-specific behavior
 * - Makes state transitions explicit
 * - Eliminates complex conditional statements
 * 
 * Cons:
 * - Increased number of classes
 * - Overhead for simple state machines
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Forward declarations
typedef struct VendingMachine VendingMachine;
typedef struct State State;

// State interface
struct State {
    char name[50];
    void (*insert_coin)(State* self, VendingMachine* machine, int amount);
    void (*select_product)(State* self, VendingMachine* machine, int product_id);
    void (*dispense)(State* self, VendingMachine* machine);
    void (*cancel)(State* self, VendingMachine* machine);
    void (*display_message)(State* self);
};

// Context: Vending Machine
struct VendingMachine {
    State* current_state;
    int inserted_amount;
    int selected_product_price;
    char selected_product_name[100];
    int product_stock[5];  // Stock for 5 different products
    char product_names[5][50];
    int product_prices[5];
    
    void (*set_state)(VendingMachine* self, State* state);
    void (*display_status)(VendingMachine* self);
};

// Forward declare states
State* get_idle_state();
State* get_coin_inserted_state();
State* get_product_selected_state();
State* get_dispensing_state();
State* get_out_of_order_state();

void vending_machine_set_state(VendingMachine* self, State* state) {
    printf("🔄 State changed: %s → %s\n", self->current_state->name, state->name);
    self->current_state = state;
    self->current_state->display_message(self->current_state);
}

void vending_machine_display_status(VendingMachine* self) {
    printf("\n📟 Vending Machine Status:\n");
    printf("   Current State: %s\n", self->current_state->name);
    printf("   Inserted Amount: $%.2f\n", self->inserted_amount / 100.0);
    printf("   Available Products:\n");
    for (int i = 0; i < 5; i++) {
        printf("     %d. %s - $%.2f (Stock: %d)\n", 
               i + 1, self->product_names[i], self->product_prices[i] / 100.0, self->product_stock[i]);
    }
    printf("-------------------\n");
}

VendingMachine* create_vending_machine() {
    VendingMachine* machine = (VendingMachine*)malloc(sizeof(VendingMachine));
    
    machine->current_state = get_idle_state();
    machine->inserted_amount = 0;
    machine->selected_product_price = 0;
    strcpy(machine->selected_product_name, "");
    
    // Initialize products
    strcpy(machine->product_names[0], "Coke");
    strcpy(machine->product_names[1], "Pepsi");
    strcpy(machine->product_names[2], "Water");
    strcpy(machine->product_names[3], "Chips");
    strcpy(machine->product_names[4], "Candy");
    
    machine->product_prices[0] = 150; // $1.50
    machine->product_prices[1] = 150; // $1.50
    machine->product_prices[2] = 100; // $1.00
    machine->product_prices[3] = 200; // $2.00
    machine->product_prices[4] = 125; // $1.25
    
    for (int i = 0; i < 5; i++) {
        machine->product_stock[i] = 5;
    }
    
    machine->set_state = vending_machine_set_state;
    machine->display_status = vending_machine_display_status;
    
    return machine;
}

// Concrete State: Idle
typedef struct {
    State base;
} IdleState;

void idle_insert_coin(State* self, VendingMachine* machine, int amount) {
    machine->inserted_amount += amount;
    printf("💰 Inserted $%.2f. Total: $%.2f\n", amount / 100.0, machine->inserted_amount / 100.0);
    machine->set_state(machine, get_coin_inserted_state());
}

void idle_select_product(State* self, VendingMachine* machine, int product_id) {
    printf("❌ Please insert coins first\n");
}

void idle_dispense(State* self, VendingMachine* machine) {
    printf("❌ No product selected\n");
}

void idle_cancel(State* self, VendingMachine* machine) {
    printf("❌ Nothing to cancel\n");
}

void idle_display_message(State* self) {
    printf("💡 Please insert coins\n");
}

State* get_idle_state() {
    static IdleState idle_state = {0};
    if (strlen(idle_state.base.name) == 0) {
        strcpy(idle_state.base.name, "Idle");
        idle_state.base.insert_coin = idle_insert_coin;
        idle_state.base.select_product = idle_select_product;
        idle_state.base.dispense = idle_dispense;
        idle_state.base.cancel = idle_cancel;
        idle_state.base.display_message = idle_display_message;
    }
    return (State*)&idle_state;
}

// Concrete State: Coin Inserted
typedef struct {
    State base;
} CoinInsertedState;

void coin_inserted_insert_coin(State* self, VendingMachine* machine, int amount) {
    machine->inserted_amount += amount;
    printf("💰 Inserted $%.2f. Total: $%.2f\n", amount / 100.0, machine->inserted_amount / 100.0);
}

void coin_inserted_select_product(State* self, VendingMachine* machine, int product_id) {
    if (product_id < 1 || product_id > 5) {
        printf("❌ Invalid product selection\n");
        return;
    }
    
    int index = product_id - 1;
    
    if (machine->product_stock[index] == 0) {
        printf("❌ %s is out of stock\n", machine->product_names[index]);
        return;
    }
    
    if (machine->inserted_amount < machine->product_prices[index]) {
        printf("❌ Insufficient funds. %s costs $%.2f, you have $%.2f\n", 
               machine->product_names[index], 
               machine->product_prices[index] / 100.0,
               machine->inserted_amount / 100.0);
        return;
    }
    
    strcpy(machine->selected_product_name, machine->product_names[index]);
    machine->selected_product_price = machine->product_prices[index];
    printf("✅ Selected: %s ($%.2f)\n", machine->selected_product_name, machine->selected_product_price / 100.0);
    
    machine->set_state(machine, get_product_selected_state());
}

void coin_inserted_dispense(State* self, VendingMachine* machine) {
    printf("❌ Please select a product first\n");
}

void coin_inserted_cancel(State* self, VendingMachine* machine) {
    printf("💰 Returning $%.2f\n", machine->inserted_amount / 100.0);
    machine->inserted_amount = 0;
    machine->set_state(machine, get_idle_state());
}

void coin_inserted_display_message(State* self) {
    printf("💡 Please select a product\n");
}

State* get_coin_inserted_state() {
    static CoinInsertedState coin_inserted_state = {0};
    if (strlen(coin_inserted_state.base.name) == 0) {
        strcpy(coin_inserted_state.base.name, "Coin Inserted");
        coin_inserted_state.base.insert_coin = coin_inserted_insert_coin;
        coin_inserted_state.base.select_product = coin_inserted_select_product;
        coin_inserted_state.base.dispense = coin_inserted_dispense;
        coin_inserted_state.base.cancel = coin_inserted_cancel;
        coin_inserted_state.base.display_message = coin_inserted_display_message;
    }
    return (State*)&coin_inserted_state;
}

// Concrete State: Product Selected
typedef struct {
    State base;
} ProductSelectedState;

void product_selected_insert_coin(State* self, VendingMachine* machine, int amount) {
    machine->inserted_amount += amount;
    printf("💰 Inserted additional $%.2f. Total: $%.2f\n", amount / 100.0, machine->inserted_amount / 100.0);
}

void product_selected_select_product(State* self, VendingMachine* machine, int product_id) {
    printf("❌ Product already selected. Press dispense or cancel\n");
}

void product_selected_dispense(State* self, VendingMachine* machine) {
    machine->set_state(machine, get_dispensing_state());
    machine->current_state->dispense(machine->current_state, machine);
}

void product_selected_cancel(State* self, VendingMachine* machine) {
    printf("💰 Returning $%.2f\n", machine->inserted_amount / 100.0);
    machine->inserted_amount = 0;
    strcpy(machine->selected_product_name, "");
    machine->selected_product_price = 0;
    machine->set_state(machine, get_idle_state());
}

void product_selected_display_message(State* self) {
    printf("💡 Press dispense button to get your product\n");
}

State* get_product_selected_state() {
    static ProductSelectedState product_selected_state = {0};
    if (strlen(product_selected_state.base.name) == 0) {
        strcpy(product_selected_state.base.name, "Product Selected");
        product_selected_state.base.insert_coin = product_selected_insert_coin;
        product_selected_state.base.select_product = product_selected_select_product;
        product_selected_state.base.dispense = product_selected_dispense;
        product_selected_state.base.cancel = product_selected_cancel;
        product_selected_state.base.display_message = product_selected_display_message;
    }
    return (State*)&product_selected_state;
}

// Concrete State: Dispensing
typedef struct {
    State base;
} DispensingState;

void dispensing_insert_coin(State* self, VendingMachine* machine, int amount) {
    printf("❌ Please wait, dispensing in progress\n");
}

void dispensing_select_product(State* self, VendingMachine* machine, int product_id) {
    printf("❌ Please wait, dispensing in progress\n");
}

void dispensing_dispense(State* self, VendingMachine* machine) {
    printf("📦 Dispensing %s...\n", machine->selected_product_name);
    
    // Find product index and reduce stock
    for (int i = 0; i < 5; i++) {
        if (strcmp(machine->product_names[i], machine->selected_product_name) == 0) {
            machine->product_stock[i]--;
            break;
        }
    }
    
    // Calculate change
    int change = machine->inserted_amount - machine->selected_product_price;
    if (change > 0) {
        printf("💰 Dispensing change: $%.2f\n", change / 100.0);
    }
    
    printf("✅ Enjoy your %s!\n", machine->selected_product_name);
    
    // Reset machine
    machine->inserted_amount = 0;
    strcpy(machine->selected_product_name, "");
    machine->selected_product_price = 0;
    
    machine->set_state(machine, get_idle_state());
}

void dispensing_cancel(State* self, VendingMachine* machine) {
    printf("❌ Cannot cancel while dispensing\n");
}

void dispensing_display_message(State* self) {
    printf("💡 Dispensing product...\n");
}

State* get_dispensing_state() {
    static DispensingState dispensing_state = {0};
    if (strlen(dispensing_state.base.name) == 0) {
        strcpy(dispensing_state.base.name, "Dispensing");
        dispensing_state.base.insert_coin = dispensing_insert_coin;
        dispensing_state.base.select_product = dispensing_select_product;
        dispensing_state.base.dispense = dispensing_dispense;
        dispensing_state.base.cancel = dispensing_cancel;
        dispensing_state.base.display_message = dispensing_display_message;
    }
    return (State*)&dispensing_state;
}

// Concrete State: Out of Order (example of error state)
typedef struct {
    State base;
} OutOfOrderState;

void out_of_order_insert_coin(State* self, VendingMachine* machine, int amount) {
    printf("❌ Machine is out of order\n");
}

void out_of_order_select_product(State* self, VendingMachine* machine, int product_id) {
    printf("❌ Machine is out of order\n");
}

void out_of_order_dispense(State* self, VendingMachine* machine) {
    printf("❌ Machine is out of order\n");
}

void out_of_order_cancel(State* self, VendingMachine* machine) {
    printf("❌ Machine is out of order\n");
}

void out_of_order_display_message(State* self) {
    printf("💡 Machine is out of order - contact service\n");
}

State* get_out_of_order_state() {
    static OutOfOrderState out_of_order_state = {0};
    if (strlen(out_of_order_state.base.name) == 0) {
        strcpy(out_of_order_state.base.name, "Out of Order");
        out_of_order_state.base.insert_coin = out_of_order_insert_coin;
        out_of_order_state.base.select_product = out_of_order_select_product;
        out_of_order_state.base.dispense = out_of_order_dispense;
        out_of_order_state.base.cancel = out_of_order_cancel;
        out_of_order_state.base.display_message = out_of_order_display_message;
    }
    return (State*)&out_of_order_state;
}

void destroy_vending_machine(VendingMachine* machine) {
    if (machine) {
        free(machine);
    }
}

// Example usage
int main() {
    printf("=== STATE PATTERN EXAMPLE ===\n\n");
    
    VendingMachine* machine = create_vending_machine();
    
    printf("--- Vending Machine Simulation ---\n");
    machine->display_status(machine);
    
    printf("\n--- Normal Purchase Flow ---\n");
    
    // Try to select product without coins
    machine->current_state->select_product(machine->current_state, machine, 1);
    
    // Insert coins
    machine->current_state->insert_coin(machine->current_state, machine, 50);  // $0.50
    machine->current_state->insert_coin(machine->current_state, machine, 100); // $1.00
    
    // Try to buy expensive item
    machine->current_state->select_product(machine->current_state, machine, 4); // Chips $2.00
    
    // Insert more coins
    machine->current_state->insert_coin(machine->current_state, machine, 75); // $0.75
    
    // Select affordable item
    machine->current_state->select_product(machine->current_state, machine, 1); // Coke $1.50
    
    // Dispense
    machine->current_state->dispense(machine->current_state, machine);
    
    machine->display_status(machine);
    
    printf("\n--- Cancellation Flow ---\n");
    
    // Insert coins and cancel
    machine->current_state->insert_coin(machine->current_state, machine, 200); // $2.00
    machine->current_state->select_product(machine->current_state, machine, 3); // Water $1.00
    machine->current_state->cancel(machine->current_state, machine);
    
    printf("\n--- Error Conditions ---\n");
    
    // Try invalid operations in dispensing state
    machine->current_state->insert_coin(machine->current_state, machine, 150); // $1.50
    machine->current_state->select_product(machine->current_state, machine, 2); // Pepsi $1.50
    
    printf("Simulating button press during dispensing:\n");
    machine->current_state->dispense(machine->current_state, machine);
    // Try to insert coin during dispensing (this will trigger dispense immediately)
    machine->current_state->insert_coin(machine->current_state, machine, 100);
    
    printf("\n--- Out of Stock Scenario ---\n");
    
    // Deplete stock and try to buy
    machine->product_stock[2] = 0; // Water out of stock
    machine->current_state->insert_coin(machine->current_state, machine, 150);
    machine->current_state->select_product(machine->current_state, machine, 3); // Water
    machine->current_state->cancel(machine->current_state, machine);
    
    printf("\n--- State Pattern Benefits Demonstrated ---\n");
    printf("✅ State-specific behavior is encapsulated in state classes\n");
    printf("✅ State transitions are explicit and controlled\n");
    printf("✅ Easy to add new states without modifying existing code\n");
    printf("✅ Eliminates complex if-else chains\n");
    printf("✅ Each state can have different responses to same input\n");
    
    destroy_vending_machine(machine);
    
    return 0;
}