/*
 * CHAIN OF RESPONSIBILITY PATTERN
 * 
 * Problem: Pass requests along a chain of handlers until one handles it
 * 
 * Use Cases:
 * - Event handling systems
 * - Logging systems with multiple levels
 * - Authorization systems
 * - Support ticket escalation
 * 
 * Pros:
 * - Decouples sender from receiver
 * - Dynamic chain configuration
 * - Single responsibility principle
 * 
 * Cons:
 * - No guarantee request will be handled
 * - Performance concerns with long chains
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Request structure
typedef struct {
    char type[50];
    char description[200];
    int priority;      // 1=Low, 2=Medium, 3=High, 4=Critical
    char requester[100];
    double amount;     // For expense requests
} Request;

// Handler interface
typedef struct Handler Handler;
struct Handler {
    char name[100];
    int max_priority;
    Handler* next_handler;
    
    void (*handle_request)(Handler* self, Request* request);
    void (*set_next)(Handler* self, Handler* next);
    void (*destroy)(Handler* self);
};

// Base handler implementation
void handler_set_next(Handler* self, Handler* next) {
    self->next_handler = next;
    printf("🔗 Linked %s → %s\n", self->name, next ? next->name : "NULL");
}

// Concrete Handler 1: Help Desk Agent
typedef struct {
    Handler base;
    int tickets_handled_today;
} HelpDeskAgent;

void help_desk_handle_request(Handler* self, Request* request) {
    HelpDeskAgent* agent = (HelpDeskAgent*)self;
    
    printf("\n📞 %s received request:\n", self->name);
    printf("   Type: %s\n", request->type);
    printf("   Description: %s\n", request->description);
    printf("   Priority: %d\n", request->priority);
    printf("   Requester: %s\n", request->requester);
    
    if (request->priority <= self->max_priority && 
        (strcmp(request->type, "password_reset") == 0 || 
         strcmp(request->type, "software_install") == 0 ||
         strcmp(request->type, "basic_support") == 0)) {
        
        printf("✅ %s handled the request\n", self->name);
        printf("   Solution: Basic troubleshooting steps provided\n");
        agent->tickets_handled_today++;
        printf("   Tickets handled today: %d\n", agent->tickets_handled_today);
    } else {
        printf("❌ %s cannot handle this request (priority %d > %d or unsupported type)\n", 
               self->name, request->priority, self->max_priority);
        
        if (self->next_handler) {
            printf("🔄 Escalating to %s\n", self->next_handler->name);
            self->next_handler->handle_request(self->next_handler, request);
        } else {
            printf("❌ No more handlers in chain - request cannot be processed\n");
        }
    }
}

void help_desk_destroy(Handler* self) {
    if (self) {
        free(self);
    }
}

Handler* create_help_desk_agent(const char* name) {
    HelpDeskAgent* agent = (HelpDeskAgent*)malloc(sizeof(HelpDeskAgent));
    
    strcpy(agent->base.name, name);
    agent->base.max_priority = 2; // Can handle Low and Medium priority
    agent->base.next_handler = NULL;
    agent->tickets_handled_today = 0;
    
    agent->base.handle_request = help_desk_handle_request;
    agent->base.set_next = handler_set_next;
    agent->base.destroy = help_desk_destroy;
    
    return (Handler*)agent;
}

// Concrete Handler 2: System Administrator
typedef struct {
    Handler base;
    int servers_managed;
} SystemAdministrator;

void sysadmin_handle_request(Handler* self, Request* request) {
    SystemAdministrator* sysadmin = (SystemAdministrator*)self;
    
    printf("\n🖥️ %s received request:\n", self->name);
    printf("   Type: %s\n", request->type);
    printf("   Description: %s\n", request->description);
    printf("   Priority: %d\n", request->priority);
    
    if (request->priority <= self->max_priority && 
        (strcmp(request->type, "server_issue") == 0 || 
         strcmp(request->type, "network_problem") == 0 ||
         strcmp(request->type, "database_access") == 0 ||
         strcmp(request->type, "software_install") == 0)) {
        
        printf("✅ %s handled the request\n", self->name);
        printf("   Solution: System-level troubleshooting completed\n");
        printf("   Servers managed: %d\n", sysadmin->servers_managed);
    } else {
        printf("❌ %s cannot handle this request (priority %d > %d or requires management approval)\n", 
               self->name, request->priority, self->max_priority);
        
        if (self->next_handler) {
            printf("🔄 Escalating to %s\n", self->next_handler->name);
            self->next_handler->handle_request(self->next_handler, request);
        } else {
            printf("❌ No more handlers in chain - request cannot be processed\n");
        }
    }
}

void sysadmin_destroy(Handler* self) {
    if (self) {
        free(self);
    }
}

Handler* create_system_administrator(const char* name) {
    SystemAdministrator* sysadmin = (SystemAdministrator*)malloc(sizeof(SystemAdministrator));
    
    strcpy(sysadmin->base.name, name);
    sysadmin->base.max_priority = 3; // Can handle Low, Medium, and High priority
    sysadmin->base.next_handler = NULL;
    sysadmin->servers_managed = 25;
    
    sysadmin->base.handle_request = sysadmin_handle_request;
    sysadmin->base.set_next = handler_set_next;
    sysadmin->base.destroy = sysadmin_destroy;
    
    return (Handler*)sysadmin;
}

// Concrete Handler 3: IT Manager
typedef struct {
    Handler base;
    double budget_authority;
} ITManager;

void it_manager_handle_request(Handler* self, Request* request) {
    ITManager* manager = (ITManager*)self;
    
    printf("\n👔 %s received request:\n", self->name);
    printf("   Type: %s\n", request->type);
    printf("   Description: %s\n", request->description);
    printf("   Priority: %d\n", request->priority);
    if (request->amount > 0) {
        printf("   Amount: $%.2f\n", request->amount);
    }
    
    if (request->priority <= self->max_priority) {
        if (strcmp(request->type, "budget_approval") == 0) {
            if (request->amount <= manager->budget_authority) {
                printf("✅ %s approved the budget request\n", self->name);
                printf("   Approved amount: $%.2f (within authority: $%.2f)\n", 
                       request->amount, manager->budget_authority);
            } else {
                printf("❌ %s cannot approve - amount exceeds authority\n", self->name);
                if (self->next_handler) {
                    printf("🔄 Escalating to %s\n", self->next_handler->name);
                    self->next_handler->handle_request(self->next_handler, request);
                }
            }
        } else {
            printf("✅ %s handled the high-priority request\n", self->name);
            printf("   Solution: Management decision made\n");
        }
    } else {
        printf("❌ %s cannot handle this critical request\n", self->name);
        
        if (self->next_handler) {
            printf("🔄 Escalating to %s\n", self->next_handler->name);
            self->next_handler->handle_request(self->next_handler, request);
        } else {
            printf("❌ No more handlers in chain - request cannot be processed\n");
        }
    }
}

void it_manager_destroy(Handler* self) {
    if (self) {
        free(self);
    }
}

Handler* create_it_manager(const char* name, double budget_authority) {
    ITManager* manager = (ITManager*)malloc(sizeof(ITManager));
    
    strcpy(manager->base.name, name);
    manager->base.max_priority = 4; // Can handle all priorities
    manager->base.next_handler = NULL;
    manager->budget_authority = budget_authority;
    
    manager->base.handle_request = it_manager_handle_request;
    manager->base.set_next = handler_set_next;
    manager->base.destroy = it_manager_destroy;
    
    return (Handler*)manager;
}

// Concrete Handler 4: CTO (Chief Technology Officer)
typedef struct {
    Handler base;
    double budget_authority;
} CTO;

void cto_handle_request(Handler* self, Request* request) {
    CTO* cto = (CTO*)self;
    
    printf("\n🎯 %s received request:\n", self->name);
    printf("   Type: %s\n", request->type);
    printf("   Description: %s\n", request->description);
    printf("   Priority: %d (CRITICAL)\n", request->priority);
    if (request->amount > 0) {
        printf("   Amount: $%.2f\n", request->amount);
    }
    
    // CTO can handle anything
    printf("✅ %s handled the critical request\n", self->name);
    if (strcmp(request->type, "budget_approval") == 0) {
        printf("   Executive approval granted for $%.2f\n", request->amount);
    } else {
        printf("   Executive decision made - all resources allocated\n");
    }
    printf("   Authority level: Executive\n");
}

void cto_destroy(Handler* self) {
    if (self) {
        free(self);
    }
}

Handler* create_cto(const char* name) {
    CTO* cto = (CTO*)malloc(sizeof(CTO));
    
    strcpy(cto->base.name, name);
    cto->base.max_priority = 4; // Can handle all priorities
    cto->base.next_handler = NULL;
    cto->budget_authority = 1000000.0; // $1M authority
    
    cto->base.handle_request = cto_handle_request;
    cto->base.set_next = handler_set_next;
    cto->base.destroy = cto_destroy;
    
    return (Handler*)cto;
}

// Helper functions
Request create_request(const char* type, const char* description, int priority, 
                      const char* requester, double amount) {
    Request request;
    strcpy(request.type, type);
    strcpy(request.description, description);
    request.priority = priority;
    strcpy(request.requester, requester);
    request.amount = amount;
    return request;
}

void print_chain_structure(Handler* first) {
    printf("\n🔗 Chain Structure:\n");
    Handler* current = first;
    int level = 1;
    while (current) {
        printf("   Level %d: %s (max priority: %d)\n", level, current->name, current->max_priority);
        current = current->next_handler;
        level++;
    }
    printf("\n");
}

// Example usage
int main() {
    printf("=== CHAIN OF RESPONSIBILITY PATTERN EXAMPLE ===\n\n");
    
    // Create the chain of handlers
    Handler* help_desk = create_help_desk_agent("Help Desk Agent");
    Handler* sysadmin = create_system_administrator("System Administrator");
    Handler* it_manager = create_it_manager("IT Manager", 10000.0);
    Handler* cto = create_cto("Chief Technology Officer");
    
    printf("--- Building the chain ---\n");
    help_desk->set_next(help_desk, sysadmin);
    sysadmin->set_next(sysadmin, it_manager);
    it_manager->set_next(it_manager, cto);
    
    print_chain_structure(help_desk);
    
    printf("--- Processing various requests ---\n");
    
    // Request 1: Simple password reset (should be handled by help desk)
    printf("=== REQUEST 1 ===");
    Request req1 = create_request("password_reset", "User forgot domain password", 1, "john.doe@company.com", 0);
    help_desk->handle_request(help_desk, &req1);
    
    // Request 2: Server issue (should escalate to sysadmin)
    printf("\n=== REQUEST 2 ===");
    Request req2 = create_request("server_issue", "Database server not responding", 3, "alice.smith@company.com", 0);
    help_desk->handle_request(help_desk, &req2);
    
    // Request 3: Budget approval (should escalate to IT manager)
    printf("\n=== REQUEST 3 ===");
    Request req3 = create_request("budget_approval", "New workstations for development team", 2, "dev.manager@company.com", 5000.0);
    help_desk->handle_request(help_desk, &req3);
    
    // Request 4: Large budget approval (should escalate to CTO)
    printf("\n=== REQUEST 4 ===");
    Request req4 = create_request("budget_approval", "Data center expansion project", 4, "it.manager@company.com", 50000.0);
    help_desk->handle_request(help_desk, &req4);
    
    // Request 5: Critical security breach (should escalate to CTO)
    printf("\n=== REQUEST 5 ===");
    Request req5 = create_request("security_breach", "Potential data breach detected", 4, "security.team@company.com", 0);
    help_desk->handle_request(help_desk, &req5);
    
    // Request 6: Start from different point in chain
    printf("\n=== REQUEST 6 (Starting from IT Manager) ===");
    Request req6 = create_request("policy_change", "Update remote work policy", 3, "hr.manager@company.com", 0);
    it_manager->handle_request(it_manager, &req6);
    
    printf("\n--- Chain of Responsibility Benefits Demonstrated ---\n");
    printf("✅ Requests automatically escalate to appropriate handler\n");
    printf("✅ Handlers are decoupled from each other\n");
    printf("✅ Easy to add/remove/reorder handlers\n");
    printf("✅ Each handler has single responsibility\n");
    printf("✅ Dynamic chain configuration possible\n");
    
    // Test dynamic reconfiguration
    printf("\n--- Dynamic Chain Reconfiguration ---\n");
    printf("Removing System Administrator from chain:\n");
    help_desk->set_next(help_desk, it_manager);
    
    printf("=== REQUEST 7 (Modified Chain) ===");
    Request req7 = create_request("server_issue", "Another server problem", 3, "ops.team@company.com", 0);
    help_desk->handle_request(help_desk, &req7);
    
    // Cleanup
    help_desk->destroy(help_desk);
    sysadmin->destroy(sysadmin);
    it_manager->destroy(it_manager);
    cto->destroy(cto);
    
    return 0;
}