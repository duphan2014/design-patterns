/*
 * COMPOSITE PATTERN
 * 
 * Problem: Compose objects into tree structures to represent part-whole hierarchies
 * 
 * Use Cases:
 * - File system structures (files and directories)
 * - UI component hierarchies
 * - Organizational structures
 * - Mathematical expressions
 * 
 * Pros:
 * - Uniform treatment of individual and composite objects
 * - Easy to add new component types
 * - Simplifies client code
 * 
 * Cons:
 * - Can make design overly general
 * - Hard to restrict component types
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Component interface
typedef struct FileSystemComponent FileSystemComponent;

struct FileSystemComponent {
    char name[256];
    void (*display)(FileSystemComponent* self, int indent);
    int (*get_size)(FileSystemComponent* self);
    void (*add)(FileSystemComponent* self, FileSystemComponent* component);
    void (*remove)(FileSystemComponent* self, FileSystemComponent* component);
    FileSystemComponent* (*get_child)(FileSystemComponent* self, int index);
    void (*destroy)(FileSystemComponent* self);
};

// Leaf: File
typedef struct {
    FileSystemComponent base;
    int size_bytes;
    char type[20];
} File;

void file_display(FileSystemComponent* self, int indent) {
    File* file = (File*)self;
    for (int i = 0; i < indent; i++) printf("  ");
    printf("📄 %s (%s, %d bytes)\n", file->base.name, file->type, file->size_bytes);
}

int file_get_size(FileSystemComponent* self) {
    File* file = (File*)self;
    return file->size_bytes;
}

void file_add(FileSystemComponent* self, FileSystemComponent* component) {
    printf("Error: Cannot add to a file\n");
}

void file_remove(FileSystemComponent* self, FileSystemComponent* component) {
    printf("Error: Cannot remove from a file\n");
}

FileSystemComponent* file_get_child(FileSystemComponent* self, int index) {
    printf("Error: File has no children\n");
    return NULL;
}

void file_destroy(FileSystemComponent* self) {
    if (self) {
        free(self);
    }
}

FileSystemComponent* create_file(const char* name, const char* type, int size) {
    File* file = (File*)malloc(sizeof(File));
    
    strcpy(file->base.name, name);
    strcpy(file->type, type);
    file->size_bytes = size;
    
    file->base.display = file_display;
    file->base.get_size = file_get_size;
    file->base.add = file_add;
    file->base.remove = file_remove;
    file->base.get_child = file_get_child;
    file->base.destroy = file_destroy;
    
    return (FileSystemComponent*)file;
}

// Composite: Directory
#define MAX_CHILDREN 50

typedef struct {
    FileSystemComponent base;
    FileSystemComponent* children[MAX_CHILDREN];
    int child_count;
} Directory;

void directory_display(FileSystemComponent* self, int indent) {
    Directory* dir = (Directory*)self;
    
    for (int i = 0; i < indent; i++) printf("  ");
    printf("📁 %s/ (%d items, %d bytes total)\n", 
           dir->base.name, dir->child_count, dir->base.get_size(self));
    
    // Display all children
    for (int i = 0; i < dir->child_count; i++) {
        dir->children[i]->display(dir->children[i], indent + 1);
    }
}

int directory_get_size(FileSystemComponent* self) {
    Directory* dir = (Directory*)self;
    int total_size = 0;
    
    // Recursively calculate size of all children
    for (int i = 0; i < dir->child_count; i++) {
        total_size += dir->children[i]->get_size(dir->children[i]);
    }
    
    return total_size;
}

void directory_add(FileSystemComponent* self, FileSystemComponent* component) {
    Directory* dir = (Directory*)self;
    
    if (dir->child_count < MAX_CHILDREN) {
        dir->children[dir->child_count] = component;
        dir->child_count++;
        printf("Added '%s' to directory '%s'\n", component->name, dir->base.name);
    } else {
        printf("Error: Directory '%s' is full\n", dir->base.name);
    }
}

void directory_remove(FileSystemComponent* self, FileSystemComponent* component) {
    Directory* dir = (Directory*)self;
    
    for (int i = 0; i < dir->child_count; i++) {
        if (dir->children[i] == component) {
            // Shift remaining elements
            for (int j = i; j < dir->child_count - 1; j++) {
                dir->children[j] = dir->children[j + 1];
            }
            dir->child_count--;
            printf("Removed '%s' from directory '%s'\n", component->name, dir->base.name);
            return;
        }
    }
    printf("Error: Component not found in directory\n");
}

FileSystemComponent* directory_get_child(FileSystemComponent* self, int index) {
    Directory* dir = (Directory*)self;
    
    if (index >= 0 && index < dir->child_count) {
        return dir->children[index];
    }
    return NULL;
}

void directory_destroy(FileSystemComponent* self) {
    Directory* dir = (Directory*)self;
    
    if (dir) {
        // Recursively destroy all children
        for (int i = 0; i < dir->child_count; i++) {
            dir->children[i]->destroy(dir->children[i]);
        }
        free(dir);
    }
}

FileSystemComponent* create_directory(const char* name) {
    Directory* dir = (Directory*)malloc(sizeof(Directory));
    
    strcpy(dir->base.name, name);
    dir->child_count = 0;
    
    dir->base.display = directory_display;
    dir->base.get_size = directory_get_size;
    dir->base.add = directory_add;
    dir->base.remove = directory_remove;
    dir->base.get_child = directory_get_child;
    dir->base.destroy = directory_destroy;
    
    return (FileSystemComponent*)dir;
}

// Client code that works uniformly with files and directories
void print_separator() {
    printf("\n" "=" * 50 "\n");
}

void demonstrate_composite_operations(FileSystemComponent* root) {
    printf("\n--- File System Structure ---\n");
    root->display(root, 0);
    
    printf("\n--- Total Size Calculation ---\n");
    printf("Total size of '%s': %d bytes\n", root->name, root->get_size(root));
}

// Example usage
int main() {
    printf("=== COMPOSITE PATTERN EXAMPLE ===\n");
    printf("Building a file system hierarchy...\n");
    
    // Create root directory
    FileSystemComponent* root = create_directory("root");
    
    // Create files
    FileSystemComponent* readme = create_file("README.md", "text", 1024);
    FileSystemComponent* main_c = create_file("main.c", "source", 2048);
    FileSystemComponent* config = create_file("config.json", "json", 512);
    
    // Create subdirectories
    FileSystemComponent* src_dir = create_directory("src");
    FileSystemComponent* docs_dir = create_directory("docs");
    FileSystemComponent* tests_dir = create_directory("tests");
    
    // Add files to root
    root->add(root, readme);
    root->add(root, main_c);
    root->add(root, config);
    
    // Add subdirectories to root
    root->add(root, src_dir);
    root->add(root, docs_dir);
    root->add(root, tests_dir);
    
    // Add files to src directory
    FileSystemComponent* utils_c = create_file("utils.c", "source", 1536);
    FileSystemComponent* utils_h = create_file("utils.h", "header", 256);
    FileSystemComponent* parser_c = create_file("parser.c", "source", 3072);
    
    src_dir->add(src_dir, utils_c);
    src_dir->add(src_dir, utils_h);
    src_dir->add(src_dir, parser_c);
    
    // Add files to docs directory
    FileSystemComponent* manual_pdf = create_file("manual.pdf", "pdf", 102400);
    FileSystemComponent* api_md = create_file("api.md", "text", 4096);
    
    docs_dir->add(docs_dir, manual_pdf);
    docs_dir->add(docs_dir, api_md);
    
    // Create nested directory structure
    FileSystemComponent* unit_tests = create_directory("unit");
    FileSystemComponent* integration_tests = create_directory("integration");
    
    tests_dir->add(tests_dir, unit_tests);
    tests_dir->add(tests_dir, integration_tests);
    
    // Add test files
    FileSystemComponent* test_utils = create_file("test_utils.c", "source", 2048);
    FileSystemComponent* test_parser = create_file("test_parser.c", "source", 1024);
    
    unit_tests->add(unit_tests, test_utils);
    unit_tests->add(unit_tests, test_parser);
    
    // Demonstrate the pattern
    demonstrate_composite_operations(root);
    
    printf("\n--- Accessing Individual Components ---\n");
    FileSystemComponent* src = root->get_child(root, 3);  // src directory
    if (src) {
        printf("Accessing src directory:\n");
        src->display(src, 0);
    }
    
    printf("\n--- Removing a Component ---\n");
    root->remove(root, config);
    demonstrate_composite_operations(root);
    
    printf("\n--- Composite Pattern Benefits ---\n");
    printf("✅ Uniform interface for files and directories\n");
    printf("✅ Easy to add new file types or directory types\n");
    printf("✅ Recursive operations work naturally\n");
    printf("✅ Client code doesn't need to distinguish leaf vs composite\n");
    
    // Cleanup
    root->destroy(root);
    config->destroy(config);  // Manually destroy removed component
    
    return 0;
}