def validate_cmake_options(filepath, allowed_options):
    """
    Reads a file, validates CMake options in each line, and removes invalid lines.

    Args:
        filepath (str): The path to the file containing CMake options.
        allowed_options (list): A list of allowed option prefixes.
    """
    try:
        with open(filepath, 'r') as f:
            lines = f.readlines()

        valid_lines = []
        for line in lines:
            line = line.strip() # Remove whitespace
            if not line: # Check if line is empty
                continue
            is_valid = False
            for option in allowed_options:
                if line.startswith(option):
                    is_valid = True
                    break
            if is_valid:
                valid_lines.append(line + '\n') # Add new line, so it would be saved correctly
            else:
               print(f"Invalid option {line} was deleted.")

        with open(filepath, 'w') as f:
            f.writelines(valid_lines)

        print("File validation finished")
    except FileNotFoundError:
        print(f"Error: File not found: {filepath}")
    except Exception as e:
        print(f"An error occurred: {e}")


if __name__ == "__main__":
    filepath = 'CMakeBuildOptions.txt' # Change to your file
    allowed_options = [
        "CMAKE_BUILD_TYPE=",
        "LIB_BASE_QT_ENABLE_FONTS=",
        "SOME_PATH=",
         "OTHER_OPTION=",
    ]
  #  validate_cmake_options(filepath, allowed_options)