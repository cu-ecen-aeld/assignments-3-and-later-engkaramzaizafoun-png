#!/bin/bash

# ======================================
# Script: writer.sh
# Accepts:
#   $1 -> full path to the file (writefile)
#   $2 -> text string to write (writestr)
# ======================================

#verify inout data
if [ -z "$1" ] || [ -z "$2" ]; then
    echo "Error: Both file path and text string must be specified."
    echo "Usage: $0 <full_file_path> <text_to_write>"
    exit 1
fi

writefile="$1"
writestr="$2"

# make directory
dirpath=$(dirname "$writefile")  # استخراج مجلد الملف
if [ ! -d "$dirpath" ]; then
    mkdir -p "$dirpath" || { echo "Error: Could not create directory '$dirpath'"; exit 1; }
fi

# write on file
echo "$writestr" > "$writefile" || { echo "Error: Could not write to file '$writefile'"; exit 1; }

# print the result is succeded
echo "File '$writefile' written successfully."

