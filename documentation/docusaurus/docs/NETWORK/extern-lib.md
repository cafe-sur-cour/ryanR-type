---
sidebar_position: 5
---

# External Libraries

## ASIO Library Integration

The R-Type network library uses **ASIO** (Asynchronous I/O) as its underlying networking implementation. This document explains how ASIO is integrated and abstracted in our architecture.

---

## Architecture Overview

### Abstraction Layers

```
┌─────────────────────────────────────────────┐
│         Client Code (R-Type)                │
│  (Uses interfaces: INetworkSocket, etc.)    │
└──────────────────┬──────────────────────────┘
                   │
                   ▼
┌─────────────────────────────────────────────┐
│      Network Abstraction Layer              │
│  ┌───────────────────────────────────────┐  │
│  │  INetworkFactory (Interface)          │  │
│  │  - createSocket()                     │  │
│  │  - createResolver()                   │  │
│  │  - createEventLoop()                  │  │
│  └───────────────────────────────────────┘  │
│  ┌───────────────────────────────────────┐  │
│  │  INetworkSocket (Interface)           │  │
│  │  INetworkEndpoint (Interface)         │  │
│  │  INetworkErrorCode (Interface)        │  │
│  └───────────────────────────────────────┘  │
└──────────────────┬──────────────────────────┘
                   │
                   ▼
┌─────────────────────────────────────────────┐
│     ASIO Implementation Layer (Hidden)      │
│  ┌───────────────────────────────────────┐  │
│  │  AsioNetworkFactory                   │  │
│  │  AsioSocket                           │  │
│  │  AsioEndpoint                         │  │
│  │  AsioErrorCode                        │  │
│  │  AsioEventLoop                        │  │
│  └───────────────────────────────────────┘  │
│              ▼                              │
│     asio::io_context                        │
│     asio::ip::udp::socket                   │
│     asio::ip::address                       │
└─────────────────────────────────────────────┘
```

---

## Why ASIO?

### Advantages

✅ **Asynchronous Operations**: Non-blocking I/O for high performance  
✅ **Cross-Platform**: Works on Windows, Linux, macOS  
✅ **Header-Only**: No need for separate compilation (with `ASIO_STANDALONE`)  
✅ **Modern C++**: Uses C++17/20 features  
✅ **UDP & TCP Support**: Complete networking stack  
✅ **Well-Tested**: Industry-standard library used in production systems

### Use Case in R-Type

- **UDP Sockets** for fast, real-time game data
- **Asynchronous Receive** to handle multiple clients without blocking
- **Event Loop** (`io_context`) for efficient event handling

---

## Abstraction Strategy

### 1. PIMPL Pattern (Pointer to Implementation)

All ASIO types are hidden behind an opaque pointer to avoid exposing ASIO headers to client code.

```cpp
// AsioSocket.hpp
class AsioSocket : public INetworkSocket {
private:
    class Impl;  // Forward declaration
    std::unique_ptr<Impl> _impl;  // Opaque pointer
    
public:
    AsioSocket(std::shared_ptr<IEventLoop> eventLoop);
    ~AsioSocket() override;
    // ...
};
```

```cpp
// AsioSocket.cpp
class AsioSocket::Impl {
 public:
    asio::ip::udp::socket socket;  // ASIO type hidden in .cpp
    
    Impl(asio::io_context& context)
        : socket(context, asio::ip::udp::v4()) {}
};
```

**Benefits:**
- ✅ ASIO headers not exposed in public interfaces
- ✅ Faster compilation (no template instantiation in headers)
- ✅ Easy to swap implementation later

---

### 2. Interface Abstraction

Every ASIO concept has a corresponding interface:

| ASIO Type | R-Type Interface | Implementation |
|-----------|------------------|----------------|
| `asio::error_code` | `INetworkErrorCode` | `AsioErrorCode` |
| `asio::ip::address` | `INetworkAddress` | `AsioAddress` |
| `asio::ip::udp::endpoint` | `INetworkEndpoint` | `AsioEndpoint` |
| `asio::ip::udp::socket` | `INetworkSocket` | `AsioSocket` |
| `asio::ip::udp::resolver` | `INetworkResolver` | `AsioResolver` |
| `asio::io_context` | `IEventLoop` | `AsioEventLoop` |

