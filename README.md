# Design Patterns in C

This repository contains implementations of all major design patterns in C. Design patterns are reusable solutions to common problems in software design.

## Pattern Categories

### Creational Patterns
1. [Singleton](./creational/singleton.c) - Ensures only one instance of a class exists
2. [Factory](./creational/factory.c) - Creates objects without specifying exact classes
3. [Builder](./creational/builder.c) - Constructs complex objects step by step
4. [Prototype](./creational/prototype.c) - Creates objects by cloning existing instances

### Structural Patterns
5. [Adapter](./structural/adapter.c) - Allows incompatible interfaces to work together
6. [Decorator](./structural/decorator.c) - Adds behavior to objects dynamically
7. [Facade](./structural/facade.c) - Provides simplified interface to complex subsystem
8. [Composite](./structural/composite.c) - Composes objects into tree structures
9. [Bridge](./structural/bridge.c) - Separates abstraction from implementation

### Behavioral Patterns
10. [Observer](./behavioral/observer.c) - Notifies multiple objects about state changes
11. [Strategy](./behavioral/strategy.c) - Encapsulates algorithms and makes them interchangeable
12. [Command](./behavioral/command.c) - Encapsulates requests as objects
13. [State](./behavioral/state.c) - Changes object behavior when internal state changes
14. [Chain of Responsibility](./behavioral/chain_of_responsibility.c) - Passes requests along handler chain
15. [Template Method](./behavioral/template_method.c) - Defines algorithm skeleton, subclasses fill in details
16. [Visitor](./behavioral/visitor.c) - Separates algorithms from object structure

## How to Compile and Run

Each pattern includes a main function with examples. To compile and run:

```bash
gcc -o pattern_name pattern_file.c
./pattern_name
```

## What You'll Learn

- **Problem**: What problem each pattern solves
- **Solution**: How the pattern works
- **Implementation**: Practical C code examples
- **Use Cases**: When to apply each pattern
- **Pros/Cons**: Benefits and drawbacks

Let's start exploring these patterns!