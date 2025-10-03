/*
 * MEMORY-EFFICIENT PROTOTYPE PATTERN
 * 
 * This version uses shared vtables to reduce memory usage
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Forward declarations
typedef struct Shape Shape;
typedef struct ShapeVTable ShapeVTable;

// Shared function table (vtable)
struct ShapeVTable {
    Shape* (*clone)(Shape* self);
    void (*draw)(Shape* self);
    void (*move)(Shape* self, int dx, int dy);
    const char* type_name;
};

// Lean Shape structure
struct Shape {
    char color[50];
    int x, y;
    ShapeVTable* vtable;  // Only 8 bytes, shared across instances
};

// Circle structure
typedef struct {
    Shape base;
    int radius;
} Circle;

// Circle implementations
Shape* circle_clone(Shape* self) {
    Circle* original = (Circle*)self;
    Circle* clone = (Circle*)malloc(sizeof(Circle));
    *clone = *original;  // Copy all data
    printf("Cloned %s: radius=%d\n", clone->base.vtable->type_name, clone->radius);
    return (Shape*)clone;
}

void circle_draw(Shape* self) {
    Circle* circle = (Circle*)self;
    printf("Drawing %s %s with radius %d at (%d,%d)\n", 
           circle->base.color, circle->base.vtable->type_name, 
           circle->radius, circle->base.x, circle->base.y);
}

void shape_move(Shape* self, int dx, int dy) {
    self->x += dx;
    self->y += dy;
    printf("Moved %s %s to (%d,%d)\n", self->color, self->vtable->type_name, self->x, self->y);
}

// Shared vtable for all circles (only one instance in memory!)
static ShapeVTable circle_vtable = {
    .clone = circle_clone,
    .draw = circle_draw,
    .move = shape_move,
    .type_name = "Circle"
};

Circle* create_circle(const char* color, int x, int y, int radius) {
    Circle* circle = (Circle*)malloc(sizeof(Circle));
    
    strcpy(circle->base.color, color);
    circle->base.x = x;
    circle->base.y = y;
    circle->base.vtable = &circle_vtable;  // Point to shared vtable
    circle->radius = radius;
    
    return circle;
}

// Rectangle structure
typedef struct {
    Shape base;
    int width, height;
} Rectangle;

// Rectangle implementations
Shape* rectangle_clone(Shape* self) {
    Rectangle* original = (Rectangle*)self;
    Rectangle* clone = (Rectangle*)malloc(sizeof(Rectangle));
    *clone = *original;
    printf("Cloned %s: size=%dx%d\n", clone->base.vtable->type_name, 
           clone->width, clone->height);
    return (Shape*)clone;
}

void rectangle_draw(Shape* self) {
    Rectangle* rect = (Rectangle*)self;
    printf("Drawing %s %s %dx%d at (%d,%d)\n", 
           rect->base.color, rect->base.vtable->type_name,
           rect->width, rect->height, rect->base.x, rect->base.y);
}

// Shared vtable for all rectangles
static ShapeVTable rectangle_vtable = {
    .clone = rectangle_clone,
    .draw = rectangle_draw,
    .move = shape_move,
    .type_name = "Rectangle"
};

Rectangle* create_rectangle(const char* color, int x, int y, int width, int height) {
    Rectangle* rect = (Rectangle*)malloc(sizeof(Rectangle));
    
    strcpy(rect->base.color, color);
    rect->base.x = x;
    rect->base.y = y;
    rect->base.vtable = &rectangle_vtable;  // Point to shared vtable
    rect->width = width;
    rect->height = height;
    
    return rect;
}

void destroy_shape(Shape* shape) {
    if (shape) {
        free(shape);
    }
}

// Memory usage analysis
void print_memory_usage() {
    printf("\n=== MEMORY USAGE ANALYSIS ===\n");
    printf("Shape base size: %zu bytes\n", sizeof(Shape));
    printf("Circle size: %zu bytes\n", sizeof(Circle));
    printf("Rectangle size: %zu bytes\n", sizeof(Rectangle));
    printf("ShapeVTable size: %zu bytes (shared)\n", sizeof(ShapeVTable));
    
    printf("\nFor 1000 circles:\n");
    printf("Old approach: %zu KB (with duplicate function pointers)\n", 
           (sizeof(Shape) + sizeof(int) + 3*sizeof(void*)) * 1000 / 1024);
    printf("New approach: %zu KB (with shared vtables)\n", 
           sizeof(Circle) * 1000 / 1024);
}

int main() {
    printf("=== MEMORY-EFFICIENT PROTOTYPE PATTERN ===\n\n");
    
    // Create prototypes
    Circle* circle_template = create_circle("Red", 0, 0, 10);
    Rectangle* rect_template = create_rectangle("Blue", 0, 0, 20, 30);
    
    printf("--- Cloning objects ---\n");
    
    // Clone objects (memory efficient!)
    Shape* circle1 = circle_template->base.vtable->clone((Shape*)circle_template);
    Shape* circle2 = circle_template->base.vtable->clone((Shape*)circle_template);
    Shape* rect1 = rect_template->base.vtable->clone((Shape*)rect_template);
    
    // Modify clones
    strcpy(circle2->color, "Green");
    circle2->vtable->move(circle2, 5, 5);
    
    // Test polymorphism
    printf("\n--- Polymorphic behavior ---\n");
    Shape* shapes[] = {circle1, circle2, rect1};
    for (int i = 0; i < 3; i++) {
        shapes[i]->vtable->draw(shapes[i]);
    }
    
    print_memory_usage();
    
    // Cleanup
    destroy_shape(circle1);
    destroy_shape(circle2);
    destroy_shape(rect1);
    destroy_shape((Shape*)circle_template);
    destroy_shape((Shape*)rect_template);
    
    return 0;
}