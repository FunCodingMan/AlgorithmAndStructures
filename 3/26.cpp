/*
Условие: Информация  о  файлах  на  жестких  дисках   компьютера
записана  с  помощью  дерева.  Обеспечить выполнение следующих
операций:
   1) загрузку дерева в память из файла;
   2) обход дерева папок в  режиме  диалога  (раскрытие папок,
      подъем на уровень и т. п.); 
   3) корректировку  дерева при создании новых папок и файлов,
их переименовании, копировании, переносе и удалении. 
   4) сохранение дерева в файле (13).

Автор: Камалов Данил Маратович

Среда выполнения: Visual Studio Code, g++

Источники информации: https://coddy.tech/docs/ru/cpp
*/

#include <iostream>
#include <sstream>
#include <string>
#include <windows.h>
#include "MyTreeNode.hpp"
#include "FileTree.hpp"

using namespace std;

void printHelpInformation()
{
	cout << "Лабораторная работа. Задание 26 (Файловый менеджер).\n";
    cout << "Команды:\n";
    cout << "  ls        - показать содержимое текущей папки\n";
    cout << "  tree      - показать дерево файлов\n";
    cout << "  cd <имя>  - перейти в папку (cd .. для возврата)\n";
    cout << "  mkdir <и> - создать папку\n";
    cout << "  touch <и> - создать файл\n";
    cout << "  rm <имя>  - удалить файл/папку\n";
    cout << "  rename <стар> <нов> - переименовать\n";
    cout << "  cp <что> <куда> - копировать\n";
    cout << "  mv <что> <куда> - переместить\n";
    cout << "  save <ф>  - сохранить в файл\n";
    cout << "  load <ф>  - загрузить из файла\n";
    cout << "  exit      - выход\n\n";
}

void printListOfChildrens(MyTreeNode* node)
{
	for (MyTreeNode* child : node->getChildren())
	{
		if (child->isFolder())
		{
			cout << "  <DIR>  ";
		}
		else
		{
			cout << "         ";
		}
		cout << child->getName() << endl;
	}
}

vector<string> splitByChar(const string& str, char splitter)
{
	vector<string> tokens;
	
	string token = "";
	for (char c: str)
	{
		if (c == splitter)
		{
			tokens.push_back(token);
			token = "";
		}
		else
		{
			token += c;
		}
	}
	if (!token.empty())
	{
		tokens.push_back(token);
	}
	return tokens;
}

MyTreeNode* recFindChild(MyTreeNode* node, const string& arg)
{
	vector<string> tokens = splitByChar(arg, '/');
	MyTreeNode* current = node;
	for (string token : tokens)
	{
		MyTreeNode* target = token == ".." ? current->getFather() : current->findChild(token);
		if (target)
		{
			current = target;
		}
		else
		{
			return nullptr;
		}
	}
	return current;
}

void ChangeDirectory(MyTreeNode*& node, const string& arg)
{
	if (arg == "..")
	{
		if (node->getFather())
		{
			node = node->getFather();
		}
	}
	else
	{
		MyTreeNode* temp = recFindChild(node, arg);
		if (temp->isFolder())
		{
			node = temp;
		}
		else
		{
			cout << "Папки '" << arg << "' не найдено!\n";
		}
	}
}

void CreateDirectory(MyTreeNode*& node, const string& arg)
{
	if (arg.empty())
	{
		cout << "Укажите имя папки!\n";
		return;
	}
	if (node->findChild(arg))
	{
		cout << "Папка с таким именем уже существует!\n";
		return;
	}
	node->addChild(new MyTreeNode(arg, MyTreeNode::NodeType::Folder));
}

void CreateFile(MyTreeNode*& node, const string& arg)
{
	if (arg.empty())
	{
		cout << "Укажите имя файла!\n";
		return;
	}
	if (node->findChild(arg))
	{
		cout << "Файл с таким именем уже существует!\n";
		return;
	}
	node->addChild(new MyTreeNode(arg, MyTreeNode::NodeType::File));
}

void Remove(MyTreeNode*& node, const string& arg)
{
	if (arg.empty())
	{
		cout << "Укажите имя!\n";
		return;
	}
	if (!node->deleteChild(arg))
	{
		cout << "Не найдено '" << arg << "' !\n";
	}
}

