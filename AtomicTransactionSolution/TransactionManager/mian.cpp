/**
 * @author： Karl
 * @date：   2025年7月18日
 *
 * 事务处理demo
 */

#include <iostream>
#include <string>
#include <vector>

// 这是一个简单的文档类，代表了关心的对象 (Originator)
class SimpleDocument {
public:
    std::string content;

    void print() const {
        std::cout << "Document content: \"" << content << "\"" << std::endl;
    }
};

int main() {
    // 1. 创建一个文档对象，并设置初始状态
    SimpleDocument doc;
    doc.content = "Hello, World!";
    std::cout << "--- Initial State ---" << std::endl;
    doc.print();

    // 2. 修改前，先手动备份它的状态
    //    这里的 backup_content 就是最原始的 "Memento" (备忘录)
    std::string backup_content = doc.content;
    std::cout << "\n...State saved manually." << std::endl;

    // 3. 修改文档内容
    doc.content = "Hello, WPS DataLayer!";
    std::cout << "\n--- Modified State ---" << std::endl;
    doc.print();

    // 4. 实现 "Undo" 这个操作
    //    我们手动从备份中恢复状态
    doc.content = backup_content;
    std::cout << "\n--- After Manual Undo ---" << std::endl;
    doc.print();

    return 0;
}