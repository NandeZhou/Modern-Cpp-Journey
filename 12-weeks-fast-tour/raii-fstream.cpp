#include <fstream>
#include <iostream>
#include <string>

class RaiiFStream {
public:
    // 构造函数：根据模式打开文件
    RaiiFStream(const std::string& filename, std::ios::openmode mode = std::ios::in | std::ios::out)
        : file(filename, mode) {
        if (!file.is_open()) {
            throw std::runtime_error("Error opening file: " + filename);
        }
        std::cout << "File opened: " << filename << "\n";
    }

    // 析构函数：自动关闭文件
    ~RaiiFStream() {
        if (file.is_open()) {
            file.close();
            std::cout << "File closed.\n";
        }
    }

    // 禁用拷贝构造 & 赋值
    RaiiFStream(const RaiiFStream&) = delete;
    RaiiFStream& operator=(const RaiiFStream&) = delete;

    // 允许移动构造 & 赋值
    RaiiFStream(RaiiFStream&& other) noexcept : file(std::move(other.file)) {}
    RaiiFStream& operator=(RaiiFStream&& other) noexcept {
        if (this != &other) {
            file = std::move(other.file);
        }
        return *this;
    }

    // 简单接口：读取一行
    std::string readLine() {
        std::string line;
        if (std::getline(file, line)) {
            return line;
        }
        return {};
    }

    // 简单接口：写一行
    void writeLine(const std::string& text) {
        file << text << "\n";
    }

private:
    std::fstream file;
};

// -------------------- 测试 --------------------
int main() {
    try {
        // 写文件
        {
            RaiiFStream writer("example.txt", std::ios::out);
            writer.writeLine("Hello RAII!");
            writer.writeLine("This is a test.");
        } // writer 离开作用域 → 自动关闭

        // 读文件
        {
            RaiiFStream reader("example.txt", std::ios::in);
            std::string line;
            while (!(line = reader.readLine()).empty()) {
                std::cout << "Read: " << line << "\n";
            }
        } // reader 离开作用域 → 自动关闭
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << "\n";
    }

    return 0;
}
