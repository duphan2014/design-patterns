/*
 * SIMPLE STATE PATTERN
 * 
 * Core Concept: An object changes its behavior when its internal state changes
 * It appears as if the object changed its class
 * 
 * Real-world analogy:
 * - A traffic light: Red state, Yellow state, Green state
 * - Each state handles the "next()" action differently
 * - Red -> Green, Yellow -> Red, Green -> Yellow
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Forward declarations
typedef struct TrafficLight TrafficLight;
typedef struct State State;

// =============================================================================
// STEP 1: Define what a State looks like
// =============================================================================

struct State {
    char name[20];
    char color[10];
    void (*handle_next)(TrafficLight* light);
    void (*show)(State* self);
};

// =============================================================================
// STEP 2: The Context (Traffic Light that has different states)
// =============================================================================

struct TrafficLight {
    State* current_state;
    int cycle_count;
};

void traffic_light_next(TrafficLight* light) {
    printf("\n🚦 Button pressed! Changing light...\n");
    light->current_state->handle_next(light);
    light->cycle_count++;
}

void traffic_light_show_status(TrafficLight* light) {
    printf("Current state: ");
    light->current_state->show(light->current_state);
    printf("   (Cycle #%d)\n", light->cycle_count);
}

void traffic_light_set_state(TrafficLight* light, State* new_state) {
    light->current_state = new_state;
    printf("🔄 Changed to: %s\n", new_state->name);
}

// =============================================================================
// STEP 3: Concrete States
// =============================================================================

// Red State
void red_handle_next(TrafficLight* light);
void red_show(State* self) {
    printf("🔴 RED - STOP");
}

State red_state = {
    .name = "Red State",
    .color = "red",
    .handle_next = red_handle_next,
    .show = red_show
};

// Yellow State  
void yellow_handle_next(TrafficLight* light);
void yellow_show(State* self) {
    printf("🟡 YELLOW - CAUTION");
}

State yellow_state = {
    .name = "Yellow State", 
    .color = "yellow",
    .handle_next = yellow_handle_next,
    .show = yellow_show
};

// Green State
void green_handle_next(TrafficLight* light);
void green_show(State* self) {
    printf("🟢 GREEN - GO");
}

State green_state = {
    .name = "Green State",
    .color = "green", 
    .handle_next = green_handle_next,
    .show = green_show
};

// =============================================================================
// STEP 4: State Transition Logic (each state knows what comes next)
// =============================================================================

void red_handle_next(TrafficLight* light) {
    printf("Red says: 'Time to go!' -> Switching to Green\n");
    traffic_light_set_state(light, &green_state);
}

void yellow_handle_next(TrafficLight* light) {
    printf("Yellow says: 'Stop now!' -> Switching to Red\n");
    traffic_light_set_state(light, &red_state);
}

void green_handle_next(TrafficLight* light) {
    printf("Green says: 'Slow down!' -> Switching to Yellow\n");
    traffic_light_set_state(light, &yellow_state);
}

// =============================================================================
// STEP 5: Create and manage traffic light
// =============================================================================

TrafficLight* create_traffic_light() {
    TrafficLight* light = malloc(sizeof(TrafficLight));
    light->current_state = &red_state;  // Start with red
    light->cycle_count = 0;
    return light;
}

void destroy_traffic_light(TrafficLight* light) {
    free(light);
}

// =============================================================================
// BONUS: Let's add a pedestrian crossing button example too
// =============================================================================

typedef struct {
    State* current_state;
    int people_waiting;
} CrosswalkLight;

// Pedestrian states
void walk_handle_next(TrafficLight* light);
void walk_show(State* self) {
    printf("🚶 WALK - Safe to cross");
}

void dont_walk_handle_next(TrafficLight* light);
void dont_walk_show(State* self) {
    printf("🚫 DON'T WALK - Wait");
}

State walk_state = {"Walk", "green", walk_handle_next, walk_show};
State dont_walk_state = {"Don't Walk", "red", dont_walk_handle_next, dont_walk_show};

void walk_handle_next(TrafficLight* light) {
    printf("Walk timer expired -> Don't Walk\n");
    traffic_light_set_state(light, &dont_walk_state);
}

void dont_walk_handle_next(TrafficLight* light) {
    printf("Button pressed -> Walk\n");
    traffic_light_set_state(light, &walk_state);
}

// =============================================================================
// DEMO: Show how different states handle the same action differently
// =============================================================================

int main() {
    printf("=== SIMPLE STATE PATTERN ===\n");
    printf("Traffic Light Example\n\n");
    
    // Create traffic light
    TrafficLight* intersection = create_traffic_light();
    
    printf("Initial state:\n");
    traffic_light_show_status(intersection);
    
    printf("\n--- Watch how each state handles 'next' differently ---\n");
    
    // Same action (next), different behavior based on current state
    for (int i = 0; i < 6; i++) {
        traffic_light_next(intersection);
        traffic_light_show_status(intersection);
        printf("\n");
    }
    
    printf("--- Let's simulate a pedestrian crossing ---\n");
    
    // Create pedestrian light (reusing TrafficLight structure)
    TrafficLight* crosswalk = create_traffic_light();
    traffic_light_set_state(crosswalk, &dont_walk_state);
    
    printf("\nPedestrian approaches:\n");
    traffic_light_show_status(crosswalk);
    
    printf("\nPedestrian presses button:\n");
    traffic_light_next(crosswalk);
    traffic_light_show_status(crosswalk);
    
    printf("\nWalk timer expires:\n");
    traffic_light_next(crosswalk);
    traffic_light_show_status(crosswalk);
    
    printf("\n✨ State Pattern Benefits:\n");
    printf("   • Same method call (next) does different things based on state\n");
    printf("   • No messy if/switch statements in the main object\n");
    printf("   • Each state encapsulates its own behavior\n");
    printf("   • Easy to add new states without changing existing code\n");
    printf("   • State transitions are explicit and clear\n");
    
    printf("\n🎯 Key Insight:\n");
    printf("   The TrafficLight doesn't need to know about state logic.\n");
    printf("   Each state knows what to do and where to go next.\n");
    printf("   This makes the code much cleaner and easier to extend!\n");
    
    // Cleanup
    destroy_traffic_light(intersection);
    destroy_traffic_light(crosswalk);
    
    return 0;
}