import argparse
import mido
import heapq

def get_note_mapping(octave_offset=0):
    base = 60 + (octave_offset * 12)
    return {
        base + 0: "LEFT",   # C
        base + 1: "UP",     # C#
        base + 2: "DOWN",   # D
        base + 3: "RIGHT"   # D#
    }

def parse_midi(file_path, sample_rate=32768, octave_offset=0):
    """
    Parse MIDI file for rhythm game using sample-based timing for MaxMod
    
    Args:
        file_path: Path to MIDI file
        sample_rate: Audio sample rate (32768 Hz typical for DS MaxMod)
        octave_offset: Octave adjustment for note mapping
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

    def ms_to_samples(ms, sample_rate):
        """Convert milliseconds to audio samples"""
        return int(ms * sample_rate / 1000)

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
                
                # Convert to samples
                start_samples = ms_to_samples(start_ms, sample_rate)
                duration_samples = ms_to_samples(duration_ms, sample_rate)
                
                # Only add events with positive duration
                if duration_samples > 0:
                    direction = NOTE_TO_DIRECTION[msg.note]
                    events.append((direction, start_samples, duration_samples))
    
    # Sort events by start time
    events.sort(key=lambda x: x[1])
    
    return events

def write_tbm(events, output_file, sample_rate):
    """
    Write TBM file with sample-based timing for MaxMod
    
    Format: DIRECTION START_SAMPLES DURATION_SAMPLES
    """
    with open(output_file, 'w') as f:
        f.write(f"# MaxMod sample-based timing (sample rate: {sample_rate} Hz)\n")
        f.write(f"# DIRECTION START_SAMPLES DURATION_SAMPLES\n")
        for direction, start_samples, duration_samples in events:
            f.write(f"{direction} {start_samples} {duration_samples}\n")

def main():
    parser = argparse.ArgumentParser(description="Convert MIDI to TBM format with MaxMod sample timing.")
    parser.add_argument("midi_file", help="Path to input MIDI file")
    parser.add_argument("output_file", help="Path to output .tbm file")
    parser.add_argument("--sample-rate", type=int, default=32768, 
                       help="Audio sample rate (32768 Hz typical for DS MaxMod)")
    parser.add_argument("--fl", action="store_true", 
                       help="Use FL Studio note convention (C5 = MIDI 60)")

    args = parser.parse_args()

    octave_offset = 0 if args.fl else 1

    events = parse_midi(args.midi_file, args.sample_rate, octave_offset)
    if not events:
        print("No valid notes (C, C#, D, D#) found in expected octave.")
    else:
        write_tbm(events, args.output_file, args.sample_rate)
        print(f"Converted {args.midi_file} to {args.output_file}")
        print(f"Generated {len(events)} events at {args.sample_rate} Hz sample rate")

if __name__ == "__main__":
    main()
