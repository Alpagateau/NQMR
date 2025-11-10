import os
import sys
import ast

def main():
    if len(sys.argv) < 2:
        print("Usage: python script.py <path_to_obj_file>")
        sys.exit(1)
    
    obj_path = sys.argv[1]

    if not obj_path.endswith(".obj"):
        print("Error: The input file must have a .obj extension")
        sys.exit(1)
    
    if not os.path.isfile(obj_path):
        print(f"Error: File not found: {obj_path}")
        sys.exit(1)
    
    base_name = os.path.splitext(obj_path)[0]
    txt_path = base_name + ".txt"

    if not os.path.isfile(txt_path):
        print(f"Error: Matching .txt file not found: {txt_path}")
        sys.exit(1)

    try:
        with open(txt_path, "r", encoding="utf-8") as f:
            content = f.read().strip()
        
        # Parse the content as a dictionary safely
        parsed_dict = ast.literal_eval(content)
        
        if not isinstance(parsed_dict, dict):
            print("Error: The content of the .txt file is not a dictionary")
            sys.exit(1)

        command = f"python /opt/wonderful/thirdparty/blocksds/external/nitro-engine/full_repo/nitro-engine/tools/img2ds/img2ds.py --input {sys.argv[1]} --output $(NITROFSDIR)/models/ --name {base_name} --format PAL256"
    
    except Exception as e:
        print(f"Error reading or parsing {txt_path}: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()
