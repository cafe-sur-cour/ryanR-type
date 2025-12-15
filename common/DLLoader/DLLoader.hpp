/*
** EPITECH PROJECT, 2025
** r-type
** File description:
** DLLoader
*/

#ifndef DLLOADER_HPP_
#define DLLOADER_HPP_

#ifdef _WIN32
    #include <windows.h>
    #define RTLD_LAZY 0
#else
    #include <dlfcn.h>
#endif

#include <iostream>
#include <ostream>
#include <memory>
#include "ILoader.hpp"

template <typename T>

class DLLoader : public ILoader {
    private:
#ifdef _WIN32
        HMODULE _handler = nullptr;
        mutable std::string _lastError;
#else
        void *_handler = nullptr;
#endif

    public:
        ~DLLoader() override {
            if (_handler != nullptr) {
                Close();
            }
        }

        void *getHandler() const override {
            return _handler;
        };

        void *Open(const char *path, int flag = RTLD_LAZY) override {
#ifdef _WIN32
            (void)flag;
            _handler = LoadLibraryA(path);
            if (!_handler) {
                _lastError = "Failed to load library: " + std::string(path);
            }
#else
            _handler = dlopen(path, flag);
#endif
            return _handler;
        };

        void *Symbol(const char *symbolName) override {
#ifdef _WIN32
            void *symbol = (void*)GetProcAddress(_handler, symbolName);
            if (!symbol) {
                _lastError = "Failed to get symbol: " + std::string(symbolName);
                std::cerr << "GetProcAddress error: " << _lastError << std::endl;
                return nullptr;
            }
            return symbol;
#else
            void *symbol = dlsym(_handler, symbolName);
            const char *error = dlerror();
            if (error) {
                std::cerr << "dlerror: " << error << std::endl;
                return nullptr;
            }
            return symbol;
#endif
        };

        T getSymbol(const char *symbolName) {
#ifdef _WIN32
            return reinterpret_cast<T>(GetProcAddress(_handler, symbolName));
#else
            return reinterpret_cast<T>(dlsym(_handler, symbolName));
#endif
        };

        int Close() override{
            if (_handler == nullptr)
                return -1;
#ifdef _WIN32
            int result = FreeLibrary(_handler) ? 0 : -1;
#else
            int result = dlclose(_handler);
#endif
            _handler = nullptr;  // Prevent double-close
            return result;
        };

        const char *Error() override {
#ifdef _WIN32
            return _lastError.c_str();
#else
            return dlerror();
#endif
        };
};

#endif /* !DLLOADER_HPP_ */
