/*
 * BRIDGE PATTERN
 * 
 * Problem: Separate abstraction from implementation so both can vary independently
 * 
 * Use Cases:
 * - Cross-platform graphics APIs
 * - Database drivers
 * - Device drivers
 * - UI toolkit abstractions
 * 
 * Pros:
 * - Decouples interface from implementation
 * - Improved extensibility
 * - Hiding implementation details from clients
 * 
 * Cons:
 * - Increased complexity
 * - Additional indirection
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Implementation interface (Bridge)
typedef struct DrawingAPI DrawingAPI;
struct DrawingAPI {
    void (*draw_circle)(DrawingAPI* self, int x, int y, int radius);
    void (*draw_rectangle)(DrawingAPI* self, int x, int y, int width, int height);
    void (*set_color)(DrawingAPI* self, const char* color);
    const char* (*get_name)(DrawingAPI* self);
};

// Concrete Implementation 1: OpenGL
typedef struct {
    DrawingAPI base;
    char current_color[50];
} OpenGLAPI;

void opengl_draw_circle(DrawingAPI* self, int x, int y, int radius) {
    OpenGLAPI* gl = (OpenGLAPI*)self;
    printf("OpenGL: Drawing %s circle at (%d,%d) with radius %d\n", 
           gl->current_color, x, y, radius);
    printf("  glColor(%s); glCircle(%d, %d, %d);\n", gl->current_color, x, y, radius);
}

void opengl_draw_rectangle(DrawingAPI* self, int x, int y, int width, int height) {
    OpenGLAPI* gl = (OpenGLAPI*)self;
    printf("OpenGL: Drawing %s rectangle at (%d,%d) size %dx%d\n", 
           gl->current_color, x, y, width, height);
    printf("  glColor(%s); glRect(%d, %d, %d, %d);\n", gl->current_color, x, y, width, height);
}

void opengl_set_color(DrawingAPI* self, const char* color) {
    OpenGLAPI* gl = (OpenGLAPI*)self;
    strcpy(gl->current_color, color);
    printf("OpenGL: Color set to %s\n", color);
}

const char* opengl_get_name(DrawingAPI* self) {
    return "OpenGL";
}

DrawingAPI* create_opengl_api() {
    OpenGLAPI* gl = (OpenGLAPI*)malloc(sizeof(OpenGLAPI));
    strcpy(gl->current_color, "white");
    
    gl->base.draw_circle = opengl_draw_circle;
    gl->base.draw_rectangle = opengl_draw_rectangle;
    gl->base.set_color = opengl_set_color;
    gl->base.get_name = opengl_get_name;
    
    return (DrawingAPI*)gl;
}

// Concrete Implementation 2: DirectX
typedef struct {
    DrawingAPI base;
    char current_color[50];
} DirectXAPI;

void directx_draw_circle(DrawingAPI* self, int x, int y, int radius) {
    DirectXAPI* dx = (DirectXAPI*)self;
    printf("DirectX: Rendering %s circle at (%d,%d) with radius %d\n", 
           dx->current_color, x, y, radius);
    printf("  D3DSetColor(%s); D3DDrawCircle(%d, %d, %d);\n", dx->current_color, x, y, radius);
}

void directx_draw_rectangle(DrawingAPI* self, int x, int y, int width, int height) {
    DirectXAPI* dx = (DirectXAPI*)self;
    printf("DirectX: Rendering %s rectangle at (%d,%d) size %dx%d\n", 
           dx->current_color, x, y, width, height);
    printf("  D3DSetColor(%s); D3DDrawRect(%d, %d, %d, %d);\n", dx->current_color, x, y, width, height);
}

void directx_set_color(DrawingAPI* self, const char* color) {
    DirectXAPI* dx = (DirectXAPI*)self;
    strcpy(dx->current_color, color);
    printf("DirectX: Color set to %s\n", color);
}

const char* directx_get_name(DrawingAPI* self) {
    return "DirectX";
}

DrawingAPI* create_directx_api() {
    DirectXAPI* dx = (DirectXAPI*)malloc(sizeof(DirectXAPI));
    strcpy(dx->current_color, "white");
    
    dx->base.draw_circle = directx_draw_circle;
    dx->base.draw_rectangle = directx_draw_rectangle;
    dx->base.set_color = directx_set_color;
    dx->base.get_name = directx_get_name;
    
    return (DrawingAPI*)dx;
}

// Concrete Implementation 3: Software Renderer
typedef struct {
    DrawingAPI base;
    char current_color[50];
} SoftwareAPI;

void software_draw_circle(DrawingAPI* self, int x, int y, int radius) {
    SoftwareAPI* sw = (SoftwareAPI*)self;
    printf("Software: Plotting %s circle at (%d,%d) with radius %d\n", 
           sw->current_color, x, y, radius);
    printf("  setPixelColor(%s); plotCirclePixels(%d, %d, %d);\n", sw->current_color, x, y, radius);
}

void software_draw_rectangle(DrawingAPI* self, int x, int y, int width, int height) {
    SoftwareAPI* sw = (SoftwareAPI*)self;
    printf("Software: Plotting %s rectangle at (%d,%d) size %dx%d\n", 
           sw->current_color, x, y, width, height);
    printf("  setPixelColor(%s); plotRectPixels(%d, %d, %d, %d);\n", sw->current_color, x, y, width, height);
}

void software_set_color(DrawingAPI* self, const char* color) {
    SoftwareAPI* sw = (SoftwareAPI*)self;
    strcpy(sw->current_color, color);
    printf("Software: Color set to %s\n", color);
}

const char* software_get_name(DrawingAPI* self) {
    return "Software Renderer";
}

DrawingAPI* create_software_api() {
    SoftwareAPI* sw = (SoftwareAPI*)malloc(sizeof(SoftwareAPI));
    strcpy(sw->current_color, "white");
    
    sw->base.draw_circle = software_draw_circle;
    sw->base.draw_rectangle = software_draw_rectangle;
    sw->base.set_color = software_set_color;
    sw->base.get_name = software_get_name;
    
    return (DrawingAPI*)sw;
}

// Abstraction: Shape
typedef struct Shape Shape;
struct Shape {
    DrawingAPI* drawing_api;
    int x, y;
    char color[50];
    
    void (*draw)(Shape* self);
    void (*move)(Shape* self, int new_x, int new_y);
    void (*set_color)(Shape* self, const char* color);
    void (*resize)(Shape* self, int factor);
};

// Refined Abstraction 1: Circle
typedef struct {
    Shape base;
    int radius;
} Circle;

void circle_draw(Shape* self) {
    Circle* circle = (Circle*)self;
    circle->base.drawing_api->set_color(circle->base.drawing_api, circle->base.color);
    circle->base.drawing_api->draw_circle(circle->base.drawing_api, 
                                         circle->base.x, circle->base.y, circle->radius);
}

void circle_resize(Shape* self, int factor) {
    Circle* circle = (Circle*)self;
    circle->radius *= factor;
    printf("Circle resized: new radius = %d\n", circle->radius);
}

void shape_move(Shape* self, int new_x, int new_y) {
    self->x = new_x;
    self->y = new_y;
    printf("Shape moved to (%d, %d)\n", new_x, new_y);
}

void shape_set_color(Shape* self, const char* color) {
    strcpy(self->color, color);
    printf("Shape color changed to %s\n", color);
}

Shape* create_circle(DrawingAPI* api, int x, int y, int radius, const char* color) {
    Circle* circle = (Circle*)malloc(sizeof(Circle));
    
    circle->base.drawing_api = api;
    circle->base.x = x;
    circle->base.y = y;
    strcpy(circle->base.color, color);
    circle->radius = radius;
    
    circle->base.draw = circle_draw;
    circle->base.move = shape_move;
    circle->base.set_color = shape_set_color;
    circle->base.resize = circle_resize;
    
    return (Shape*)circle;
}

// Refined Abstraction 2: Rectangle
typedef struct {
    Shape base;
    int width, height;
} Rectangle;

void rectangle_draw(Shape* self) {
    Rectangle* rect = (Rectangle*)self;
    rect->base.drawing_api->set_color(rect->base.drawing_api, rect->base.color);
    rect->base.drawing_api->draw_rectangle(rect->base.drawing_api, 
                                          rect->base.x, rect->base.y, 
                                          rect->width, rect->height);
}

void rectangle_resize(Shape* self, int factor) {
    Rectangle* rect = (Rectangle*)self;
    rect->width *= factor;
    rect->height *= factor;
    printf("Rectangle resized: new size = %dx%d\n", rect->width, rect->height);
}

Shape* create_rectangle(DrawingAPI* api, int x, int y, int width, int height, const char* color) {
    Rectangle* rect = (Rectangle*)malloc(sizeof(Rectangle));
    
    rect->base.drawing_api = api;
    rect->base.x = x;
    rect->base.y = y;
    strcpy(rect->base.color, color);
    rect->width = width;
    rect->height = height;
    
    rect->base.draw = rectangle_draw;
    rect->base.move = shape_move;
    rect->base.set_color = shape_set_color;
    rect->base.resize = rectangle_resize;
    
    return (Shape*)rect;
}

void destroy_shape(Shape* shape) {
    if (shape) {
        free(shape);
    }
}

void destroy_api(DrawingAPI* api) {
    if (api) {
        free(api);
    }
}

// Example usage
int main() {
    printf("=== BRIDGE PATTERN EXAMPLE ===\n\n");
    
    // Create different drawing implementations
    DrawingAPI* opengl = create_opengl_api();
    DrawingAPI* directx = create_directx_api();
    DrawingAPI* software = create_software_api();
    
    printf("Available drawing APIs:\n");
    printf("- %s\n", opengl->get_name(opengl));
    printf("- %s\n", directx->get_name(directx));
    printf("- %s\n", software->get_name(software));
    
    printf("\n--- Creating shapes with different APIs ---\n");
    
    // Create shapes using different implementations
    Shape* circle1 = create_circle(opengl, 10, 20, 15, "red");
    Shape* circle2 = create_circle(directx, 30, 40, 10, "blue");
    Shape* circle3 = create_circle(software, 50, 60, 20, "green");
    
    Shape* rect1 = create_rectangle(opengl, 100, 200, 50, 30, "yellow");
    Shape* rect2 = create_rectangle(directx, 150, 250, 40, 25, "purple");
    
    printf("\n--- Drawing shapes ---\n");
    printf("Circle 1 (%s):\n", circle1->drawing_api->get_name(circle1->drawing_api));
    circle1->draw(circle1);
    
    printf("\nCircle 2 (%s):\n", circle2->drawing_api->get_name(circle2->drawing_api));
    circle2->draw(circle2);
    
    printf("\nCircle 3 (%s):\n", circle3->drawing_api->get_name(circle3->drawing_api));
    circle3->draw(circle3);
    
    printf("\nRectangle 1 (%s):\n", rect1->drawing_api->get_name(rect1->drawing_api));
    rect1->draw(rect1);
    
    printf("\nRectangle 2 (%s):\n", rect2->drawing_api->get_name(rect2->drawing_api));
    rect2->draw(rect2);
    
    printf("\n--- Modifying shapes ---\n");
    circle1->set_color(circle1, "orange");
    circle1->move(circle1, 15, 25);
    circle1->resize(circle1, 2);
    printf("Modified Circle 1:\n");
    circle1->draw(circle1);
    
    printf("\n--- Bridge Pattern Benefits ---\n");
    printf("✅ Abstraction (Shape) and Implementation (DrawingAPI) vary independently\n");
    printf("✅ Same shape can use different rendering backends\n");
    printf("✅ Easy to add new shapes or new drawing APIs\n");
    printf("✅ Client code doesn't depend on specific implementation\n");
    
    // Cleanup
    destroy_shape(circle1);
    destroy_shape(circle2);
    destroy_shape(circle3);
    destroy_shape(rect1);
    destroy_shape(rect2);
    
    destroy_api(opengl);
    destroy_api(directx);
    destroy_api(software);
    
    return 0;
}