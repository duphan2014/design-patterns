/*
 * SINGLETON PATTERN
 * 
 * Problem: Ensure only one instance of a class exists and provide global access to it
 * 
 * Use Cases:
 * - Database connections
 * - Logger instances
 * - Configuration managers
 * - Device drivers
 * 
 * Pros: 
 * - Controlled access to sole instance
 * - Reduced memory footprint
 * - Global access point
 * 
 * Cons:
 * - Difficult to test
 * - Can violate single responsibility principle
 * - Problems in multithreaded environments
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Logger singleton structure
typedef struct {
    char log_file[256];
    int log_level;
    char messages[10][512];  // Store up to 10 messages, 512 chars each
    int message_count;       // Number of stored messages
} Logger;

// Static instance
static Logger* logger_instance = NULL;

// Get singleton instance
Logger* get_logger_instance() {
    if (logger_instance == NULL) {
        logger_instance = (Logger*)malloc(sizeof(Logger));
        strcpy(logger_instance->log_file, "application.log");
        logger_instance->log_level = 1; // INFO level
        logger_instance->message_count = 0; // Initialize message count
        printf("Logger instance created!\n");
    }
    return logger_instance;
}

// Logger operations
void log_message(Logger* logger, const char* message) {
    printf("[LOG:%s] %s\n", logger->log_file, message);
    
    // Store the message in memory if there's space
    if (logger->message_count < 10) {
        strcpy(logger->messages[logger->message_count], message);
        logger->message_count++;
    } else {
        printf("Warning: Message storage full, message not stored\n");
    }
}

// New function to display all stored messages
void display_stored_messages(Logger* logger) {
    printf("\n=== STORED MESSAGES ===\n");
    printf("Total messages stored: %d\n", logger->message_count);
    for (int i = 0; i < logger->message_count; i++) {
        printf("Message %d: %s\n", i + 1, logger->messages[i]);
    }
    printf("=======================\n");
}

void set_log_level(Logger* logger, int level) {
    logger->log_level = level;
    printf("Log level set to: %d\n", level);
}

// Cleanup
void destroy_logger() {
    if (logger_instance != NULL) {
        free(logger_instance);
        logger_instance = NULL;
        printf("Logger instance destroyed!\n");
    }
}

// Example usage
int main() {
    printf("=== SINGLETON PATTERN EXAMPLE ===\n\n");
    
    // Get first reference
    Logger* logger1 = get_logger_instance();
    log_message(logger1, "First log message");
    
    // Get second reference - should be same instance
    Logger* logger2 = get_logger_instance();
    log_message(logger2, "Second log message");
    
    // Verify same instance
    printf("\nLogger1 address: %p\n", (void*)logger1);
    printf("Logger2 address: %p\n", (void*)logger2);
    printf("Same instance? %s\n", (logger1 == logger2) ? "Yes" : "No");
    
    // Modify through one reference, visible through other
    set_log_level(logger1, 3);
    printf("Logger2 level: %d\n", logger2->log_level);
    
    // Add more messages to demonstrate storage
    log_message(logger1, "Third log message");
    log_message(logger2, "Fourth log message");
    log_message(logger1, "Final log message");
    
    // Show all stored messages
    display_stored_messages(logger2);
    
    destroy_logger();
    return 0;
}