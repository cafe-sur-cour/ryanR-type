---
sidebar_position: 3
---

# User Account Security

This document describes the security measures implemented in R-Type for user account management, authentication, and data protection.

## Overview

R-Type implements a secure user account system that allows players to:
- Register new accounts with username and password
- Authenticate using their credentials
- Track personal statistics (wins, high scores, games played)
- Maintain persistent profiles across sessions

## Authentication System

### Registration Process

**Client-Side:**
1. User provides username (max 8 characters) and password (max 8 characters)
2. Client sends `REGISTER_PACKET (0x13)` with credentials
3. Client waits for server response

**Server-Side:**
1. Server receives registration request
2. Validates username doesn't already exist
3. **Encrypts password** using XOR-based encryption with base64 encoding
4. Stores encrypted credentials in `saves/users.json`
5. Sends `CONNECT_USER_PACKET (0x14)` on success or `REGISTER_FAIL_PACKET (0x1B)` on failure

**Packet Format:**
- Total length: 16 bytes (fixed)
- Username: 8 bytes (padded with null bytes if shorter)
- Password: 8 bytes (padded with null bytes if shorter)

### Login Process

**Client-Side:**
1. User provides username and password
2. Client sends `LOGIN_PACKET (0x15)` with credentials
3. Client waits for authentication confirmation

**Server-Side:**
1. Server receives login request
2. Retrieves stored user data from `saves/users.json`
3. **Decrypts stored password** and compares with provided password
4. Sends `CONNECT_USER_PACKET (0x14)` on success with username confirmation
5. Updates client's internal username for session tracking

**Packet Format:**
- Same as registration: 16 bytes total
- Username: 8 bytes
- Password: 8 bytes

## Password Encryption

### Encryption Method

R-Type uses a custom encryption system implemented in `common/utils/Encryption.hpp`:

**Algorithm:**
- **XOR Cipher** with a fixed encryption key
- **Base64 Encoding** for safe storage in JSON format

**Key Details:**
- Encryption key: `"R-Type_Secure_Key_2026"` (defined in `Encryption.hpp`)
- Process: Plain text → XOR with key → Base64 encode → Store

**Implementation:**
```cpp
std::string encryptedPassword = utils::Encryption::encrypt(password);
// Stored in users.json

// During login:
std::string decryptedPassword = utils::Encryption::decrypt(storedPassword);
if (decryptedPassword == password) {
    // Authentication successful
}
```

### Security Considerations

**Current Implementation:**
- ✅ Passwords are never stored in plain text
- ✅ Encrypted passwords are base64-encoded for JSON compatibility
- ✅ Same encryption used for all sensitive data storage

**Limitations:**
- ⚠️ XOR encryption with fixed key is symmetric and reversible
- ⚠️ Not suitable for production environments with real user data
- ⚠️ Base64 is encoding, not encryption (provides no additional security)

**Recommendations for Production:**
- Use industry-standard password hashing (bcrypt, Argon2, PBKDF2)
- Implement salting for each password
- Use HTTPS/TLS for network communication
- Add rate limiting for login attempts
- Implement password strength requirements

## Data Storage

### User Data Structure

User accounts are stored in `saves/users.json` with the following structure:

```json
[
    {
        "username": "player1",
        "password": "ENCRYPTED_BASE64_STRING",
        "wins": 0,
        "high_score": 0,
        "games_played": 0,
        "time_spent": 0
    }
]
```

**Fields:**
- `username` (string, 8 chars max): Unique player identifier
- `password` (string): Encrypted password with base64 encoding
- `wins` (integer): Total number of games won
- `high_score` (integer): Highest score achieved
- `games_played` (integer): Total games participated in
- `time_spent` (integer): Total time spent in games (in seconds)

### File Access

**Location:** `saves/users.json`

**Permissions:**
- File is created with default system permissions
- Directory structure is automatically created if missing
- No additional access restrictions applied

**Concurrent Access:**
- Server handles single-threaded read/write operations
- No file locking mechanism implemented
- Potential race conditions in multi-server scenarios

## Network Security

### Packet Transmission

**Registration Packet (0x13):**
- Username and password sent as **plain text** over UDP
- Fixed 16-byte payload
- No encryption during transmission

**Login Packet (0x15):**
- Same format as registration
- Credentials transmitted **unencrypted** over network

**Security Implications:**
- ⚠️ Credentials can be intercepted via packet sniffing
- ⚠️ UDP provides no built-in encryption
- ⚠️ Man-in-the-middle attacks possible

