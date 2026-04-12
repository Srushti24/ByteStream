// main.cpp
#include "bytestream.hpp"

#include <cassert>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

using std::make_tuple;
using std::string;
using std::vector;

class Person {
    int age;
    std::string name;

    friend ByteStream::Serializer& operator<<(ByteStream::Serializer& stream, const Person& p) {
        stream << p.age << p.name;
        return stream;
    }
    friend ByteStream::Deserializer& operator>>(ByteStream::Deserializer& stream, Person& p) {
        stream >> p.age >> p.name;
        return stream;
    }

  public:
    Person() : age(0), name("") {}
    Person(int a, std::string n) : age(a), name(n) {}
    bool operator==(const Person& other) const { return age == other.age && name == other.name; }
};

// --- Address class ---
class Address {
    std::string city;
    int zipcode;

    friend ByteStream::Serializer& operator<<(ByteStream::Serializer& stream, const Address& a) {
        stream << a.city << a.zipcode;
        return stream;
    }
    friend ByteStream::Deserializer& operator>>(ByteStream::Deserializer& stream, Address& a) {
        stream >> a.city >> a.zipcode;
        return stream;
    }

  public:
    Address() : city(""), zipcode(0) {}
    Address(std::string c, int z) : city(c), zipcode(z) {}
    bool operator==(const Address& other) const {
        return city == other.city && zipcode == other.zipcode;
    }
};

// --- Employee with nested Address ---
class Employee {
    std::string name;
    Address address;
    double salary;

    friend ByteStream::Serializer& operator<<(ByteStream::Serializer& stream, const Employee& e) {
        stream << e.name << e.address << e.salary;
        return stream;
    }
    friend ByteStream::Deserializer& operator>>(ByteStream::Deserializer& stream, Employee& e) {
        stream >> e.name >> e.address >> e.salary;
        return stream;
    }

  public:
    Employee() : salary(0) {}
    Employee(std::string n, Address a, double s) : name(n), address(a), salary(s) {}
    bool operator==(const Employee& other) const {
        return name == other.name && address == other.address && salary == other.salary;
    }
};

// --- Student with STL types ---
class Student {
    std::string name;
    std::vector<int> grades;
    std::map<std::string, int> scores;

    friend ByteStream::Serializer& operator<<(ByteStream::Serializer& stream, const Student& s) {
        stream << s.name << s.grades << s.scores;
        return stream;
    }
    friend ByteStream::Deserializer& operator>>(ByteStream::Deserializer& stream, Student& s) {
        stream >> s.name >> s.grades >> s.scores;
        return stream;
    }

  public:
    Student() {}
    Student(std::string n, std::vector<int> g, std::map<std::string, int> sc)
        : name(n), grades(g), scores(sc) {}
    bool operator==(const Student& other) const {
        return name == other.name && grades == other.grades && scores == other.scores;
    }
};

void testingPrimitiveTypes() {
    int x1 = 11, y1 = 555333, z1 = 3, x2, y2, z2;
    double d1 = 44.55, d2;
    float f1 = 1.2f, f2;
    bool b1 = true, b2;
    std::string s1 = "Abc", s2;
    enum A { AA, BB, CC, DD };
    A e1 = DD, e2;
    vector<int> v1 = {1, 2, 3, 4, 5, 6}, v2;
    std::array<int, 3> a1 = {1, 2, 3};
    std::array<int, 3> a2;
    std::pair<int, string> p1 = {6, "S"}, p2;
    std::optional<int> op1 = 10, op2;
    std::optional<float> op3, op4;
    std::tuple<int, double, std::string> t1(1, 3.14, "hello"), t2;
    std::unordered_map<int, string> unm1 = {{1, "test1"}, {2, "test2"}}, unm2;
    std::map<std::string, int> m1{{"CPU", 10}, {"GPU", 15}, {"RAM", 20}}, m2;
    std::set<int> set1{1, 5, 3}, set2;
    ByteStream::Serializer obs("testingPrimitiveTypes.txt");
    obs << x1 << y1 << d1 << f1 << b1 << s1 << e1 << v1 << a1 << p1 << op1 << unm1 << m1 << set1;
    obs.close();

    ByteStream::Deserializer ibs("testingPrimitiveTypes.txt");
    ibs >> x2 >> y2 >> d2 >> f2 >> b2 >> s2 >> e2 >> v2 >> a2 >> p2 >> op2 >> unm2 >> m2 >> set2;
    ibs.close();
    assert(x1 == x2);
    assert(y1 == y2);
    assert(d1 == d2);
    assert(f1 == f2);
    assert(b1 == b2);
    assert(s1 == s2);
    assert(e1 == e2);
    assert(v1 == v2);
    assert(a1 == a2);
    assert(p1 == p2);
    assert(op1 == op2);
    assert(op3 == op4);
    assert(unm1 == unm2);
    assert(m1 == m2);
    assert(set1 == set2);
}

