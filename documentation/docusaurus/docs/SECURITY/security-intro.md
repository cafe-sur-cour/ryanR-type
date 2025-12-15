---
sidebar_position: 1
---

# R-Type Security

## Authentication & Session Management
- Clients are identified by `Client ID` in packets but this is not authentication by itself.
- Recommended: establish a short-lived session token at handshake (server-signed token) and include it in header for privileged operations.

Handshake flow (recommended):
1. Client → Server: `CONNECTION_CLIENT` (includes client public metadata)
2. Server validates and returns `ACCEPTATION` with `session_token`
3. Client includes `session_token` in subsequent critical requests

## Transport Security
- UDP is used for gameplay. For any sensitive control plane traffic consider DTLS or move authentication to a TCP/TLS control channel.
- If DTLS is not feasible, sign critical packets with HMAC-SHA256 using server-managed secret.

## Integrity & Checksums
- Use a strong checksum/validation for packet payloads. Recommended: CRC32 for corruption detection + HMAC-SHA256 for integrity/authenticity of critical packets.
- Include `version` and `timestamp` fields to allow replay protection.

## Rate Limiting and Abuse Controls
- Per-client token bucket rate limiter (configurable thresholds).
- Blacklist clients that exceed thresholds repeatedly and log details for manual review.

## Secrets & Keys
- Do not commit server secrets/keys to the repo. Use environment variables or secret store (e.g., Vault).

## CI/CD and Dependency Security
- Add dependency scanning in CI (e.g., `vcpkg` vulnerability checks or Snyk/OSS scanners).

## Threat Model (short)
- Threats considered: spoofing, replay, flooding, packet corruption. See `POCS/network/poc-network-reliability-security.md` for POC mitigations.

## Operational Recommendations
- Enable logging for handshake failures; collect minimal telemetry (no PII).
- Provide an admin command to rotate server signing keys and invalidate old sessions.
