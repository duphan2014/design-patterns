/*
 * COMMAND PATTERN
 * 
 * Problem: Encapsulate a request as an object to parameterize clients with different 
 *          requests, queue operations, and support undo operations
 * 
 * Use Cases:
 * - GUI buttons and menu items
 * - Macro recording
 * - Undo/Redo operations
 * - Remote controls
 * - Transaction processing
 * 
 * Pros:
 * - Decouples sender from receiver
 * - Easy to add new commands
 * - Supports undo/redo
 * - Supports queuing and logging
 * 
 * Cons:
 * - Increased number of classes
 * - Indirection between sender and receiver
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Forward declarations
typedef struct Command Command;
typedef struct TextEditor TextEditor;

// Command interface
struct Command {
    char name[100];
    void (*execute)(Command* self);
    void (*undo)(Command* self);
    void (*destroy)(Command* self);
};

// Receiver: Text Editor
struct TextEditor {
    char content[1000];
    int cursor_position;
    char clipboard[500];
    
    void (*insert_text)(TextEditor* self, const char* text, int position);
    void (*delete_text)(TextEditor* self, int start, int length);
    void (*copy_text)(TextEditor* self, int start, int length);
    void (*paste_text)(TextEditor* self, int position);
    void (*display)(TextEditor* self);
};

void text_editor_insert_text(TextEditor* self, const char* text, int position) {
    int text_len = strlen(text);
    int content_len = strlen(self->content);
    
    // Move existing text to make room
    for (int i = content_len; i >= position; i--) {
        self->content[i + text_len] = self->content[i];
    }
    
    // Insert new text
    for (int i = 0; i < text_len; i++) {
        self->content[position + i] = text[i];
    }
    
    self->cursor_position = position + text_len;
    printf("✏️ Inserted '%s' at position %d\n", text, position);
}

void text_editor_delete_text(TextEditor* self, int start, int length) {
    int content_len = strlen(self->content);
    
    // Shift text left to fill the gap
    for (int i = start; i <= content_len - length; i++) {
        self->content[i] = self->content[i + length];
    }
    
    self->cursor_position = start;
    printf("🗑️ Deleted %d characters from position %d\n", length, start);
}

void text_editor_copy_text(TextEditor* self, int start, int length) {
    strncpy(self->clipboard, &self->content[start], length);
    self->clipboard[length] = '\\0';
    printf("📋 Copied '%s' to clipboard\n", self->clipboard);
}

void text_editor_paste_text(TextEditor* self, int position) {
    text_editor_insert_text(self, self->clipboard, position);
    printf("📋 Pasted from clipboard\n");
}

void text_editor_display(TextEditor* self) {
    printf("📄 Document: \"%s\" (cursor at %d)\n", self->content, self->cursor_position);
}

TextEditor* create_text_editor() {
    TextEditor* editor = (TextEditor*)malloc(sizeof(TextEditor));
    
    strcpy(editor->content, "");
    editor->cursor_position = 0;
    strcpy(editor->clipboard, "");
    
    editor->insert_text = text_editor_insert_text;
    editor->delete_text = text_editor_delete_text;
    editor->copy_text = text_editor_copy_text;
    editor->paste_text = text_editor_paste_text;
    editor->display = text_editor_display;
    
    return editor;
}

// Concrete Commands

// Insert Command
typedef struct {
    Command base;
    TextEditor* editor;
    char text[200];
    int position;
} InsertCommand;

void insert_command_execute(Command* self) {
    InsertCommand* cmd = (InsertCommand*)self;
    cmd->editor->insert_text(cmd->editor, cmd->text, cmd->position);
}

void insert_command_undo(Command* self) {
    InsertCommand* cmd = (InsertCommand*)self;
    cmd->editor->delete_text(cmd->editor, cmd->position, strlen(cmd->text));
    printf("↩️ Undone: Insert '%s'\n", cmd->text);
}

void insert_command_destroy(Command* self) {
    if (self) {
        free(self);
    }
}

Command* create_insert_command(TextEditor* editor, const char* text, int position) {
    InsertCommand* cmd = (InsertCommand*)malloc(sizeof(InsertCommand));
    
    strcpy(cmd->base.name, "Insert");
    cmd->editor = editor;
    strcpy(cmd->text, text);
    cmd->position = position;
    
    cmd->base.execute = insert_command_execute;
    cmd->base.undo = insert_command_undo;
    cmd->base.destroy = insert_command_destroy;
    
    return (Command*)cmd;
}

// Delete Command
typedef struct {
    Command base;
    TextEditor* editor;
    int start_position;
    int length;
    char deleted_text[200];
} DeleteCommand;

void delete_command_execute(Command* self) {
    DeleteCommand* cmd = (DeleteCommand*)self;
    
    // Save deleted text for undo
    strncpy(cmd->deleted_text, &cmd->editor->content[cmd->start_position], cmd->length);
    cmd->deleted_text[cmd->length] = '\\0';
    
    cmd->editor->delete_text(cmd->editor, cmd->start_position, cmd->length);
}

void delete_command_undo(Command* self) {
    DeleteCommand* cmd = (DeleteCommand*)self;
    cmd->editor->insert_text(cmd->editor, cmd->deleted_text, cmd->start_position);
    printf("↩️ Undone: Delete '%s'\n", cmd->deleted_text);
}

void delete_command_destroy(Command* self) {
    if (self) {
        free(self);
    }
}

Command* create_delete_command(TextEditor* editor, int start, int length) {
    DeleteCommand* cmd = (DeleteCommand*)malloc(sizeof(DeleteCommand));
    
    strcpy(cmd->base.name, "Delete");
    cmd->editor = editor;
    cmd->start_position = start;
    cmd->length = length;
    strcpy(cmd->deleted_text, "");
    
    cmd->base.execute = delete_command_execute;
    cmd->base.undo = delete_command_undo;
    cmd->base.destroy = delete_command_destroy;
    
    return (Command*)cmd;
}

// Copy Command
typedef struct {
    Command base;
    TextEditor* editor;
    int start_position;
    int length;
} CopyCommand;

void copy_command_execute(Command* self) {
    CopyCommand* cmd = (CopyCommand*)self;
    cmd->editor->copy_text(cmd->editor, cmd->start_position, cmd->length);
}

void copy_command_undo(Command* self) {
    // Copy operation doesn't need undo
    printf("↩️ Copy operation cannot be undone\n");
}

void copy_command_destroy(Command* self) {
    if (self) {
        free(self);
    }
}

Command* create_copy_command(TextEditor* editor, int start, int length) {
    CopyCommand* cmd = (CopyCommand*)malloc(sizeof(CopyCommand));
    
    strcpy(cmd->base.name, "Copy");
    cmd->editor = editor;
    cmd->start_position = start;
    cmd->length = length;
    
    cmd->base.execute = copy_command_execute;
    cmd->base.undo = copy_command_undo;
    cmd->base.destroy = copy_command_destroy;
    
    return (Command*)cmd;
}

// Paste Command
typedef struct {
    Command base;
    TextEditor* editor;
    int position;
    char pasted_text[200];
    int pasted_length;
} PasteCommand;

void paste_command_execute(Command* self) {
    PasteCommand* cmd = (PasteCommand*)self;
    
    // Save what will be pasted for undo
    strcpy(cmd->pasted_text, cmd->editor->clipboard);
    cmd->pasted_length = strlen(cmd->pasted_text);
    
    cmd->editor->paste_text(cmd->editor, cmd->position);
}

void paste_command_undo(Command* self) {
    PasteCommand* cmd = (PasteCommand*)self;
    cmd->editor->delete_text(cmd->editor, cmd->position, cmd->pasted_length);
    printf("↩️ Undone: Paste '%s'\n", cmd->pasted_text);
}

void paste_command_destroy(Command* self) {
    if (self) {
        free(self);
    }
}

Command* create_paste_command(TextEditor* editor, int position) {
    PasteCommand* cmd = (PasteCommand*)malloc(sizeof(PasteCommand));
    
    strcpy(cmd->base.name, "Paste");
    cmd->editor = editor;
    cmd->position = position;
    strcpy(cmd->pasted_text, "");
    cmd->pasted_length = 0;
    
    cmd->base.execute = paste_command_execute;
    cmd->base.undo = paste_command_undo;
    cmd->base.destroy = paste_command_destroy;
    
    return (Command*)cmd;
}

// Invoker: Command Manager (supports undo/redo)
#define MAX_COMMANDS 50

typedef struct {
    Command* command_history[MAX_COMMANDS];
    int current_index;
    int history_size;
} CommandManager;

CommandManager* create_command_manager() {
    CommandManager* manager = (CommandManager*)malloc(sizeof(CommandManager));
    manager->current_index = -1;
    manager->history_size = 0;
    return manager;
}

void execute_command(CommandManager* manager, Command* command) {
    printf("\n🎬 Executing: %s\n", command->name);
    command->execute(command);
    
    // Clear any commands after current position (for redo)
    for (int i = manager->current_index + 1; i < manager->history_size; i++) {
        manager->command_history[i]->destroy(manager->command_history[i]);
    }
    
    // Add command to history
    manager->current_index++;
    manager->command_history[manager->current_index] = command;
    manager->history_size = manager->current_index + 1;
}

void undo_command(CommandManager* manager) {
    if (manager->current_index >= 0) {
        Command* command = manager->command_history[manager->current_index];
        printf("\n↩️ Undoing: %s\n", command->name);
        command->undo(command);
        manager->current_index--;
    } else {
        printf("\n❌ Nothing to undo\n");
    }
}

void redo_command(CommandManager* manager) {
    if (manager->current_index + 1 < manager->history_size) {
        manager->current_index++;
        Command* command = manager->command_history[manager->current_index];
        printf("\n↪️ Redoing: %s\n", command->name);
        command->execute(command);
    } else {
        printf("\n❌ Nothing to redo\n");
    }
}

void print_command_history(CommandManager* manager) {
    printf("\n📋 Command History:\n");
    for (int i = 0; i < manager->history_size; i++) {
        char marker = (i == manager->current_index) ? '→' : ' ';
        printf("  %c %d. %s\n", marker, i + 1, manager->command_history[i]->name);
    }
}

void destroy_command_manager(CommandManager* manager) {
    if (manager) {
        for (int i = 0; i < manager->history_size; i++) {
            manager->command_history[i]->destroy(manager->command_history[i]);
        }
        free(manager);
    }
}

void destroy_text_editor(TextEditor* editor) {
    if (editor) {
        free(editor);
    }
}

// Example usage
int main() {
    printf("=== COMMAND PATTERN EXAMPLE ===\n\n");
    
    // Create the text editor (receiver)
    TextEditor* editor = create_text_editor();
    
    // Create command manager (invoker)
    CommandManager* manager = create_command_manager();
    
    printf("--- Building a document with commands ---\n");
    
    // Initial state
    editor->display(editor);
    
    // Execute a series of commands
    execute_command(manager, create_insert_command(editor, "Hello", 0));
    editor->display(editor);
    
    execute_command(manager, create_insert_command(editor, " World", 5));
    editor->display(editor);
    
    execute_command(manager, create_insert_command(editor, "!", 11));
    editor->display(editor);
    
    execute_command(manager, create_copy_command(editor, 0, 5)); // Copy "Hello"
    
    execute_command(manager, create_insert_command(editor, " ", 12));
    editor->display(editor);
    
    execute_command(manager, create_paste_command(editor, 13));
    editor->display(editor);
    
    execute_command(manager, create_delete_command(editor, 6, 5)); // Delete "World"
    editor->display(editor);
    
    print_command_history(manager);
    
    printf("\n--- Testing Undo/Redo ---\n");
    
    // Undo last few operations
    undo_command(manager);
    editor->display(editor);
    
    undo_command(manager);
    editor->display(editor);
    
    undo_command(manager);
    editor->display(editor);
    
    print_command_history(manager);
    
    // Redo operations
    printf("\n--- Redoing operations ---\n");
    redo_command(manager);
    editor->display(editor);
    
    redo_command(manager);
    editor->display(editor);
    
    print_command_history(manager);
    
    // Try to undo/redo beyond limits
    printf("\n--- Testing boundaries ---\n");
    for (int i = 0; i < 10; i++) {
        undo_command(manager);
    }
    editor->display(editor);
    
    for (int i = 0; i < 10; i++) {
        redo_command(manager);
    }
    editor->display(editor);
    
    printf("\n--- Command Pattern Benefits Demonstrated ---\n");
    printf("✅ Commands are decoupled from their receivers\n");
    printf("✅ Easy to add new command types\n");
    printf("✅ Support for undo/redo operations\n");
    printf("✅ Commands can be queued, logged, and replayed\n");
    printf("✅ Macro recording capability (sequence of commands)\n");
    
    // Cleanup
    destroy_command_manager(manager);
    destroy_text_editor(editor);
    
    return 0;
}