**Example:**

```cpp
// INetworkSocket.hpp (Interface - No ASIO dependency)
class INetworkSocket {
public:
    virtual ~INetworkSocket() = default;
    
    virtual bool open(INetworkErrorCode& ec) = 0;
    virtual bool bind(const INetworkEndpoint& endpoint, INetworkErrorCode& ec) = 0;
    virtual std::size_t sendTo(const std::vector<uint8_t>& data, 
                                const INetworkEndpoint& endpoint, 
                                int flags, 
                                INetworkErrorCode& ec) = 0;
    // ...
};
```

```cpp
// AsioSocket.hpp (Implementation - ASIO hidden in .cpp)
class AsioSocket : public INetworkSocket {
private:
    class Impl;  // ASIO types hidden here
    std::unique_ptr<Impl> _impl;
    
public:
    bool open(INetworkErrorCode& ec) override;
    bool bind(const INetworkEndpoint& endpoint, INetworkErrorCode& ec) override;
    // ...
};
```

---

### 3. Factory Pattern

The `NetworkFactory` provides a centralized way to create network objects without exposing the underlying implementation.

```cpp
// NetworkFactory.hpp
class NetworkFactory {
public:
    static std::shared_ptr<INetworkFactory> getFactory() {
        static auto factory = std::make_shared<AsioNetworkFactory>();
        return factory;
    }
};
```

**Usage:**

```cpp
// Client code (no ASIO dependency)
auto factory = NetworkFactory::getFactory();
auto socket = factory->createSocket(eventLoop);
auto resolver = factory->createResolver(eventLoop);
auto endpoint = factory->createEndpoint(address, port);

INetworkErrorCode ec;
if (!socket->open(ec)) {
    std::cerr << "Failed to open socket: " << ec.message() << std::endl;
}
```

---

## Configuration

### NetworkConfig.hpp

Central configuration file to choose the implementation:

```cpp
// NetworkConfig.hpp
#ifndef NETWORK_CONFIG_HPP_
#define NETWORK_CONFIG_HPP_

// Configuration: Choose network implementation
#define NETWORK_USE_ASIO

#ifdef NETWORK_USE_ASIO
    #include "Asio/AsioErrorCode.hpp"
    #include "Asio/AsioAddress.hpp"
    #include "Asio/AsioEndpoint.hpp"
    #include "Asio/AsioSocket.hpp"
    #include "Asio/AsioResolver.hpp"
    #include "Asio/AsioEventLoop.hpp"

    namespace net {
        using NetworkErrorCode = AsioErrorCode;
        using NetworkAddress = AsioAddress;
        using NetworkEndpoint = AsioEndpoint;
        using NetworkSocket = AsioSocket;
        using NetworkResolver = AsioResolver;
        using NetworkEventLoop = AsioEventLoop;
    }
#endif

#endif /* !NETWORK_CONFIG_HPP_ */
```

**To switch implementation later:**

```cpp
// Future: Replace ASIO with another library
#define NETWORK_USE_BOOST_ASIO  // or
#define NETWORK_USE_LIBUV       // or
#define NETWORK_USE_CUSTOM_IMPL
```

---

## Memory Management

### Smart Pointers Strategy

All ASIO resources are managed with **smart pointers** to prevent memory leaks:

```cpp
// No raw pointers - everything is RAII
std::shared_ptr<INetworkSocket> _socket;
std::shared_ptr<INetworkEndpoint> _endpoint;
std::shared_ptr<IEventLoop> _eventLoop;
```

**Internal Implementation (PIMPL):**

```cpp
// AsioSocket.cpp
class AsioSocket::Impl {
 public:
    asio::ip::udp::socket socket;  // Value semantics - no manual delete needed
    
    Impl(asio::io_context& context)
        : socket(context, asio::ip::udp::v4()) {}
    
    ~Impl() {
        // Socket closes automatically (RAII)
    }
};
```

