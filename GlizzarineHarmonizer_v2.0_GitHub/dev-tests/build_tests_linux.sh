#!/usr/bin/env bash
# Builds the offline test tools against JUCE 8.0.4 on Linux (needs: g++, freetype2, fontconfig, Xvfb for screenshots).
# Usage: JUCE=/path/to/JUCE ./build_tests_linux.sh
set -e
J="${JUCE:?set JUCE=/path/to/JUCE}/modules"
SRC="$(cd "$(dirname "$0")/.." && pwd)"
OUT="$(pwd)/build-tests"; mkdir -p "$OUT"; cd "$OUT"
cp "$SRC/dev-tests/cfg.h" .
FL="-std=c++17 -O2 -I$J -I. -I$SRC $(pkg-config --cflags freetype2 fontconfig) -DNDEBUG=1 -DJUCE_DEBUG=0 -w"

# 1) JUCE modules (once)
for m in juce_core juce_events juce_data_structures juce_graphics juce_gui_basics juce_gui_extra juce_audio_basics juce_audio_formats juce_audio_processors juce_dsp; do
  [ -f m_$m.o ] || { printf '#include "cfg.h"\n#include <%s/%s.cpp>\n' $m $m > m_$m.cpp; g++ $FL -c m_$m.cpp -o m_$m.o & }
done
[ -f x_ct.o ] || { printf '#include "cfg.h"\n#include <juce_core/juce_core_CompilationTime.cpp>\n' > x_ct.cpp; g++ $FL -c x_ct.cpp -o x_ct.o & }
[ -f x_hb.o ] || { printf '#include "cfg.h"\n#include <juce_graphics/juce_graphics_Harfbuzz.cpp>\n' > x_hb.cpp; g++ $FL -c x_hb.cpp -o x_hb.o & }
[ -f x_sb.o ] || { printf '#include "cfg.h"\n#include <juce_graphics/juce_graphics_Sheenbidi.c>\n' > x_sb.c; gcc -O1 -I$J -I. -w -c x_sb.c -o x_sb.o & }
wait

LIBS="$(pkg-config --libs freetype2 fontconfig) -ldl -lpthread -lrt"
# 2) plugin code
g++ $FL -include cfg.h -c "$SRC/PluginProcessor.cpp" -o pp.o &
g++ $FL -include cfg.h -c "$SRC/PluginEditor.cpp" -o pe.o &
wait
# 3) tools
for t in qa screenshots; do g++ $FL -include cfg.h -c "$SRC/dev-tests/$t.cpp" -o $t.o; g++ -o $t $t.o pp.o pe.o m_*.o x_*.o $LIBS; done
# fuzz with AddressSanitizer + UBSan (finds real crashes)
ASAN="-fsanitize=address,undefined -g -fno-omit-frame-pointer"
g++ $FL $ASAN -include cfg.h -c "$SRC/PluginProcessor.cpp" -o pp_asan.o
g++ $FL $ASAN -include cfg.h -c "$SRC/dev-tests/fuzz.cpp" -o fuzz.o
g++ $ASAN -o fuzz fuzz.o pp_asan.o pe.o m_*.o x_*.o $LIBS
# engine-only tests (no JUCE needed)
for t in t10 stress pitch bench2; do g++ -O2 -std=c++17 -I"$SRC" "$SRC/dev-tests/$t.cpp" -o $t; done
echo "Run: ./qa   |   ASAN_OPTIONS=detect_leaks=0 ./fuzz 150 1   |   xvfb-run -a ./screenshots   |   ./t10 ./stress ./pitch ./bench2"
