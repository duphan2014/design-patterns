/*
 * ADAPTER PATTERN
 * 
 * Problem: Allow incompatible interfaces to work together
 * 
 * Use Cases:
 * - Integrating third-party libraries
 * - Legacy code integration
 * - Converting data formats
 * - Hardware abstraction layers
 * 
 * Pros:
 * - Allows incompatible interfaces to work together
 * - Reuses existing code
 * - Separates interface conversion from business logic
 * 
 * Cons:
 * - Increases code complexity
 * - Additional layer of abstraction
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Target interface (what client expects)
typedef struct {
    void (*play_audio)(const char* filename);
    void (*stop_audio)(void);
} MediaPlayer;

// Adaptee 1: MP3 Player (already compatible)
typedef struct {
    char current_file[256];
} MP3Player;

void mp3_play(MP3Player* player, const char* filename) {
    strcpy(player->current_file, filename);
    printf("Playing MP3 file: %s\n", filename);
}

void mp3_stop(MP3Player* player) {
    printf("Stopped MP3: %s\n", player->current_file);
    strcpy(player->current_file, "");
}

// Adaptee 2: Advanced Audio Player (incompatible interface)
typedef struct {
    char loaded_track[256];
} AdvancedAudioPlayer;

void advanced_play_wav(AdvancedAudioPlayer* player, const char* filename) {
    strcpy(player->loaded_track, filename);
    printf("Playing WAV file with advanced codec: %s\n", filename);
}

void advanced_play_flac(AdvancedAudioPlayer* player, const char* filename) {
    strcpy(player->loaded_track, filename);
    printf("Playing FLAC file with lossless compression: %s\n", filename);
}

void advanced_halt(AdvancedAudioPlayer* player) {
    printf("Advanced player halted: %s\n", player->loaded_track);
    strcpy(player->loaded_track, "");
}

// Adapter for Advanced Audio Player
typedef struct {
    MediaPlayer interface;
    AdvancedAudioPlayer* advanced_player;
} AudioAdapter;

void adapter_play_audio(const char* filename) {
    // This is a simplified approach - in real implementation,
    // you'd need to access the adapter instance
    static AdvancedAudioPlayer advanced;
    
    // Determine file type and call appropriate method
    if (strstr(filename, ".wav")) {
        advanced_play_wav(&advanced, filename);
    } else if (strstr(filename, ".flac")) {
        advanced_play_flac(&advanced, filename);
    } else {
        printf("Unsupported format: %s\n", filename);
    }
}

void adapter_stop_audio(void) {
    static AdvancedAudioPlayer advanced;
    advanced_halt(&advanced);
}

AudioAdapter* create_audio_adapter() {
    AudioAdapter* adapter = (AudioAdapter*)malloc(sizeof(AudioAdapter));
    adapter->advanced_player = (AdvancedAudioPlayer*)malloc(sizeof(AdvancedAudioPlayer));
    strcpy(adapter->advanced_player->loaded_track, "");
    
    adapter->interface.play_audio = adapter_play_audio;
    adapter->interface.stop_audio = adapter_stop_audio;
    
    return adapter;
}

// Universal Media Player (uses adapters)
typedef struct {
    MediaPlayer* mp3_player;
    MediaPlayer* adapter;
    char current_format[10];
} UniversalMediaPlayer;

void universal_play_audio(UniversalMediaPlayer* player, const char* filename) {
    strcpy(player->current_format, "unknown");
    
    if (strstr(filename, ".mp3")) {
        strcpy(player->current_format, "mp3");
        printf("Using MP3 player directly:\n");
        player->mp3_player->play_audio(filename);
    } 
    else if (strstr(filename, ".wav") || strstr(filename, ".flac")) {
        if (strstr(filename, ".wav")) strcpy(player->current_format, "wav");
        if (strstr(filename, ".flac")) strcpy(player->current_format, "flac");
        printf("Using adapter for advanced formats:\n");
        player->adapter->play_audio(filename);
    }
    else {
        printf("Unsupported audio format: %s\n", filename);
    }
}

void universal_stop_audio(UniversalMediaPlayer* player) {
    if (strcmp(player->current_format, "mp3") == 0) {
        player->mp3_player->stop_audio();
    } else {
        player->adapter->stop_audio();
    }
}

// MP3 Player wrapper to match MediaPlayer interface
void mp3_wrapper_play(const char* filename) {
    static MP3Player mp3;
    mp3_play(&mp3, filename);
}

void mp3_wrapper_stop(void) {
    static MP3Player mp3;
    mp3_stop(&mp3);
}

MediaPlayer* create_mp3_player_interface() {
    MediaPlayer* interface = (MediaPlayer*)malloc(sizeof(MediaPlayer));
    interface->play_audio = mp3_wrapper_play;
    interface->stop_audio = mp3_wrapper_stop;
    return interface;
}

UniversalMediaPlayer* create_universal_player() {
    UniversalMediaPlayer* player = (UniversalMediaPlayer*)malloc(sizeof(UniversalMediaPlayer));
    
    player->mp3_player = create_mp3_player_interface();
    
    AudioAdapter* adapter = create_audio_adapter();
    player->adapter = &(adapter->interface);
    
    strcpy(player->current_format, "none");
    
    return player;
}

void destroy_universal_player(UniversalMediaPlayer* player) {
    if (player) {
        if (player->mp3_player) free(player->mp3_player);
        // Note: adapter cleanup would need more sophisticated memory management
        free(player);
    }
}

// Example usage
int main() {
    printf("=== ADAPTER PATTERN EXAMPLE ===\n\n");
    
    UniversalMediaPlayer* player = create_universal_player();
    
    printf("--- Playing different audio formats ---\n");
    
    // Play MP3 (direct compatibility)
    universal_play_audio(player, "song1.mp3");
    universal_stop_audio(player);
    
    printf("\n");
    
    // Play WAV (through adapter)
    universal_play_audio(player, "song2.wav");
    universal_stop_audio(player);
    
    printf("\n");
    
    // Play FLAC (through adapter)
    universal_play_audio(player, "song3.flac");
    universal_stop_audio(player);
    
    printf("\n");
    
    // Try unsupported format
    universal_play_audio(player, "video.mp4");
    
    destroy_universal_player(player);
    
    return 0;
}