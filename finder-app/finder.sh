#!/bin/bash

# ======================================
# Script: finder.sh
# Accepts:
#   $1 -> directory path (filesdir)
#   $2 -> search string (searchstr)
# ======================================

# verification data 
if [ -z "$1" ] || [ -z "$2" ]; then
    echo "Error: Both directory path and search string must be specified."
    echo "Usage: $0 <directory> <search_string>"
    exit 1
fi

filesdir="$1"
searchstr="$2"

# verify filedir is a folder
if [ ! -d "$filesdir" ]; then
    echo "Error: '$filesdir' is not a directory."
    exit 1
fi

# get number of files in the folder
num_files=$(find "$filesdir" -type f | wc -l)

#get the number of files with the string searchstr
num_matches=$(grep -r -F -- "$searchstr" "$filesdir" | wc -l)

# the result output
echo "The number of files are $num_files and the number of matching lines are $num_matches"

