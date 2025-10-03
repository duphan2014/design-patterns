# Design Patterns Study Guide

## Quick Reference

### When to Use Each Pattern

| Pattern | Use When | Real-World Example |
|---------|----------|-------------------|
| **Singleton** | Need exactly one instance | Database connection pool, Logger |
| **Factory** | Create objects without specifying exact class | UI elements for different platforms |
| **Builder** | Construct complex objects step-by-step | SQL query builder, Computer assembly |
| **Prototype** | Create objects by cloning | Game character templates |
| **Adapter** | Make incompatible interfaces work together | Legacy system integration |
| **Decorator** | Add behavior dynamically | Coffee shop add-ons, Stream processing |
| **Facade** | Simplify complex subsystem | Operating system API |
| **Composite** | Treat individual and composite objects uniformly | File system, UI components |
| **Bridge** | Separate abstraction from implementation | Graphics APIs, Database drivers |
| **Observer** | Notify multiple objects of state changes | Event systems, MVC architecture |
| **Strategy** | Make algorithms interchangeable | Payment processing, Sorting |
| **Command** | Encapsulate requests as objects | GUI actions, Undo/Redo |
| **State** | Change behavior based on internal state | State machines, Game characters |
| **Chain of Responsibility** | Pass requests along handler chain | Event handling, Authorization |
| **Template Method** | Define algorithm skeleton | Data processing pipelines |
| **Visitor** | Separate algorithms from object structure | Compiler AST traversal |

## Pattern Relationships

### Creational Patterns
- **Singleton** vs **Factory**: Singleton ensures one instance, Factory creates instances
- **Builder** vs **Factory**: Builder for complex construction, Factory for simple creation
- **Prototype** vs **Factory**: Prototype clones existing, Factory creates new

### Structural Patterns
- **Adapter** vs **Bridge**: Adapter fixes incompatibility, Bridge separates abstraction
- **Decorator** vs **Composite**: Decorator adds behavior, Composite creates hierarchies
- **Facade** vs **Adapter**: Facade simplifies, Adapter translates

### Behavioral Patterns
- **Observer** vs **Chain**: Observer notifies all, Chain finds first handler
- **Strategy** vs **State**: Strategy chooses algorithm, State changes behavior
- **Command** vs **Strategy**: Command encapsulates request, Strategy encapsulates algorithm

## Implementation Tips in C

### Memory Management
```c
// Always provide destroy methods
typedef struct {
    // ... data members
    void (*destroy)(MyStruct* self);
} MyStruct;

// In destroy method, clean up all allocated memory
void my_struct_destroy(MyStruct* self) {
    if (self) {
        // Free any allocated members first
        if (self->some_allocated_member) {
            free(self->some_allocated_member);
        }
        // Then free the structure itself
        free(self);
    }
}
```

### Function Pointers for Polymorphism
```c
// Define interface with function pointers
typedef struct {
    void (*operation)(void* self);
    int (*calculate)(void* self, int input);
} Interface;

// Implement in concrete types
void concrete_operation(void* self) {
    ConcreteType* concrete = (ConcreteType*)self;
    // Implementation
}
```

### Error Handling
```c
// Always check for NULL pointers
MyStruct* create_something() {
    MyStruct* obj = malloc(sizeof(MyStruct));
    if (!obj) {
        return NULL; // Memory allocation failed
    }
    // Initialize...
    return obj;
}

// Check return values
MyStruct* obj = create_something();
if (!obj) {
    printf("Error: Failed to create object\\n");
    return -1;
}
```

## Common Pitfalls

1. **Singleton**: Thread safety issues, global state problems
2. **Factory**: Too many parameters, unclear responsibilities
3. **Observer**: Memory leaks from unremoved observers
4. **Decorator**: Deep wrapper chains, performance overhead
5. **Visitor**: Breaking encapsulation, hard to add new element types

## Best Practices

1. **Favor Composition over Inheritance**: Use function pointers and structures
2. **Program to Interfaces**: Define clear contracts with function pointers
3. **Keep It Simple**: Don't over-engineer, use patterns when they solve real problems
4. **Consider Alternatives**: Sometimes simple solutions are better than patterns
5. **Document Intent**: Clearly explain why a pattern was chosen

## Study Sequence

### Beginner (Start Here)
1. **Singleton** - Simple concept, easy to understand
2. **Factory** - Basic object creation abstraction
3. **Observer** - Fundamental event handling pattern

### Intermediate
4. **Strategy** - Algorithm variation
5. **Decorator** - Flexible object enhancement
6. **Facade** - System simplification
7. **Command** - Action encapsulation

### Advanced
8. **Builder** - Complex object construction
9. **Composite** - Tree structures
10. **State** - Behavior variation
11. **Template Method** - Algorithm framework
12. **Chain of Responsibility** - Request handling
13. **Visitor** - Operation separation
14. **Adapter** - Interface adaptation
15. **Bridge** - Implementation separation
16. **Prototype** - Object cloning

## Exercises

### Practice Projects
1. **Text Editor**: Command (undo/redo), Decorator (formatting), Observer (document changes)
2. **Game Engine**: State (character states), Strategy (AI behaviors), Factory (entity creation)
3. **Graphics Library**: Bridge (rendering APIs), Composite (scene graph), Visitor (operations)
4. **Web Server**: Chain of Responsibility (middleware), Template Method (request handling)

### Code Challenges
1. Implement a simple calculator using Command pattern for operations
2. Create a file system using Composite pattern
3. Build a notification system using Observer pattern
4. Design a payment system using Strategy pattern

## Resources for Further Learning

- **Books**: "Design Patterns" by Gang of Four, "Head First Design Patterns"
- **Practice**: Implement each pattern in a different domain (games, web, embedded)
- **Analysis**: Study open-source C projects to see patterns in action
- **Comparison**: Implement the same problem with and without patterns