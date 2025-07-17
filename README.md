# 实现事务机制
### 1：最原始的备忘录 (Manual State Backup)

- 目标： 不使用任何框架，手动为一个对象备份和恢复状态。理解最核心的需求。
- 组件： 一个简单的 Document 类，一个 main 函数。
- 需求：创建一个简单的文档对象，它有一个字符串内容。我们修改这个内容，然后能够手动把它恢复到修改之前的状态。

```c++
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
```



###  2：引入负责人 (The First DataLayer)

- 目标： 创建一个专门的类（KDataLayer 的雏形）来管理历史状态，实现一个简单的 Commit 和 Undo。
- 组件： KDataLayer_V1，Document。

###  3：支持多对象事务 (The Command Pattern)

- 目标： 让 DataLayer 能够在一个“事务”中记录来自多个对象的变化。
- 组件： 引入 DLCommand 结构体，改造 KDataLayer。

###  4：自动化备份 (The "Atom" Magic)

- 目标： 让对象在被修改时，能“自动”通知 DataLayer 并创建备份。这是整个框架的精髓所在。
- 组件： 引入 IKAtomData 接口、"元数据前置"技巧 (__impl 宏)、_kso_WriteLockAtom。

###  5：实现完整的 Undo/Redo

- 目标： 基于 DLCommand::Swap，实现真正能在新旧状态间来回切换的 Undo 和 Redo 功能。
- 组件： 完善 KDataLayer::UndoRedo。

###  6：高级功能与优化

- 目标： 加入 MergeWithLast, Rollback, 历史记录上限等功能，并引入像 DlVector 这样的复杂原子。