void testNestedTypes() {
    std::vector<std::map<std::string, int>> n1 = {{{"a", 1}, {"b", 2}}, {{"x", 10}}}, n2;

    std::map<std::string, std::vector<int>> n3 = {{"first", {1, 2, 3}}, {"second", {4, 5}}}, n4;

    std::unordered_map<int, std::set<std::string>> n5 = {{1, {"a", "b"}}, {2, {"x", "y"}}}, n6;

    std::optional<std::map<int, std::string>> n7 = std::map<int, std::string>{{1, "one"},
                                                                              {2, "two"}},
                                              n8;
    std::optional<std::map<int, std::string>> n9, n10;
    std::tuple<std::vector<int>, std::map<std::string, int>, std::optional<std::string>>
        n11 = {{1, 2, 3}, {{"a", 1}, {"b", 2}}, "done"},
        n12;
    ByteStream::Serializer obs("testNestedTypes.txt");
    obs << n1 << n3 << n5 << n7 << n9;
    obs.close();
    ByteStream::Deserializer ibs("testNestedTypes.txt");
    ibs >> n2 >> n4 >> n6 >> n8 >> n10;
    ibs.close();
    assert(n1 == n2);
    assert(n3 == n4);
    assert(n5 == n6);
    assert(n7 == n8);
    assert(n9 == n10);
}

void testPrimitiveTypesTwo() {
    int i1 = 4, i2;
    int i3 = 0, i4;
    int i5 = -42, i6;

    long l1 = 123456789L, l2;
    long long ll1 = -9876543210LL, ll2;

    unsigned int ui1 = 42u, ui2;
    unsigned long ul1 = 99999ul, ul2;

    short sh1 = -12, sh2;
    unsigned short ush1 = 655, ush2;

    char c1 = 'A', c2;
    char c3 = '\n', c4;

    bool b1 = true, b2;
    bool b3 = false, b4;

    float f1 = 1.25f, f2;
    float f3 = 0.0f, f4;
    float f5 = -9.75f, f6;

    double d1 = 3.14159, d2;
    double d3 = 0.0, d4;
    double d5 = -123.456, d6;

    std::string s1 = "hello", s2;
    std::string s3 = "", s4;
    std::string s5 = "hello world", s6;
    std::string s7 = "  leading and trailing  ", s8;
    std::string s9 = "special_!@#$%^&*()", s10;

    ByteStream::Serializer obs("testPrimitiveTypesTwo.txt");
    obs << ui1 << ul1 << l1 << ll1 << i1 << i3 << i5 << sh1 << ush1 << c1 << c3 << b1 << b3 << f1
        << f3 << f5 << d1 << d3 << d5 << s1 << s3 << s5 << s7 << s9;
    obs.close();
    ByteStream::Deserializer ibs("testPrimitiveTypesTwo.txt");
    ibs >> ui2 >> ul2 >> l2 >> ll2 >> i2 >> i4 >> i6 >> sh2 >> ush2 >> c2 >> c4 >> b2 >> b4 >> f2 >>
        f4 >> f6 >> d2 >> d4 >> d6 >> s2 >> s4 >> s6 >> s8 >> s10;
    ibs.close();
    assert(i1 == i2);
    assert(i3 == i4);
    assert(i5 == i6);
    assert(l1 == l2);
    assert(ll1 == ll2);
    assert(ul1 == ul2);
    assert(ui1 == ui2);
    assert(sh1 == sh2);
    assert(ush1 == ush2);
    assert(c1 == c2);
    assert(c3 == c4);
    assert(b1 == b2);
    assert(b3 == b4);
    assert(f1 == f2);
    assert(f3 == f4);
    assert(f5 == f6);
    assert(d1 == d2);
    assert(d3 == d4);
    assert(d5 == d6);
    assert(s1 == s2);
    assert(s3 == s4);
    assert(s5 == s6);
    assert(s7 == s8);
    assert(s9 == s10);
}

