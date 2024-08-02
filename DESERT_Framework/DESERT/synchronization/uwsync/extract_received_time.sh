#!/bin/bash

# Define input and output file paths
input_file="output1.txt"
output_file="received_times.txt"

# Run the command and redirect output and error to the input file
ns test_uwsync.tcl > "$input_file" 2>&1

# Check if the input file exists
if [[ ! -f "$input_file" ]]; then
    echo "Input file $input_file does not exist."
    exit 1
fi

# Extract lines containing 'Received Time            :', 'Generated clock skew (alpha):', and 'clock offset (beta):'
# and save to the output file with prefixes

# Received Time, skipping lines where all received times are 0
grep -E "Received Time\s+:" "$input_file" | awk '!/Received Time\s+:\s+0 0 0 0/' | sed 's/^/Received Time: /' > "$output_file"

# Generated clock skew (alpha)
grep -E "Generated clock skew \(alpha\):" "$input_file" | sed 's/^/Generated clock skew (alpha): /' >> "$output_file"

echo "Lines related to 'Received Time            :', 'Generated clock skew (alpha):', and 'clock offset (beta):' have been saved to $output_file."

