# 一个基于备忘录模式的通用事务与撤销重做(Undo/Redo)框架
### 核心作用：

想象一下你在使用WPS写文档、做表格或者PPT。你输入了一段文字，然后又设置了字体为“红色、粗体”，然后又插入了一张图片。这时你发现图片插错了，想撤销。你按下了 Ctrl+Z，图片消失了。你再按一下，字体变回了原来的样子。再按一下，刚输入的文字也没了。这就是**撤销(Undo)**。然后你又觉得刚才的操作没错，你按下了 Ctrl+Y，文字又回来了，字体又变红了，图片也再次出现了。这就是**重做(Redo)**。

本文构建的，就是支撑这个 Ctrl+Z / Ctrl+Y 功能的**底层核心框架**。这个框架不关心你是在修改文字、单元格颜色还是幻灯片布局，它提供一套通用的机制，让任何类型的对象（后面称之为“原子”）都能轻松地接入，从而获得撤销和重做的能力。

### 核心功能：

1. 事务管理 (Transaction): 将一连串的修改操作打包成一个逻辑单元。比如“设置单元格背景为红色并加粗”可以是一个事务。
2. 状态备份 (Memento): 在对象被修改时，能自动保存其修改前的状态。
3. 撤销 (Undo): 将所有被修改的对象恢复到上一个事务提交后的状态。
4. 重做 (Redo): 在执行撤销后，能重新执行之前的修改，恢复到撤销前的状态。
5. 历史管理 (History): 管理一个修改历史记录的链表，并允许设置历史记录的步数上限。

### 程序设计知识：

- 核心：设计模式 - 备忘录模式 (Memento Pattern): 这是整个框架的灵魂。
- 核心：设计模式 - 命令模式 (Command Pattern): DLCommand 不仅是备忘录，也扮演了命令的角色，封装了“如何执行恢复”这一动作。
- 软件设计原则：
  - 单一职责原则： KDataLayer 只负责管理历史，IKAtomData 只负责自身业务和状态备份。
  - 开闭原则： 框架设计好后，可以轻松添加新的“原子”类型（如DemoTableAtom, DemoChartAtom），而无需修改KDataLayer的核心代码。

### 1：最原始的备忘录 (Manual State Backup)

#### 需求

正在开发一个最简单的文本编辑器。用户现在只有一个核心需求：“我输入了一段文字，如果我后悔了，希望能撤销这次输入，回到之前的样子。”

- 设计思路

  - 谁是需要被备份状态的对象？(Originator - 发起人)

    - 是我们的文档对象。它持有需要被记住的数据（文本内容）。我们叫它 Document。

    - 备份出来的状态应该是什么样子的？(Memento - 备忘录)
      - 最简单的，就是一份文档内容的拷贝。在这里，就是一个 std::string。

  - 谁来负责保管这份备份？(Caretaker - 负责人)
    - 在最简单的场景下，我们的主程序（main函数）就可以临时充当这个角色。它在修改前，向 Document 索要一个“备忘录”，然后保管起来。当需要撤销时，再把这个“备忘录”还给 Document，让它自己恢复。

- 组件： 一个简单的 Document 类，一个 main 函数。
- 需求-开发：创建一个简单的文档对象，它有一个字符串内容。我们修改这个内容，然后能够手动把它恢复到修改之前的状态。

```c++
#include <iostream>
#include <string>

// Originator: 需要被备份状态的对象
class Document {
public:
    std::string content;
    // 创建一个备忘录 (Memento)，包含当前需要保存的状态
    std::string createMemento() const {
        return content;
    }
    // 从备忘录中恢复状态
    void restoreFromMemento(const std::string& memento) {
        content = memento;
    }
    void print() const {
        std::cout << "Document content: \"" << content << "\"" << std::endl;
    }
};

int main() {
    // Caretaker (负责人) 的角色由 main 函数扮演

    // 1. 创建 Originator
    Document doc;
    doc.content = "Version 1";
    std::cout << "--- Initial State ---" << std::endl;
    doc.print();

    // 2. Caretaker 决定要进行一个危险操作(修改)，于是在操作前，向 Originator 索要一个 Memento
    std::cout << "\nCaretaker: 'I'm about to change you, Document! Give me a memento first.'" << std::endl;
    std::string memento = doc.createMemento(); // 得到备忘录

    // 3. Originator 的状态被改变
    doc.content = "Version 2";
    std::cout << "\n--- State Modified ---" << std::endl;
    doc.print();

    // 4. 用户请求撤销。Caretaker 将之前保管的 Memento 还给 Originator
    std::cout << "\nCaretaker: 'Undo! Document, restore yourself using this memento.'" << std::endl;
    doc.restoreFromMemento(memento);
    std::cout << "\n--- State Restored ---" << std::endl;
    doc.print();

    return 0;
}
```

