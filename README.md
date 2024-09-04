# ft_ping

## Description

`ft_ping` is a custom implementation of the `ping` network utility, written in C. It sends ICMP ECHO_REQUEST packets to a target machine (IPv4 address or domain name) to check its connectivity. The tool uses raw sockets to send and receive ICMP packets and calculates the round-trip time (RTT) for each packet. This project is designed to help you understand the ICMP and IP protocols, as well as the basics of socket programming in C.

## Features

- Sends ICMP ECHO_REQUEST packets to a specified target.
- Calculates and displays the round-trip time (RTT) for each packet.
- Handles timeouts and reception errors.
- Displays detailed ping statistics, including packet loss and RTT statistics.
- Supports IPv4 addresses and Fully Qualified Domain Names (FQDNs).
- Verbose mode (`-v`) for additional debugging information.

## Usage

To run `ft_ping`, you must have root privileges because it uses raw sockets. Below is an example of how to use the tool:

```bash
sudo ./ft_ping [OPTIONS] <destination>
```

## Options

- `-v`: Verbose mode. Provides detailed output, including ICMP packets other than ECHO_REPLY that are received.
- `-h` or `-?`: Displays help information

## Implementation Details

### ICMP Packet Structure

`ft_ping` constructs and sends ICMP ECHO_REQUEST packets and processes ICMP ECHO_REPLY packets. The following fields are included in the ICMP packet:

- Type: Specifies the type of ICMP message (8 for ECHO_REQUEST).
- Code: Specifies the code for the ICMP message (0 for ECHO_REQUEST).
- Checksum: Ensures data integrity.
- Identifier: Used to match requests with replies.
- Sequence Number: Incremented with each packet sent.

### Error Handling

`ft_ping` includes error handling for:

- Invalid IPv4 addresses or hostnames.
- Network timeouts.
- Invalid ICMP packets (e.g., incorrect checksums).

## Installation

**1.Clone the repository:**

```
git clone <repository-url>
cd ft_ping
```

**2.Compile the project:**

```
make
```

**3.Run the tool with root privileges:**

```
sudo ./ft_ping <destination>
```

## Testing

Several tests can be performed to ensure the functionality of ft_ping:

**1. Basic Connectivity:**

- Test with a reachable IP address, such as 8.8.8.8.
```
Example: sudo ./ft_ping 8.8.8.8.
```

**2.Unreachable Address:**

- Test with an IP address that does not respond to pings, such as 10.255.255.1.
```
Example: sudo ./ft_ping 10.255.255.1.
```

**3. Invalid Address:**

- Test with an invalid IP address format, such as 999.999.999.999.
```
Example: sudo ./ft_ping 999.999.999.999.
```

**4. Localhost:**

- Test with the loopback address 127.0.0.1.
```
Example: sudo ./ft_ping 127.0.0.1.
```

