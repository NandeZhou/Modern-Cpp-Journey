#include <iostream>
#include <memory>

template<typename T, typename Deleter = std::default_delete<T>>
class SUniquePtr
{
public:
    // 构造函数
    explicit SUniquePtr(T* ptr = nullptr, Deleter deleter = Deleter()) 
        : ptr_(ptr), deleter_(deleter) {}

    // 禁用拷贝构造和赋值
    SUniquePtr(const SUniquePtr&) = delete;
    SUniquePtr& operator=(const SUniquePtr&) = delete;

    // 允许移动构造和赋值
    SUniquePtr(SUniquePtr&& other) noexcept 
        : ptr_(other.ptr_), deleter_(std::move(other.deleter_)) {
        other.ptr_ = nullptr;
    }
    
    SUniquePtr& operator=(SUniquePtr&& other) noexcept {
        if (this != &other) {
            reset();
            ptr_ = other.ptr_;
            deleter_ = std::move(other.deleter_);
            other.ptr_ = nullptr;
        }
        return *this;
    }

    // 析构函数
    ~SUniquePtr() {
        reset();
    }

    // 重置指针
    void reset(T* ptr = nullptr) {
        if (ptr_) {
            deleter_(ptr_);
        }
        ptr_ = ptr;
    }

    // 重载解引用运算符
    T& operator*() const {
        return *ptr_;
    }

    // 重载箭头运算符
    T* operator->() const {
        return ptr_;
    }

private:
    T* ptr_; // 原始指针
    Deleter deleter_; // 自定义删除器
};

template<typename T, typename Deleter = std::default_delete<T>> 
class SSharedPtr
{
public:
    // 构造函数
    explicit SSharedPtr(T* ptr = nullptr, Deleter deleter = Deleter()) 
        : ptr_(ptr), ref_count_(new size_t(1)), deleter_(deleter) {}

    // 拷贝构造函数
    SSharedPtr(const SSharedPtr& other) 
        : ptr_(other.ptr_), ref_count_(other.ref_count_), deleter_(other.deleter_) {
        if (ref_count_) {
            ++(*ref_count_);
        }
    }

    // 拷贝赋值运算符
    SSharedPtr& operator=(const SSharedPtr& other) {
        if (this != &other) {
            reset();
            ptr_ = other.ptr_;
            ref_count_ = other.ref_count_;
            deleter_ = other.deleter_;
            if (ref_count_) {
                ++(*ref_count_);
            }
        }
        return *this;
    }

    // 移动构造函数
    SSharedPtr(SSharedPtr&& other) noexcept 
        : ptr_(other.ptr_), ref_count_(other.ref_count_), deleter_(std::move(other.deleter_)) {
        other.ptr_ = nullptr;
        other.ref_count_ = nullptr;
    }
    
    // 移动赋值运算符
    SSharedPtr& operator=(SSharedPtr&& other) noexcept {
        if (this != &other) {
            reset();
            ptr_ = other.ptr_;
            ref_count_ = other.ref_count_;
            deleter_ = std::move(other.deleter_);
            other.ptr_ = nullptr;
            other.ref_count_ = nullptr;
        }
        return *this;
    }

    // 析构函数
    ~SSharedPtr() {
        reset();
    }

    // 重置指针
    void reset(T* ptr = nullptr) {
        if (ref_count_ && --(*ref_count_) == 0) {
            deleter_(ptr_);
            delete ref_count_;
        }
        ptr_ = ptr;
        ref_count_ = ptr ? new size_t(1) : nullptr;
    }

    // 重载解引用运算符
    T& operator*() const {
        return *ptr_;
    }

    // 重载箭头运算符
    T* operator->() const {
        return ptr_;
    }

private:
    T* ptr_; // 原始指针
    size_t* ref_count_; // 引用计数
    Deleter deleter_; // 自定义删除器
};