#### 局限性

局限1：

无法实现多步撤销。 main 函数里只用了一个 std::string memento 变量，它只能保存一次备份。如果用户连续修改了10次，我们想撤销3次，怎么办？

局限2：

负责人职责不明确。 现在是 main 函数在做管理，这很乱。如果程序变复杂，到处都是这种备份逻辑，代码将无法维护。我们需要一个专门的“历史管理员”。

局限3：

备忘录暴露了内部细节。 Document 的备忘录是 std::string，main 函数知道得一清二楚。这破坏了封装。理想情况下，Caretaker 只应该把 Memento 当成一个不透明的“令牌”，它只负责传递，而不关心里面是什么。





###  2：引入负责人 (The First DataLayer)

#### 需求

希望能像真正的编辑器一样，连续撤销好几步操作，而不是只能撤销一次。

**目标：**

1. 创建一个专门的类来管理历史记录。
2. 实现多步 Undo 功能。

**设计思路 (备忘录模式的演进):**

1. **Caretaker (负责人) 的进化：** 我们不再让 main 函数承担保管责任。我们将创建一个新的类，就叫 HistoryManager。它的核心职责就是维护一个“备忘录列表”。
2. **Memento (备忘录) 的容器：** HistoryManager 内部需要一个容器来存储一系列的备忘录。std::vector 是一个非常自然的选择。
3. **Undo 逻辑：** 当需要 Undo 时，HistoryManager 从它的列表中取出**最后一个**备忘录，并用它来恢复 Document 的状态，然后将这个备忘录从列表中移除。

```c++
#include <iostream>
#include <string>
#include <vector> // 需要 vector 来存储历史记录

// Originator: 需要被备份状态的对象 (和之前一样，没有变化)
class Document {
public:
    std::string content;

    // 创建一个备忘录 (Memento)，包含当前需要保存的状态
    std::string createMemento() const {
        return content;
    }

    // 从备忘录中恢复状态
    void restoreFromMemento(const std::string& memento) {
        content = memento;
    }

    void print() const {
        std::cout << "Document content: \"" << content << "\"" << std::endl;
    }
};

// Caretaker: 负责人，现在是一个专门的类
class HistoryManager {
private:
    Document& m_doc; // 负责人需要知道它在为哪个文档服务
    std::vector<std::string> m_history; // 用一个 vector 来存储备忘录列表

public:
    // 构造函数，传入它需要管理的文档
    HistoryManager(Document& doc) : m_doc(doc) {}

    // 备份当前状态
    void backup() {
        std::cout << "HistoryManager: Backing up current state..." << std::endl;
        m_history.push_back(m_doc.createMemento());
    }

    // 撤销操作
    void undo() {
        if (m_history.empty()) {
            std::cout << "HistoryManager: No history to undo." << std::endl;
            return;
        }

        // 1. 从历史记录中取出最后一个备忘录
        std::string lastMemento = m_history.back();
        m_history.pop_back(); // 从历史记录中移除

        // 2. 用这个备忘录恢复文档
        std::cout << "HistoryManager: Undoing..." << std::endl;
        m_doc.restoreFromMemento(lastMemento);
    }
};


int main() {
    // 1. 创建 Originator 和 Caretaker
    Document doc;
    HistoryManager history(doc);

    // --- 流程开始 ---
    // 操作 1: 设置初始内容
    doc.content = "Version 1";
    history.backup(); // 备份 V1
    doc.print();

    // 操作 2: 修改内容
    doc.content = "Version 2";
    history.backup(); // 备份 V2
    doc.print();

    // 操作 3: 再次修改内容
    doc.content = "Version 3";
    // 注意：这里我们不备份 V3，因为我们即将修改它
    // 历史记录里现在有 [V1, V2]
    std::cout << "\n--- Current State ---" << std::endl;
    doc.print();


    // --- 开始撤销 ---
    std::cout << "\n--- Start Undoing ---" << std::endl;

    // 第一次撤销：应该恢复到 V2
    history.undo();
    doc.print(); // 期望输出 "Version 2"

    // 第二次撤销：应该恢复到 V1
    history.undo();
    doc.print(); // 期望输出 "Version 1"

    // 第三次撤销：历史记录已空
    history.undo();
    doc.print(); // 期望输出 "Version 1"，并提示没有历史记录

    return 0;
}
```

