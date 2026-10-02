#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

class MyTreeNode
{
    public:
        enum class NodeType
        {
            File, Folder
        };
    private:
        std::string name;
        NodeType type;
        MyTreeNode* fatherNode = nullptr;
        std::vector<MyTreeNode*> childNodes;
    public:
        MyTreeNode(const std::string& name, NodeType type): name(name), type(type) {}
        
        ~MyTreeNode()
        {
            for (MyTreeNode* child: childNodes)
            {
                delete child;
            }
        }

        const std::string& getName() const
        {
            return name;
        }

        void setName(const std::string& newName)
        {
            name = newName;
        }
        
        bool isFolder() const
        {
            return type == NodeType::Folder;
        }
        bool isFile() const
        {
            return type == NodeType::File;
        }
        MyTreeNode* getFather() const
        {
            return fatherNode;
        }
        const std::vector<MyTreeNode*>& getChildren() const
        {
            return childNodes;
        }

        void addChild(MyTreeNode* child)
        {
            if (!child) return;
            child->fatherNode = this;
            childNodes.push_back(child);
        }

        void removeChild(MyTreeNode* child)
        {
            if (!child) return;
            for (auto node = childNodes.begin(); node != childNodes.end(); ++node)
            {
                if (*node == child)
                {
                    child->fatherNode = nullptr;
                    childNodes.erase(node);
                    return;
                }
            }
        }

        bool deleteChild(const std::string& name)
        {
            MyTreeNode* node = findChild(name);
            if (!node) return false;
            removeChild(node);
            delete node;
            return true;
        }

        MyTreeNode* findChild(const std::string& childName) const
        {
            for (MyTreeNode* child: childNodes)
            {
                if (child->getName() == childName)
                {
                    return child;
                }
            }
            return nullptr;
        }

        MyTreeNode* clone() const
        {
            MyTreeNode* copy = new MyTreeNode(name, type);
            for (MyTreeNode* child: childNodes)
            {
                copy->addChild(child->clone());
            }
            return copy;
        }
        std::string getFullPath() const
        {
            std::vector<std::string> path;
            const MyTreeNode* temp = this;
            
            while (temp)
            {
                path.push_back(temp->getName());
                temp = temp->getFather();
            }
            std::reverse(path.begin(), path.end());
            
            std::string result = "";
            for (size_t i = 0; i < path.size(); ++i)
            {
                result += path[i];
                if (i < path.size() - 1 && path[i] != "C:") result += "/";
                if (path[i] == "C:") result += "/";
            }
            return result;
        }

        void printTree(std::string indent = "", bool isLast = true) const
        {
            std::cout << indent;
            if (fatherNode != nullptr)
            {
                std::cout << (isLast ? "\\-- " : "|-- ");
                indent += (isLast ? "    " : "|   ");
            }

            if (isFolder())
            {
                std::cout << "[DIR]  " << name << "\n";
            }
            else
            {
                std::cout << "[FILE] " << name << "\n";
            }

            for (size_t i = 0; i < childNodes.size(); ++i)
            {
                childNodes[i]->printTree(indent, i == childNodes.size() - 1);
            }
        }
};