**Benefits:**
- ✅ No `new`/`delete` in user code
- ✅ Automatic cleanup when objects go out of scope
- ✅ Exception-safe
- ✅ Thread-safe reference counting with `shared_ptr`

---

## Error Handling

### No Exceptions - Error Codes Pattern

ASIO supports both exceptions and error codes. We use **error codes** for better control:

```cpp
// ASIO with exceptions (we avoid this)
try {
    socket.open(asio::ip::udp::v4());
} catch (const asio::system_error& e) {
    // Handle error
}

// ASIO with error codes (our approach)
asio::error_code ec;
socket.open(asio::ip::udp::v4(), ec);
if (ec) {
    // Handle error gracefully
}
```

**Our Wrapper:**

```cpp
bool AsioSocket::open(INetworkErrorCode& ec) {
    asio::error_code asio_ec;
    _impl->socket.open(asio::ip::udp::v4(), asio_ec);
    
    // Convert ASIO error to our interface
    static_cast<AsioErrorCode&>(ec).setFromInternal(
        std::make_shared<asio::error_code>(asio_ec)
    );
    
    return !asio_ec;
}
```

**Benefits:**
- ✅ No exception overhead
- ✅ Explicit error handling
- ✅ Compatible with game engines (often no-exception)

---

## Performance Considerations

### 1. Zero-Copy Operations

```cpp
// Efficient: uses vector's internal buffer
std::vector<uint8_t> buffer(1024);
auto bytes_received = socket->receiveFrom(buffer, sender, 0, ec);
buffer.resize(bytes_received);  // Shrink to actual size
```

### 2. Asynchronous I/O

```cpp
void AsioSocket::asyncReceiveFrom(
    std::vector<uint8_t>& buffer,
    INetworkEndpoint& sender,
    std::function<void(const INetworkErrorCode&, std::size_t)> handler) {
    
    _impl->socket.async_receive_from(
        asio::buffer(buffer),
        /* endpoint */,
        [handler](const asio::error_code& ec, std::size_t bytes) {
            AsioErrorCode error;
            error.setFromInternal(std::make_shared<asio::error_code>(ec));
            handler(error, bytes);
        }
    );
}
```

### 3. Buffer Reuse

```cpp
class NetworkSocket {
private:
    std::vector<uint8_t> _receiveBuffer;  // Reusable buffer
    
public:
    std::size_t receive() {
        // Reuse buffer - no allocation per call
        _receiveBuffer.resize(MAX_PACKET_SIZE);
        auto bytes = receiveFrom(_receiveBuffer, ...);
        _receiveBuffer.resize(bytes);
        return bytes;
    }
};
```

---

## Testing Strategy

### Mock Factory for Unit Tests

```cpp
// MockNetworkFactory.hpp
class MockNetworkFactory : public INetworkFactory {
public:
    std::shared_ptr<INetworkSocket> createSocket(
        std::shared_ptr<IEventLoop> eventLoop) override {
        return std::make_shared<MockSocket>();
    }
    
    std::shared_ptr<INetworkResolver> createResolver(
        std::shared_ptr<IEventLoop> eventLoop) override {
        return std::make_shared<MockResolver>();
    }
    // ...
};
```

**Usage in Tests:**

```cpp
TEST(ClientNetwork, ConnectToServer) {
    // Inject mock factory
    auto mockFactory = std::make_shared<MockNetworkFactory>();
    ClientNetwork client(mockFactory);
    
    // Test without real network calls
    client.init(8080, "localhost");
    ASSERT_TRUE(client.isConnected());
}
```

---

## Migration Path (Future)

If we need to replace ASIO with another library:

### Step 1: Create New Implementation

```cpp
// LibUVSocket.hpp
class LibUVSocket : public INetworkSocket {
    // Implement using libuv instead of ASIO
};

// LibUVNetworkFactory.hpp
class LibUVNetworkFactory : public INetworkFactory {
    std::shared_ptr<INetworkSocket> createSocket(...) override {
        return std::make_shared<LibUVSocket>(...);
    }
};
```

