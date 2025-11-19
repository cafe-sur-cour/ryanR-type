# Network POCs Documentation

## Overview

This document presents our network Proof of Concept (POC) focused on critical communications for R-Type multiplayer gameplay.

## Critical Communications POC

We have implemented a single POC that compares two approaches for handling critical information in real-time multiplayer games:

- **TCP**: Traditional reliable protocol
- **UDP with Handshake**: Low-latency protocol with custom reliability layer

See [Critical Communications Comparison POC](poc-network-critical-comparison.md)

## Objective

The goal of this POC is to determine the best network approach for R-Type by evaluating:

- **Reliability**: Ensuring critical game state is delivered
- **Latency**: Minimizing delay for responsive gameplay
- **Performance**: Optimizing for real-time shooter mechanics
- **Implementation complexity**: Balancing features with development effort

## Conclusion

Based on our POC evaluation, we will adopt **UDP with Handshake** for critical communications in R-Type. This approach provides the necessary reliability for game-critical information while maintaining the low latency required for competitive multiplayer gameplay. Additionally, it allows us to unify our entire network stack under UDP, since non-critical communications are already implemented using this protocol.