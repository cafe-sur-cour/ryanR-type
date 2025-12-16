---
sidebar_position: 1
---

# UDP handshake example (hex dump)

This example shows a minimal handshake exchange (client → server → client) as hex bytes with offsets.

Common header recap (11 bytes): Magic(1), ClientID(1), Seq(4), Type(1), Length(4)

Example: Client `CONNECTION_CLIENT` (Type 0x01)

Hex dump (spaces separate bytes):

```
00: 93 0A 00 00 00 01 01 00 00 00 08  01 02 03 04 05 06 07 08
```

Interpretation:
- `93` : Magic
- `0A` : Client ID = 10
- `00 00 00 01` : Sequence = 1
- `01` : Type = CONNECTION_CLIENT (0x01)
- `00 00 00 08` : Length = 8 bytes payload
- payload (8 bytes): `01 02 03 04 05 06 07 08` (client metadata placeholder)

Server `ACCEPTATION` (Type 0x02) example reply:

```
00: 93 0A 00 00 00 02 02 00 00 00 01  05
```

- Sequence = 2, Type = 0x02, Length = 1, payload `05` = assigned slot id

How to use: copy the hex (without offsets) into a binary file and inspect with Wireshark or `xxd -r -p`.
