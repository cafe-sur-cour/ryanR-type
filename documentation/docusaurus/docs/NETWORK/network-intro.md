---
sidebar_position: 1
---

# R-Type Network

This section covers the network architecture and implementations for R-Type, focusing on real-time multiplayer communication.

## Overview

R-Type requires efficient, low-latency network communication for multiplayer gameplay. We've explored different protocols and patterns to ensure reliable delivery of game state while maintaining performance.

## Key Concepts

- **TCP**: Reliable, ordered delivery but higher latency
- **UDP**: Fast, low-latency but unreliable
- **Handshake**: Mechanism to ensure critical information delivery over UDP

## Proof of Concepts

For detailed analysis and comparison of network approaches, see our [Network POCs](../POCS/network/pocs-network.md).