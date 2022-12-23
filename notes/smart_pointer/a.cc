#include <iostream>

/*

RAII(资源的获得即初始化) : 资源获取后立即放进管理对象; 管理对象通过析构函数保证资源被释放

这个机制利用栈和析构函数来管理内存(包括堆内存)

其原理是:函数返回后，移动栈顶指针释放掉栈帧, 此时编译器会自动对本地变量调用其析构函数。
即便是返回前程序触发了异常, 仍会调用析构函数。

利用RAII机制, 可以实现智能指针来包裹起裸指针,智能指针是栈上的本地变量, 从而安全使用
下面是一个RAII实现简易版智能指针的例子


*/


enum class shape_type {
    circle,
    triangle,
};

// 偷点懒省略了虚析构
class shape {};
class circle : public shape  {};
class triangle : public shape {};

shape* create_shape(shape_type type) 
{
    if (type == shape_type::circle) {
        return new circle();
    }
    else {
        return new triangle();
    }
};

class shape_wrapper {
public:
    explicit shape_wrapper(shape* ptr = nullptr) 
        : ptr_(ptr) {}
    ~shape_wrapper() {
        delete ptr_;
    }
private:
    shape* ptr_;
};

void foo() 
{
    // ptr_wrap是本地变量, foo返回时编译器会自动调用其析构函数
    // 从而安全使用create_shape()返回的裸指针
    shape_wrapper ptr_wrap(create_shape(shape_type::circle));
}

int main() 
{
    std::cout << "hello, modern c++" << std::endl;
    return 0; 
}

/*
RAII机制不仅可以用来管理内存, 还能关闭文件、释放锁等等
std::mutex mtx;
void some_func() 
{
    mtx.lock();
    ....
    ....
    mtx.unlock(); 
    上述是错误的,因为如果中间发生异常或者提前返回, 锁不会被释放
    
    应该利用RAII机制,用一个本地变量管理这个全局的锁
    std::lock_guard<std::mutex> guard(mtx);
}
*/

