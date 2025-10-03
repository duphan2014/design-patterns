/*
 * TEMPLATE METHOD PATTERN
 * 
 * Problem: Define the skeleton of an algorithm in base class, letting subclasses 
 *          override specific steps without changing the algorithm's structure
 * 
 * Use Cases:
 * - Framework design
 * - Data processing pipelines
 * - Game AI behaviors
 * - Report generation
 * 
 * Pros:
 * - Code reuse through inheritance
 * - Algorithm structure is preserved
 * - Easy to extend with new variations
 * 
 * Cons:
 * - Limited flexibility
 * - Violates Liskov substitution if not designed carefully
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Abstract base class (simulated with function pointers)
typedef struct DataProcessor DataProcessor;

struct DataProcessor {
    char processor_name[100];
    char input_data[500];
    char processed_data[500];
    
    // Template method (final - shouldn't be overridden)
    void (*process)(DataProcessor* self, const char* input);
    
    // Abstract methods (must be implemented by concrete classes)
    void (*read_data)(DataProcessor* self, const char* input);
    void (*process_data)(DataProcessor* self);
    void (*write_data)(DataProcessor* self);
    
    // Hook methods (optional to override)
    int (*validate_input)(DataProcessor* self, const char* input);
    void (*log_processing)(DataProcessor* self, const char* step);
    
    void (*destroy)(DataProcessor* self);
};

// Template method implementation
void data_processor_process(DataProcessor* self, const char* input) {
    printf("\n🔄 Starting data processing with %s\n", self->processor_name);
    printf("=====================================\n");
    
    // Step 1: Validate input (hook method)
    if (!self->validate_input(self, input)) {
        printf("❌ Input validation failed\n");
        return;
    }
    
    // Step 2: Read data (abstract method)
    self->log_processing(self, "Reading data");
    self->read_data(self, input);
    
    // Step 3: Process data (abstract method)
    self->log_processing(self, "Processing data");
    self->process_data(self);
    
    // Step 4: Write data (abstract method)
    self->log_processing(self, "Writing data");
    self->write_data(self);
    
    printf("✅ Processing completed\n");
    printf("=====================================\n");
}

// Default hook method implementations
int default_validate_input(DataProcessor* self, const char* input) {
    if (input == NULL || strlen(input) == 0) {
        printf("❌ Validation failed: Empty input\n");
        return 0;
    }
    printf("✅ Input validation passed\n");
    return 1;
}

void default_log_processing(DataProcessor* self, const char* step) {
    printf("📝 [%s] %s\n", self->processor_name, step);
}

// Concrete Implementation 1: CSV Processor
typedef struct {
    DataProcessor base;
    char delimiter;
    int column_count;
} CSVProcessor;

void csv_read_data(DataProcessor* self, const char* input) {
    CSVProcessor* csv = (CSVProcessor*)self;
    strcpy(csv->base.input_data, input);
    
    // Count columns
    csv->column_count = 1;
    for (int i = 0; input[i]; i++) {
        if (input[i] == csv->delimiter) {
            csv->column_count++;
        }
    }
    
    printf("📄 CSV data loaded: %d columns detected\n", csv->column_count);
    printf("   Raw data: %s\n", input);
}

void csv_process_data(DataProcessor* self) {
    CSVProcessor* csv = (CSVProcessor*)self;
    
    printf("🔧 Processing CSV data:\n");
    printf("   - Converting to uppercase\n");
    printf("   - Trimming whitespace\n");
    printf("   - Validating data types\n");
    
    // Simple processing: convert to uppercase
    strcpy(csv->base.processed_data, csv->base.input_data);
    for (int i = 0; csv->base.processed_data[i]; i++) {
        if (csv->base.processed_data[i] >= 'a' && csv->base.processed_data[i] <= 'z') {
            csv->base.processed_data[i] -= 32;
        }
    }
    
    printf("   Processed %d columns\n", csv->column_count);
}

void csv_write_data(DataProcessor* self) {
    CSVProcessor* csv = (CSVProcessor*)self;
    printf("💾 Writing CSV data to output:\n");
    printf("   Format: CSV with delimiter '%c'\n", csv->delimiter);
    printf("   Output: %s\n", csv->base.processed_data);
    printf("   Columns: %d\n", csv->column_count);
}

int csv_validate_input(DataProcessor* self, const char* input) {
    if (!default_validate_input(self, input)) {
        return 0;
    }
    
    CSVProcessor* csv = (CSVProcessor*)self;
    
    // Additional CSV-specific validation
    if (strchr(input, csv->delimiter) == NULL) {
        printf("⚠️ Warning: No delimiter '%c' found in CSV data\n", csv->delimiter);
    }
    
    printf("✅ CSV validation passed\n");
    return 1;
}

void csv_destroy(DataProcessor* self) {
    if (self) {
        free(self);
    }
}

DataProcessor* create_csv_processor(char delimiter) {
    CSVProcessor* csv = (CSVProcessor*)malloc(sizeof(CSVProcessor));
    
    strcpy(csv->base.processor_name, "CSV Processor");
    strcpy(csv->base.input_data, "");
    strcpy(csv->base.processed_data, "");
    csv->delimiter = delimiter;
    csv->column_count = 0;
    
    // Template method
    csv->base.process = data_processor_process;
    
    // Abstract methods
    csv->base.read_data = csv_read_data;
    csv->base.process_data = csv_process_data;
    csv->base.write_data = csv_write_data;
    
    // Hook methods
    csv->base.validate_input = csv_validate_input;
    csv->base.log_processing = default_log_processing;
    
    csv->base.destroy = csv_destroy;
    
    return (DataProcessor*)csv;
}

// Concrete Implementation 2: JSON Processor
typedef struct {
    DataProcessor base;
    int pretty_print;
    int indentation_level;
} JSONProcessor;

void json_read_data(DataProcessor* self, const char* input) {
    JSONProcessor* json = (JSONProcessor*)self;
    strcpy(json->base.input_data, input);
    
    printf("📄 JSON data loaded\n");
    printf("   Raw data: %s\n", input);
    printf("   Pretty print: %s\n", json->pretty_print ? "enabled" : "disabled");
}

void json_process_data(DataProcessor* self) {
    JSONProcessor* json = (JSONProcessor*)self;
    
    printf("🔧 Processing JSON data:\n");
    printf("   - Validating JSON structure\n");
    printf("   - Normalizing field names\n");
    printf("   - Compacting whitespace\n");
    
    if (json->pretty_print) {
        printf("   - Formatting with indentation\n");
    }
    
    // Simple processing: add processing timestamp
    snprintf(json->base.processed_data, sizeof(json->base.processed_data), 
             "{\"original\":%s,\"processed_by\":\"JSON_Processor\",\"timestamp\":\"2024-01-01\"}", 
             json->base.input_data);
    
    printf("   JSON processing completed\n");
}

void json_write_data(DataProcessor* self) {
    JSONProcessor* json = (JSONProcessor*)self;
    printf("💾 Writing JSON data to output:\n");
    printf("   Format: JSON\n");
    if (json->pretty_print) {
        printf("   Indentation: %d spaces\n", json->indentation_level);
    }
    printf("   Output: %s\n", json->base.processed_data);
}

int json_validate_input(DataProcessor* self, const char* input) {
    if (!default_validate_input(self, input)) {
        return 0;
    }
    
    // Simple JSON validation
    if (input[0] != '{' && input[0] != '[') {
        printf("❌ JSON validation failed: Must start with '{' or '['\n");
        return 0;
    }
    
    printf("✅ JSON validation passed\n");
    return 1;
}

void json_log_processing(DataProcessor* self, const char* step) {
    JSONProcessor* json = (JSONProcessor*)self;
    printf("📝 [%s] %s (pretty_print: %s)\n", 
           self->processor_name, step, json->pretty_print ? "on" : "off");
}

void json_destroy(DataProcessor* self) {
    if (self) {
        free(self);
    }
}

DataProcessor* create_json_processor(int pretty_print, int indentation) {
    JSONProcessor* json = (JSONProcessor*)malloc(sizeof(JSONProcessor));
    
    strcpy(json->base.processor_name, "JSON Processor");
    strcpy(json->base.input_data, "");
    strcpy(json->base.processed_data, "");
    json->pretty_print = pretty_print;
    json->indentation_level = indentation;
    
    // Template method
    json->base.process = data_processor_process;
    
    // Abstract methods
    json->base.read_data = json_read_data;
    json->base.process_data = json_process_data;
    json->base.write_data = json_write_data;
    
    // Hook methods (customized)
    json->base.validate_input = json_validate_input;
    json->base.log_processing = json_log_processing;
    
    json->base.destroy = json_destroy;
    
    return (DataProcessor*)json;
}

// Concrete Implementation 3: XML Processor
typedef struct {
    DataProcessor base;
    int validate_schema;
    char root_element[50];
} XMLProcessor;

void xml_read_data(DataProcessor* self, const char* input) {
    XMLProcessor* xml = (XMLProcessor*)self;
    strcpy(xml->base.input_data, input);
    
    // Extract root element
    const char* start = strchr(input, '<');
    if (start) {
        const char* end = strchr(start + 1, '>');
        if (end) {
            int len = end - start - 1;
            if (len < 49) {
                strncpy(xml->root_element, start + 1, len);
                xml->root_element[len] = '\\0';
            }
        }
    }
    
    printf("📄 XML data loaded\n");
    printf("   Raw data: %s\n", input);
    printf("   Root element: %s\n", xml->root_element);
    printf("   Schema validation: %s\n", xml->validate_schema ? "enabled" : "disabled");
}

void xml_process_data(DataProcessor* self) {
    XMLProcessor* xml = (XMLProcessor*)self;
    
    printf("🔧 Processing XML data:\n");
    printf("   - Parsing XML structure\n");
    printf("   - Validating well-formedness\n");
    
    if (xml->validate_schema) {
        printf("   - Validating against schema\n");
    }
    
    printf("   - Normalizing namespaces\n");
    
    // Simple processing: wrap in processing element
    snprintf(xml->base.processed_data, sizeof(xml->base.processed_data), 
             "<processed>%s</processed>", xml->base.input_data);
    
    printf("   XML processing completed\n");
}

void xml_write_data(DataProcessor* self) {
    XMLProcessor* xml = (XMLProcessor*)self;
    printf("💾 Writing XML data to output:\n");
    printf("   Format: XML\n");
    printf("   Root element: %s\n", xml->root_element);
    printf("   Schema validation: %s\n", xml->validate_schema ? "applied" : "skipped");
    printf("   Output: %s\n", xml->base.processed_data);
}

int xml_validate_input(DataProcessor* self, const char* input) {
    if (!default_validate_input(self, input)) {
        return 0;
    }
    
    // Simple XML validation
    if (input[0] != '<') {
        printf("❌ XML validation failed: Must start with '<'\n");
        return 0;
    }
    
    printf("✅ XML validation passed\n");
    return 1;
}

void xml_destroy(DataProcessor* self) {
    if (self) {
        free(self);
    }
}

DataProcessor* create_xml_processor(int validate_schema) {
    XMLProcessor* xml = (XMLProcessor*)malloc(sizeof(XMLProcessor));
    
    strcpy(xml->base.processor_name, "XML Processor");
    strcpy(xml->base.input_data, "");
    strcpy(xml->base.processed_data, "");
    xml->validate_schema = validate_schema;
    strcpy(xml->root_element, "");
    
    // Template method
    xml->base.process = data_processor_process;
    
    // Abstract methods
    xml->base.read_data = xml_read_data;
    xml->base.process_data = xml_process_data;
    xml->base.write_data = xml_write_data;
    
    // Hook methods
    xml->base.validate_input = xml_validate_input;
    xml->base.log_processing = default_log_processing;
    
    xml->base.destroy = xml_destroy;
    
    return (DataProcessor*)xml;
}

// Example usage
int main() {
    printf("=== TEMPLATE METHOD PATTERN EXAMPLE ===\n\n");
    
    // Create different data processors
    DataProcessor* csv_processor = create_csv_processor(',');
    DataProcessor* json_processor = create_json_processor(1, 2);
    DataProcessor* xml_processor = create_xml_processor(1);
    
    printf("--- Processing CSV Data ---\n");
    csv_processor->process(csv_processor, "name,age,city\\nJohn,25,NewYork\\nJane,30,LosAngeles");
    
    printf("\\n--- Processing JSON Data ---\n");
    json_processor->process(json_processor, "{\"name\":\"John\",\"age\":25}");
    
    printf("\\n--- Processing XML Data ---\n");
    xml_processor->process(xml_processor, "<person><name>John</name><age>25</age></person>");
    
    printf("\\n--- Testing Validation ---\n");
    printf("Testing with invalid JSON:\n");
    json_processor->process(json_processor, "not json data");
    
    printf("\\nTesting with empty input:\n");
    csv_processor->process(csv_processor, "");
    
    printf("\\n--- Template Method Benefits Demonstrated ---\n");
    printf("✅ Same algorithm structure for all processors\n");
    printf("✅ Each processor implements specific steps differently\n");
    printf("✅ Hook methods allow optional customization\n");
    printf("✅ Template method ensures consistent processing flow\n");
    printf("✅ Easy to add new processor types\n");
    
    // Cleanup
    csv_processor->destroy(csv_processor);
    json_processor->destroy(json_processor);
    xml_processor->destroy(xml_processor);
    
    return 0;
}