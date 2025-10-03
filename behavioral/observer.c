/*
 * OBSERVER PATTERN
 * 
 * Problem: Define a one-to-many dependency between objects so that when one object 
 *          changes state, all dependents are notified automatically
 * 
 * Use Cases:
 * - Event handling systems
 * - Model-View architectures
 * - News subscription systems
 * - Stock price monitoring
 * 
 * Pros:
 * - Loose coupling between subject and observers
 * - Dynamic subscription/unsubscription
 * - Broadcast communication
 * 
 * Cons:
 * - Unexpected updates
 * - Memory leaks if observers aren't properly removed
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Forward declarations
typedef struct Observer Observer;
typedef struct Subject Subject;

// Observer interface
struct Observer {
    char name[100];
    void (*update)(Observer* self, Subject* subject, const char* event_data);
    void (*destroy)(Observer* self);
};

// Subject interface
#define MAX_OBSERVERS 20

struct Subject {
    Observer* observers[MAX_OBSERVERS];
    int observer_count;
    char state[256];
    
    void (*attach)(Subject* self, Observer* observer);
    void (*detach)(Subject* self, Observer* observer);
    void (*notify)(Subject* self, const char* event_data);
    void (*set_state)(Subject* self, const char* new_state);
    const char* (*get_state)(Subject* self);
    void (*destroy)(Subject* self);
};

// Concrete Subject: News Agency
typedef struct {
    Subject base;
    char latest_news[512];
    char category[100];
} NewsAgency;

void news_agency_attach(Subject* self, Observer* observer) {
    NewsAgency* agency = (NewsAgency*)self;
    if (agency->base.observer_count < MAX_OBSERVERS) {
        agency->base.observers[agency->base.observer_count] = observer;
        agency->base.observer_count++;
        printf("📰 %s subscribed to %s news\n", observer->name, agency->category);
    } else {
        printf("Error: Too many subscribers\n");
    }
}

void news_agency_detach(Subject* self, Observer* observer) {
    NewsAgency* agency = (NewsAgency*)self;
    for (int i = 0; i < agency->base.observer_count; i++) {
        if (agency->base.observers[i] == observer) {
            // Shift remaining observers
            for (int j = i; j < agency->base.observer_count - 1; j++) {
                agency->base.observers[j] = agency->base.observers[j + 1];
            }
            agency->base.observer_count--;
            printf("📰 %s unsubscribed from %s news\n", observer->name, agency->category);
            return;
        }
    }
    printf("Error: Observer not found\n");
}

void news_agency_notify(Subject* self, const char* event_data) {
    NewsAgency* agency = (NewsAgency*)self;
    printf("\n🔔 Broadcasting %s news to %d subscribers...\n", 
           agency->category, agency->base.observer_count);
    
    for (int i = 0; i < agency->base.observer_count; i++) {
        agency->base.observers[i]->update(agency->base.observers[i], self, event_data);
    }
}

void news_agency_set_state(Subject* self, const char* new_state) {
    NewsAgency* agency = (NewsAgency*)self;
    strcpy(agency->base.state, new_state);
    strcpy(agency->latest_news, new_state);
    agency->base.notify(self, new_state);
}

const char* news_agency_get_state(Subject* self) {
    NewsAgency* agency = (NewsAgency*)self;
    return agency->latest_news;
}

void news_agency_destroy(Subject* self) {
    if (self) {
        free(self);
    }
}

Subject* create_news_agency(const char* category) {
    NewsAgency* agency = (NewsAgency*)malloc(sizeof(NewsAgency));
    
    strcpy(agency->category, category);
    strcpy(agency->latest_news, "No news yet");
    strcpy(agency->base.state, "No news yet");
    agency->base.observer_count = 0;
    
    agency->base.attach = news_agency_attach;
    agency->base.detach = news_agency_detach;
    agency->base.notify = news_agency_notify;
    agency->base.set_state = news_agency_set_state;
    agency->base.get_state = news_agency_get_state;
    agency->base.destroy = news_agency_destroy;
    
    return (Subject*)agency;
}

// Concrete Observer: News Channel
typedef struct {
    Observer base;
    char channel_type[50];
    int viewer_count;
} NewsChannel;

void news_channel_update(Observer* self, Subject* subject, const char* event_data) {
    NewsChannel* channel = (NewsChannel*)self;
    NewsAgency* agency = (NewsAgency*)subject;
    
    printf("📺 %s (%s, %d viewers): Breaking - %s\n", 
           channel->base.name, channel->channel_type, channel->viewer_count, event_data);
}

void news_channel_destroy(Observer* self) {
    if (self) {
        free(self);
    }
}

Observer* create_news_channel(const char* name, const char* type, int viewers) {
    NewsChannel* channel = (NewsChannel*)malloc(sizeof(NewsChannel));
    
    strcpy(channel->base.name, name);
    strcpy(channel->channel_type, type);
    channel->viewer_count = viewers;
    
    channel->base.update = news_channel_update;
    channel->base.destroy = news_channel_destroy;
    
    return (Observer*)channel;
}

// Concrete Observer: Mobile App
typedef struct {
    Observer base;
    char platform[50];
    int push_enabled;
} MobileApp;

void mobile_app_update(Observer* self, Subject* subject, const char* event_data) {
    MobileApp* app = (MobileApp*)self;
    
    if (app->push_enabled) {
        printf("📱 %s (%s): PUSH NOTIFICATION - %s\n", 
               app->base.name, app->platform, event_data);
    } else {
        printf("📱 %s (%s): News updated (notifications disabled)\n", 
               app->base.name, app->platform);
    }
}

void mobile_app_destroy(Observer* self) {
    if (self) {
        free(self);
    }
}

Observer* create_mobile_app(const char* name, const char* platform, int push_enabled) {
    MobileApp* app = (MobileApp*)malloc(sizeof(MobileApp));
    
    strcpy(app->base.name, name);
    strcpy(app->platform, platform);
    app->push_enabled = push_enabled;
    
    app->base.update = mobile_app_update;
    app->base.destroy = mobile_app_destroy;
    
    return (Observer*)app;
}

// Concrete Observer: Email Subscriber
typedef struct {
    Observer base;
    char email[100];
    char subscription_type[50];
} EmailSubscriber;

void email_subscriber_update(Observer* self, Subject* subject, const char* event_data) {
    EmailSubscriber* subscriber = (EmailSubscriber*)self;
    
    printf("📧 Email to %s (%s): %s\n", 
           subscriber->email, subscriber->subscription_type, event_data);
}

void email_subscriber_destroy(Observer* self) {
    if (self) {
        free(self);
    }
}

Observer* create_email_subscriber(const char* name, const char* email, const char* type) {
    EmailSubscriber* subscriber = (EmailSubscriber*)malloc(sizeof(EmailSubscriber));
    
    strcpy(subscriber->base.name, name);
    strcpy(subscriber->email, email);
    strcpy(subscriber->subscription_type, type);
    
    subscriber->base.update = email_subscriber_update;
    subscriber->base.destroy = email_subscriber_destroy;
    
    return (Observer*)subscriber;
}

// Example usage
int main() {
    printf("=== OBSERVER PATTERN EXAMPLE ===\n\n");
    
    // Create news agencies (subjects)
    Subject* tech_news = create_news_agency("Technology");
    Subject* sports_news = create_news_agency("Sports");
    
    // Create news consumers (observers)
    Observer* cnn = create_news_channel("CNN Tech", "Cable TV", 1000000);
    Observer* bbc = create_news_channel("BBC Technology", "Public TV", 800000);
    Observer* tech_app = create_mobile_app("TechNews App", "iOS", 1);
    Observer* sports_app = create_mobile_app("SportsFan App", "Android", 1);
    Observer* john_email = create_email_subscriber("John Doe", "john@email.com", "Daily Digest");
    Observer* jane_email = create_email_subscriber("Jane Smith", "jane@email.com", "Breaking News");
    
    printf("--- Setting up subscriptions ---\n");
    
    // Subscribe to tech news
    tech_news->attach(tech_news, cnn);
    tech_news->attach(tech_news, bbc);
    tech_news->attach(tech_news, tech_app);
    tech_news->attach(tech_news, john_email);
    
    // Subscribe to sports news
    sports_news->attach(sports_news, sports_app);
    sports_news->attach(sports_news, jane_email);
    
    // Cross-subscription: BBC also wants sports news
    sports_news->attach(sports_news, bbc);
    
    printf("\n--- Publishing news updates ---\n");
    
    // Publish tech news
    tech_news->set_state(tech_news, "Apple announces new iPhone with revolutionary AI features!");
    
    // Publish sports news
    sports_news->set_state(sports_news, "World Cup Final: Argentina beats France 4-2 in penalty shootout!");
    
    // Another tech news
    tech_news->set_state(tech_news, "OpenAI releases GPT-5 with unprecedented capabilities!");
    
    printf("\n--- Subscription changes ---\n");
    
    // John unsubscribes from tech news
    tech_news->detach(tech_news, john_email);
    
    // Create new subscriber
    Observer* mobile_digest = create_mobile_app("Mobile Digest", "Cross-platform", 0);
    tech_news->attach(tech_news, mobile_digest);
    
    // Publish another news item
    tech_news->set_state(tech_news, "Tesla unveils fully autonomous driving system!");
    
    printf("\n--- Observer Pattern Benefits Demonstrated ---\n");
    printf("✅ Loose coupling: Subjects don't know specific observer types\n");
    printf("✅ Dynamic subscription: Observers can subscribe/unsubscribe at runtime\n");
    printf("✅ Broadcast communication: One update notifies all interested parties\n");
    printf("✅ Open/Closed principle: Easy to add new observer types\n");
    
    // Cleanup
    tech_news->destroy(tech_news);
    sports_news->destroy(sports_news);
    
    cnn->destroy(cnn);
    bbc->destroy(bbc);
    tech_app->destroy(tech_app);
    sports_app->destroy(sports_app);
    john_email->destroy(john_email);
    jane_email->destroy(jane_email);
    mobile_digest->destroy(mobile_digest);
    
    return 0;
}