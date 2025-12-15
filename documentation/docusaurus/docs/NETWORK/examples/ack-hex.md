---
sidebar_position: 2
---

# ACK example (hex dump)

This example shows a selective ACK packet layout described in `udp-protocol.md`.

Hex dump example:

```
00: 93 0A 00 00 00 05 FE 00 02  00 03
```

Interpretation:
- `93` : Magic
- `0A` : Client ID = 10
- `00 00 00 05` : Cumulative ACK = 5 (all packets ≤5 received in order)
- `FE` : Type = ACK (0xFE)
- `00 02` : Bitfield length = 2 bytes
- `00 03` : Bitfield `0x0003` → recent receipt bitmap (LSB is most recent after cumulative)

Usage: server sends this to inform the client which recent packets were received, enabling selective retransmit.
