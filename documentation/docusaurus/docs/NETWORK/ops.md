---
sidebar_position: 3
---

# Network operations & scaling

Operational guidance for deploying and operating R-Type servers.

## Ports & Firewall
- Default UDP port: `4242` (documented in README). Ensure `ufw`/firewall allows UDP 4242 for game traffic.

## NAT traversal & matchmaking
- Typical approach: clients initiate outbound UDP to server (no special NAT punch-through required for client-server). For peer-to-peer features consider STUN/TURN.

## Scaling
- For larger deployments: separate matchmaker and game servers. Matchmaker (TCP/TLS) handles authentication and directs clients to an appropriate game server.
- Game servers remain UDP authoritative; use a small control TCP channel for admin/metrics.

## Monitoring & Metrics
- Expose metrics: players_connected, packet_loss_rate, rtt_ms, retransmit_rate, server_tick_time.
- Recommended: push metrics to Prometheus and set alerting for high packet loss or high tick times.

## MTU and Fragmentation
- Respect common MTU 1200–1400 bytes. Implement application-level fragmentation for large `GAME_STATE` packets: split, sequence, reassemble.

## Logging & Debugging
- Log handshake attempts, invalid packets and rate-limit events. Keep logs configurable (verbosity levels).