void Rename(MyTreeNode*& node, const string& oldName, const string& newName)
{
	if (oldName.empty() || newName.empty())
	{
		cout << "Укажите изменямое имя и новое имя файла или директории!\n";
		cout << "Usage: rename <изменямое имя> <новое имя>\n";
		return;
	}
	MyTreeNode* target = node->findChild(oldName);
	if (!target)
	{
		cout << "'" << oldName << "' не найдено!\n";
		return;
	}
	if (node->findChild(newName))
	{
		cout << "'" << newName << "' уже существует! Имя занято, попробуйте другое.\n";
		return;
	}
	target->setName(newName);
}

bool isCapableToMoveOrCopy(MyTreeNode* src, MyTreeNode* dest, const string& srcName, const string& destName)
{
	if (src == nullptr)
	{
		cout << "'" << srcName << "' не найдено!\n";
		return false;
	}
	if (dest == nullptr)
	{
		cout << "'" << destName << "' не найдено!\n";
		return false;
	}
	if (!dest->isFolder())
	{
		cout << "'" << destName << "' не является папкой!\n";
		return false;
	}
	return true;
}

void Copy(MyTreeNode*& node, const string& srcName, const string& destName)
{
	MyTreeNode* src = recFindChild(node, srcName);
	MyTreeNode* dest = recFindChild(node, destName);

	if (!isCapableToMoveOrCopy(src, dest, srcName, destName))
	{
		return;
	}
	dest->addChild(src->clone());
}

void Move(MyTreeNode*& node, const string& srcName, const string& destName)
{
	MyTreeNode* src = recFindChild(node, srcName);
	MyTreeNode* dest = recFindChild(node, destName);

	if (!isCapableToMoveOrCopy(src, dest, srcName, destName))
	{
		return;
	}

	MyTreeNode* fatherOfSrc = src->getFather();
	if (!fatherOfSrc)
	{
		cout << "Ошибка перемещения!\n";
		return;
	}

	fatherOfSrc->removeChild(src);
	dest->addChild(src);
}

void SaveTree(FileTree& tree, string& filename)
{
	if (filename.empty())
	{
		cout << "Укажите имя файла!\n";
		return;
	}

	if (!tree.save(filename))
	{
		cout << "Ошибка сохранения!\n";
		return;
	}
	cout << "Успешное сохранение!\n";	
}

void LoadTree(FileTree& tree, MyTreeNode*& node, string& filename)
{
	if (filename.empty())
	{
		cout << "Укажите имя файла!\n";
		return;
	}

	if (!tree.load(filename))
	{
		cout << "Ошибка загрузки!\n";
		return;
	}
	node = tree.getRoot();
	cout << "Успешно загружено!\n";	
}

bool CommandManager(FileTree& tree, MyTreeNode*& node, string cmd, string arg1, string arg2)
{
	if (cmd == "exit")
	{
		return false;
	}
	if (cmd == "ls")
	{
		printListOfChildrens(node);
	}
	else if (cmd == "tree")
	{
		node->printTree();
	}
	else if (cmd == "cd")
	{
		ChangeDirectory(node, arg1);
	}
	else if (cmd == "mkdir")
	{
		CreateDirectory(node, arg1);
	}
	else if (cmd == "touch")
	{
		CreateFile(node, arg1);
	}
	else if (cmd == "rm")
	{
		Remove(node, arg1);
	}
	else if (cmd == "rename")
	{
		Rename(node, arg1, arg2);
	}
	else if (cmd == "cp")
	{
		Copy(node, arg1, arg2);
	}
	else if (cmd == "mv")
	{
		Move(node, arg1, arg2);
	}
	else if (cmd == "save")
	{
		SaveTree(tree, arg1);
	}
	else if (cmd == "load")
	{
		LoadTree(tree, node, arg1);
	}
	else if (cmd == "help")
	{
		printHelpInformation();
	}
	else
	{
		cout << "Неизвестная команда! Попробуйте команду help.\n";
	}
	return true;
}

void Loop()
{
	FileTree tree;
	tree.setRoot(new MyTreeNode("C:", MyTreeNode::NodeType::Folder));
    MyTreeNode* current = tree.getRoot();
    
	printHelpInformation();

	string line;
	while (true)
	{
		cout << current->getFullPath() << "> ";

		if (!getline(cin, line)) break;
		if(line.empty()) continue;

		stringstream ss(line);
		string cmd, arg1, arg2;
		ss >> cmd >> arg1 >> arg2;

		if (!CommandManager(tree, current, cmd, arg1, arg2))
		{
			break;
		}
	}


}


int main()
{
	SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    
	Loop();
	return 0;
}