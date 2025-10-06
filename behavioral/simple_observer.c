/*
 * SIMPLE OBSERVER PATTERN
 * 
 * Core Concept: When something changes, automatically notify everyone who cares
 * 
 * Real-world analogy: 
 * - You subscribe to a YouTube channel (you're an observer)
 * - When they upload a video (subject changes state)
 * - You get a notification (observer gets updated)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// =============================================================================
// STEP 1: Define what an Observer looks like
// =============================================================================

typedef struct Observer Observer;
struct Observer {
    char name[50];
    void (*notify)(Observer* self, const char* message);
};

// =============================================================================
// STEP 2: Define what a Subject (thing being observed) looks like
// =============================================================================

#define MAX_OBSERVERS 5

typedef struct {
    Observer* observers[MAX_OBSERVERS];  // List of who's watching
    int observer_count;                  // How many are watching
    char current_status[100];            // Current state
} Subject;

// Add someone to watch this subject
void subscribe(Subject* subject, Observer* observer) {
    if (subject->observer_count < MAX_OBSERVERS) {
        subject->observers[subject->observer_count] = observer;
        subject->observer_count++;
        printf("✅ %s subscribed\n", observer->name);
    }
}

// Remove someone from watching
void unsubscribe(Subject* subject, Observer* observer) {
    for (int i = 0; i < subject->observer_count; i++) {
        if (subject->observers[i] == observer) {
            // Move everyone else down
            for (int j = i; j < subject->observer_count - 1; j++) {
                subject->observers[j] = subject->observers[j + 1];
            }
            subject->observer_count--;
            printf("❌ %s unsubscribed\n", observer->name);
            return;
        }
    }
}

// Tell everyone something changed!
void notify_all(Subject* subject, const char* message) {
    strcpy(subject->current_status, message);
    printf("\n📢 NOTIFICATION: %s\n", message);
    printf("   Notifying %d observers...\n", subject->observer_count);
    
    // Tell each observer
    for (int i = 0; i < subject->observer_count; i++) {
        subject->observers[i]->notify(subject->observers[i], message);
    }
}

// =============================================================================
// STEP 3: Create some concrete observers (people who want notifications)
// =============================================================================

// Observer Type 1: Email Subscriber
void email_notify(Observer* self, const char* message) {
    printf("   📧 Email to %s: %s\n", self->name, message);
}

Observer* create_email_subscriber(const char* name) {
    Observer* obs = malloc(sizeof(Observer));
    strcpy(obs->name, name);
    obs->notify = email_notify;
    return obs;
}

// Observer Type 2: Phone App
void phone_notify(Observer* self, const char* message) {
    printf("   📱 Push notification to %s: %s\n", self->name, message);
}

Observer* create_phone_app(const char* name) {
    Observer* obs = malloc(sizeof(Observer));
    strcpy(obs->name, name);
    obs->notify = phone_notify;
    return obs;
}

// Observer Type 3: Text Messages
void sms_notify(Observer* self, const char* message) {
    printf("   💬 SMS to %s: %s\n", self->name, message);
}

Observer* create_sms_subscriber(const char* name) {
    Observer* obs = malloc(sizeof(Observer));
    strcpy(obs->name, name);
    obs->notify = sms_notify;
    return obs;
}

// =============================================================================
// STEP 4: Create a subject (thing that changes and sends notifications)
// =============================================================================

Subject* create_weather_station() {
    Subject* station = malloc(sizeof(Subject));
    station->observer_count = 0;
    strcpy(station->current_status, "Unknown");
    return station;
}

// =============================================================================
// DEMO: Show how it works
// =============================================================================

int main() {
    printf("=== SIMPLE OBSERVER PATTERN ===\n\n");
    
    // Create the thing that will change (weather station)
    Subject* weather_station = create_weather_station();
    
    // Create people who want to know about weather changes
    Observer* alice_email = create_email_subscriber("Alice");
    Observer* bob_phone = create_phone_app("Bob");
    Observer* charlie_sms = create_sms_subscriber("Charlie");
    
    printf("--- Setting up subscriptions ---\n");
    // People subscribe to weather updates
    subscribe(weather_station, alice_email);
    subscribe(weather_station, bob_phone);
    subscribe(weather_station, charlie_sms);
    
    printf("\n--- Weather changes, everyone gets notified ---\n");
    // Weather changes -> everyone gets notified automatically
    notify_all(weather_station, "Sunny, 75°F");
    notify_all(weather_station, "Rain incoming!");
    
    printf("\n--- Someone unsubscribes ---\n");
    // Bob doesn't want notifications anymore
    unsubscribe(weather_station, bob_phone);
    
    printf("\n--- Weather changes again ---\n");
    // Only Alice and Charlie get notified now
    notify_all(weather_station, "Snow expected tonight");
    
    printf("\n--- Add new subscriber ---\n");
    Observer* diana_email = create_email_subscriber("Diana");
    subscribe(weather_station, diana_email);
    notify_all(weather_station, "Clear skies tomorrow");
    
    printf("\n✨ Observer Pattern Benefits:\n");
    printf("   • Weather station doesn't need to know who wants updates\n");
    printf("   • Easy to add/remove subscribers\n");
    printf("   • One change notifies everyone automatically\n");
    printf("   • Loose coupling between weather station and subscribers\n");
    
    // Cleanup
    free(weather_station);
    free(alice_email);
    free(bob_phone);
    free(charlie_sms);
    free(diana_email);
    
    return 0;
}