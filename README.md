# ByteStream

A lightweight C++ binary serialization library that supports reading and writing 
a wide range of types to and from binary files using a simple `<<` and `>>` operator interface.

## Usage

```cpp
// Writing
ByteStream::Serializer obs("file.bin");
obs << myInt << myString << myVector;
obs.close();

// Reading
ByteStream::Deserializer ibs("file.bin");
ibs >> myInt >> myString >> myVector;
ibs.close();
```

## Supported Types

### Primitives
- All integral types — `int`, `long`, `long long`, `short`, `unsigned` variants
- Floating point — `float`, `double`
- `bool`, `char`
- `std::string`

### Enums
- Plain enums
- Scoped enums (`enum class`)
- Enums with custom underlying types (`enum class X : char`)

### STL Containers
- `std::vector`
- `std::array`
- `std::pair`
- `std::map`
- `std::unordered_map`
- `std::set`
- `std::optional`
- `std::tuple`

## Coming Soon
- std::variant
- std::unordered_set
- std::list

### Nested Types
All container types support arbitrary nesting:
```cpp
std::vector<std::map>
std::optional<std::vector>
std::tuple<std::vector, std::map, std::optional>
```

### Custom Classes
Any user-defined class can be serialized by defining friend `operator<<` and `operator>>`:
```cpp
class Person {
    int age;
    std::string name;

    friend ByteStream::Serializer& operator<>(ByteStream::Deserializer& stream, Person& p) {
        stream >> p.age >> p.name;
        return stream;
    }
};
```

Custom classes also work inside STL containers:
```cpp
std::vector people;
obs << people;  // just works
```

## Requirements
- C++20 or later
- CMake 3.x+