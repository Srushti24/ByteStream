#pragma once
#include <array>
#include <concepts>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <type_traits>

namespace traits {

template <typename T>
struct is_vector {
    static constexpr bool value = false;
};

template <typename T, typename Alloc>
struct is_vector<std::vector<T, Alloc>> {
    static constexpr bool value = true;
};

template <typename T>
concept Vector = is_vector<T>::value;

template <typename T>
struct is_array {
    static constexpr bool value = false;
};

template <typename T, size_t N>
struct is_array<std::array<T, N>> {
    static constexpr bool value = true;
};

template <typename T>
concept Array = is_array<T>::value;

template <typename T>
struct is_pair {
    static constexpr bool value = false;
};

template <typename T, typename P>
struct is_pair<std::pair<T, P>> {
    static constexpr bool value = true;
};

template <typename T>
concept Pair = is_pair<T>::value;

template <typename T>
struct is_optional {
    static constexpr bool value = false;
};

template <typename T>
struct is_optional<std::optional<T>> {
    static constexpr bool value = true;
};

template <typename T>
concept Optional = is_optional<T>::value;

template <typename T>
struct is_unordered_map {
    static constexpr bool value = false;
};

template <typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
struct is_unordered_map<std::unordered_map<Key, T, Hash, KeyEqual, Allocator>> {
    static constexpr bool value = true;
};

template <typename T>
concept UnorderedMap = is_unordered_map<T>::value;

template <typename T>
struct is_map {
    static constexpr bool value = false;
};

template <class Key, class T, class Compare, class Allocator>
struct is_map<std::map<Key, T, Compare, Allocator>> {
    static constexpr bool value = true;
};

template <typename T>
concept Map = is_map<T>::value;

template <typename T>
struct is_set {
    static constexpr bool value = false;
};

template <typename T, typename Compare, typename Allocator>
struct is_set<std::set<T, Compare, Allocator>> {
    static constexpr bool value = true;
};

template <typename T>
concept Set = is_set<T>::value;

template <typename T>
struct is_tuple {
    static constexpr bool value = false;
};

template <typename... Args>
struct is_tuple<std::tuple<Args...>> {
    static constexpr bool value = true;
};

template <typename T>
concept Tuple = is_tuple<T>::value;

} // namespace traits

namespace ByteStream {

class Serializer {
  private:
    std::ofstream stream;

  public:
    Serializer(const std::string& filename);
    ~Serializer();

    void close();

    template <std::integral T>
    Serializer& operator<<(const T& value) {
        stream.write(reinterpret_cast<const char*>(&value), sizeof(T));
        return *this;
    }

    template <std::floating_point T>
    Serializer& operator<<(const T& value) {
        stream.write(reinterpret_cast<const char*>(&value), sizeof(value));
        return *this;
    }

    template <typename T>
    requires std::same_as<T, std::string>
    Serializer& operator<<(const T& value) {
        size_t len = value.length();
        stream.write(reinterpret_cast<const char*>(&len), sizeof(size_t));
        stream.write(value.data(), len);
        return *this;
    }

    template <typename T>
    requires std::is_enum_v<T>
    Serializer& operator<<(const T& value) {
        using underlyingType = std::underlying_type_t<T>;
        underlyingType val = static_cast<underlyingType>(value);
        return (*this) << val;
    }

    template <typename T>
    requires traits::Vector<T>
    Serializer& operator<<(const T& vector) {
        size_t vectorLength = vector.size();
        (*this) << vectorLength;
        for (size_t i = 0; i < vectorLength; i++) {
            (*this) << vector[i];
        }
        return *this;
    }

    template <typename T>
    requires traits::Array<T>
    Serializer& operator<<(const T& array) {
        size_t arrayLength = array.size();
        (*this) << arrayLength;
        for (size_t i = 0; i < arrayLength; i++) {
            (*this) << array[i];
        }
        return *this;
    }

    template <typename T>
    requires traits::Pair<T>
    Serializer& operator<<(const T& pair) {
        (*this) << pair.first;
        (*this) << pair.second;
        return *this;
    }

    template <typename T>
    requires traits::Optional<T>
    Serializer& operator<<(const T& element) {
        (*this) << element.has_value();
        if (element.has_value()) {
            (*this) << *element;
        }
        return *this;
    }

    template <typename T>
    requires traits::UnorderedMap<T>
    Serializer& operator<<(const T& unorderedMap) {
        size_t mapSize = unorderedMap.size();
        (*this) << mapSize;
        for (const auto& kv : unorderedMap) {
            (*this) << kv.first;
            (*this) << kv.second;
        }
        return *this;
    }

