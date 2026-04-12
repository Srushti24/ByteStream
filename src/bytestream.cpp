#include "bytestream.hpp"

#include <cassert>
namespace ByteStream {

Serializer::Serializer(const std::string& filename) {
    stream.open(filename, std::ios::binary | std::ios::app);
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
