#!/bin/bsh

source_folder="."
destination_folder="./adf/"
file_to_exclude="vfio"

for file in "$source_folder"/*; do
	if [ "$file" != "$source_folder/$file_to_exclude" ]; then
		mv "$file" "$destination_folder"
	fi
done


