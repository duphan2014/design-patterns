#!/bin/bash

# Design Patterns Compilation and Execution Script
# This script compiles and runs all design pattern examples

echo "🎯 Design Patterns in C - Compilation and Execution Script"
echo "=========================================================="

# Function to compile and run a pattern
compile_and_run() {
    local category=$1
    local pattern=$2
    local file_path="$category/$pattern.c"
    local executable="$category/$pattern"
    
    echo ""
    echo "📁 Category: $category"
    echo "🔧 Compiling $pattern..."
    
    # Compile with math library for patterns that need it
    if gcc -o "$executable" "$file_path" -lm 2>/dev/null; then
        echo "✅ Compilation successful"
        echo "🚀 Running $pattern example:"
        echo "----------------------------------------"
        ./"$executable"
        echo "----------------------------------------"
        echo "✅ $pattern completed successfully"
        
        # Clean up executable
        rm -f "$executable"
    else
        echo "❌ Compilation failed for $pattern"
        gcc "$file_path" -lm  # Show error details
    fi
}

# Function to show available patterns
show_patterns() {
    echo ""
    echo "📋 Available Design Patterns:"
    echo ""
    echo "🏗️  CREATIONAL PATTERNS:"
    echo "   1. singleton"
    echo "   2. factory"
    echo "   3. builder"
    echo "   4. prototype"
    echo ""
    echo "🔧 STRUCTURAL PATTERNS:"
    echo "   5. adapter"
    echo "   6. decorator"
    echo "   7. facade"
    echo "   8. composite"
    echo "   9. bridge"
    echo ""
    echo "🎭 BEHAVIORAL PATTERNS:"
    echo "   10. observer"
    echo "   11. strategy"
    echo "   12. command"
    echo "   13. state"
    echo "   14. chain_of_responsibility"
    echo "   15. template_method"
    echo "   16. visitor"
}

# Main execution
if [ $# -eq 0 ]; then
    echo "Usage: $0 [pattern_name|all|list|category]"
    echo ""
    echo "Examples:"
    echo "  $0 singleton          # Run singleton pattern"
    echo "  $0 all               # Run all patterns"
    echo "  $0 list              # Show available patterns"
    echo "  $0 creational        # Run all creational patterns"
    echo "  $0 structural        # Run all structural patterns"
    echo "  $0 behavioral        # Run all behavioral patterns"
    show_patterns
    exit 1
fi

case $1 in
    "list")
        show_patterns
        ;;
    "all")
        echo "🎯 Running ALL design patterns..."
        
        # Creational patterns
        compile_and_run "creational" "singleton"
        compile_and_run "creational" "factory"
        compile_and_run "creational" "builder"
        compile_and_run "creational" "prototype"
        
        # Structural patterns
        compile_and_run "structural" "adapter"
        compile_and_run "structural" "decorator"
        compile_and_run "structural" "facade"
        compile_and_run "structural" "composite"
        compile_and_run "structural" "bridge"
        
        # Behavioral patterns
        compile_and_run "behavioral" "observer"
        compile_and_run "behavioral" "strategy"
        compile_and_run "behavioral" "command"
        compile_and_run "behavioral" "state"
        compile_and_run "behavioral" "chain_of_responsibility"
        compile_and_run "behavioral" "template_method"
        compile_and_run "behavioral" "visitor"
        
        echo ""
        echo "🎉 All patterns executed successfully!"
        ;;
    "creational")
        echo "🏗️ Running CREATIONAL patterns..."
        compile_and_run "creational" "singleton"
        compile_and_run "creational" "factory"
        compile_and_run "creational" "builder"
        compile_and_run "creational" "prototype"
        ;;
    "structural")
        echo "🔧 Running STRUCTURAL patterns..."
        compile_and_run "structural" "adapter"
        compile_and_run "structural" "decorator"
        compile_and_run "structural" "facade"
        compile_and_run "structural" "composite"
        compile_and_run "structural" "bridge"
        ;;
    "behavioral")
        echo "🎭 Running BEHAVIORAL patterns..."
        compile_and_run "behavioral" "observer"
        compile_and_run "behavioral" "strategy"
        compile_and_run "behavioral" "command"
        compile_and_run "behavioral" "state"
        compile_and_run "behavioral" "chain_of_responsibility"
        compile_and_run "behavioral" "template_method"
        compile_and_run "behavioral" "visitor"
        ;;
    "singleton")
        compile_and_run "creational" "singleton"
        ;;
    "factory")
        compile_and_run "creational" "factory"
        ;;
    "builder")
        compile_and_run "creational" "builder"
        ;;
    "prototype")
        compile_and_run "creational" "prototype"
        ;;
    "adapter")
        compile_and_run "structural" "adapter"
        ;;
    "decorator")
        compile_and_run "structural" "decorator"
        ;;
    "facade")
        compile_and_run "structural" "facade"
        ;;
    "composite")
        compile_and_run "structural" "composite"
        ;;
    "bridge")
        compile_and_run "structural" "bridge"
        ;;
    "observer")
        compile_and_run "behavioral" "observer"
        ;;
    "strategy")
        compile_and_run "behavioral" "strategy"
        ;;
    "command")
        compile_and_run "behavioral" "command"
        ;;
    "state")
        compile_and_run "behavioral" "state"
        ;;
    "chain_of_responsibility")
        compile_and_run "behavioral" "chain_of_responsibility"
        ;;
    "template_method")
        compile_and_run "behavioral" "template_method"
        ;;
    "visitor")
        compile_and_run "behavioral" "visitor"
        ;;
    *)
        echo "❌ Unknown pattern: $1"
        show_patterns
        exit 1
        ;;
esac