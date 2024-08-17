#!/bin/bash

# Set the path to the Windows Downloads folder
# Replace 'YourUsername' with your actual Windows username
downloads_folder="/mnt/c/Users/samue/Downloads"

# Set the name of the output zip file
output_zip="$downloads_folder/cpp_files.zip"

# Check if the Downloads folder exists and is accessible
if [ ! -d "$downloads_folder" ]; then
    echo "Error: Cannot access Windows Downloads folder at $downloads_folder"
    echo "Please make sure the path is correct and the folder is accessible from WSL."
    exit 1
fi

# Find all .h and .cpp files in the current directory and subdirectories
# and add them to the zip file
find . -type f \( -name "*.h" -o -name "*.cpp" \) -not -path "*/tests/*" -print0 | xargs -0 zip -j "$output_zip"

echo "All .h and .cpp files have been collected into $output_zip"