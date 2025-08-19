#include <iostream>
#include <utility>
#include <algorithm>


    // Using a constexpr function to compute the sum
    constexpr int sum(int a, int b) {
        return a + b;
    }
int main(int argc, char* argv[]) {
    constexpr int x = 10;
    constexpr int y = 20;

    constexpr int result = sum(x, y);

    std::cout << "The sum of " << x << " and " << y << " is: " << result << std::endl;
    const int a = 10;
    constexpr int b = a;
    int c = 10;
    //constexpr int d = c; // This line would cause a compilation error because c is not a constant expression
    return 0;
}