/*
 * SIMPLE COMMAND PATTERN
 * 
 * Core Concept: Turn actions into objects so you can:
 * - Store them
 * - Pass them around  
 * - Undo them
 * - Queue them
 * 
 * Real-world analogy:
 * - TV remote control buttons (each button is a command object)
 * - Press "Volume Up" -> command gets executed
 * - You can undo it, or program it into a macro
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// =============================================================================
// STEP 1: Define what a Command looks like
// =============================================================================

typedef struct Command Command;
struct Command {
    char name[50];
    void (*execute)(Command* self);
    void (*undo)(Command* self);
};

// =============================================================================
// STEP 2: Create the device we want to control (a simple light)
// =============================================================================

typedef struct {
    int is_on;
    int brightness;  // 0-100
} Light;

Light* create_light() {
    Light* light = malloc(sizeof(Light));
    light->is_on = 0;      // Start off
    light->brightness = 50; // Medium brightness
    return light;
}

void show_light_status(Light* light) {
    if (light->is_on) {
        printf("💡 Light is ON (brightness: %d%%)\n", light->brightness);
    } else {
        printf("🔲 Light is OFF\n");
    }
}

// =============================================================================
// STEP 3: Create concrete commands
// =============================================================================

// Command 1: Turn Light On
typedef struct {
    Command base;
    Light* light;
} TurnOnCommand;

void turn_on_execute(Command* self) {
    TurnOnCommand* cmd = (TurnOnCommand*)self;
    cmd->light->is_on = 1;
    printf("🔘 Turned light ON\n");
    show_light_status(cmd->light);
}

void turn_on_undo(Command* self) {
    TurnOnCommand* cmd = (TurnOnCommand*)self;
    cmd->light->is_on = 0;
    printf("↩️ Undoing: Turned light OFF\n");
    show_light_status(cmd->light);
}

Command* create_turn_on_command(Light* light) {
    TurnOnCommand* cmd = malloc(sizeof(TurnOnCommand));
    strcpy(cmd->base.name, "Turn On");
    cmd->light = light;
    cmd->base.execute = turn_on_execute;
    cmd->base.undo = turn_on_undo;
    return (Command*)cmd;
}

// Command 2: Turn Light Off
typedef struct {
    Command base;
    Light* light;
} TurnOffCommand;

void turn_off_execute(Command* self) {
    TurnOffCommand* cmd = (TurnOffCommand*)self;
    cmd->light->is_on = 0;
    printf("🔘 Turned light OFF\n");
    show_light_status(cmd->light);
}

void turn_off_undo(Command* self) {
    TurnOffCommand* cmd = (TurnOffCommand*)self;
    cmd->light->is_on = 1;
    printf("↩️ Undoing: Turned light ON\n");
    show_light_status(cmd->light);
}

Command* create_turn_off_command(Light* light) {
    TurnOffCommand* cmd = malloc(sizeof(TurnOffCommand));
    strcpy(cmd->base.name, "Turn Off");
    cmd->light = light;
    cmd->base.execute = turn_off_execute;
    cmd->base.undo = turn_off_undo;
    return (Command*)cmd;
}

// Command 3: Change Brightness
typedef struct {
    Command base;
    Light* light;
    int new_brightness;
    int old_brightness;  // For undo
} BrightnessCommand;

void brightness_execute(Command* self) {
    BrightnessCommand* cmd = (BrightnessCommand*)self;
    cmd->old_brightness = cmd->light->brightness;  // Save for undo
    cmd->light->brightness = cmd->new_brightness;
    cmd->light->is_on = 1;  // Turn on if changing brightness
    printf("🔆 Set brightness to %d%%\n", cmd->new_brightness);
    show_light_status(cmd->light);
}

void brightness_undo(Command* self) {
    BrightnessCommand* cmd = (BrightnessCommand*)self;
    cmd->light->brightness = cmd->old_brightness;
    printf("↩️ Undoing: Restored brightness to %d%%\n", cmd->old_brightness);
    show_light_status(cmd->light);
}

Command* create_brightness_command(Light* light, int brightness) {
    BrightnessCommand* cmd = malloc(sizeof(BrightnessCommand));
    snprintf(cmd->base.name, sizeof(cmd->base.name), "Set Brightness %d%%", brightness);
    cmd->light = light;
    cmd->new_brightness = brightness;
    cmd->old_brightness = 0;
    cmd->base.execute = brightness_execute;
    cmd->base.undo = brightness_undo;
    return (Command*)cmd;
}

// =============================================================================
// STEP 4: Simple Remote Control (stores and executes commands)
// =============================================================================

#define MAX_HISTORY 10

typedef struct {
    Command* history[MAX_HISTORY];
    int history_count;
} RemoteControl;

RemoteControl* create_remote() {
    RemoteControl* remote = malloc(sizeof(RemoteControl));
    remote->history_count = 0;
    return remote;
}

void press_button(RemoteControl* remote, Command* command) {
    printf("\n🔘 Pressing '%s' button\n", command->name);
    command->execute(command);
    
    // Save to history for undo
    if (remote->history_count < MAX_HISTORY) {
        remote->history[remote->history_count] = command;
        remote->history_count++;
    }
}

void press_undo(RemoteControl* remote) {
    if (remote->history_count > 0) {
        Command* last_command = remote->history[remote->history_count - 1];
        printf("\n↩️ Pressing UNDO button\n");
        last_command->undo(last_command);
        remote->history_count--;
    } else {
        printf("\n❌ Nothing to undo\n");
    }
}

// =============================================================================
// DEMO: Show how it works
// =============================================================================

int main() {
    printf("=== SIMPLE COMMAND PATTERN ===\n");
    
    // Create our light and remote control
    Light* bedroom_light = create_light();
    RemoteControl* remote = create_remote();
    
    printf("\nInitial state:\n");
    show_light_status(bedroom_light);
    
    // Create command objects (like programming remote buttons)
    Command* turn_on = create_turn_on_command(bedroom_light);
    Command* turn_off = create_turn_off_command(bedroom_light);
    Command* dim = create_brightness_command(bedroom_light, 25);
    Command* bright = create_brightness_command(bedroom_light, 90);
    
    printf("\n--- Using the remote control ---\n");
    
    // Press buttons (execute commands)
    press_button(remote, turn_on);
    press_button(remote, bright);
    press_button(remote, dim);
    press_button(remote, turn_off);
    
    printf("\n--- Using UNDO ---\n");
    
    // Undo last few actions
    press_undo(remote);  // Undo turn off
    press_undo(remote);  // Undo dim
    press_undo(remote);  // Undo bright
    press_undo(remote);  // Undo turn on
    press_undo(remote);  // Nothing to undo
    
    printf("\n--- Command Pattern as Macro ---\n");
    
    // You can store commands and execute them later (like a macro)
    Command* bedtime_routine[] = {turn_on, dim, turn_off};
    
    printf("Executing bedtime routine:\n");
    for (int i = 0; i < 3; i++) {
        printf("  Step %d: ", i + 1);
        bedtime_routine[i]->execute(bedtime_routine[i]);
    }
    
    printf("\n✨ Command Pattern Benefits:\n");
    printf("   • Actions become objects you can store and pass around\n");
    printf("   • Easy undo/redo functionality\n");
    printf("   • Can create macros (sequences of commands)\n");
    printf("   • Decouple the button from the action\n");
    printf("   • Same button can do different things in different contexts\n");
    
    // Cleanup
    free(bedroom_light);
    free(remote);
    free(turn_on);
    free(turn_off);
    free(dim);
    free(bright);
    
    return 0;
}