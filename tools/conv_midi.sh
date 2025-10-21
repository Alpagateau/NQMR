#! /bin/sh

#!/bin/bash

# Directory to search — default to current directory if not given
DIR="${1:-.}"

# Step 1: Find all MIDI files
find "$DIR" -type f -name "*.mid" | while read -r midfile; do
    # Derive .tbm filename
    tbmfile="${midfile%.mid}.tbm"

    echo "Processing $midfile → $tbmfile"
    # Step 2: Run comm1 on each MIDI file
    python3 ./tools/midi_to_tbm.py "$midfile" "$tbmfile" --sample-rate 11025
done

# Step 3: Find all TBM files and run comm2 on each
find "$DIR" -type f -name "*.tbm" | while read -r tbmfile; do
    # Derive .bbm filename
    bbmfile="${tbmfile%.tbm}.bbm"

    echo "Processing $tbmfile → $bbmfile"
    python3 ./tools/bmc.py  "$tbmfile" "$bbmfile"
    cp "$bbmfile" nitrofiles/bms/
done

