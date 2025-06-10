import argparse
import mido
import heapq

NOTE_TO_DIRECTION = {
    60: "LEFT",   # C5
    61: "UP",     # C#5
    62: "DOWN",   # D5
    63: "RIGHT"   # D#5
}

def ticks_to_ms(ticks, tempo, ticks_per_beat):
    return (ticks * tempo) / (ticks_per_beat * 1000)

def parse_midi(file_path):
    mid = mido.MidiFile(file_path)
    ticks_per_beat = mid.ticks_per_beat

    # Initialize priority queue for all events
    track_states = []
    for i, track in enumerate(mid.tracks):
        if len(track) > 0:
            track_states.append({
                "track": track,
                "index": 0,
                "next_time": track[0].time,
                "elapsed": 0
            })

    tempo = 500000  # Default tempo (microseconds per beat)
    current_time_ticks = 0
    current_time_ms = 0
    note_on_times = {}
    events = []

    # Heap to process events in time order
    heap = []
    for i, state in enumerate(track_states):
        msg = state["track"][0]
        heapq.heappush(heap, (msg.time, i, msg))

    while heap:
        delta_ticks, track_idx, msg = heapq.heappop(heap)

        # Advance time
        current_time_ticks += delta_ticks
        delta_ms = ticks_to_ms(delta_ticks, tempo, ticks_per_beat)
        current_time_ms += delta_ms

        # Update track state
        state = track_states[track_idx]
        state["elapsed"] += delta_ticks
        state["index"] += 1

        # Handle the message
        if msg.type == 'set_tempo':
            tempo = msg.tempo

        elif msg.type == 'note_on' and msg.velocity > 0:
            if msg.note in NOTE_TO_DIRECTION:
                note_on_times[(track_idx, msg.note)] = current_time_ms

        elif msg.type == 'note_off' or (msg.type == 'note_on' and msg.velocity == 0):
            key = (track_idx, msg.note)
            if msg.note in NOTE_TO_DIRECTION and key in note_on_times:
                start = note_on_times.pop(key)
                duration = current_time_ms - start
                direction = NOTE_TO_DIRECTION[msg.note]
                events.append((direction, round(start), round(duration)))

        # Queue up the next message from this track
        if state["index"] < len(state["track"]):
            next_msg = state["track"][state["index"]]
            next_time = next_msg.time
            heapq.heappush(heap, (next_time, track_idx, next_msg))

    return events

def write_tbm(events, output_file):
    with open(output_file, 'w') as f:
        f.write("# Converted from MIDI to TBM format\n")
        for direction, start, duration in events:
            f.write(f"{direction} {start} {duration}\n")

def main():
    parser = argparse.ArgumentParser(description="Convert MIDI to TBM format.")
    parser.add_argument("midi_file", help="Path to input MIDI file")
    parser.add_argument("output_file", help="Path to output .tbm file")
    args = parser.parse_args()

    events = parse_midi(args.midi_file)
    if not events:
        print("No valid notes found (C5, C#5, D5, D#5).")
    else:
        write_tbm(events, args.output_file)
        print(f"Converted {args.midi_file} to {args.output_file} with {len(events)} events.")

if __name__ == "__main__":
    main()
