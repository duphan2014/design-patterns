/*
 * VISITOR PATTERN
 * 
 * Problem: Separate algorithms from the objects on which they operate
 * 
 * Use Cases:
 * - Compiler design (AST traversal)
 * - Document processing
 * - Geometry calculations
 * - File system operations
 * 
 * Pros:
 * - Easy to add new operations
 * - Related operations are grouped together
 * - Accumulates state during traversal
 * 
 * Cons:
 * - Hard to add new element types
 * - May break encapsulation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Forward declarations
typedef struct Visitor Visitor;
typedef struct Shape Shape;
typedef struct Circle Circle;
typedef struct Rectangle Rectangle;
typedef struct Triangle Triangle;

// Visitor interface
struct Visitor {
    char name[100];
    double result;
    char description[200];
    
    void (*visit_circle)(Visitor* self, Circle* circle);
    void (*visit_rectangle)(Visitor* self, Rectangle* rectangle);
    void (*visit_triangle)(Visitor* self, Triangle* triangle);
    void (*reset)(Visitor* self);
    void (*display_result)(Visitor* self);
    void (*destroy)(Visitor* self);
};

// Element interface
struct Shape {
    char type[50];
    char color[50];
    double x, y; // position
    
    void (*accept)(Shape* self, Visitor* visitor);
    void (*display_info)(Shape* self);
    void (*destroy)(Shape* self);
};

// Concrete Element 1: Circle
struct Circle {
    Shape base;
    double radius;
};

void circle_accept(Shape* self, Visitor* visitor) {
    Circle* circle = (Circle*)self;
    visitor->visit_circle(visitor, circle);
}

void circle_display_info(Shape* self) {
    Circle* circle = (Circle*)self;
    printf("🔵 Circle: radius=%.1f, color=%s, position=(%.1f,%.1f)\n", 
           circle->radius, circle->base.color, circle->base.x, circle->base.y);
}

void circle_destroy(Shape* self) {
    if (self) {
        free(self);
    }
}

Shape* create_circle(double x, double y, double radius, const char* color) {
    Circle* circle = (Circle*)malloc(sizeof(Circle));
    
    strcpy(circle->base.type, "Circle");
    strcpy(circle->base.color, color);
    circle->base.x = x;
    circle->base.y = y;
    circle->radius = radius;
    
    circle->base.accept = circle_accept;
    circle->base.display_info = circle_display_info;
    circle->base.destroy = circle_destroy;
    
    return (Shape*)circle;
}

// Concrete Element 2: Rectangle
struct Rectangle {
    Shape base;
    double width, height;
};

void rectangle_accept(Shape* self, Visitor* visitor) {
    Rectangle* rectangle = (Rectangle*)self;
    visitor->visit_rectangle(visitor, rectangle);
}

void rectangle_display_info(Shape* self) {
    Rectangle* rectangle = (Rectangle*)self;
    printf("🟦 Rectangle: size=%.1fx%.1f, color=%s, position=(%.1f,%.1f)\n", 
           rectangle->width, rectangle->height, rectangle->base.color, 
           rectangle->base.x, rectangle->base.y);
}

void rectangle_destroy(Shape* self) {
    if (self) {
        free(self);
    }
}

Shape* create_rectangle(double x, double y, double width, double height, const char* color) {
    Rectangle* rectangle = (Rectangle*)malloc(sizeof(Rectangle));
    
    strcpy(rectangle->base.type, "Rectangle");
    strcpy(rectangle->base.color, color);
    rectangle->base.x = x;
    rectangle->base.y = y;
    rectangle->width = width;
    rectangle->height = height;
    
    rectangle->base.accept = rectangle_accept;
    rectangle->base.display_info = rectangle_display_info;
    rectangle->base.destroy = rectangle_destroy;
    
    return (Shape*)rectangle;
}

// Concrete Element 3: Triangle
struct Triangle {
    Shape base;
    double base_length, height;
};

void triangle_accept(Shape* self, Visitor* visitor) {
    Triangle* triangle = (Triangle*)self;
    visitor->visit_triangle(visitor, triangle);
}

void triangle_display_info(Shape* self) {
    Triangle* triangle = (Triangle*)self;
    printf("🔺 Triangle: base=%.1f, height=%.1f, color=%s, position=(%.1f,%.1f)\n", 
           triangle->base_length, triangle->height, triangle->base.color, 
           triangle->base.x, triangle->base.y);
}

void triangle_destroy(Shape* self) {
    if (self) {
        free(self);
    }
}

Shape* create_triangle(double x, double y, double base_length, double height, const char* color) {
    Triangle* triangle = (Triangle*)malloc(sizeof(Triangle));
    
    strcpy(triangle->base.type, "Triangle");
    strcpy(triangle->base.color, color);
    triangle->base.x = x;
    triangle->base.y = y;
    triangle->base_length = base_length;
    triangle->height = height;
    
    triangle->base.accept = triangle_accept;
    triangle->base.display_info = triangle_display_info;
    triangle->base.destroy = triangle_destroy;
    
    return (Shape*)triangle;
}

// Base visitor methods
void visitor_reset(Visitor* self) {
    self->result = 0.0;
}

void visitor_display_result(Visitor* self) {
    printf("📊 %s Result: %.2f (%s)\n", self->name, self->result, self->description);
}

void visitor_destroy(Visitor* self) {
    if (self) {
        free(self);
    }
}

// Concrete Visitor 1: Area Calculator
typedef struct {
    Visitor base;
    int shape_count;
} AreaCalculatorVisitor;

void area_calculator_visit_circle(Visitor* self, Circle* circle) {
    AreaCalculatorVisitor* calc = (AreaCalculatorVisitor*)self;
    double area = M_PI * circle->radius * circle->radius;
    calc->base.result += area;
    calc->shape_count++;
    printf("   🔵 Circle area: %.2f\n", area);
}

void area_calculator_visit_rectangle(Visitor* self, Rectangle* rectangle) {
    AreaCalculatorVisitor* calc = (AreaCalculatorVisitor*)self;
    double area = rectangle->width * rectangle->height;
    calc->base.result += area;
    calc->shape_count++;
    printf("   🟦 Rectangle area: %.2f\n", area);
}

void area_calculator_visit_triangle(Visitor* self, Triangle* triangle) {
    AreaCalculatorVisitor* calc = (AreaCalculatorVisitor*)self;
    double area = 0.5 * triangle->base_length * triangle->height;
    calc->base.result += area;
    calc->shape_count++;
    printf("   🔺 Triangle area: %.2f\n", area);
}

void area_calculator_reset(Visitor* self) {
    AreaCalculatorVisitor* calc = (AreaCalculatorVisitor*)self;
    visitor_reset(self);
    calc->shape_count = 0;
}

void area_calculator_display_result(Visitor* self) {
    AreaCalculatorVisitor* calc = (AreaCalculatorVisitor*)self;
    printf("📊 %s: Total area = %.2f from %d shapes\n", 
           self->name, self->result, calc->shape_count);
}

Visitor* create_area_calculator() {
    AreaCalculatorVisitor* calc = (AreaCalculatorVisitor*)malloc(sizeof(AreaCalculatorVisitor));
    
    strcpy(calc->base.name, "Area Calculator");
    strcpy(calc->base.description, "square units");
    calc->base.result = 0.0;
    calc->shape_count = 0;
    
    calc->base.visit_circle = area_calculator_visit_circle;
    calc->base.visit_rectangle = area_calculator_visit_rectangle;
    calc->base.visit_triangle = area_calculator_visit_triangle;
    calc->base.reset = area_calculator_reset;
    calc->base.display_result = area_calculator_display_result;
    calc->base.destroy = visitor_destroy;
    
    return (Visitor*)calc;
}

// Concrete Visitor 2: Perimeter Calculator
typedef struct {
    Visitor base;
    int shape_count;
} PerimeterCalculatorVisitor;

void perimeter_calculator_visit_circle(Visitor* self, Circle* circle) {
    PerimeterCalculatorVisitor* calc = (PerimeterCalculatorVisitor*)self;
    double perimeter = 2 * M_PI * circle->radius;
    calc->base.result += perimeter;
    calc->shape_count++;
    printf("   🔵 Circle perimeter: %.2f\n", perimeter);
}

void perimeter_calculator_visit_rectangle(Visitor* self, Rectangle* rectangle) {
    PerimeterCalculatorVisitor* calc = (PerimeterCalculatorVisitor*)self;
    double perimeter = 2 * (rectangle->width + rectangle->height);
    calc->base.result += perimeter;
    calc->shape_count++;
    printf("   🟦 Rectangle perimeter: %.2f\n", perimeter);
}

void perimeter_calculator_visit_triangle(Visitor* self, Triangle* triangle) {
    PerimeterCalculatorVisitor* calc = (PerimeterCalculatorVisitor*)self;
    // Assuming equilateral triangle for simplicity
    double side = triangle->base_length;
    double perimeter = 3 * side;
    calc->base.result += perimeter;
    calc->shape_count++;
    printf("   🔺 Triangle perimeter: %.2f (assuming equilateral)\n", perimeter);
}

void perimeter_calculator_reset(Visitor* self) {
    PerimeterCalculatorVisitor* calc = (PerimeterCalculatorVisitor*)self;
    visitor_reset(self);
    calc->shape_count = 0;
}

void perimeter_calculator_display_result(Visitor* self) {
    PerimeterCalculatorVisitor* calc = (PerimeterCalculatorVisitor*)self;
    printf("📊 %s: Total perimeter = %.2f from %d shapes\n", 
           self->name, self->result, calc->shape_count);
}

Visitor* create_perimeter_calculator() {
    PerimeterCalculatorVisitor* calc = (PerimeterCalculatorVisitor*)malloc(sizeof(PerimeterCalculatorVisitor));
    
    strcpy(calc->base.name, "Perimeter Calculator");
    strcpy(calc->base.description, "linear units");
    calc->base.result = 0.0;
    calc->shape_count = 0;
    
    calc->base.visit_circle = perimeter_calculator_visit_circle;
    calc->base.visit_rectangle = perimeter_calculator_visit_rectangle;
    calc->base.visit_triangle = perimeter_calculator_visit_triangle;
    calc->base.reset = perimeter_calculator_reset;
    calc->base.display_result = perimeter_calculator_display_result;
    calc->base.destroy = visitor_destroy;
    
    return (Visitor*)calc;
}

// Concrete Visitor 3: Paint Cost Calculator
typedef struct {
    Visitor base;
    double cost_per_square_unit;
    int shapes_painted;
} PaintCostCalculatorVisitor;

void paint_cost_visit_circle(Visitor* self, Circle* circle) {
    PaintCostCalculatorVisitor* calc = (PaintCostCalculatorVisitor*)self;
    double area = M_PI * circle->radius * circle->radius;
    double cost = area * calc->cost_per_square_unit;
    calc->base.result += cost;
    calc->shapes_painted++;
    printf("   🔵 Circle (%s): area=%.2f, cost=$%.2f\n", 
           circle->base.color, area, cost);
}

void paint_cost_visit_rectangle(Visitor* self, Rectangle* rectangle) {
    PaintCostCalculatorVisitor* calc = (PaintCostCalculatorVisitor*)self;
    double area = rectangle->width * rectangle->height;
    double cost = area * calc->cost_per_square_unit;
    calc->base.result += cost;
    calc->shapes_painted++;
    printf("   🟦 Rectangle (%s): area=%.2f, cost=$%.2f\n", 
           rectangle->base.color, area, cost);
}

void paint_cost_visit_triangle(Visitor* self, Triangle* triangle) {
    PaintCostCalculatorVisitor* calc = (PaintCostCalculatorVisitor*)self;
    double area = 0.5 * triangle->base_length * triangle->height;
    double cost = area * calc->cost_per_square_unit;
    calc->base.result += cost;
    calc->shapes_painted++;
    printf("   🔺 Triangle (%s): area=%.2f, cost=$%.2f\n", 
           triangle->base.color, area, cost);
}

void paint_cost_reset(Visitor* self) {
    PaintCostCalculatorVisitor* calc = (PaintCostCalculatorVisitor*)self;
    visitor_reset(self);
    calc->shapes_painted = 0;
}

void paint_cost_display_result(Visitor* self) {
    PaintCostCalculatorVisitor* calc = (PaintCostCalculatorVisitor*)self;
    printf("📊 %s: Total cost = $%.2f for %d shapes (rate: $%.2f/sq unit)\n", 
           self->name, self->result, calc->shapes_painted, calc->cost_per_square_unit);
}

Visitor* create_paint_cost_calculator(double cost_per_square_unit) {
    PaintCostCalculatorVisitor* calc = (PaintCostCalculatorVisitor*)malloc(sizeof(PaintCostCalculatorVisitor));
    
    strcpy(calc->base.name, "Paint Cost Calculator");
    strcpy(calc->base.description, "dollars");
    calc->base.result = 0.0;
    calc->cost_per_square_unit = cost_per_square_unit;
    calc->shapes_painted = 0;
    
    calc->base.visit_circle = paint_cost_visit_circle;
    calc->base.visit_rectangle = paint_cost_visit_rectangle;
    calc->base.visit_triangle = paint_cost_visit_triangle;
    calc->base.reset = paint_cost_reset;
    calc->base.display_result = paint_cost_display_result;
    calc->base.destroy = visitor_destroy;
    
    return (Visitor*)calc;
}

// Shape collection helper
#define MAX_SHAPES 10

typedef struct {
    Shape* shapes[MAX_SHAPES];
    int count;
} ShapeCollection;

ShapeCollection* create_shape_collection() {
    ShapeCollection* collection = (ShapeCollection*)malloc(sizeof(ShapeCollection));
    collection->count = 0;
    return collection;
}

void add_shape(ShapeCollection* collection, Shape* shape) {
    if (collection->count < MAX_SHAPES) {
        collection->shapes[collection->count] = shape;
        collection->count++;
        printf("➕ Added shape: ");
        shape->display_info(shape);
    } else {
        printf("❌ Collection is full\n");
    }
}

void apply_visitor(ShapeCollection* collection, Visitor* visitor) {
    printf("\n🎯 Applying %s to %d shapes:\n", visitor->name, collection->count);
    visitor->reset(visitor);
    
    for (int i = 0; i < collection->count; i++) {
        collection->shapes[i]->accept(collection->shapes[i], visitor);
    }
    
    visitor->display_result(visitor);
}

void destroy_shape_collection(ShapeCollection* collection) {
    if (collection) {
        for (int i = 0; i < collection->count; i++) {
            collection->shapes[i]->destroy(collection->shapes[i]);
        }
        free(collection);
    }
}

// Example usage
int main() {
    printf("=== VISITOR PATTERN EXAMPLE ===\n\n");
    
    // Create shape collection
    ShapeCollection* shapes = create_shape_collection();
    
    printf("--- Building shape collection ---\n");
    add_shape(shapes, create_circle(10, 20, 5, "red"));
    add_shape(shapes, create_rectangle(30, 40, 8, 6, "blue"));
    add_shape(shapes, create_triangle(50, 60, 4, 3, "green"));
    add_shape(shapes, create_circle(70, 80, 3, "yellow"));
    add_shape(shapes, create_rectangle(90, 100, 10, 4, "purple"));
    
    // Create visitors
    Visitor* area_calc = create_area_calculator();
    Visitor* perimeter_calc = create_perimeter_calculator();
    Visitor* paint_cost_calc = create_paint_cost_calculator(2.50); // $2.50 per square unit
    
    printf("\n--- Applying different visitors ---\n");
    
    // Apply area calculator
    apply_visitor(shapes, area_calc);
    
    // Apply perimeter calculator
    apply_visitor(shapes, perimeter_calc);
    
    // Apply paint cost calculator
    apply_visitor(shapes, paint_cost_calc);
    
    printf("\n--- Applying visitors to subset ---\n");
    
    // Create a subset and apply visitor
    ShapeCollection* circles_only = create_shape_collection();
    add_shape(circles_only, create_circle(0, 0, 2, "orange"));
    add_shape(circles_only, create_circle(5, 5, 4, "pink"));
    
    apply_visitor(circles_only, area_calc);
    apply_visitor(circles_only, paint_cost_calc);
    
    printf("\n--- Visitor Pattern Benefits Demonstrated ---\n");
    printf("✅ Easy to add new operations (visitors) without modifying shapes\n");
    printf("✅ Related operations are grouped in visitor classes\n");
    printf("✅ Visitors can accumulate state during traversal\n");
    printf("✅ Different visitors can be applied to same object structure\n");
    printf("✅ Double dispatch mechanism ensures correct method is called\n");
    
    // Cleanup
    destroy_shape_collection(shapes);
    destroy_shape_collection(circles_only);
    
    area_calc->destroy(area_calc);
    perimeter_calc->destroy(perimeter_calc);
    paint_cost_calc->destroy(paint_cost_calc);
    
    return 0;
}