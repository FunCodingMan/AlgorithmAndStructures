#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include <windows.h>
#include "MyTreeNode.hpp"
#include "FileTree.hpp"

using namespace std;

const int KEY_UP = 72;
const int KEY_DOWN = 80;
const int KEY_LEFT = 75;
const int KEY_RIGHT = 77;
const int KEY_ENTER = 13;
const int KEY_BACKSPACE = 8;
const int KEY_DELETE = 83;
const int KEY_ESC = 27;

struct UIContext 
{
    MyTreeNode* current;           
    size_t selectedIndex = 0;      
    MyTreeNode* clipboard = nullptr;
    bool isCutMode = false;        
    string status = "Готово.";     
    bool isRunning = true;
};

void clearConsole()
{
    system("cls");
}

string promptInput(const string& message)
{
    cout << "\n" << message;
    string input;
    getline(cin, input);
    return input;
}

MyTreeNode* getSelectedNode(UIContext& ctx)
{
    auto children = ctx.current->getChildren();
    if (children.empty() || ctx.selectedIndex >= children.size()) return nullptr;
    return children[ctx.selectedIndex];
}

bool isNodeInSubTree(MyTreeNode* root, MyTreeNode* node)
{
    if (!root || !node) return false;
    if (root == node) return true;

    for (MyTreeNode* child: root->getChildren())
    {
        if (isNodeInSubTree(child, node))
        {
            return true;
        }
    }
    return false;
}

void validateSelection(UIContext& ctx)
{
    size_t count = ctx.current->getChildren().size();
    if (count == 0) 
    {
        ctx.selectedIndex = 0;
    }
    else if (ctx.selectedIndex >= count)
    {
        ctx.selectedIndex = count - 1;
    }
}

void printInterface(UIContext& ctx)
{
    clearConsole();
    cout << "=====================================================================\n";
    cout << " ПУТЬ: " << ctx.current->getFullPath() << "\n";
    cout << "=====================================================================\n";

    auto children = ctx.current->getChildren();
    if (children.empty())
    {
        cout << "   (Папка пуста)\n";
    }
    else
    {
        for (size_t i = 0; i < children.size(); ++i)
        {
            cout << (i == ctx.selectedIndex ? " > " : "   ");
            cout << (children[i]->isFolder() ? "[DIR]  " : "       ");
            cout << children[i]->getName() << "\n";
        }
    }

    cout << "---------------------------------------------------------------------\n";
    cout << " Буфер: " << (ctx.clipboard ? ctx.clipboard->getName() : "пуст");
    cout << (ctx.clipboard ? (ctx.isCutMode ? " (ВЫРЕЗАН)" : " (СКОПИРОВАН)") : "") << "\n";
    cout << " Статус: " << ctx.status << "\n";
    cout << "---------------------------------------------------------------------\n";
    cout << " [Стрелки] Навигация | [Enter] Войти | [Backspace] Наверх \n";
    cout << " [C] Копировать | [X] Вырезать | [V] Вставить | [Del/D] Удалить\n";
    cout << " [M] Нов. Папка | [F] Нов. Файл | [R] Переименовать\n";
    cout << " [S] Сохранить | [L] Загрузить | [Esc/Q] Выход\n";
    cout << "=====================================================================\n";
}

void MoveCursorUp(UIContext& ctx)
{
    if (ctx.selectedIndex > 0)
    {
        ctx.selectedIndex--;
    }
    ctx.status = "";
}

void MoveCursorDown(UIContext& ctx)
{
    if (ctx.selectedIndex + 1 < ctx.current->getChildren().size())
    {
        ctx.selectedIndex++;
    }
    ctx.status = "";
}

void EnterDirectory(UIContext& ctx)
{
    MyTreeNode* target = getSelectedNode(ctx);
    if (target && target->isFolder())
    {
        ctx.current = target;
        ctx.selectedIndex = 0;
        ctx.status = "Переход выполнен.";
    }
    else
    {
        ctx.status = "Невозможно войти (это файл или пустая папка).";
    }
}

void GoUpDirectory(UIContext& ctx)
{
    if (ctx.current->getFather())
    {
        ctx.current = ctx.current->getFather();
        ctx.selectedIndex = 0;
        ctx.status = "Переход на уровень вверх.";
    }
    else
    {
        ctx.status = "Вы в корневом каталоге!";
    }
}

void SetClipboard(UIContext& ctx, bool cutMode)
{
    MyTreeNode* target = getSelectedNode(ctx);
    if (target)
    {
        ctx.clipboard = target;
        ctx.isCutMode = cutMode;
        ctx.status = cutMode ? "Объект вырезан." : "Объект скопирован.";
    }
}

