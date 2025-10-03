/*
 * FACADE PATTERN
 * 
 * Problem: Provide a simplified interface to a complex subsystem
 * 
 * Use Cases:
 * - API wrappers
 * - Complex library abstractions
 * - System initialization
 * - Database access layers
 * 
 * Pros:
 * - Isolates clients from complex subsystem components
 * - Promotes loose coupling
 * - Simplifies the interface
 * 
 * Cons:
 * - Facade can become a god object
 * - May not provide all functionality needed
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Complex Subsystem Components

// CPU Subsystem
typedef struct {
    char model[100];
    double frequency;
    bool is_running;
} CPU;

void cpu_initialize(CPU* cpu) {
    strcpy(cpu->model, "Intel Core i7");
    cpu->frequency = 3.5;
    cpu->is_running = false;
    printf("CPU initialized: %s @ %.1fGHz\n", cpu->model, cpu->frequency);
}

void cpu_start(CPU* cpu) {
    cpu->is_running = true;
    printf("CPU started\n");
}

void cpu_shutdown(CPU* cpu) {
    cpu->is_running = false;
    printf("CPU shutdown\n");
}

// Memory Subsystem
typedef struct {
    int size_gb;
    int used_mb;
    bool is_initialized;
} Memory;

void memory_initialize(Memory* memory) {
    memory->size_gb = 16;
    memory->used_mb = 0;
    memory->is_initialized = true;
    printf("Memory initialized: %dGB RAM\n", memory->size_gb);
}

void memory_load_os(Memory* memory) {
    memory->used_mb = 2048;  // 2GB for OS
    printf("Operating system loaded into memory (%dMB used)\n", memory->used_mb);
}

void memory_cleanup(Memory* memory) {
    memory->used_mb = 0;
    printf("Memory cleaned up\n");
}

// Hard Drive Subsystem
typedef struct {
    int capacity_gb;
    char boot_sector[256];
    bool is_ready;
} HardDrive;

void hdd_initialize(HardDrive* hdd) {
    hdd->capacity_gb = 1000;
    strcpy(hdd->boot_sector, "Boot sector loaded");
    hdd->is_ready = false;
    printf("Hard drive initialized: %dGB capacity\n", hdd->capacity_gb);
}

void hdd_read_boot_sector(HardDrive* hdd) {
    hdd->is_ready = true;
    printf("Boot sector read: %s\n", hdd->boot_sector);
}

void hdd_spin_down(HardDrive* hdd) {
    hdd->is_ready = false;
    printf("Hard drive spinning down\n");
}

// Graphics Subsystem
typedef struct {
    char gpu_model[100];
    int vram_mb;
    bool display_active;
} Graphics;

void graphics_initialize(Graphics* graphics) {
    strcpy(graphics->gpu_model, "NVIDIA RTX 4070");
    graphics->vram_mb = 8192;
    graphics->display_active = false;
    printf("Graphics initialized: %s with %dMB VRAM\n", 
           graphics->gpu_model, graphics->vram_mb);
}

void graphics_enable_display(Graphics* graphics) {
    graphics->display_active = true;
    printf("Display enabled\n");
}

void graphics_disable_display(Graphics* graphics) {
    graphics->display_active = false;
    printf("Display disabled\n");
}

// Network Subsystem
typedef struct {
    char interface[50];
    bool is_connected;
    char ip_address[16];
} Network;

void network_initialize(Network* network) {
    strcpy(network->interface, "Ethernet");
    network->is_connected = false;
    strcpy(network->ip_address, "0.0.0.0");
    printf("Network interface initialized: %s\n", network->interface);
}

void network_connect(Network* network) {
    network->is_connected = true;
    strcpy(network->ip_address, "192.168.1.100");
    printf("Network connected: IP %s\n", network->ip_address);
}

void network_disconnect(Network* network) {
    network->is_connected = false;
    strcpy(network->ip_address, "0.0.0.0");
    printf("Network disconnected\n");
}

// FACADE: Computer System
typedef struct {
    CPU cpu;
    Memory memory;
    HardDrive hdd;
    Graphics graphics;
    Network network;
    bool is_running;
} ComputerFacade;

ComputerFacade* create_computer() {
    ComputerFacade* computer = (ComputerFacade*)malloc(sizeof(ComputerFacade));
    computer->is_running = false;
    return computer;
}

// Simplified interface methods
void computer_start(ComputerFacade* computer) {
    printf("=== Starting Computer ===\n");
    
    // Complex startup sequence hidden from client
    cpu_initialize(&computer->cpu);
    memory_initialize(&computer->memory);
    hdd_initialize(&computer->hdd);
    graphics_initialize(&computer->graphics);
    network_initialize(&computer->network);
    
    printf("\nBooting sequence:\n");
    cpu_start(&computer->cpu);
    hdd_read_boot_sector(&computer->hdd);
    memory_load_os(&computer->memory);
    graphics_enable_display(&computer->graphics);
    network_connect(&computer->network);
    
    computer->is_running = true;
    printf("\n✅ Computer startup complete!\n");
}

void computer_shutdown(ComputerFacade* computer) {
    printf("\n=== Shutting Down Computer ===\n");
    
    // Complex shutdown sequence hidden from client
    network_disconnect(&computer->network);
    graphics_disable_display(&computer->graphics);
    memory_cleanup(&computer->memory);
    hdd_spin_down(&computer->hdd);
    cpu_shutdown(&computer->cpu);
    
    computer->is_running = false;
    printf("✅ Computer shutdown complete!\n");
}

void computer_restart(ComputerFacade* computer) {
    printf("\n=== Restarting Computer ===\n");
    if (computer->is_running) {
        computer_shutdown(computer);
    }
    printf("\nWaiting...\n");
    computer_start(computer);
}

void computer_status(ComputerFacade* computer) {
    printf("\n=== Computer Status ===\n");
    printf("System Status: %s\n", computer->is_running ? "Running" : "Shutdown");
    if (computer->is_running) {
        printf("CPU: %s (%.1fGHz) - %s\n", 
               computer->cpu.model, computer->cpu.frequency,
               computer->cpu.is_running ? "Running" : "Stopped");
        printf("Memory: %dGB total, %dMB used\n", 
               computer->memory.size_gb, computer->memory.used_mb);
        printf("Storage: %dGB HDD - %s\n", 
               computer->hdd.capacity_gb,
               computer->hdd.is_ready ? "Ready" : "Not Ready");
        printf("Graphics: %s - Display %s\n", 
               computer->graphics.gpu_model,
               computer->graphics.display_active ? "Active" : "Inactive");
        printf("Network: %s - %s (IP: %s)\n", 
               computer->network.interface,
               computer->network.is_connected ? "Connected" : "Disconnected",
               computer->network.ip_address);
    }
    printf("========================\n");
}

void destroy_computer(ComputerFacade* computer) {
    if (computer) {
        if (computer->is_running) {
            computer_shutdown(computer);
        }
        free(computer);
    }
}

// Example usage
int main() {
    printf("=== FACADE PATTERN EXAMPLE ===\n\n");
    
    // Client uses simple interface, doesn't need to know about subsystems
    ComputerFacade* my_computer = create_computer();
    
    // Simple operations hide complex subsystem interactions
    computer_start(my_computer);
    
    computer_status(my_computer);
    
    computer_restart(my_computer);
    
    computer_status(my_computer);
    
    computer_shutdown(my_computer);
    
    computer_status(my_computer);
    
    printf("\nFacade Pattern Benefits Demonstrated:\n");
    printf("✅ Client doesn't need to know about CPU, Memory, HDD, etc.\n");
    printf("✅ Complex startup/shutdown sequences are simplified\n");
    printf("✅ Easy to use interface for common operations\n");
    printf("✅ Subsystem changes don't affect client code\n");
    
    destroy_computer(my_computer);
    
    return 0;
}