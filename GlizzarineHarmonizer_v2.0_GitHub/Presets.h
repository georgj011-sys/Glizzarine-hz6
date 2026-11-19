#pragma once
// Harmonizer presets. The sound always comes from your voice; a preset sets everything else.
// Values are plain parameter values. Anything not listed goes back to its default,
// except your setup (input, sensitivity, gate, lowest note, tempo, output, solo timing, latency, MIDI out).
//
// v2: ten factory presets, each checked with the qa tool (clean, no repairs, sensible level).
// Your own presets are files in Documents/Glizzarine Presets (*.glzpreset), see PluginProcessor.
#include <vector>
#include <utility>

struct HarmPreset
{
    const char* name;
    std::vector<std::pair<const char*, float>> values;
};

inline const std::vector<HarmPreset>& harmPresets()
{
    static const std::vector<HarmPreset> p = {
        // plain and close: the voices sit right on your singing
        { "Harmonizer Clean", {
            { "harmony", 0.9f }, { "follow", 0.7f }, { "width", 0.6f }, { "consonants", 0.45f },
            { "release", 0.4f }, { "sustain", 2.0f }, { "chorus_mix", 0.0f }, { "reverb", 0.15f }, { "size", 0.5f },
            { "at_shimmer_on", 0 }, { "at_reverb_on", 0 } } },

        // the classic: voices slide into the next chord when you play legato, bass doubled underneath
        { "Collier Choir", {
            { "harmony", 1.2f }, { "leveling", 0.5f }, { "follow", 0.6f }, { "width", 0.85f },
            { "chord_glide", 0.35f }, { "glide_mode", 1 }, { "voicing", 1 }, { "humanize", 0.35f },
            { "chorus_mix", 0.15f }, { "reverb", 0.35f }, { "size", 0.8f }, { "release", 0.8f }, { "sustain", 4.0f } } },

        // big and wide: octave above and below, loose like a real choir
        { "Wide Choir", {
            { "harmony", 0.95f }, { "width", 1.0f }, { "voicing", 3 }, { "humanize", 0.6f },
            { "attack", 40.0f }, { "release", 1.0f }, { "sustain", 4.0f },
            { "chorus_mix", 0.3f }, { "chorus_depth", 0.4f }, { "reverb", 0.4f }, { "size", 0.85f } } },

        // every chord change is a slow slide, open voicing
        { "Glide Choir", {
            { "harmony", 1.0f }, { "width", 0.9f }, { "chord_glide", 0.8f }, { "glide_mode", 0 },
            { "voicing", 4 }, { "humanize", 0.3f }, { "attack", 60.0f }, { "release", 1.2f }, { "sustain", 5.0f },
            { "chorus_mix", 0.15f }, { "reverb", 0.4f }, { "size", 0.85f } } },

        // one voice enters per 1/8, short room so every entry is heard
        { "Pyramid", {
            { "harmony", 1.3f }, { "arp_mode", 1 }, { "arp_order", 0 }, { "arp_rate", 1 }, { "arp_style", 0 }, { "arp_release", 0.6f },
            { "arp_spread", 0.8f }, { "humanize", 0.15f }, { "chorus_mix", 0.0f }, { "reverb", 0.15f }, { "size", 0.5f },
            { "at_shimmer_on", 0 } } },

        // up-down 1/16 pluck, nearly dry
        { "Arp", {
            { "harmony", 1.5f }, { "arp_mode", 2 }, { "arp_order", 2 }, { "arp_rate", 2 }, { "arp_gate", 0.6f },
            { "arp_style", 0 }, { "arp_release", 0.18f }, { "arp_spread", 0.6f },
            { "chorus_mix", 0.0f }, { "reverb", 0.12f }, { "size", 0.45f }, { "at_shimmer_on", 0 }, { "at_reverb_on", 0 } } },

        { "Vocoder Pad", {
            { "blend", 1.0f }, { "voc_type", 0 }, { "voc_resp", 0.35f }, { "attack", 30.0f }, { "release", 0.9f },
            { "chorus_mix", 0.25f }, { "reverb", 0.3f } } },

        { "Vocoder Strings", {
            { "blend", 1.0f }, { "voc_type", 1 }, { "voc_resp", 0.5f }, { "attack", 120.0f }, { "release", 1.5f },
            { "chord_glide", 0.25f }, { "glide_mode", 1 }, { "chorus_mix", 0.15f }, { "reverb", 0.4f }, { "size", 0.9f } } },

        // slow fade-ins, an octave on top, long shimmer trail
        { "Shimmer Cathedral", {
            { "harmony", 1.3f }, { "leveling", 0.6f }, { "width", 1.0f }, { "voicing", 2 }, { "humanize", 0.3f },
            { "attack", 120.0f }, { "release", 2.0f }, { "sustain", 6.0f },
            { "chorus_mix", 0.2f }, { "reverb", 0.5f }, { "size", 0.95f }, { "shimmer", 0.3f }, { "shim_decay", 0.5f } } },

        { "Lo-Fi Bedroom", {
            { "harmony", 1.15f }, { "warmth", 0.3f }, { "tone", 6000.0f }, { "humanize", 0.25f },
            { "lofi_on", 1 }, { "lofi_mix", 0.5f }, { "lofi_amount", 0.4f }, { "lofi_bw", 0.45f },
            { "trem_on", 1 }, { "trem_depth", 0.15f }, { "trem_rate", 1 },
            { "chorus_mix", 0.3f }, { "reverb", 0.25f }, { "size", 0.5f } } },
    };
    return p;
}

// Parameters that belong to your setup, not to the sound: presets never change them
inline bool isSetupParam (const juce::String& id)
{
    return id == "input" || id == "gate" || id == "lowest" || id == "tempo_src"
        || id == "bpm" || id == "output" || id == "dry_timing" || id == "pad_latch" || id == "dry" || id == "sens"
        || id == "latency_mode" || id == "midi_out";
}
