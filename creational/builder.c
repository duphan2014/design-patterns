/*
 * BUILDER PATTERN
 * 
 * Problem: Construct complex objects step by step
 * 
 * Use Cases:
 * - SQL query builders
 * - Configuration object creation
 * - Complex document generation
 * - Computer assembly
 * 
 * Pros:
 * - Step-by-step construction
 * - Same construction process can create different representations
 * - Better control over construction process
 * 
 * Cons:
 * - Increased code complexity
 * - More classes to maintain
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Product: Computer
typedef struct {
    char cpu[100];
    char gpu[100];
    int ram_gb;
    int storage_gb;
    char motherboard[100];
    char case_type[50];
    int total_cost;
} Computer;

// Builder structure
typedef struct {
    Computer* computer;
} ComputerBuilder;

// Builder functions
ComputerBuilder* create_builder() {
    ComputerBuilder* builder = (ComputerBuilder*)malloc(sizeof(ComputerBuilder));
    builder->computer = (Computer*)malloc(sizeof(Computer));
    
    // Initialize with defaults
    strcpy(builder->computer->cpu, "None");
    strcpy(builder->computer->gpu, "Integrated");
    builder->computer->ram_gb = 0;
    builder->computer->storage_gb = 0;
    strcpy(builder->computer->motherboard, "Basic");
    strcpy(builder->computer->case_type, "Mini ITX");
    builder->computer->total_cost = 0;
    
    return builder;
}

ComputerBuilder* set_cpu(ComputerBuilder* builder, const char* cpu, int cost) {
    strcpy(builder->computer->cpu, cpu);
    builder->computer->total_cost += cost;
    printf("Added CPU: %s (+$%d)\n", cpu, cost);
    return builder;
}

ComputerBuilder* set_gpu(ComputerBuilder* builder, const char* gpu, int cost) {
    strcpy(builder->computer->gpu, gpu);
    builder->computer->total_cost += cost;
    printf("Added GPU: %s (+$%d)\n", gpu, cost);
    return builder;
}

ComputerBuilder* set_ram(ComputerBuilder* builder, int gb, int cost) {
    builder->computer->ram_gb = gb;
    builder->computer->total_cost += cost;
    printf("Added RAM: %dGB (+$%d)\n", gb, cost);
    return builder;
}

ComputerBuilder* set_storage(ComputerBuilder* builder, int gb, int cost) {
    builder->computer->storage_gb = gb;
    builder->computer->total_cost += cost;
    printf("Added Storage: %dGB (+$%d)\n", gb, cost);
    return builder;
}

ComputerBuilder* set_motherboard(ComputerBuilder* builder, const char* motherboard, int cost) {
    strcpy(builder->computer->motherboard, motherboard);
    builder->computer->total_cost += cost;
    printf("Added Motherboard: %s (+$%d)\n", motherboard, cost);
    return builder;
}

ComputerBuilder* set_case(ComputerBuilder* builder, const char* case_type, int cost) {
    strcpy(builder->computer->case_type, case_type);
    builder->computer->total_cost += cost;
    printf("Added Case: %s (+$%d)\n", case_type, cost);
    return builder;
}

Computer* build(ComputerBuilder* builder) {
    Computer* result = builder->computer;
    free(builder);
    return result;
}

// Display computer specs
void display_computer(Computer* computer) {
    printf("\n=== COMPUTER SPECIFICATIONS ===\n");
    printf("CPU: %s\n", computer->cpu);
    printf("GPU: %s\n", computer->gpu);
    printf("RAM: %dGB\n", computer->ram_gb);
    printf("Storage: %dGB\n", computer->storage_gb);
    printf("Motherboard: %s\n", computer->motherboard);
    printf("Case: %s\n", computer->case_type);
    printf("Total Cost: $%d\n", computer->total_cost);
    printf("===============================\n");
}

// Director - knows how to build specific configurations
Computer* build_gaming_computer() {
    printf("\nBuilding Gaming Computer:\n");
    return build(
        set_case(
            set_motherboard(
                set_storage(
                    set_ram(
                        set_gpu(
                            set_cpu(create_builder(), "Intel i7-12700K", 400),
                            "RTX 4070", 600),
                        32, 200),
                    1000, 100),
                "ASUS ROG Strix", 250),
            "Mid Tower RGB", 150)
    );
}

Computer* build_office_computer() {
    printf("\nBuilding Office Computer:\n");
    return build(
        set_case(
            set_storage(
                set_ram(
                    set_cpu(create_builder(), "Intel i5-12400", 200),
                    16, 80),
                500, 50),
            "Small Form Factor", 80)
    );
}

void destroy_computer(Computer* computer) {
    if (computer) {
        free(computer);
    }
}

// Example usage
int main() {
    printf("=== BUILDER PATTERN EXAMPLE ===\n");
    
    // Build gaming computer using director
    Computer* gaming_pc = build_gaming_computer();
    display_computer(gaming_pc);
    
    // Build office computer using director
    Computer* office_pc = build_office_computer();
    display_computer(office_pc);
    
    // Build custom computer step by step
    printf("\nBuilding Custom Computer:\n");
    Computer* custom_pc = build(
        set_storage(
            set_ram(
                set_gpu(
                    set_cpu(create_builder(), "AMD Ryzen 9 7900X", 500),
                    "RTX 4090", 1200),
                64, 400),
            2000, 200)
    );
    display_computer(custom_pc);
    
    // Cleanup
    destroy_computer(gaming_pc);
    destroy_computer(office_pc);
    destroy_computer(custom_pc);
    
    return 0;
}