void testEnumTypes() {
    enum BasicEnum { BA, BB, BC, BD };
    BasicEnum e1 = BD, e2;
    enum ExplicitEnum { EA = 10, EB = 20, EC = 30 };
    ExplicitEnum e3 = EB, e4;
    enum class ScopedEnum { SA, SB, SC };
    ScopedEnum e5 = ScopedEnum::SC, e6;
    enum class SmallEnum : char { XA = 1, XB = 2, XC = 3 };
    SmallEnum e7 = SmallEnum::XB, e8;
    ByteStream::Serializer obs("testEnumTypes.txt");
    obs << e1 << e3 << e5 << e7;
    obs.close();
    ByteStream::Deserializer ibs("testEnumTypes.txt");
    ibs >> e2 >> e4 >> e6 >> e8;
    ibs.close();
    assert(e1 == e2);
    assert(e1 == e2);
    assert(e1 == e2);
    assert(e1 == e2);
}

void testPairTypes() {
    std::pair<int, std::string> p1 = {7, "seven"}, p2;
    std::pair<std::string, int> p3 = {"age", 25}, p4;
    std::pair<int, std::vector<int>> p5 = {5, {1, 2, 3}}, p6;
    std::pair<std::string, std::optional<int>> p7 = {"maybe", 10}, p8;
    ByteStream::Serializer obs("text4.txt");
    obs << p1 << p3 << p5 << p7;
    obs.close();
    ByteStream::Deserializer ibs("text4.txt");
    ibs >> p2 >> p4 >> p6 >> p8;
    assert(p1 == p2);
    assert(p3 == p4);
    assert(p5 == p6);
    assert(p7 == p8);
}

void testOptionalTypes() {
    std::optional<int> op1 = 10, op2;
    std::optional<int> op3, op4;
    std::optional<float> op5 = 2.5f, op6;
    std::optional<float> op7, op8;
    std::optional<std::string> op9 = "hello", op10;
    std::optional<std::string> op11, op12;
    std::optional<std::vector<int>> op13 = std::vector<int>{1, 2, 3}, op14;
    std::optional<std::vector<int>> op15, op16;
    ByteStream::Serializer obs("testOptionalTypes.txt");
    obs << op1 << op3 << op5 << op7 << op9 << op11 << op13 << op15;
    obs.close();
    ByteStream::Deserializer ibs("testOptionalTypes.txt");
    ibs >> op2 >> op4 >> op6 >> op8 >> op10 >> op12 >> op14 >> op16;
    assert(op1 == op2);
    assert(op3 == op4);
    assert(op5 == op6);
    assert(op7 == op8);
    assert(op9 == op10);
    assert(op11 == op12);
    assert(op13 == op14);
    assert(op15 == op16);
}

void testSetTypes() {
    std::set<int> set1 = {1, 5, 3}, set2;
    std::set<int> set3 = {}, set4;
    std::set<int> set5 = {0, -1, 10, -5}, set6;
    std::set<std::string> set7 = {"aa", "bb", "cc"}, set8;
    std::set<std::vector<int>> set9 = {{1, 2}, {3, 4}}, set10;
    ByteStream::Serializer obs("testSetTypes.txt");
    obs << set1 << set3 << set5 << set7 << set9;
    obs.close();
    ByteStream::Deserializer ibs("testSetTypes.txt");
    ibs >> set2 >> set4 >> set6 >> set8 >> set10;
    assert(set1 == set2);
    assert(set3 == set4);
    assert(set5 == set6);
    assert(set7 == set8);
    assert(set9 == set10);
}

