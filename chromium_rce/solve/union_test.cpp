#include <iostream>

class Test {
private:
    bool iscaca1_;
    bool iscaca2_;
    bool iscaca3_;
    bool iscaca4_;
    bool iscaca5_;
public:
    Test(bool iscaca1, bool iscaca2, bool iscaca3, bool iscaca4, bool iscaca5) : iscaca1_(iscaca1), iscaca2_(iscaca2), iscaca3_(iscaca3), iscaca4_(iscaca4), iscaca5_(iscaca5) {
        std::cout << "Test constructor" << std::endl;
    }
};


int main() {
    // Test *test = new Test(false, true, false, true, false);
    std::cout << sizeof(size_t) << std::endl;
}