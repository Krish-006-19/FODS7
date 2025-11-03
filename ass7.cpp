#include <iostream>
using namespace std;

struct Node {
    int key;
    Node* left;
    Node* right;
    int rightThread;
};

Node* createNode(int key) {
    Node* n = new Node;
    n->key = key;
    n->left = nullptr;
    n->right = nullptr;
    n->rightThread = 1;
    return n;
}

Node* insert(Node* root, int key) {
    Node* ptr = root;
    Node* parent = nullptr;

    while (ptr) {
        if (key == ptr->key) {
            cout << "Duplicate key not allowed\n";
            return root;
        }

        parent = ptr;
        if (key < ptr->key)
            ptr = ptr->left;
        else if (!ptr->rightThread)
            ptr = ptr->right;
        else
            break;
    }

    Node* node = createNode(key);

    if (!parent) {
        root = node;
    } else if (key < parent->key) {
        node->left = nullptr;
        node->right = parent;
        parent->left = node;
    } else {
        node->right = parent->right;
        parent->right = node;
        parent->rightThread = 0;
    }

    return root;
}

Node* inorderSuccessor(Node* ptr) {
    Node* res = ptr;
    if (ptr->rightThread == 1) res = ptr->right;
    else {
        res = ptr->right;
        while (res->left != nullptr) res = res->left;
    }
    return res;
}

void inorder(Node* root) {
    if (root == nullptr) {
        cout << endl;
        return;
    }

    Node* curr = root;
    while (curr->left != nullptr)
        curr = curr->left;

    while (curr != nullptr) {
        cout << curr->key << " ";
        curr = inorderSuccessor(curr);
    }
    cout << endl;
}

void preorder(Node* root) {
    Node* curr = root;
    while (curr != nullptr) {
        cout << curr->key << " ";
        if (curr->left != nullptr)
            curr = curr->left;
        else {
            while (curr != nullptr && curr->rightThread == 1)
                curr = curr->right;
            if (curr != nullptr)
                curr = curr->right;
        }
    }
    cout << endl;
}

Node* search(Node* root, int key) {
    Node* curr = root;
    Node* res = nullptr;
    int found = 0;
    while (curr != nullptr && found == 0) {
        if (key == curr->key) {
            res = curr;
            found = 1;
        } else if (key < curr->key) curr = curr->left;
        else if (curr->rightThread == 0) curr = curr->right;
        else curr = nullptr;
    }
    return res;
}

Node* deleteNode(Node* root, int key) {
    Node* parent = nullptr;
    Node* curr = root;
    int found = 0;

    while (curr != nullptr && found == 0) {
        if (key == curr->key) found = 1;
        else {
            parent = curr;
            if (key < curr->key) curr = curr->left;
            else if (curr->rightThread == 0) curr = curr->right;
            else curr = nullptr;
        }
    }

    if (found == 0) return root;

    // Node has both children
    if (curr->left != nullptr && curr->rightThread == 0) {
        Node* parSucc = curr;
        Node* succ = curr->right;
        while (succ->left != nullptr) {
            parSucc = succ;
            succ = succ->left;
        }
        curr->key = succ->key;
        parent = parSucc;
        curr = succ;
    }

    Node* child = nullptr;
    if (curr->left != nullptr)
        child = curr->left;
    else if (curr->rightThread == 0)
        child = curr->right;

    if (parent == nullptr) {
        delete curr;
        root = child;
    } else if (curr == parent->left) {
        parent->left = child;
        if (child == nullptr) {
            parent->rightThread = 1;
            parent->right = curr->right;
        } else {
            Node* temp = child;
            while (temp->rightThread == 0) temp = temp->right;
            temp->right = curr->right;
        }
        delete curr;
    } else {
        if (parent->rightThread == 0) {
            if (child != nullptr) {
                parent->right = child;
                Node* temp = child;
                while (temp->rightThread == 0) temp = temp->right;
                temp->right = curr->right;
            } else {
                parent->rightThread = 1;
                parent->right = curr->right;
            }
        }
        delete curr;
    }

    return root;
}

Node* updateKey(Node* root, int oldKey, int newKey) {
    Node* target = search(root, oldKey);
    if (target != nullptr) {
        root = deleteNode(root, oldKey);
        root = insert(root, newKey);
    }
    return root;
}

void freeTree(Node* root) {
    while (root != nullptr)
        root = deleteNode(root, root->key);
}

int main() {
    Node* root = nullptr;
    int running = 1, val, oldv, newv;
    string choice;

    while (running == 1) {
        cout << "\n===== Threaded Binary Tree (Right Thread Only) =====\n";
        cout << "1. Insert\n2. Delete\n3. Update Key\n4. Search\n5. Inorder\n6. Preorder\n7. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        bool isNumeric = true;
        for (char c : choice) {
            if (!isdigit(c)) {
                isNumeric = false;
                break;
            }
        }

        if (!isNumeric) {
            cout << "Invalid input! Please enter a number between 1–7.\n";
            continue;
        }

        int ch = stoi(choice);

        switch (ch) {
            case 1:
                cout << "Enter value: ";
                cin >> val;
                root = insert(root, val);
                cout << "Inserted " << val << endl;
                break;

            case 2:
                cout << "Enter value: ";
                cin >> val;
                root = deleteNode(root, val);
                cout << "Deleted " << val << " (if existed)\n";
                break;

            case 3:
                cout << "Enter old key: ";
                cin >> oldv;
                cout << "Enter new key: ";
                cin >> newv;
                root = updateKey(root, oldv, newv);
                cout << "Updated " << oldv << " -> " << newv << endl;
                break;

            case 4:
                cout << "Enter value to search: ";
                cin >> val;
                {
                    Node* f = search(root, val);
                    if (f != nullptr)
                        cout << "Found " << f->key << endl;
                    else
                        cout << val << " not found\n";
                }
                break;

            case 5:
                cout << "Inorder: ";
                inorder(root);
                break;

            case 6:
                cout << "Preorder: ";
                preorder(root);
                break;

            case 7:
                running = 0;
                cout << "Exiting program...\n";
                break;

            default:
                cout << "Invalid choice! Please enter a number between 1–7.\n";
                break;
        }
    }

    freeTree(root);
    return 0;
}
