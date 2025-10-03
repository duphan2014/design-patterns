// Let's create a simple example to demonstrate forward declaration

#include <stdio.h>
#include <stdlib.h>

// WITHOUT forward declaration - this would cause problems
// Let's see what happens...

/*
// ❌ This approach has issues:
struct Shape {
    char name[20];
    Shape* (*clone)(Shape* self);  // Error: 'Shape' not defined yet!
};
*/

// ✅ SOLUTION: Forward declaration
typedef struct Shape Shape;  // "Shape exists, I'll define it below"

// Now I can define Shape with self-references
struct Shape {
    char name[20];
    Shape* (*clone)(Shape* self);  // ✅ Works! Compiler knows Shape exists
    void (*print)(Shape* self);
};

// Implementation functions
Shape* circle_clone(Shape* self) {
    Shape* copy = malloc(sizeof(Shape));
    *copy = *self;  // Simple copy
    printf("Cloned %s\n", self->name);
    return copy;
}

void circle_print(Shape* self) {
    printf("I am a %s\n", self->name);
}

// Factory function
Shape* create_circle() {
    Shape* circle = malloc(sizeof(Shape));
    strcpy(circle->name, "Circle");
    circle->clone = circle_clone;  // Function pointer assignment
    circle->print = circle_print;
    return circle;
}

int main() {
    printf("=== Forward Declaration Demo ===\n\n");
    
    // Create original
    Shape* original = create_circle();
    original->print(original);
    
    // Clone it using the function pointer
    Shape* copy = original->clone(original);
    copy->print(copy);
    
    printf("\nOriginal address: %p\n", (void*)original);
    printf("Copy address: %p\n", (void*)copy);
    printf("Different objects? %s\n", (original != copy) ? "Yes" : "No");
    
    free(original);
    free(copy);
    return 0;
}