### Step 2: Update Configuration

```cpp
// NetworkConfig.hpp
#define NETWORK_USE_LIBUV  // Changed from NETWORK_USE_ASIO

#ifdef NETWORK_USE_LIBUV
    #include "LibUV/LibUVSocket.hpp"
    // ...
    namespace net {
        using NetworkSocket = LibUVSocket;
        // ...
    }
#endif
```

### Step 3: Client Code Unchanged

```cpp
// Client code doesn't change at all!
auto factory = NetworkFactory::getFactory();
auto socket = factory->createSocket(eventLoop);
```

---

## Dependencies

### vcpkg Configuration

```json
{
  "name": "r-type",
  "dependencies": [
    "asio"
  ]
}
```

### CMakeLists.txt

```cmake
find_package(asio CONFIG REQUIRED)

add_library(AsioNetwork
    Asio/AsioSocket.cpp
    Asio/AsioEndpoint.cpp
    Asio/AsioErrorCode.cpp
    Asio/AsioResolver.cpp
    Asio/AsioEventLoop.cpp
)

target_link_libraries(AsioNetwork PRIVATE asio::asio)
target_compile_definitions(AsioNetwork PRIVATE ASIO_STANDALONE)
```

---

## Best Practices

### ✅ DO

- Use the factory pattern to create network objects
- Always check error codes after operations
- Use `shared_ptr` for all network objects
- Reuse buffers when possible
- Use asynchronous operations for scalability

### ❌ DON'T

- Don't include ASIO headers in client code
- Don't use raw pointers for network objects
- Don't ignore error codes
- Don't allocate buffers on every receive
- Don't use synchronous operations in game loop

---

## Example: Complete Usage

```cpp
#include "NetworkFactory.hpp"

int main() {
    // 1. Get factory (no ASIO dependency)
    auto factory = NetworkFactory::getFactory();
    
    // 2. Create network objects
    auto eventLoop = factory->createEventLoop();
    auto socket = factory->createSocket(eventLoop);
    auto resolver = factory->createResolver(eventLoop);
    
    // 3. Resolve server address
    INetworkErrorCode ec;
    auto endpoints = resolver->resolve("localhost", "8080", ec);
    
    if (ec) {
        std::cerr << "Resolve failed: " << ec.message() << std::endl;
        return 1;
    }
    
    // 4. Open and bind socket
    if (!socket->open(ec)) {
        std::cerr << "Open failed: " << ec.message() << std::endl;
        return 1;
    }
    
    // 5. Send data
    std::vector<uint8_t> data = {0x01, 0x02, 0x03};
    auto bytesSent = socket->sendTo(data, *endpoints[0], 0, ec);
    
    if (ec) {
        std::cerr << "Send failed: " << ec.message() << std::endl;
        return 1;
    }
    
    std::cout << "Sent " << bytesSent << " bytes" << std::endl;
    
    // 6. Run event loop
    eventLoop->run();
    
    return 0;
}
```

---

## Summary

| Aspect | Implementation |
|--------|----------------|
| **Library** | ASIO (standalone, header-only) |
| **Abstraction** | PIMPL pattern + Interface layer |
| **Memory** | Smart pointers (`shared_ptr`, `unique_ptr`) |
| **Errors** | Error codes (no exceptions) |
| **Factory** | `NetworkFactory` for dependency injection |
| **Testing** | Easy mocking via factory pattern |
| **Performance** | Zero-copy, async I/O, buffer reuse |
| **Portability** | Windows, Linux, macOS |
| **Future-Proof** | Easy to swap ASIO with another library |

---

## References

- [ASIO Documentation](https://think-async.com/Asio/)
- [ASIO Tutorial](https://think-async.com/Asio/asio-1.28.0/doc/asio/tutorial.html)
- R-Type Network Architecture
- Network Factory Pattern

