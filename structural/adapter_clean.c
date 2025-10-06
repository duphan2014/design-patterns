/*
 * CLEANER ADAPTER PATTERN EXAMPLE
 * 
 * Problem: You have two different audio libraries with incompatible interfaces
 * Solution: Create adapters so both can be used through the same interface
 * Key Takeaways from the Adapter Pattern:
 * The Core Idea:
 * Problem: You have incompatible interfaces that do similar things
 * Solution: Create a "translator" (adapter) that makes them work through a common interface
 * Why It's Useful:
 * Legacy Integration: Use old libraries with new code
 * Third-party APIs: Standardize different vendor APIs
 * Flexibility: Swap implementations without changing main code
 * Clean Architecture: Keep your business logic separate from library specifics
 * The Pattern Structure:
 * Instead of:

 * Real-World Applications:
 * Database Drivers: Same SQL interface, different databases (MySQL, PostgreSQL, SQLite)
 * Payment Systems: Same payment interface, different providers (Stripe, PayPal, Square)
 * Graphics APIs: Same drawing interface, different backends (OpenGL, DirectX, Vulkan)
 * File Systems: Same file operations, different storage (local, cloud, network)
 * The adapter pattern is one of the most practical patterns - you'll probably use it more than some of the fancier ones!
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// =============================================================================
// COMMON INTERFACE (what your main code wants to use)
// =============================================================================

typedef struct AudioPlayer AudioPlayer;
struct AudioPlayer {
    void (*play)(AudioPlayer* self, const char* filename);
    void (*stop)(AudioPlayer* self);
    void (*destroy)(AudioPlayer* self);
};

// =============================================================================
// LEGACY LIBRARY 1: Old MP3 Library (you can't change this)
// =============================================================================

typedef struct {
    char current_song[256];
    int is_playing;
} OldMP3Library;

void old_mp3_start_song(OldMP3Library* lib, const char* song) {
    strcpy(lib->current_song, song);
    lib->is_playing = 1;
    printf("🎵 [OLD MP3 LIB] Started: %s\n", song);
}

void old_mp3_halt(OldMP3Library* lib) {
    printf("⏹️ [OLD MP3 LIB] Stopped: %s\n", lib->current_song);
    lib->is_playing = 0;
    strcpy(lib->current_song, "");
}

// =============================================================================
// NEW LIBRARY 2: Modern Audio Library (you can't change this either)
// =============================================================================

typedef struct {
    char loaded_file[256];
    char format[10];
} ModernAudioEngine;

void modern_load_and_play(ModernAudioEngine* engine, const char* file, const char* format) {
    strcpy(engine->loaded_file, file);
    strcpy(engine->format, format);
    printf("🎧 [MODERN ENGINE] Playing %s as %s\n", file, format);
}

void modern_stop_playback(ModernAudioEngine* engine) {
    printf("⏸️ [MODERN ENGINE] Stopped %s (%s)\n", engine->loaded_file, engine->format);
    strcpy(engine->loaded_file, "");
}

// =============================================================================
// ADAPTER 1: Makes Old MP3 Library work with our common interface
// =============================================================================

typedef struct {
    AudioPlayer base;           // Common interface
    OldMP3Library* old_lib;     // The actual old library
} MP3Adapter;

void mp3_adapter_play(AudioPlayer* self, const char* filename) {
    MP3Adapter* adapter = (MP3Adapter*)self; // cast AudioPlayer to MP3Adapter, because they all pointing to the same memory address because AudioPlayer base is the first field in the struct mp3Adapter
    old_mp3_start_song(adapter->old_lib, filename);  // Translate the call
}

void mp3_adapter_stop(AudioPlayer* self) {
    MP3Adapter* adapter = (MP3Adapter*)self;
    old_mp3_halt(adapter->old_lib);  // Translate the call
}

void mp3_adapter_destroy(AudioPlayer* self) {
    MP3Adapter* adapter = (MP3Adapter*)self;
    free(adapter->old_lib);
    free(adapter);
}

AudioPlayer* create_mp3_adapter() {
    MP3Adapter* adapter = malloc(sizeof(MP3Adapter));
    adapter->old_lib = malloc(sizeof(OldMP3Library));
    
    // Initialize the old library
    strcpy(adapter->old_lib->current_song, "");
    adapter->old_lib->is_playing = 0;
    
    // Set up the common interface
    adapter->base.play = mp3_adapter_play;
    adapter->base.stop = mp3_adapter_stop;
    adapter->base.destroy = mp3_adapter_destroy;
    
    return (AudioPlayer*)adapter; // cast MP3Adapter to AudioPlayer, because they all pointing to the same memory address because AudioPlayer base is the first field in the struct mp3Adapter
}

// =============================================================================
// ADAPTER 2: Makes Modern Audio Engine work with our common interface
// =============================================================================

typedef struct {
    AudioPlayer base;               // Common interface
    ModernAudioEngine* engine;      // The actual modern engine
} ModernAdapter;

void modern_adapter_play(AudioPlayer* self, const char* filename) {
    ModernAdapter* adapter = (ModernAdapter*)self;
    
    // Figure out format from filename
    const char* format;
    if (strstr(filename, ".mp3")) format = "MP3";
    else if (strstr(filename, ".wav")) format = "WAV";
    else if (strstr(filename, ".flac")) format = "FLAC";
    else format = "UNKNOWN";
    
    modern_load_and_play(adapter->engine, filename, format);  // Translate the call
}

void modern_adapter_stop(AudioPlayer* self) {
    ModernAdapter* adapter = (ModernAdapter*)self;
    modern_stop_playback(adapter->engine);  // Translate the call
}

void modern_adapter_destroy(AudioPlayer* self) {
    ModernAdapter* adapter = (ModernAdapter*)self;
    free(adapter->engine);
    free(adapter);
}

AudioPlayer* create_modern_adapter() {
    ModernAdapter* adapter = malloc(sizeof(ModernAdapter));
    adapter->engine = malloc(sizeof(ModernAudioEngine));
    
    // Initialize the modern engine
    strcpy(adapter->engine->loaded_file, "");
    strcpy(adapter->engine->format, "");
    
    // Set up the common interface
    adapter->base.play = modern_adapter_play;
    adapter->base.stop = modern_adapter_stop;
    adapter->base.destroy = modern_adapter_destroy;
    
    return (AudioPlayer*)adapter;
}

// =============================================================================
// YOUR MAIN CODE - Works with any audio player!
// =============================================================================

void test_audio_player(AudioPlayer* player, const char* name) {
    printf("\n--- Testing %s ---\n", name);
    player->play(player, "song.mp3");
    player->play(player, "music.wav");
    player->stop(player);
}

int main() {
    printf("=== CLEAN ADAPTER PATTERN EXAMPLE ===\n");
    
    // Create different players using adapters
    AudioPlayer* mp3_player = create_mp3_adapter();
    AudioPlayer* modern_player = create_modern_adapter();
    
    // Same code works with both! (That's the point of adapters)
    test_audio_player(mp3_player, "MP3 Adapter");
    test_audio_player(modern_player, "Modern Adapter");
    
    // Create array of different players - polymorphism in action!
    AudioPlayer* players[] = {mp3_player, modern_player};
    
    printf("\n--- Playing same song on all players ---\n");
    for (int i = 0; i < 2; i++) {
        players[i]->play(players[i], "favorite_song.flac");
    }
    
    printf("\n--- Stopping all players ---\n");
    for (int i = 0; i < 2; i++) {
        players[i]->stop(players[i]);
    }
    
    // Cleanup
    mp3_player->destroy(mp3_player);
    modern_player->destroy(modern_player);
    
    printf("\n✅ The beauty of Adapter Pattern:\n");
    printf("   - Your main code doesn't know about specific libraries\n");
    printf("   - You can swap audio engines without changing main code\n");
    printf("   - Legacy and modern libraries work through same interface\n");
    
    return 0;
}