**Recommendations:**
- Implement TLS/DTLS for UDP encryption
- Use secure key exchange (Diffie-Hellman)
- Add client-side hashing before transmission
- Implement certificate validation

## Session Management

### Connection Tracking

**Server-Side:**
- Each client assigned unique `clientId` (1 byte)
- Username stored in client tuple: `(clientId, endpoint, username)`
- Session persists until disconnection
- No session timeout implemented

**Authentication State:**
- Clients can play without authentication
- Authenticated users gain access to:
  - Profile statistics
  - Leaderboard submissions
  - Game history tracking
  - Chat functionality

### Lobby System

**Security Features:**
- Lobby codes (8 characters) required for joining
- Only lobby master can start game
- Game rules modification restricted to authenticated users
- Automatic games_played increment on game start

## Profile Data Protection

### Statistics Tracking

**Automatic Updates:**
- `games_played` incremented when lobby master starts game
- Statistics updated in real-time during gameplay
- Profile data sent via `PROFILE_PACKET (0x1D)` (40 bytes)

**Data Integrity:**
- No checksum or validation for statistics
- Direct JSON file modification possible
- No backup or versioning system

### Leaderboard System

**Data Transmission:**
- Top 10 players sent via `LEADERBOARD_PACKET (0x1A)` (160 bytes)
- Format: 10 entries × (8 bytes username + 8 bytes score)
- Sorted by high score server-side

**Security Issues:**
- No prevention of score manipulation
- Direct file editing can modify rankings
- No audit trail for leaderboard changes

## Chat Security

### Message Broadcasting

**System Flow:**
1. Client sends `NEW_CHAT_PACKET (0x20)` with message
2. Server validates sender is authenticated
3. Server broadcasts via `BROADCASTED_CHAT_PACKET (0x21)` to all clients

**Security Measures:**
- Only authenticated users can send messages
- Messages include sender username (8 bytes)
- No content filtering or sanitization

**Vulnerabilities:**
- No rate limiting on messages
- No profanity filter
- No message length restrictions enforced
- Potential for spam or abuse

## Best Practices for Developers

### Current Implementation Strengths

1. **Separation of Concerns:** Authentication logic isolated in dedicated packet handlers
2. **Consistent Encryption:** Same encryption method used throughout
3. **Modular Design:** Encryption utilities in reusable `utils::Encryption` class
4. **Error Handling:** Registration failures properly communicated to clients

### Recommended Improvements

1. **Upgrade Encryption:**
   ```cpp
   // Instead of XOR encryption:
   #include <bcrypt/BCrypt.hpp>
   std::string hashedPassword = BCrypt::generateHash(password);
   ```

2. **Add Password Requirements:**
   - Minimum 8 characters
   - Mix of letters, numbers, symbols
   - Prevent common passwords

3. **Implement Rate Limiting:**
   - Max 5 login attempts per minute per IP
   - Temporary account lockout after failed attempts
   - CAPTCHA for suspicious activity

4. **Secure Network Communication:**
   - Implement DTLS for UDP encryption
   - Use secure key exchange protocols
   - Validate certificates

5. **Enhance Data Protection:**
   - Regular backups of `users.json`
   - Implement access control lists
   - Add integrity checks (checksums/signatures)

## Security Audit Checklist

- [ ] Passwords encrypted at rest
- [ ] Unique usernames enforced
- [ ] Session management implemented
- [ ] Authentication required for sensitive operations
- [x] Encrypted passwords stored (with limitations)
- [ ] Network traffic encrypted
- [ ] Rate limiting on authentication
- [ ] Input validation and sanitization
- [ ] Audit logging for security events
- [ ] Regular security updates and patches

## References

**Related Documentation:**
- [RFC Protocol Documentation](../SERVER/rfc.md)
- [Network Architecture](../../static/pdfs/R-Type-Doc.pdf)
- [Error Handling](./error_handling.md)

**Code Locations:**
- Encryption: `common/utils/Encryption.hpp` & `Encryption.cpp`
- Server Auth: `server/ServerReceivePacket.cpp`
- Packet Definitions: `common/interfaces/IPacketManager.hpp`
- User Data: `saves/users.json`

**Security Standards:**
- OWASP Top 10: https://owasp.org/www-project-top-ten/
- NIST Password Guidelines: https://pages.nist.gov/800-63-3/
- UDP Security Best Practices: https://tools.ietf.org/html/rfc8085