#### 局限性

局限1：

没有 Redo (重做) 功能。 在 undo() 方法中，我们直接用 pop_back() 把备忘录丢弃了。一旦撤销，就再也回不去了。

局限2：

一个负责人只能管一个对象。 HistoryManager 在构造时就绑定了一个 Document。如果我们的应用有 Document 和 StyleSheet 两个对象，我想把它们俩的修改（比如，改了文字，又改了颜色）看作一个操作来一起撤销，现在的架构做不到。

局限3：

备份时机是手动的。 我们仍然需要在每次修改后，手动调用 history.backup()。这很麻烦且容易出错。如果程序员忘了调用，撤销功能就会出 bug。

###  3：支持多对象事务 (The Command Pattern)

#### 需求

目标： 让 DataLayer 能够在一个“事务”中记录来自多个对象的变化。

- HistoryManager 不需要知道它管理的是 Document 还是 Style
- 备忘录 Memento 本身也存在类型问题（Document 的是 string，Style 的可能是个结构体）
- 引入一个中间层：**命令（Command）**。

实现redo

- 不再用 pop_back 删除历史记录。引入一个整数 m_currentVersion 作为游标，指向历史列表中的当前状态
- **Undo**：游标向前移动 (--m_currentVersion)，然后执行该位置命令的 undo。
- **Redo**：游标向后移动 (++m_currentVersion)，然后执行该位置命令的 execute (或 redo)。
- **新操作**：如果在游标之后还有历史记录（意味着用户先 undo 了几次），此时再执行一个新操作，那么游标之后的所有历史记录都将被清除。

使用，**接口**、**多态**、**命令模式**

#### document

```c++
// document.h
#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <iostream>
#include <string>

// Originator: 我们的文档类
class Document {
public:
    std::string content;
    void print() const {
        std::cout << "Document content: \"" << content << "\"" << std::endl;
    }
};

#endif // DOCUMENT_H
```

#### command.h

```c++
// command.h
#ifndef COMMAND_H
#define COMMAND_H

#include "document.h" // 需要知道 Document 类的定义
#include <string>
#include <memory>

// Command 接口: 定义了所有命令必须实现的行为
class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0; // 执行/重做
    virtual void unexecute() = 0; // 撤销
};

// ConcreteCommand: 修改文档内容的具体命令
class ChangeContentCommand : public Command {
private:
    Document& m_doc;
    std::string m_oldContent;
    std::string m_newContent;

public:
    ChangeContentCommand(Document& doc, const std::string& newContent);

    void execute() override;
    void unexecute() override;
};

#endif // COMMAND_H
```

#### **command.cpp**

```C++
// command.cpp
#include "command.h"

ChangeContentCommand::ChangeContentCommand(Document& doc, const std::string& newContent)
    : m_doc(doc), m_newContent(newContent) {
    // 在命令创建时，就保存好旧状态
    m_oldContent = doc.content;
}

// 执行/重做：将文档内容设置为新内容
void ChangeContentCommand::execute() {
    m_doc.content = m_newContent;
}

// 撤销：将文档内容恢复为旧内容
void ChangeContentCommand::unexecute() {
    m_doc.content = m_oldContent;
}
```

#### **datalayer.h**

```c++
// datalayer.h
#ifndef DATALAYER_H
#define DATALAYER_H

#include <vector>
#include <memory>

// 前向声明 Command，避免在头文件中引入 command.h
// 这是一个优化，因为 DataLayer 的接口只需要知道 Command 是一个类型，
// 而不需要知道它的具体实现。这可以减少编译依赖。
class Command;

// 通用数据层，我们的 Undo/Redo 管理器
class DataLayer {
private:
    std::vector<std::shared_ptr<Command>> m_history;
    int m_currentVersion; // 游标

public:
    DataLayer(); // 构造函数

    void addCommand(std::shared_ptr<Command> command);
    void undo();
    void redo();
};

#endif // DATALAYER_H
```

####  **datalayer.cpp**

