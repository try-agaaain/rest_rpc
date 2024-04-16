#include <iostream>
#include <type_traits>

enum MyEnum {
    VALUE_1,
    VALUE_2,
    VALUE_3
};

// 针对枚举类型的特化
template <typename T>
typename std::enable_if<std::is_enum<T>::value, void>::type
print(T value) {
    std::cout << "Enum value: " << static_cast<int>(value) << '\n';
}

// 针对非枚举类型的特化
template <typename T>
typename std::enable_if<!std::is_enum<T>::value, void>::type
print(T value) {
    std::cout << "Non-enum value: " << value << '\n';
}
// g++ -g ./ab
int main() {
    print(MyEnum::VALUE_1);	// 输出 Enum value: 0
    print(123);	// 输出 Non-enum value: 123
  
    return 0;
}