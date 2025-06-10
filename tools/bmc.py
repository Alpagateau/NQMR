import struct
import argparse
import os

DIRECTION_TO_BYTE = {
    "UP": 1,
    "DOWN": 2,
    "LEFT": 0,
    "RIGHT": 3,
}

BYTE_TO_DIRECTION = {v: k for k, v in DIRECTION_TO_BYTE.items()}


def text_to_binary(text_path, binary_path):
    with open(text_path, 'r') as txt_file, open(binary_path, 'wb') as bin_file:
        for line in txt_file:
            line = line.strip()
            if line.startswith("#") or not line:
                continue
            direction, start, duration = line.split()
            direction_byte = DIRECTION_TO_BYTE[direction.upper()]
            bin_file.write(struct.pack('<B I H', direction_byte, int(start), int(duration)))


def binary_to_text(binary_path, text_path):
    with open(binary_path, 'rb') as bin_file, open(text_path, 'w') as txt_file:
        txt_file.write("#direction start(ms) duration(ms)\n")
        while True:
            chunk = bin_file.read(7)
            if len(chunk) < 7:
                break
            direction_byte, start, duration = struct.unpack('<B I H', chunk)
            direction = BYTE_TO_DIRECTION.get(direction_byte, f"UNKNOWN({direction_byte})")
            txt_file.write(f"{direction} {start} {duration}\n")


def main():
    parser = argparse.ArgumentParser(description="Convert beat map between text and binary formats.")
    parser.add_argument("mode", choices=["to-bin", "to-text"], help="Conversion mode.")
    parser.add_argument("input", help="Input file path.")
    parser.add_argument("output", help="Output file path.")

    args = parser.parse_args()

    if not os.path.exists(args.input):
        print(f"Input file '{args.input}' does not exist.")
        return

    if args.mode == "to-bin":
        text_to_binary(args.input, args.output)
        print(f"Converted text to binary: {args.output}")
    elif args.mode == "to-text":
        binary_to_text(args.input, args.output)
        print(f"Converted binary to text: {args.output}")


if __name__ == "__main__":
    main()
