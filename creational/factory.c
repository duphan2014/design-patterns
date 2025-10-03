/*
 * FACTORY PATTERN
 * 
 * Problem: Create objects without specifying their exact classes
 * 
 * Use Cases:
 * - Creating UI elements for different platforms
 * - Database connection creation
 * - File format parsers
 * - Vehicle manufacturing
 * 
 * Pros:
 * - Loose coupling between creator and concrete products
 * - Single responsibility principle
 * - Open/closed principle
 * 
 * Cons:
 * - Code complexity increases
 * - Need to create many subclasses
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Product interface (Vehicle)
typedef struct {
    char type[50];
    int wheels;
    void (*start)(void);
    void (*stop)(void);
} Vehicle;

// Concrete Products
void car_start() { printf("Car engine started with key!\n"); }
void car_stop() { printf("Car engine stopped!\n"); }

void motorcycle_start() { printf("Motorcycle kicked to start!\n"); }
void motorcycle_stop() { printf("Motorcycle engine stopped!\n"); }

void bicycle_start() { printf("Started pedaling bicycle!\n"); }
void bicycle_stop() { printf("Stopped pedaling!\n"); }

// Factory function
Vehicle* create_vehicle(const char* type) {
    Vehicle* vehicle = (Vehicle*)malloc(sizeof(Vehicle));
    
    if (strcmp(type, "car") == 0) {
        strcpy(vehicle->type, "Car");
        vehicle->wheels = 4;
        vehicle->start = car_start;
        vehicle->stop = car_stop;
    }
    else if (strcmp(type, "motorcycle") == 0) {
        strcpy(vehicle->type, "Motorcycle");
        vehicle->wheels = 2;
        vehicle->start = motorcycle_start;
        vehicle->stop = motorcycle_stop;
    }
    else if (strcmp(type, "bicycle") == 0) {
        strcpy(vehicle->type, "Bicycle");
        vehicle->wheels = 2;
        vehicle->start = bicycle_start;
        vehicle->stop = bicycle_stop;
    }
    else {
        free(vehicle);
        return NULL;
    }
    
    return vehicle;
}

// Vehicle operations
void drive_vehicle(Vehicle* vehicle) {
    printf("\nDriving %s with %d wheels:\n", vehicle->type, vehicle->wheels);
    vehicle->start();
    printf("Vehicle is moving...\n");
    vehicle->stop();
}

void destroy_vehicle(Vehicle* vehicle) {
    if (vehicle) {
        free(vehicle);
    }
}

// Example usage
int main() {
    printf("=== FACTORY PATTERN EXAMPLE ===\n\n");
    
    // Create different vehicles using factory
    Vehicle* car = create_vehicle("car");
    Vehicle* motorcycle = create_vehicle("motorcycle");
    Vehicle* bicycle = create_vehicle("bicycle");
    Vehicle* invalid = create_vehicle("spaceship");
    
    // Use the vehicles
    if (car) {
        drive_vehicle(car);
        destroy_vehicle(car);
    }
    
    if (motorcycle) {
        drive_vehicle(motorcycle);
        destroy_vehicle(motorcycle);
    }
    
    if (bicycle) {
        drive_vehicle(bicycle);
        destroy_vehicle(bicycle);
    }
    
    if (invalid == NULL) {
        printf("\nInvalid vehicle type - factory returned NULL\n");
    }
    
    return 0;
}