```c++
// datalayer.cpp
#include "datalayer.h"
#include "command.h" // 在这里才需要 Command 的完整定义
#include <iostream>

DataLayer::DataLayer() : m_currentVersion(-1) {}

void DataLayer::addCommand(std::shared_ptr<Command> command) {
    if (m_currentVersion + 1 < m_history.size()) {
        m_history.resize(m_currentVersion + 1);
    }
    command->execute();
    m_history.push_back(command);
    m_currentVersion++;
    std::cout << "DataLayer: Command executed and added. History size: "
              << m_history.size() << ", Version: " << m_currentVersion << std::endl;
}

void DataLayer::undo() {
    if (m_currentVersion < 0) {
        std::cout << "DataLayer: Cannot undo. At initial state." << std::endl;
        return;
    }
    std::cout << "DataLayer: Undoing..." << std::endl;
    m_history[m_currentVersion]->unexecute();
    m_currentVersion--;
}

void DataLayer::redo() {
    if (m_currentVersion + 1 >= m_history.size()) {
        std::cout << "DataLayer: Cannot redo. At latest state." << std::endl;
        return;
    }
    std::cout << "DataLayer: Redoing..." << std::endl;
    m_currentVersion++;
    m_history[m_currentVersion]->execute();
}
```

#### **main.cpp**

```C++
// main.cpp
#include "datalayer.h"
#include "command.h"
#include "document.h"

// Helper function to create and add command
void changeContent(DataLayer& dl, Document& doc, const std::string& newContent) {
    auto cmd = std::make_shared<ChangeContentCommand>(doc, newContent);
    dl.addCommand(cmd);
}

int main() {
    DataLayer dataLayer;
    Document doc;
    doc.content = "Initial Version";
    std::cout << "--- Start ---" << std::endl;
    doc.print();

    // 操作1: 修改为 Version 1
    std::cout << "\n--- Action 1 ---" << std::endl;
    changeContent(dataLayer, doc, "Version 1");
    doc.print();

    // 操作2: 修改为 Version 2
    std::cout << "\n--- Action 2 ---" << std::endl;
    changeContent(dataLayer, doc, "Version 2");
    doc.print();

    // --- 开始撤销/重做 ---
    std::cout << "\n--- Testing Undo/Redo ---" << std::endl;
    
    // Undo 1: 恢复到 V1
    dataLayer.undo();
    doc.print();

    // Undo 2: 恢复到 Initial
    dataLayer.undo();
    doc.print();

    // Redo 1: 前进到 V1
    dataLayer.redo();
    doc.print();

    // 此时历史状态是 V1，我们执行一个新操作
    std::cout << "\n--- New Action after Undo ---" << std::endl;
    changeContent(dataLayer, doc, "Version 3 (new branch)");
    doc.print();

    // 尝试 Redo: 应该失败，因为 V2 的历史已被覆盖
    std::cout << "\n--- Trying to Redo to V2 ---" << std::endl;
    dataLayer.redo();
    doc.print();

    return 0;
}
```

#### 代码分析

通过创建文档对象 Document 管理原始数据内容。然后创建datalayer对象管理命令ChangeContentCommand。将每一次对文档的操作都抽象为命令ChangeContentCommand。这样撤销与重做的行为就变成了执行这条命令或是执行上一条命令。通过将每次对文档的操作都作为命令存储，完成了撤销与重做功能的实现

#### 局限性

为了修改一下 Document 的内容，我们不能直接写 doc.content = "新内容";。而是必须“规规矩矩”地调用一个辅助函数 changeContent(...)，在这个函数里，我们手动地 std::make_shared<ChangeContentCommand>(...) 创建了一个命令对象，然后手动地 dl.addCommand(cmd) 把它交给数据层。

Document 对象和 DataLayer 对象是完全独立的两个实体。它们之间没有任何内在的联系。只有在 main 函数里，我们才把它们“撮合”到一起。

如果一个 Document 对象被多个 DataLayer 管理怎么办？或者更重要的是，如果一个对象在创建时就注定要被某个 DataLayer 管理，为什么不直接让它们“绑定”在一起？

###  4：自动化备份 (The "Atom" Magic)

- 目标： 让对象在被修改时，能“自动”通知 DataLayer 并创建备份。这是整个框架的精髓所在。
- 组件： 引入 IKAtomData 接口、"元数据前置"技巧 (__impl 宏)、_kso_WriteLockAtom。

###  5：实现完整的 Undo/Redo

- 目标： 基于 DLCommand::Swap，实现真正能在新旧状态间来回切换的 Undo 和 Redo 功能。
- 组件： 完善 KDataLayer::UndoRedo。

###  6：高级功能与优化

- 目标： 加入 MergeWithLast, Rollback, 历史记录上限等功能，并引入像 DlVector 这样的复杂原子。
