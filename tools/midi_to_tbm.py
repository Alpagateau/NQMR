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

def ticks_to_ms(ticks, tempo, ticks_per_beat):
    return (ticks * tempo) / (ticks_per_beat * 1000)

def ms_to_frames(ms, fps):
    return round(ms * fps / 1000)

def parse_midi(file_path, fps, octave_offset=0):
    mid = mido.MidiFile(file_path)
    ticks_per_beat = mid.ticks_per_beat

    NOTE_TO_DIRECTION = get_note_mapping(octave_offset)

    track_states = []
    for i, track in enumerate(mid.tracks):
        if len(track) > 0:
            track_states.append({
                "track": track,
                "index": 0,
                "elapsed": 0
            })

    tempo = 500000  # Default tempo
    current_time_ticks = 0
    current_time_ms = 0
    note_on_times = {}
    events = []

    heap = []
    for i, state in enumerate(track_states):
        msg = state["track"][0]
        heapq.heappush(heap, (msg.time, i, msg))

    while heap:
        delta_ticks, track_idx, msg = heapq.heappop(heap)

        current_time_ticks += delta_ticks
        delta_ms = ticks_to_ms(delta_ticks, tempo, ticks_per_beat)
        current_time_ms += delta_ms

        state = track_states[track_idx]
        state["elapsed"] += delta_ticks
        state["index"] += 1

        if msg.type == 'set_tempo':
            tempo = msg.tempo

        elif msg.type == 'note_on' and msg.velocity > 0:
            if msg.note in NOTE_TO_DIRECTION:
                note_on_times[(track_idx, msg.note)] = current_time_ms

        elif msg.type == 'note_off' or (msg.type == 'note_on' and msg.velocity == 0):
            key = (track_idx, msg.note)
            if msg.note in NOTE_TO_DIRECTION and key in note_on_times:
                start_ms = note_on_times.pop(key)
                duration_ms = current_time_ms - start_ms
                start_frame = ms_to_frames(start_ms, fps)
                duration_frames = ms_to_frames(duration_ms, fps)
                direction = NOTE_TO_DIRECTION[msg.note]
                events.append((direction, start_frame, duration_frames))

        if state["index"] < len(state["track"]):
            next_msg = state["track"][state["index"]]
            heapq.heappush(heap, (next_msg.time, track_idx, next_msg))

    return events

def write_tbm(events, output_file):
    with open(output_file, 'w') as f:
        f.write("# Converted from MIDI to TBM format with frame timestamps\n")
        for direction, start, duration in events:
            f.write(f"{direction} {start} {duration}\n")

def main():
    parser = argparse.ArgumentParser(description="Convert MIDI to TBM format with frame timing.")
    parser.add_argument("midi_file", help="Path to input MIDI file")
    parser.add_argument("output_file", help="Path to output .tbm file")
    parser.add_argument("--fps", type=int, default=60, help="Frame rate (frames per second)")
    parser.add_argument("--fl", action="store_true", help="Use FL Studio note convention (C5 = MIDI 60)")

    args = parser.parse_args()

    octave_offset = 0 if args.fl else 1  # 0 for FL Studio, 1 for General MIDI

    events = parse_midi(args.midi_file, args.fps, octave_offset)
    if not events:
        print("No valid notes (C, C#, D, D#) found in expected octave.")
    else:
        write_tbm(events, args.output_file)
        print(f"Converted {args.midi_file} to {args.output_file} with {len(events)} events at {args.fps} FPS.")

if __name__ == "__main__":
    main()
