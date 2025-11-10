import struct
import argparse

# Mapping of directions to channel numbers
direction_map = {
    "LEFT": 1,
    "UP": 2,
    "DOWN": 3,
    "RIGHT": 4
}

def convert_text_to_binary(input_file, output_file):
    with open(input_file, "r") as fin, open(output_file, "wb") as fout:
        for line_number, line in enumerate(fin, 1):
            line = line.strip()
            if not line:
                continue  # Skip empty lines

            parts = line.split()
            if len(parts) != 3:
                print(f"Skipping malformed line {line_number}: {line}")
                continue

            direction, start_str, duration_str = parts
            if direction not in direction_map:
                print(f"Unknown direction on line {line_number}: {direction}")
                continue

            try:
                channel = direction_map[direction]
                start = int(start_str)
                duration = int(duration_str)

                # < = little endian, B = uint8, I = uint32, H = uint16
                packed_data = struct.pack('<BIH', channel, start, duration)
                fout.write(packed_data)
            except ValueError as e:
                print(f"Error parsing line {line_number}: {line} -- {e}")

def main():
    parser = argparse.ArgumentParser(description="Convert a text file to a binary format.")
    parser.add_argument("input", help="Path to the input text file")
    parser.add_argument("output", help="Path to the output binary file")
    args = parser.parse_args()

    convert_text_to_binary(args.input, args.output)
    print(f"Conversion complete. Binary saved to: {args.output}")

if __name__ == "__main__":
    main()
