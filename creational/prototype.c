/*
 * PROTOTYPE PATTERN
 * 
 * Problem: Create objects by cloning existing instances instead of creating from scratch
 * 
 * Use Cases:
 * - Expensive object creation
 * - Game character templates
 * - Document templates
 * - Configuration presets
 * 
 * Pros:
 * - Reduced subclassing
 * - Clone objects without knowing their concrete classes
 * - Performance benefits for expensive operations
 * 
 * Cons:
 * - Complex objects with circular references are hard to clone
 * - Deep vs shallow copy considerations
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Prototype interface
typedef struct Shape Shape;
struct Shape {
    char type[50];
    char color[50];
    int x, y;
    Shape* (*clone)(Shape* self);
    void (*draw)(Shape* self);
    void (*move)(Shape* self, int dx, int dy);
};

// Concrete Prototype: Circle
typedef struct {
    Shape base;
    int radius;
} Circle;

Shape* circle_clone(Shape* self) {
    Circle* original = (Circle*)self;
    Circle* clone = (Circle*)malloc(sizeof(Circle));
    
    // Copy all properties
    *clone = *original;
    
    printf("Cloned Circle: color=%s, radius=%d, position=(%d,%d)\n", 
           clone->base.color, clone->radius, clone->base.x, clone->base.y);
    
    return (Shape*)clone;
}

void circle_draw(Shape* self) {
    Circle* circle = (Circle*)self;
    printf("Drawing %s circle with radius %d at (%d,%d)\n", 
           circle->base.color, circle->radius, circle->base.x, circle->base.y);
}

void shape_move(Shape* self, int dx, int dy) {
    self->x += dx;
    self->y += dy;
    printf("Moved %s %s to (%d,%d)\n", self->color, self->type, self->x, self->y);
}

Circle* create_circle(const char* color, int x, int y, int radius) {
    Circle* circle = (Circle*)malloc(sizeof(Circle));
    
    strcpy(circle->base.type, "Circle");
    strcpy(circle->base.color, color);
    circle->base.x = x;
    circle->base.y = y;
    circle->base.clone = circle_clone;
    circle->base.draw = circle_draw;
    circle->base.move = shape_move;
    circle->radius = radius;
    
    return circle;
}

// Concrete Prototype: Rectangle
typedef struct {
    Shape base;
    int width, height;
} Rectangle;

Shape* rectangle_clone(Shape* self) {
    Rectangle* original = (Rectangle*)self;
    Rectangle* clone = (Rectangle*)malloc(sizeof(Rectangle));
    
    *clone = *original;
    
    printf("Cloned Rectangle: color=%s, size=%dx%d, position=(%d,%d)\n", 
           clone->base.color, clone->width, clone->height, 
           clone->base.x, clone->base.y);
    
    return (Shape*)clone;
}

void rectangle_draw(Shape* self) {
    Rectangle* rect = (Rectangle*)self;
    printf("Drawing %s rectangle %dx%d at (%d,%d)\n", 
           rect->base.color, rect->width, rect->height, 
           rect->base.x, rect->base.y);
}

Rectangle* create_rectangle(const char* color, int x, int y, int width, int height) {
    Rectangle* rect = (Rectangle*)malloc(sizeof(Rectangle));
    
    strcpy(rect->base.type, "Rectangle");
    strcpy(rect->base.color, color);
    rect->base.x = x;
    rect->base.y = y;
    rect->base.clone = rectangle_clone;
    rect->base.draw = rectangle_draw;
    rect->base.move = shape_move;
    rect->width = width;
    rect->height = height;
    
    return rect;
}

// Prototype Registry
#define MAX_PROTOTYPES 10

typedef struct {
    Shape* prototypes[MAX_PROTOTYPES];
    char names[MAX_PROTOTYPES][50];
    int count;
} PrototypeRegistry;

PrototypeRegistry* create_registry() {
    PrototypeRegistry* registry = (PrototypeRegistry*)malloc(sizeof(PrototypeRegistry));
    registry->count = 0;
    return registry;
}

void register_prototype(PrototypeRegistry* registry, const char* name, Shape* prototype) {
    if (registry->count < MAX_PROTOTYPES) {
        strcpy(registry->names[registry->count], name);
        registry->prototypes[registry->count] = prototype;
        registry->count++;
        printf("Registered prototype: %s\n", name);
    }
}

Shape* get_prototype(PrototypeRegistry* registry, const char* name) {
    for (int i = 0; i < registry->count; i++) {
        if (strcmp(registry->names[i], name) == 0) {
            return registry->prototypes[i]->clone(registry->prototypes[i]);
        } //(*(*registry).prototypes[i]).clone((*registry).prototypes[i]);
    }
    return NULL;
}

void destroy_shape(Shape* shape) {
    if (shape) {
        free(shape);
    }
}

void destroy_registry(PrototypeRegistry* registry) {
    if (registry) {
        for (int i = 0; i < registry->count; i++) {
            destroy_shape(registry->prototypes[i]);
        }
        free(registry);
    }
}

// Example usage
int main() {
    printf("=== PROTOTYPE PATTERN EXAMPLE ===\n\n");
    
    // Create prototype registry
    PrototypeRegistry* registry = create_registry();
    
    // Create and register prototypes
    Circle* red_circle = create_circle("Red", 0, 0, 10);
    Rectangle* blue_rect = create_rectangle("Blue", 0, 0, 20, 30);
    
    register_prototype(registry, "default_circle", (Shape*)red_circle);
    register_prototype(registry, "default_rectangle", (Shape*)blue_rect);
    
    printf("\n--- Creating objects by cloning prototypes ---\n");
    
    // Clone prototypes to create new objects
    Shape* circle1 = get_prototype(registry, "default_circle");
    Shape* circle2 = get_prototype(registry, "default_circle");
    Shape* rect1 = get_prototype(registry, "default_rectangle");
    
    // Modify cloned objects
    strcpy(circle2->color, "Green");
    circle2->move(circle2, 5, 5);
    
    rect1->move(rect1, 10, 15);
    
    printf("\n--- Drawing all shapes ---\n");
    circle1->draw(circle1);
    circle2->draw(circle2);
    rect1->draw(rect1);
    
    // Try to clone non-existent prototype
    Shape* invalid = get_prototype(registry, "triangle");
    if (invalid == NULL) {
        printf("\nPrototype 'triangle' not found in registry\n");
    }
    
    // Cleanup
    destroy_shape(circle1);
    destroy_shape(circle2);
    destroy_shape(rect1);
    destroy_registry(registry);
    
    return 0;
}