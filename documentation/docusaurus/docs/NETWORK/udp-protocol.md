---
sidebar_position: 4
---

# UDP protocol (binary specification)

Detailed byte-level specification for R-Type UDP packets. Header is common to all packets.

## Common header (offsets, sizes)

Bytes are shown as offsets from 0.

| Offset | Size | Field |
|--------|------|-------|
| 0 | 1 | Magic (0x93) |
| 1 | 1 | Client ID |
| 2 | 4 | Sequence Number (uint32, network order) |
| 6 | 1 | Type (uint8) |
| 7 | 4 | Length (uint32) — payload length |

Payload starts at offset 11.

## Packet types (examples)

- `0x01` CONNECTION_CLIENT: payload (8 bytes) — client metadata (version, requested name hash)
- `0x02` ACCEPTATION: payload (1 byte) — assigned client slot
- `0x04` EVENT: payload (5 bytes) — event id + params

## ACK / Reliability

Recommended ACK packet layout (server → client):

| Offset | Size | Field |
|--------|------|-------|
| 0 | 1 | Magic (0x93) |
| 1 | 1 | Client ID |
| 2 | 4 | Cumulative ACK (uint32) — highest in-order seq |
| 6 | 1 | Type = 0xFE (ACK) |
| 7 | 2 | Bitfield length (n) |
| 9 | n | Bitfield (recent receipt bitmap, 1 == received) |

This allows selective acknowledgements: cumulative ack + recent bitmap for selective retransmit.

## Fragmentation
- If payload length + 11 > MTU (recommended 1200), split into fragments with a small fragment header: `frag_id (2 bytes)`, `frag_index (1 byte)`, `frag_count (1 byte)` prefixing the payload.

## Checksum & Integrity
- Recommend CRC32 of header+payload placed at end of packet or HMAC-SHA256 for critical messages.

## Versioning
- Add a protocol `version` field in the connection exchange so clients/servers can negotiate compatibility.

## Examples
- Minimal handshake sequence and an ACK example should be saved as hex dumps in `documentation/examples/` for debugging (not committed secrets).