void PasteClipboard(UIContext& ctx)
{
    if (!ctx.clipboard)
    {
        ctx.status = "Буфер пуст!";
        return;
    }

    if (ctx.isCutMode && isNodeInSubTree(ctx.clipboard, ctx.current))
    {
        ctx.status = "Нельзя вырезать папку в себя или своего потомка!";
    }

    if (ctx.current->findChild(ctx.clipboard->getName()))
    {
        ctx.status = "Объект с таким именем уже есть в папке!";
        return;
    }

    if (ctx.isCutMode)
    {
        MyTreeNode* father = ctx.clipboard->getFather();
        if (father) father->removeChild(ctx.clipboard);
        ctx.current->addChild(ctx.clipboard);
        
        ctx.clipboard = nullptr;
        ctx.status = "Успешно перемещено!";
    }
    else
    {
        ctx.current->addChild(ctx.clipboard->clone());
        ctx.status = "Успешно вставлена копия!";
    }
}

void CreateItem(UIContext& ctx, bool isFolder)
{
    string name = promptInput(isFolder ? "Имя новой папки: " : "Имя нового файла: ");
    if (name.empty()) return;

    if (ctx.current->findChild(name))
    {
        ctx.status = "Ошибка: Такое имя уже существует!";
    }
    else
    {
        auto type = isFolder ? MyTreeNode::NodeType::Folder : MyTreeNode::NodeType::File;
        ctx.current->addChild(new MyTreeNode(name, type));
        ctx.status = "Успешно создано!";
    }
}

void RenameItem(UIContext& ctx)
{
    MyTreeNode* target = getSelectedNode(ctx);
    if (!target) return;

    string newName = promptInput("Новое имя для '" + target->getName() + "': ");
    if (!newName.empty() && !ctx.current->findChild(newName))
    {
        target->setName(newName);
        ctx.status = "Переименовано.";
    }
    else
    {
        ctx.status = "Отмена или имя занято.";
    }
}

void DeleteItem(UIContext& ctx)
{
    MyTreeNode* target = getSelectedNode(ctx);
    if (!target) return;
    bool clipboardCleared = false;

    if (ctx.clipboard && isNodeInSubTree(target, ctx.clipboard))
    {
        ctx.clipboard = nullptr;
        ctx.isCutMode = false;
        clipboardCleared = true;
    }
    
    string name = target->getName();
    ctx.current->deleteChild(name);
    validateSelection(ctx);
    ctx.status = string(clipboardCleared ? "Буфер очищен. " : "") + "Удалено: " + name;
}

void SaveState(FileTree& tree, UIContext& ctx)
{
    string file = promptInput("Имя файла для сохранения: ");
    if (!file.empty() && tree.save(file)) ctx.status = "Дерево сохранено.";
    else ctx.status = "Ошибка сохранения.";
}

void LoadState(FileTree& tree, UIContext& ctx)
{
    string file = promptInput("Имя файла для загрузки: ");

    if (file.empty()) return;

    if (tree.load(file))
    {
        ctx.current = tree.getRoot();
        ctx.selectedIndex = 0;
        ctx.clipboard = nullptr;
        ctx.isCutMode = false;
        ctx.status = "Дерево загружено.";
    }
    else
    {
        ctx.status = "Ошибка загрузки. Старое дерево сохранено.";
    }
}

void CommandManager(FileTree& tree, UIContext& ctx, int key)
{
    switch (key)
    {
        case KEY_UP:        MoveCursorUp(ctx); break;
        case KEY_DOWN:      MoveCursorDown(ctx); break;
        case KEY_RIGHT:
        case KEY_ENTER:     EnterDirectory(ctx); break;
        case KEY_LEFT:
        case KEY_BACKSPACE: GoUpDirectory(ctx); break;
        case KEY_DELETE:
        case 'd':           DeleteItem(ctx); break;
        case 'c':           SetClipboard(ctx, false); break;
        case 'x':           SetClipboard(ctx, true); break;
        case 'v':           PasteClipboard(ctx); break;
        case 'm':           CreateItem(ctx, true); break;
        case 'f':           CreateItem(ctx, false); break;
        case 'r':           RenameItem(ctx); break;
        case 's':           SaveState(tree, ctx); break;
        case 'l':           LoadState(tree, ctx); break;
        case KEY_ESC:
        case 'q':           ctx.isRunning = false; break;
        default:            ctx.status = "Неизвестная команда."; break;
    }
}

void Loop()
{
    FileTree tree;
    tree.setRoot(new MyTreeNode("C:", MyTreeNode::NodeType::Folder));
    
    UIContext ctx;
    ctx.current = tree.getRoot();

    while (ctx.isRunning)
    {
        printInterface(ctx);

        int key = _getch();
        if (key == 0 || key == 224)
        {
            key = _getch();
        }
        else
        {
            key = tolower(key);
        }

        CommandManager(tree, ctx, key);
    }
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    
    Loop();
    return 0;
}