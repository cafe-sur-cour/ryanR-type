---
sidebar_position: 2
---

## R-Type

This is the overview on how we handle errors and security in our program.

If you want a more technical overview of this please refer to the project architecture pdf : [R-Type-Doc](../../static/pdfs/R-Type-Doc.pdf)

## Error Classes

Our error handling system is built around a hierarchy of error classes located in `common/Error/`. These classes provide a standardized way to handle and report errors throughout the application.

### IError Interface

The `IError` class is the base interface for all errors in the system. It inherits from `std::exception` and defines the following pure virtual methods:

- `what()`: Returns a C-style string describing the error (inherited from `std::exception`).
- `getCode()`: Returns an integer error code.
- `getType()`: Returns a string representing the error type.
- `getDetails()`: Returns a formatted string with detailed error information.

### AError Abstract Class

`AError` is an abstract base class that implements most of the `IError` interface. It provides:

- A constructor that takes a message string and an optional error code.
- Implementations for `what()`, `getCode()`, and `getDetails()`.
- The `getType()` method remains pure virtual, requiring subclasses to implement it.

The `getDetails()` method formats the error information as: `[Type] Code: X - Message`

### ServerError Class

`ServerError` is a concrete implementation of `AError` specifically for server-related errors. It defines an enum of predefined error codes:

- `UNKNOWN` (1000): Generic unknown error.
- `CONNECTION_FAILED` (1001): Failed to establish a connection.
- `TIMEOUT` (1002): Operation timed out.
- `INVALID_REQUEST` (1003): Invalid request received.
- `INTERNAL_ERROR` (1004): Internal server error.
- `LIBRARY_LOAD_FAILED` (1005): Failed to load a required library.
- `CONFIG_ERROR` (1006): Configuration-related error.

The `getType()` method returns "ServerError".

### Usage Example

```cpp
try {
    // Some server operation
    throw err::ServerError("Failed to connect to client", err::ServerError::CONNECTION_FAILED);
} catch (const err::IError& e) {
    std::cerr << e.getDetails() << std::endl;
    // Output: [ServerError] Code: 1001 - Failed to connect to client
}
```

This error handling system ensures consistent error reporting and allows for easy extension with new error types by inheriting from `AError`.
