#include "bytestream.hpp"

#include <cassert>
namespace ByteStream {

Serializer::Serializer(const std::string& filename) {
    stream.open(filename, std::ios::binary | std::ios::trunc);
    if(!stream.is_open())
    {
        throw std::runtime_error("Failed to open file to write");
    }
}

Serializer::~Serializer() {
    if (stream.is_open()) {
        stream.close();
    }
}

void Serializer::close() {
    if (stream.is_open()) {
        stream.close();
    }
}

Deserializer::Deserializer(const std::string& filename) {
    stream.open(filename, std::ios::binary);
    if(!stream.is_open())
    {
        throw std::runtime_error("Failed to open file to read");
    }
}

Deserializer::~Deserializer() {
    if (stream.is_open()) {
        stream.close();
    }
}

void Deserializer::close() {
    if (stream.is_open()) {
        stream.close();
    }
}

} // namespace ByteStream
