#!/bin/bash

# Clear Ip Table First
sudo iptables -Z

# Remove any existing rule on udp packets on port 4242
sudo iptables -D INPUT  -p udp --dport 4242
sudo iptables -D OUTPUT -p udp --sport 4242

# Add a rule on udp packets on port 4242
sudo iptables -I INPUT  -p udp --dport 4242
sudo iptables -I OUTPUT -p udp --sport 4242


# Start the server with time
time ./r-type_server -p 4242 -i 127.0.0.1 -tps 20

sudo iptables -L -v -n

echo "To calculate kb usage :"
echo "( input + output ) / Time = KB/sec"
