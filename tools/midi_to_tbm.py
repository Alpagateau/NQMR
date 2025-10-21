import argparse
import mido

def get_note_mapping(octave_offset=0):
    base = 60 + (octave_offset * 12)
    return {
        base + 0: "LEFT",   # C
        base + 1: "UP",     # C#
        base + 2: "DOWN",   # D
        base + 3: "RIGHT"   # D#
    }

def parse_midi(file_path, octave_offset=0):
    """
    Parse MIDI file and return arrow events with millisecond timing
    """
    mid = mido.MidiFile(file_path)
    ticks_per_beat = mid.ticks_per_beat

    NOTE_TO_DIRECTION = get_note_mapping(octave_offset)

    # Build tempo map from all tracks
    def build_tempo_map(mid):
        tempo_map = []  # (tick_position, tempo)
        current_tempo = 500000  # Default tempo (120 BPM)
        
        # Process all tracks to find all tempo changes
        all_tempo_events = []
        for track in mid.tracks:
            absolute_tick = 0
            for msg in track:
                absolute_tick += msg.time
                if msg.type == 'set_tempo':
                    all_tempo_events.append((absolute_tick, msg.tempo))
        
        # Sort by tick and ensure we have tempo at tick 0
        all_tempo_events.sort(key=lambda x: x[0])
        
        if not all_tempo_events or all_tempo_events[0][0] != 0:
            tempo_map = [(0, current_tempo)]
        else:
            tempo_map = []
        
        # Add all tempo events, removing duplicates at same tick
        last_tick = -1
        for tick, tempo in all_tempo_events:
            if tick != last_tick:
                tempo_map.append((tick, tempo))
                last_tick = tick
        
        return tempo_map

    def ticks_to_ms_with_tempo_map(ticks, tempo_map, ticks_per_beat):
        """Convert absolute ticks to milliseconds using tempo map"""
        if ticks == 0:
            return 0.0
        
        # Find the appropriate tempo segment
        current_tick = 0
        total_ms = 0.0
        
        for i in range(len(tempo_map) - 1):
            segment_start_tick, tempo = tempo_map[i]
            segment_end_tick = tempo_map[i + 1][0]
            
            if ticks <= segment_end_tick:
                # Target is within this segment
                segment_ticks = ticks - current_tick
                segment_ms = (segment_ticks * tempo) / (ticks_per_beat * 1000)
                return total_ms + segment_ms
            else:
                # Process entire segment
                segment_ticks = segment_end_tick - current_tick
                segment_ms = (segment_ticks * tempo) / (ticks_per_beat * 1000)
                total_ms += segment_ms
                current_tick = segment_end_tick
        
        # Process remaining ticks with last tempo
        if current_tick < ticks:
            last_tempo = tempo_map[-1][1]
            segment_ticks = ticks - current_tick
            segment_ms = (segment_ticks * last_tempo) / (ticks_per_beat * 1000)
            total_ms += segment_ms
        
        return total_ms

    # Build the tempo map
    tempo_map = build_tempo_map(mid)
    
    # Collect all messages with absolute timing
    all_messages = []
    for track_idx, track in enumerate(mid.tracks):
        absolute_ticks = 0
        for msg in track:
            absolute_ticks += msg.time
            all_messages.append((absolute_ticks, track_idx, msg))
    
    # Sort all messages by absolute tick time
    all_messages.sort(key=lambda x: x[0])
    
    # Process messages in chronological order
    events = []
    note_on_times = {}  # (track_idx, note, channel) -> (start_ms, start_ticks)
    
    for absolute_ticks, track_idx, msg in all_messages:
        current_ms = ticks_to_ms_with_tempo_map(absolute_ticks, tempo_map, ticks_per_beat)
        
        if msg.type == 'note_on' and msg.velocity > 0:
            if msg.note in NOTE_TO_DIRECTION:
                note_on_times[(track_idx, msg.note, msg.channel)] = (current_ms, absolute_ticks)
        
        elif msg.type == 'note_off' or (msg.type == 'note_on' and msg.velocity == 0):
            key = (track_idx, msg.note, msg.channel)
            if msg.note in NOTE_TO_DIRECTION and key in note_on_times:
                start_ms, start_ticks = note_on_times.pop(key)
                duration_ms = current_ms - start_ms
                
                # Only add events with positive duration
                if duration_ms > 0:
                    direction = NOTE_TO_DIRECTION[msg.note]
                    events.append((direction, start_ms, duration_ms))
    
    # Sort events by start time
    events.sort(key=lambda x: x[1])
    
    return events

def write_tbm(events, output_file):
    """
    Write TBM file with millisecond timing
    
    Format: DIRECTION START_MS DURATION_MS
    """
    with open(output_file, 'w') as f:
        f.write("# Millisecond timing format\n")
        f.write("# DIRECTION START_MS DURATION_MS\n")
        for direction, start_ms, duration_ms in events:
            f.write(f"{direction} {int(start_ms)} {int(duration_ms)}\n")

def main():
    parser = argparse.ArgumentParser(description="Convert MIDI to TBM format with millisecond timing.")
    parser.add_argument("midi_file", help="Path to input MIDI file")
    parser.add_argument("output_file", help="Path to output .tbm file")
    parser.add_argument("--fl", action="store_true", 
                       help="Use FL Studio note convention (C5 = MIDI 60)")

    args = parser.parse_args()

    octave_offset = 0 if args.fl else 1

    events = parse_midi(args.midi_file, octave_offset)
    if not events:
        print("No valid notes (C, C#, D, D#) found in expected octave.")
    else:
        write_tbm(events, args.output_file)
        print(f"Converted {args.midi_file} to {args.output_file}")
        print(f"Generated {len(events)} events with millisecond timing")
        print(f"First few events:")
        for i, (direction, start_ms, duration_ms) in enumerate(events[:5]):
            print(f"  {direction}: starts at {start_ms:.1f}ms, lasts {duration_ms:.1f}ms")

if __name__ == "__main__":
    main()
