/*
 * STRATEGY PATTERN
 * 
 * Problem: Define a family of algorithms, encapsulate each one, and make them interchangeable
 * 
 * Use Cases:
 * - Payment processing systems
 * - Sorting algorithms
 * - Compression algorithms
 * - Route planning systems
 * 
 * Pros:
 * - Algorithms can be switched at runtime
 * - Easy to add new strategies
 * - Eliminates conditional statements
 * 
 * Cons:
 * - Clients must be aware of different strategies
 * - Increased number of objects
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Strategy interface
typedef struct PaymentStrategy PaymentStrategy;
struct PaymentStrategy {
    char name[50];
    double (*calculate_fee)(PaymentStrategy* self, double amount);
    void (*process_payment)(PaymentStrategy* self, double amount, const char* description);
    void (*destroy)(PaymentStrategy* self);
};

// Concrete Strategy 1: Credit Card Payment
typedef struct {
    PaymentStrategy base;
    char card_number[20];
    char cardholder_name[100];
    double transaction_fee_rate;
} CreditCardStrategy;

double credit_card_calculate_fee(PaymentStrategy* self, double amount) {
    CreditCardStrategy* cc = (CreditCardStrategy*)self;
    return amount * cc->transaction_fee_rate;
}

void credit_card_process_payment(PaymentStrategy* self, double amount, const char* description) {
    CreditCardStrategy* cc = (CreditCardStrategy*)self;
    double fee = cc->base.calculate_fee(self, amount);
    double total = amount + fee;
    
    printf("💳 Processing Credit Card Payment:\n");
    printf("   Card: ****%s\n", cc->card_number + strlen(cc->card_number) - 4);
    printf("   Cardholder: %s\n", cc->cardholder_name);
    printf("   Amount: $%.2f\n", amount);
    printf("   Transaction Fee: $%.2f (%.1f%%)\n", fee, cc->transaction_fee_rate * 100);
    printf("   Total Charged: $%.2f\n", total);
    printf("   Description: %s\n", description);
    printf("   Status: ✅ Payment Successful\n");
}

void credit_card_destroy(PaymentStrategy* self) {
    if (self) {
        free(self);
    }
}

PaymentStrategy* create_credit_card_strategy(const char* card_number, const char* cardholder_name) {
    CreditCardStrategy* cc = (CreditCardStrategy*)malloc(sizeof(CreditCardStrategy));
    
    strcpy(cc->base.name, "Credit Card");
    strcpy(cc->card_number, card_number);
    strcpy(cc->cardholder_name, cardholder_name);
    cc->transaction_fee_rate = 0.029; // 2.9%
    
    cc->base.calculate_fee = credit_card_calculate_fee;
    cc->base.process_payment = credit_card_process_payment;
    cc->base.destroy = credit_card_destroy;
    
    return (PaymentStrategy*)cc;
}

// Concrete Strategy 2: PayPal Payment
typedef struct {
    PaymentStrategy base;
    char email[100];
    double fixed_fee;
    double percentage_fee;
} PayPalStrategy;

double paypal_calculate_fee(PaymentStrategy* self, double amount) {
    PayPalStrategy* pp = (PayPalStrategy*)self;
    return pp->fixed_fee + (amount * pp->percentage_fee);
}

void paypal_process_payment(PaymentStrategy* self, double amount, const char* description) {
    PayPalStrategy* pp = (PayPalStrategy*)self;
    double fee = pp->base.calculate_fee(self, amount);
    double total = amount + fee;
    
    printf("🅿️ Processing PayPal Payment:\n");
    printf("   Account: %s\n", pp->email);
    printf("   Amount: $%.2f\n", amount);
    printf("   PayPal Fee: $%.2f ($%.2f fixed + %.1f%%)\n", 
           fee, pp->fixed_fee, pp->percentage_fee * 100);
    printf("   Total Charged: $%.2f\n", total);
    printf("   Description: %s\n", description);
    printf("   Status: ✅ Payment Successful\n");
}

void paypal_destroy(PaymentStrategy* self) {
    if (self) {
        free(self);
    }
}

PaymentStrategy* create_paypal_strategy(const char* email) {
    PayPalStrategy* pp = (PayPalStrategy*)malloc(sizeof(PayPalStrategy));
    
    strcpy(pp->base.name, "PayPal");
    strcpy(pp->email, email);
    pp->fixed_fee = 0.30;
    pp->percentage_fee = 0.034; // 3.4%
    
    pp->base.calculate_fee = paypal_calculate_fee;
    pp->base.process_payment = paypal_process_payment;
    pp->base.destroy = paypal_destroy;
    
    return (PaymentStrategy*)pp;
}

// Concrete Strategy 3: Bank Transfer
typedef struct {
    PaymentStrategy base;
    char bank_name[100];
    char account_number[20];
    double flat_fee;
} BankTransferStrategy;

double bank_transfer_calculate_fee(PaymentStrategy* self, double amount) {
    BankTransferStrategy* bt = (BankTransferStrategy*)self;
    return bt->flat_fee;
}

void bank_transfer_process_payment(PaymentStrategy* self, double amount, const char* description) {
    BankTransferStrategy* bt = (BankTransferStrategy*)self;
    double fee = bt->base.calculate_fee(self, amount);
    double total = amount + fee;
    
    printf("🏦 Processing Bank Transfer:\n");
    printf("   Bank: %s\n", bt->bank_name);
    printf("   Account: ****%s\n", bt->account_number + strlen(bt->account_number) - 4);
    printf("   Amount: $%.2f\n", amount);
    printf("   Transfer Fee: $%.2f (flat rate)\n", fee);
    printf("   Total: $%.2f\n", total);
    printf("   Description: %s\n", description);
    printf("   Status: ✅ Transfer Initiated (1-3 business days)\n");
}

void bank_transfer_destroy(PaymentStrategy* self) {
    if (self) {
        free(self);
    }
}

PaymentStrategy* create_bank_transfer_strategy(const char* bank_name, const char* account_number) {
    BankTransferStrategy* bt = (BankTransferStrategy*)malloc(sizeof(BankTransferStrategy));
    
    strcpy(bt->base.name, "Bank Transfer");
    strcpy(bt->bank_name, bank_name);
    strcpy(bt->account_number, account_number);
    bt->flat_fee = 1.50;
    
    bt->base.calculate_fee = bank_transfer_calculate_fee;
    bt->base.process_payment = bank_transfer_process_payment;
    bt->base.destroy = bank_transfer_destroy;
    
    return (PaymentStrategy*)bt;
}

// Concrete Strategy 4: Cryptocurrency Payment
typedef struct {
    PaymentStrategy base;
    char crypto_type[20];
    char wallet_address[50];
    double network_fee;
} CryptocurrencyStrategy;

double crypto_calculate_fee(PaymentStrategy* self, double amount) {
    CryptocurrencyStrategy* crypto = (CryptocurrencyStrategy*)self;
    return crypto->network_fee; // Fixed network fee
}

void crypto_process_payment(PaymentStrategy* self, double amount, const char* description) {
    CryptocurrencyStrategy* crypto = (CryptocurrencyStrategy*)self;
    double fee = crypto->base.calculate_fee(self, amount);
    double total = amount + fee;
    
    printf("₿ Processing Cryptocurrency Payment:\n");
    printf("   Currency: %s\n", crypto->crypto_type);
    printf("   Wallet: %s...%s\n", 
           crypto->wallet_address, crypto->wallet_address + strlen(crypto->wallet_address) - 6);
    printf("   Amount: $%.2f\n", amount);
    printf("   Network Fee: $%.2f\n", fee);
    printf("   Total: $%.2f\n", total);
    printf("   Description: %s\n", description);
    printf("   Status: ✅ Transaction Broadcast to Network\n");
}

void crypto_destroy(PaymentStrategy* self) {
    if (self) {
        free(self);
    }
}

PaymentStrategy* create_cryptocurrency_strategy(const char* crypto_type, const char* wallet_address) {
    CryptocurrencyStrategy* crypto = (CryptocurrencyStrategy*)malloc(sizeof(CryptocurrencyStrategy));
    
    strcpy(crypto->base.name, crypto_type);
    strcpy(crypto->crypto_type, crypto_type);
    strcpy(crypto->wallet_address, wallet_address);
    crypto->network_fee = 2.50; // Fixed network fee
    
    crypto->base.calculate_fee = crypto_calculate_fee;
    crypto->base.process_payment = crypto_process_payment;
    crypto->base.destroy = crypto_destroy;
    
    return (PaymentStrategy*)crypto;
}

// Context: Payment Processor
typedef struct {
    PaymentStrategy* strategy;
    char merchant_name[100];
    double total_processed;
    int transaction_count;
} PaymentProcessor;

PaymentProcessor* create_payment_processor(const char* merchant_name) {
    PaymentProcessor* processor = (PaymentProcessor*)malloc(sizeof(PaymentProcessor));
    
    strcpy(processor->merchant_name, merchant_name);
    processor->strategy = NULL;
    processor->total_processed = 0.0;
    processor->transaction_count = 0;
    
    return processor;
}

void set_payment_strategy(PaymentProcessor* processor, PaymentStrategy* strategy) {
    processor->strategy = strategy;
    printf("🔄 Payment method changed to: %s\n", strategy->name);
}

void process_payment(PaymentProcessor* processor, double amount, const char* description) {
    if (processor->strategy == NULL) {
        printf("❌ Error: No payment strategy set\n");
        return;
    }
    
    printf("\n--- Payment Processing for %s ---\n", processor->merchant_name);
    processor->strategy->process_payment(processor->strategy, amount, description);
    
    processor->total_processed += amount;
    processor->transaction_count++;
    
    printf("--- End of Transaction ---\n");
}

double calculate_total_fees(PaymentProcessor* processor, double amount) {
    if (processor->strategy == NULL) {
        return 0.0;
    }
    return processor->strategy->calculate_fee(processor->strategy, amount);
}

void print_processor_stats(PaymentProcessor* processor) {
    printf("\n📊 Payment Processor Statistics for %s:\n", processor->merchant_name);
    printf("   Total Transactions: %d\n", processor->transaction_count);
    printf("   Total Amount Processed: $%.2f\n", processor->total_processed);
    if (processor->strategy) {
        printf("   Current Payment Method: %s\n", processor->strategy->name);
    }
}

void destroy_payment_processor(PaymentProcessor* processor) {
    if (processor) {
        free(processor);
    }
}

// Example usage
int main() {
    printf("=== STRATEGY PATTERN EXAMPLE ===\n\n");
    
    // Create payment processor (context)
    PaymentProcessor* processor = create_payment_processor("TechStore Online");
    
    // Create different payment strategies
    PaymentStrategy* credit_card = create_credit_card_strategy("1234567812345678", "John Doe");
    PaymentStrategy* paypal = create_paypal_strategy("john.doe@email.com");
    PaymentStrategy* bank_transfer = create_bank_transfer_strategy("First National Bank", "1234567890");
    PaymentStrategy* bitcoin = create_cryptocurrency_strategy("Bitcoin", "1A1zP1eP5QGefi2DMPTfTL5SLmv7DivfNa");
    
    printf("--- Demonstrating different payment strategies ---\n");
    
    // Process payment with credit card
    set_payment_strategy(processor, credit_card);
    process_payment(processor, 299.99, "Wireless Headphones");
    
    // Switch to PayPal
    set_payment_strategy(processor, paypal);
    process_payment(processor, 89.50, "USB-C Cable");
    
    // Switch to bank transfer for large amount
    set_payment_strategy(processor, bank_transfer);
    process_payment(processor, 1299.00, "Gaming Laptop");
    
    // Switch to cryptocurrency
    set_payment_strategy(processor, bitcoin);
    process_payment(processor, 45.75, "Phone Case");
    
    printf("\n--- Comparing fees for the same amount ---\n");
    double test_amount = 100.00;
    
    printf("For a $%.2f purchase:\n", test_amount);
    printf("Credit Card fee: $%.2f\n", credit_card->calculate_fee(credit_card, test_amount));
    printf("PayPal fee: $%.2f\n", paypal->calculate_fee(paypal, test_amount));
    printf("Bank Transfer fee: $%.2f\n", bank_transfer->calculate_fee(bank_transfer, test_amount));
    printf("Bitcoin fee: $%.2f\n", bitcoin->calculate_fee(bitcoin, test_amount));
    
    print_processor_stats(processor);
    
    printf("\n--- Strategy Pattern Benefits Demonstrated ---\n");
    printf("✅ Algorithms (payment methods) are interchangeable at runtime\n");
    printf("✅ Easy to add new payment strategies without changing existing code\n");
    printf("✅ Client code doesn't depend on specific payment implementation\n");
    printf("✅ Each strategy encapsulates its own fee calculation logic\n");
    
    // Cleanup
    destroy_payment_processor(processor);
    credit_card->destroy(credit_card);
    paypal->destroy(paypal);
    bank_transfer->destroy(bank_transfer);
    bitcoin->destroy(bitcoin);
    
    return 0;
}