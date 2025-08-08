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