void testEdgeCases() {
    int i1 = std::numeric_limits<int>::min(), i2;
    int i3 = std::numeric_limits<int>::max(), i4;
    int i5 = 0, i6;
    int i7 = -1, i8;

    long long ll1 = std::numeric_limits<long long>::min(), ll2;
    long long ll3 = std::numeric_limits<long long>::max(), ll4;

    unsigned int ui1 = std::numeric_limits<unsigned int>::min(), ui2;
    unsigned int ui3 = std::numeric_limits<unsigned int>::max(), ui4;

    short sh1 = std::numeric_limits<short>::min(), sh2;
    short sh3 = std::numeric_limits<short>::max(), sh4;

    unsigned short ush1 = std::numeric_limits<unsigned short>::min(), ush2;
    unsigned short ush3 = std::numeric_limits<unsigned short>::max(), ush4;

    char c1 = '\0', c2;
    char c3 = '\t', c4;
    char c5 = ' ', c6;

    bool b1 = false, b2;
    bool b3 = true, b4;

    float f1 = -0.0f, f2;
    float f3 = std::numeric_limits<float>::lowest(), f4;
    float f5 = std::numeric_limits<float>::max(), f6;

    double d1 = -0.0, d2;
    double d3 = std::numeric_limits<double>::lowest(), d4;
    double d5 = std::numeric_limits<double>::max(), d6;

    std::string s1 = "", s2;
    std::string s3 = " ", s4;
    std::string s5 = "\n\t", s6;
    std::string s7(1000, 'x'), s8;

    enum class WeirdEnum : short { A = -10, B = 0, C = 32767 };
    WeirdEnum e1 = WeirdEnum::A, e2;
    WeirdEnum e3 = WeirdEnum::C, e4;

    std::vector<int> v1 = {}, v2;
    std::vector<int> v3 = {1, 1, 2, 2, 3, 3}, v4;
    std::vector<std::string> v5 = {"", "a", "aa"}, v6;
    std::vector<std::vector<int>> v7 = {{}, {1, 2}, {3}}, v8;

    std::array<int, 3> a1 = {0, -1, 1}, a2;
    std::array<std::string, 3> a3 = {"", "hello", "world"}, a4;

    std::pair<int, std::string> p1 = {0, ""}, p2;
    std::pair<std::string, int> p3 = {"edge", -42}, p4;
    std::pair<int, std::vector<int>> p5 = {5, {}}, p6;

    std::optional<int> op1, op2;
    std::optional<int> op3 = 0, op4;
    std::optional<std::string> op5 = "", op6;
    std::optional<std::vector<int>> op7 = std::vector<int>{}, op8;

    std::unordered_map<int, std::string> um1 = {}, um2;
    std::unordered_map<int, std::string> um3 = {{0, ""}, {1, "one"}}, um4;

    std::map<std::string, int> m1 = {}, m2;
    std::map<std::string, int> m3 = {{"", 0}, {"neg", -1}, {"big", 100}}, m4;

    std::set<int> set1 = {}, set2;
    std::set<int> set3 = {0, -1, 1, std::numeric_limits<int>::min(),
                          std::numeric_limits<int>::max()},
                  set4;
    std::set<std::string> set5 = {"", "a", "abc", "   "}, set6;

    ByteStream::Serializer obs("edge_cases.txt");
    obs << i1 << i3 << i5 << i7 << ll1 << ll3 << ui1 << ui3 << sh1 << sh3 << ush1 << ush3 << c1
        << c3 << c5 << b1 << b3 << f1 << f3 << f5 << d1 << d3 << d5 << s1 << s3 << s5 << s7 << e1
        << e3 << v1 << v3 << v5 << v7 << a1 << a3 << p1 << p3 << p5 << op1 << op3 << op5 << op7
        << um1 << um3 << m1 << m3 << set1 << set3 << set5;
    obs.close();

    ByteStream::Deserializer ibs("edge_cases.txt");
    ibs >> i2 >> i4 >> i6 >> i8 >> ll2 >> ll4 >> ui2 >> ui4 >> sh2 >> sh4 >> ush2 >> ush4 >> c2 >>
        c4 >> c6 >> b2 >> b4 >> f2 >> f4 >> f6 >> d2 >> d4 >> d6 >> s2 >> s4 >> s6 >> s8 >> e2 >>
        e4 >> v2 >> v4 >> v6 >> v8 >> a2 >> a4 >> p2 >> p4 >> p6 >> op2 >> op4 >> op6 >> op8 >>
        um2 >> um4 >> m2 >> m4 >> set2 >> set4 >> set6;
    ibs.close();

    assert(i1 == i2);
    assert(i3 == i4);
    assert(i5 == i6);
    assert(i7 == i8);
    assert(ll1 == ll2);
    assert(ll3 == ll4);
    assert(ui1 == ui2);
    assert(ui3 == ui4);
    assert(sh1 == sh2);
    assert(sh3 == sh4);
    assert(ush1 == ush2);
    assert(ush3 == ush4);
    assert(c1 == c2);
    assert(c3 == c4);
    assert(c5 == c6);
    assert(b1 == b2);
    assert(b3 == b4);
    assert(f1 == f2);
    assert(f3 == f4);
    assert(f5 == f6);
    assert(d1 == d2);
    assert(d3 == d4);
    assert(d5 == d6);
    assert(s1 == s2);
    assert(s3 == s4);
    assert(s5 == s6);
    assert(s7 == s8);
    assert(e1 == e2);
    assert(e3 == e4);
    assert(v1 == v2);
    assert(v3 == v4);
    assert(v5 == v6);
    assert(v7 == v8);
    assert(a1 == a2);
    assert(a3 == a4);
    assert(p1 == p2);
    assert(p3 == p4);
    assert(p5 == p6);
    assert(op1 == op2);
    assert(op3 == op4);
    assert(op5 == op6);
    assert(op7 == op8);
    assert(um1 == um2);
    assert(um3 == um4);
    assert(m1 == m2);
    assert(m3 == m4);
    assert(set1 == set2);
    assert(set3 == set4);
    assert(set5 == set6);
}

