#!/usr/bin/env python3
import os
import subprocess
import sys

def main():
    if len(sys.argv) != 3:
        print("Usage: python convert_image.py <image_file> <output_dir>")
        sys.exit(1)

    image_file = sys.argv[1]
    output_dir = sys.argv[2]

    if not os.path.isfile(image_file):
        print(f"Error: {image_file} does not exist.")
        sys.exit(1)

    if not os.path.isdir(output_dir):
        print(f"Error: {output_dir} is not a directory.")
        sys.exit(1)

    base_name = os.path.splitext(os.path.basename(image_file))[0]
    txt_file = os.path.join(os.path.dirname(image_file), base_name + ".txt")

    if not os.path.isfile(txt_file):
        print(f"Warning: {txt_file} not found. Using default options.")
        extra_args = ""
    else:
        with open(txt_file, "r", encoding="utf-8") as f:
            extra_args = f.read().strip()

    # Ensure img2ds is available in PATH
    img2ds_cmd = [
        "python", 
        "/opt/wonderful/thirdparty/blocksds/external/nitro-engine/tools/img2ds/img2ds.py", 
        "--input", image_file, 
        "--output", output_dir, 
        "--name", base_name, "--f"]
    if extra_args:
        img2ds_cmd += extra_args.split()

    print(f"Running command: {' '.join(img2ds_cmd)}")

    try:
        subprocess.run(img2ds_cmd, check=True)
        print("Conversion successful.")
    except subprocess.CalledProcessError as e:
        print(f"Error during conversion: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()

