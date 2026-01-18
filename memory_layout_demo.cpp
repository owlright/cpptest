#include <cstdlib>
#include <iostream>

// ======================
// 全局 / 静态区
// ======================

// .data（已初始化）
int global_init = 42;

// .bss（未初始化）
int global_uninit;

// .rodata（只读数据）
const char* global_str = "Hello .rodata";

// .data（const 但可取地址，注意区别）
const int global_const_int = 100;

// ======================
// 函数（.text）
// ======================
int add(int a, int b) { return a + b; }

int main() {
    // ======================
    // 栈（stack）
    // ======================
    int local_var = 10;

    // ======================
    // 堆（heap）
    // ======================
    int* heap_var = new int(20);

    std::cout << "Addresses:\n";

    std::cout << "add (text)             : " << (void*)&add << "\n";
    std::cout << "global_init (.data)    : " << (void*)&global_init << "\n";
    std::cout << "global_uninit (.bss)   : " << (void*)&global_uninit << "\n";
    std::cout << "global_str (.rodata)   : " << (void*)global_str << "\n";
    std::cout << "global_const_int       : " << (void*)&global_const_int
              << "\n";
    std::cout << "heap_var (heap)        : " << (void*)heap_var << "\n";
    std::cout << "local_var (stack)      : " << (void*)&local_var << "\n";

    delete heap_var;
    return 0;
}