void testTupleTypes() {
    std::tuple<int, double, std::string> t1 = {1, 3.14, "hello"};
    std::tuple<int, double, std::string> t2; // empty, will be filled by deserialization

    ByteStream::Serializer obs("testTupleTypes.txt");
    obs << t1;
    obs.close();

    ByteStream::Deserializer ibs("testTupleTypes.txt");
    ibs >> t2;
    ibs.close();

    assert(t1 == t2);
}

void testClassesAndNested() {
    // --- Simple Person class ---
    // --- Test 1: Simple class ---
    Person p1(25, "Alice"), p2;
    ByteStream::Serializer obs1("test_class1.txt");
    obs1 << p1;
    obs1.close();
    ByteStream::Deserializer ibs1("test_class1.txt");
    ibs1 >> p2;
    ibs1.close();
    assert(p1 == p2);

    // --- Test 2: Class with STL types ---
    Student s1("Bob", {90, 85, 92}, {{"math", 95}, {"english", 88}}), s2;
    ByteStream::Serializer obs2("test_class2.txt");
    obs2 << s1;
    obs2.close();
    ByteStream::Deserializer ibs2("test_class2.txt");
    ibs2 >> s2;
    ibs2.close();
    assert(s1 == s2);

    // --- Test 3: Nested class ---
    Employee e1("Alice", Address("NYC", 10001), 95000.50), e2;
    ByteStream::Serializer obs3("test_class3.txt");
    obs3 << e1;
    obs3.close();
    ByteStream::Deserializer ibs3("test_class3.txt");
    ibs3 >> e2;
    ibs3.close();
    assert(e1 == e2);

    // --- Test 4: Vector of classes ---
    std::vector<Person> v1 = {Person(25, "Alice"), Person(30, "Bob"), Person(22, "Charlie")}, v2;
    ByteStream::Serializer obs4("test_class4.txt");
    obs4 << v1;
    obs4.close();
    ByteStream::Deserializer ibs4("test_class4.txt");
    ibs4 >> v2;
    ibs4.close();
    assert(v1 == v2);

    std::cout << "All class tests passed!" << std::endl;
}

int main() {
    testingPrimitiveTypes();
    testNestedTypes();
    testPrimitiveTypesTwo();
    testEnumTypes();
    testPairTypes();
    testOptionalTypes();
    testSetTypes();
    testEdgeCases();
    testTupleTypes();
    testClassesAndNested();
    return 0;
}