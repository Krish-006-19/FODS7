#include <iostream>
#include <string>
#include <limits>
#include <cctype>
using namespace std;

struct Node {
    int val;
    Node* left;
    Node* right;
    bool rightThread; // true if right pointer is a thread (not a real child)

    Node(int val) {
        val = val;
        left = nullptr;
        right = nullptr;
        rightThread = true;
    }
};

Node* insert(Node* root, int val) {
    Node* ptr = root;
    Node* par = nullptr;

    while (ptr != nullptr) {
        if (val == ptr->val) {
            cout << "Duplicate vals not allowed.\n";
            return root;
        }

        par = ptr;

        if (val < ptr->val) {
            if (ptr->left == nullptr)
                break;
            ptr = ptr->left;
        } else {
            if (ptr->rightThread == false)
                ptr = ptr->right;
            else
                break;
        }
    }

    Node* temp = new Node(val);

    if (par == nullptr)
        root = temp;
    else if (val < par->val) {
        temp->left = par->left;
        temp->right = par;
        par->left = temp;
    } else {
        temp->right = par->right;
        temp->rightThread = true;
        par->right = temp;
        par->rightThread = false;
    }
    return root;
}

Node* inorderSuccessor(Node* ptr) {
    if (ptr->rightThread)
        return ptr->right;
    ptr = ptr->right;
    while (ptr->left != nullptr)
        ptr = ptr->left;
    return ptr;
}

void inorder(Node* root) {
    if (root == nullptr) return;
    Node* ptr = root;
    while (ptr->left != nullptr)
        ptr = ptr->left;
    while (ptr != nullptr) {
        cout << ptr->val << " ";
        ptr = inorderSuccessor(ptr);
    }
}

void preorder(Node* root) {
    if (root == nullptr) return;
    Node* ptr = root;

    while (ptr != nullptr) {
        cout << ptr->val << " ";

        if (ptr->left != nullptr)
            ptr = ptr->left;
        else if (!ptr->rightThread)
            ptr = ptr->right;
        else {
            while (ptr != nullptr && ptr->rightThread)
                ptr = ptr->right;
            if (ptr != nullptr)
                ptr = ptr->right;
        }
    }
}

Node* search(Node* root, int val) {
    Node* ptr = root;
    while (ptr != nullptr) {
        if (val == ptr->val)
            return ptr;
        else if (val < ptr->val)
            ptr = ptr->left;
        else if (ptr->rightThread == false)
            ptr = ptr->right;
        else
            break;
    }
    return nullptr;
}

Node* deleteNode(Node* root, int val) {
    Node* par = nullptr;
    Node* ptr = root;
    bool found = false;

    while (ptr != nullptr) {
        if (val == ptr->val) {
            found = true;
            break;
        }
        par = ptr;
        if (val < ptr->val)
            ptr = ptr->left;
        else if (!ptr->rightThread)
            ptr = ptr->right;
        else
            break;
    }

    if (!found) {
        cout << "val not found.\n";
        return root;
    }

    Node* child = nullptr;
    if (ptr->left != nullptr && !ptr->rightThread) {
        // Node with two children
        Node* parSucc = ptr;
        Node* succ = ptr->right;
        while (succ->left != nullptr) {
            parSucc = succ;
            succ = succ->left;
        }

        ptr->val = succ->val;
        ptr = succ;
        par = parSucc;
    }

    // Now ptr has at most one child
    if (ptr->left != nullptr)
        child = ptr->left;
    else if (!ptr->rightThread)
        child = ptr->right;
    else
        child = nullptr;

    if (par == nullptr)
        root = child;
    else if (ptr == par->left)
        par->left = child;
    else {
        if (ptr->rightThread)
            par->right = ptr->right;
        else
            par->right = child;
    }

    // Fix threads
    if (child != nullptr) {
        Node* s = inorderSuccessor(ptr);
        Node* p = root;
        while (p != nullptr) {
            if (p->right == ptr)
                break;
            else if (!p->rightThread)
                p = p->right;
            else
                p = nullptr;
        }
        if (p != nullptr && p->rightThread)
            p->right = s;
    }

    delete ptr;
    cout << "Node deleted successfully.\n";
    return root;
}

Node* update(Node* root, int oldVal, int newVal) {
    Node* node = search(root, oldVal);
    if (node == nullptr) {
        cout << "Value not found.\n";
        return root;
    }
    root = deleteNode(root, oldVal);
    root = insert(root, newVal);
    cout << "Node updated successfully.\n";
    return root;
}

bool isValidInteger(const string& str) {
    if (str.empty()) return false;
    int start = (str[0] == '+' || str[0] == '-') ? 1 : 0;
    if (start == str.size()) return false;
    for (int i = start; i < str.size(); ++i)
        if (!isdigit(str[i])) return false;
    return true;
}

int getValidInteger(const string& prompt) {
    string input;
    int val;
    while (true) {
        cout << prompt;
        cin >> input;
        if (!isValidInteger(input)) {
            cout << "Invalid input. Enter a valid integer.\n";
            continue;
        }
        try {
            val = stoi(input);
            return val;
        } catch (...) {
            cout << "Error: Try again.\n";
        }
    }
}

int main() {
    Node* root = nullptr;
    string choice;
    int val, newVal;

    while (true) {
        cout << "\n\n==== Threaded Binary Tree Menu ====\n";
        cout << "1. Insert Node\n";
        cout << "2. Delete Node\n";
        cout << "3. Update Node\n";
        cout << "4. Search Node\n";
        cout << "5. Inorder Traversal\n";
        cout << "6. Preorder Traversal\n";
        cout << "7. Exit\n";
        cout << "Choose an option: ";
        cin >> choice;

        if (choice.size() != 1 || choice[0] < '1' || choice[0] > '7') {
            cout << "Invalid choice. Enter a number 1–7.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice[0]) {
            case '1':
                val = getValidInteger("Enter value to insert: ");
                root = insert(root, val);
                break;
            case '2':
                val = getValidInteger("Enter value to delete: ");
                root = deleteNode(root, val);
                break;
            case '3':
                val = getValidInteger("Enter old value: ");
                newVal = getValidInteger("Enter new value: ");
                root = update(root, val, newVal);
                break;
            case '4':
                val = getValidInteger("Enter value to search: ");
                cout << (search(root, val) ? "Found.\n" : "Not found.\n");
                break;
            case '5':
                cout << "Inorder Traversal: ";
                inorder(root);
                cout << endl;
                break;
            case '6':
                cout << "Preorder Traversal: ";
                preorder(root);
                cout << endl;
                break;
            case '7':
                cout << "Exiting program...\n";
                return 0;
        }
    }
}
