#!/bin/bash

# Clear Ip Table First
sudo iptables -Z

# Remove any existing rule on udp packets on port 4242
sudo iptables -D INPUT  -p udp --dport 4242
sudo iptables -D OUTPUT -p udp --sport 4242

# Add a rule on udp packets on port 4242
sudo iptables -I INPUT  -p udp --dport 4242
sudo iptables -I OUTPUT -p udp --sport 4242

# Start the server with time, capture time output
{ time ./r-type_server -p 4242 -i 127.0.0.1 -tps 20; } 2> time_output.txt

# Get iptables output
iptables_output=$(sudo iptables -L -v -n)

# Parse time
real_time=$(grep "real" time_output.txt | awk '{print $2}')
minutes=$(echo $real_time | cut -dm -f1)
seconds_str=$(echo $real_time | cut -ds -f1 | cut -dm -f2 | tr ',' '.')
total_seconds=$(echo "$minutes * 60 + $seconds_str" | bc -l)

# Parse bytes from rules
input_line=$(echo "$iptables_output" | grep "udp dpt:4242")
input_bytes_str=$(echo "$input_line" | awk '{print $2}')
if [[ $input_bytes_str == *K ]]; then
    input_kb=$(echo "$input_bytes_str" | sed 's/K//' | awk '{print $1}')
else
    input_kb=$(echo "scale=2; $input_bytes_str / 1024" | bc)
fi

output_line=$(echo "$iptables_output" | grep "udp spt:4242")
output_bytes_str=$(echo "$output_line" | awk '{print $2}')
if [[ $output_bytes_str == *K ]]; then
    output_kb=$(echo "$output_bytes_str" | sed 's/K//' | awk '{print $1}')
else
    output_kb=$(echo "scale=2; $output_bytes_str / 1024" | bc)
fi

# Calculate total KB
total_kb=$(echo "scale=2; $input_kb + $output_kb" | bc)

# Calculate KB/sec
kb_per_sec=$(echo "scale=2; $total_kb / $total_seconds" | bc)

# Output
echo "$iptables_output"
echo ""
echo "Execution time:"
cat time_output.txt
echo ""
echo "Calculations:"
echo "INPUT: $input_bytes_str Bytes / 1024 = $input_kb KB"
echo "OUTPUT: $output_bytes_str Bytes / 1024 = $output_kb KB"
echo "TIME: $real_time ($total_seconds seconds)"
echo "Total KB: $input_kb + $output_kb = $total_kb KB"
echo "KB/s: $total_kb / $total_seconds = $kb_per_sec"

# Clean up
rm time_output.txt