    template <typename T>
    requires traits::Map<T>
    Serializer& operator<<(const T& map) {
        size_t mapSize = map.size();
        (*this) << mapSize;
        for (const auto& kv : map) {
            (*this) << kv.first;
            (*this) << kv.second;
        }
        return *this;
    }

    template <typename T>
    requires traits::Set<T>
    Serializer& operator<<(const T& set) {
        size_t setSize = set.size();
        (*this) << setSize;
        for (const auto& element : set) {
            (*this) << element;
        }
        return *this;
    }

    template <typename T>
    requires traits::Tuple<T>
    Serializer& operator<<(const T& tuple) {
        constexpr size_t sizeOfTuple = std::tuple_size_v<T>;
        (*this) << sizeOfTuple;
        std::apply([this](auto... elements) { (((*this) << elements), ...); }, tuple);
        return *this;
    }
};

class Deserializer {
  private:
    std::ifstream stream;

  public:
    Deserializer(const std::string& filename);
    ~Deserializer();
    void close();

    template <std::integral T>
    Deserializer& operator>>(T& value) {
        stream.read(reinterpret_cast<char*>(&value), sizeof(T));
        return *this;
    }

    template <std::floating_point T>
    Deserializer& operator>>(T& value) {
        stream.read(reinterpret_cast<char*>(&value), sizeof(T));
        return *this;
    }

    Deserializer& operator>>(std::string& value) {
        size_t arrayLength;
        stream.read(reinterpret_cast<char*>(&arrayLength), sizeof(size_t));
        value.resize(arrayLength);
        stream.read(value.data(), arrayLength);
        return *this;
    }

    template <typename T>
    requires std::is_enum_v<T>
    Deserializer& operator>>(T& value) {
        using underlyingType = std::underlying_type_t<T>;
        underlyingType val{};
        (*this) >> val;
        value = static_cast<T>(val);
        return *this;
    }

    template <typename T>
    requires traits::Vector<T>
    Deserializer& operator>>(T& vector) {
        size_t vectorLength = 0;
        (*this) >> vectorLength;
        vector.resize(vectorLength);
        for (size_t i = 0; i < vectorLength; i++) {
            (*this) >> vector[i];
        }
        return *this;
    }

    template <typename T>
    requires traits::Array<T>
    Deserializer& operator>>(T& array) {
        size_t arrayLength = 0;
        (*this) >> arrayLength;
        for (size_t i = 0; i < arrayLength; i++) {
            (*this) >> array[i];
        }
        return *this;
    }

    template <typename T>
    requires traits::Pair<T>
    Deserializer& operator>>(T& pair) {
        (*this) >> pair.first;
        (*this) >> pair.second;
        return *this;
    }

    template <typename T>
    requires traits::Optional<T>
    Deserializer& operator>>(T& element) {
        bool hasValue;
        (*this) >> hasValue;
        if (hasValue) {
            element.emplace();
            (*this) >> *element;
        } else {
            element.reset();
        }
        return *this;
    }

    template <typename T>
    requires traits::UnorderedMap<T>
    Deserializer& operator>>(T& unorderedMap) {
        size_t mapSize;
        (*this) >> mapSize;
        using key = T::key_type;
        using value = T::mapped_type;
        for (size_t i = 0; i < mapSize; i++) {
            key k{};
            value v{};
            (*this) >> k;
            (*this) >> v;
            unorderedMap[k] = v;
        }
        return *this;
    }

    template <typename T>
    requires traits::Map<T>
    Deserializer& operator>>(T& map) {
        size_t mapSize;
        (*this) >> mapSize;
        using key = T::key_type;
        using value = T::mapped_type;
        for (size_t i = 0; i < mapSize; i++) {
            key k{};
            value v{};
            (*this) >> k;
            (*this) >> v;
            map[k] = v;
        }
        return *this;
    }

    template <typename T>
    requires traits::Set<T>
    Deserializer& operator>>(T& set) {
        size_t setSize;
        (*this) >> setSize;
        using Elem = typename T::value_type;
        for (size_t i = 0; i < setSize; i++) {
            Elem val{};
            (*this) >> val;
            set.insert(val);
        }
        return *this;
    }

    template <typename T>
    requires traits::Tuple<T>
    Deserializer& operator>>(T& tuple) {
        size_t sizeOfTuple;
        (*this) >> sizeOfTuple;
        std::apply([this](auto&... elements) { (((*this) >> elements), ...); }, tuple);
        return *this;
    }
};

} // namespace ByteStream
