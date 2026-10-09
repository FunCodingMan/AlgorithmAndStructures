#pragma once
#include "MyTreeNode.hpp"
#include <fstream>
#include <string>

class FileTree
{
    private:
        MyTreeNode* root;
    public:
        FileTree(): root(nullptr) {}

        ~FileTree()
        {
            delete root;
        }

        MyTreeNode* getRoot() const
        {
            return root;
        }

        void setRoot(MyTreeNode* newRoot)
        {
            if (newRoot == root) return;
            delete root;
            root = newRoot;
        }

        bool save(const std::string& filename) const
        {
            std::ofstream out(filename);
            if (!out) return false;
            saveRec(out, root, 0);
            return true;
        }

        bool load(const std::string& filename)
        {
            std::ifstream in(filename);
            if (!in) return false;

            MyTreeNode* newRoot = nullptr;
            std::string line;
            std::vector<std::pair<int, MyTreeNode*>> stack;

            while (std::getline(in, line))
            {
                if (line.empty())
                {
                    delete newRoot;
                    return false;
                }
                
                int spaceCount = 0;

                while (spaceCount < (int)line.size() && line[spaceCount] == ' ')
                {
                    spaceCount++;
                }

                int depth = 0;
                depth = spaceCount / 4;

                std::string name = line.substr(spaceCount);
                bool isFolder = !name.empty() && line.back() == '/';
                
                if (!name.empty() && name.back() == '\r') name.pop_back();

                if (isFolder) name.pop_back();

                MyTreeNode* node = new MyTreeNode(
                    name,
                    isFolder ? MyTreeNode::NodeType::Folder : MyTreeNode::NodeType::File
                );

                while (!stack.empty() && stack.back().first >= depth)
                {
                    stack.pop_back();
                }

                if (stack.empty())
                {
                    if (newRoot)
                    {
                        delete node;
                        delete newRoot;
                        return false;
                    }
                    newRoot = node;
                }
                else
                {
                    stack.back().second->addChild(node);
                }
                stack.push_back({depth, node});
            }

            if (!newRoot)
            {
                return false;
            }

            delete root;
            root = newRoot;
            return true;
        }
    private:
        static void saveRec(std::ofstream& out, MyTreeNode* node, int depth)
        {
            if (!node) return;
            
            out << std::string(depth * 4, ' ');
            out << node->getName();
            
            if (node->isFolder())
            {
                out << '/';
            }
            out << std::endl;
            for (MyTreeNode* child: node->getChildren())
            {       
                saveRec(out, child, depth + 1);
            }
        }

};