#include <iostream>
#include <fstream>
#include <queue>
using namespace std;
//multilevel membership 
class bstnode {
public:
    int id, p_id, l_id, r_id;
    bstnode* left;
    bstnode* right;
    double earns;
    string name;

    bstnode(int member_id, const string str) {
        id = member_id;
        p_id = NULL;
        l_id = 0;
        r_id = 0;
        name = str;
        left = NULL;
        right = NULL;
        earns = 0.00;
    }
};

class memeber_details {
private:
    bstnode* head;
    int count;

public:
    memeber_details() {
        head = NULL;
        count = 0;
    }

    int counting() {
        count += 1;
        return count;
    }

    bstnode* find(int parent_id, bstnode* h, int currentLevel = 1) {
        if (h == NULL) {
            return NULL; // Node with parent_id not found in the tree
        }

        if (h->id == parent_id) {
            return h; // Found the node with parent_id
        }

        // Check the left child at the next level
        bstnode* leftResult = find(parent_id, h->left, currentLevel + 1);
        if (leftResult != NULL) {
            return leftResult; // Found in the left subtree
        }

        // Check the right child at the next level
        return find(parent_id, h->right, currentLevel + 1);
    }

    void insert_for_parent_P(int parent_id, const string member_name) {
        bstnode* P = find(parent_id, head);
        if (P == NULL) {
            return;
        }
        else {
            bstnode* newNode = new bstnode(counting(), member_name);
            if (P->left != NULL && P->right != NULL) {
                count--;
                delete newNode;
                return;
            }
            else {
                if (P->left == NULL) {
                    P->left = newNode;
                    newNode->p_id = P->id;
                    P->l_id = newNode->id;
                    head->earns += 500;
                }
                else {
                    P->right = newNode;
                    newNode->p_id = P->id;
                    P->r_id = newNode->id;
                    updateEarnings(P);
                }
            }
        }
    }

    void updateEarnings(bstnode* node, double currentEarning = 500.0) {
        if (node == NULL) {
            return; // Base case: No member or level reached, no earning
        }

        // Calculate earnings for the current node based on level
        node->earns += currentEarning;

        // Recursively update earnings for the parent node
        updateEarnings(find(node->p_id, head), currentEarning / 2.0);
    }

    void make_head(const string member_name) {
        bstnode* newNode = new bstnode(1, member_name);
        counting();
        head = newNode;
    }

    void traverse(int parent_id) {
        bstnode* P = find(parent_id, head);
        if (P == NULL) {
            return;
        }
        else {
            traverse(P);
        }
    }

    void traverse(bstnode* P) {
        queue<bstnode*> nodeQueue;
        nodeQueue.push(P);

        while (!nodeQueue.empty()) {
            bstnode* frontNode = nodeQueue.front();
            nodeQueue.pop();

            cout << frontNode->p_id << " " << frontNode->id << " " << frontNode->name << " " << endl;

            if (frontNode->left != NULL) {
                nodeQueue.push(frontNode->left);
            }

            if (frontNode->right != NULL) {
                nodeQueue.push(frontNode->right);
            }
        }
    }

    int get_member_count(int parent_id) {
        bstnode* P = find(parent_id, head);
        if (P == NULL) {
            return 0;
        }
        else {
            int member_count = get_member_count(P);
            return member_count;
        }
    }

    int get_member_count(bstnode* P, int count_members = 0) {
        if (P == NULL) {
            return 0;
        }
        if (P->left == NULL && P->right == NULL) {
            return count_members + 1;
        }
        int l_count = 0;
        int r_count = 0;
        if (P->left != NULL)
            l_count = get_member_count(P->left, count_members + 1);
        if (P->right != NULL)
            r_count = get_member_count(P->right, count_members + 1);
        return l_count + r_count;
    }

    void Display() {
        if (head == NULL) {
            cout << "NO MEMBERSHIP IS CURRENTLY ACTIVE" << endl;
            return;
        }

        queue<bstnode*> nodeQueue;
        nodeQueue.push(head);

        while (!nodeQueue.empty()) {
            bstnode* frontNode = nodeQueue.front();
            nodeQueue.pop();

            cout << frontNode->id << " " << frontNode->earns << " ";

            if (frontNode->left != NULL) {
                nodeQueue.push(frontNode->left);
            }

            if (frontNode->right != NULL) {
                nodeQueue.push(frontNode->right);
            }
        }

        cout << endl;
        cout << "\n count is " << count << endl;
    }

    void saveToFile(const string& filename) {
        ofstream outfile;
        outfile.open(filename.c_str(), ios::out); // Open the file for writing (output)
        if (!outfile) {
            cout << "Failed to open the file for saving: " << filename << endl;
            return;
        }

        saveTreeToFile(head, outfile);

        outfile.close();
    }

    void saveTreeToFile(bstnode* node, ofstream& outfile) {
        if (node == NULL) {
            return;
        }

        outfile << node->id << " " << node->p_id << " " << node->l_id << " " << node->r_id << " " << node->name << " " << node->earns << endl;

        saveTreeToFile(node->left, outfile);
        saveTreeToFile(node->right, outfile);
    }

    void readFromFile(const string& filename) {
        ifstream infile(filename.c_str());
        if (!infile) {
            cout << "Failed to open the file for reading: " << filename << endl;
            return;
        }

        int member_id, parent_id, left_id, right_id;
        string member_name;
        double earns;

        while (infile >> member_id >> parent_id >> left_id >> right_id >> member_name >> earns) {
            bstnode* newNode = new bstnode(member_id, member_name);
            newNode->p_id = parent_id;
            newNode->l_id = left_id;
            newNode->r_id = right_id;
            newNode->earns = earns;

            if (head == NULL) {
                head = newNode;
            }
            else {
                bstnode* parent = find(parent_id, head);
                if (parent == NULL) {
                    delete newNode;
                    cout << "Parent with ID " << parent_id << " not found. Skipping member with ID " << member_id << endl;
                    continue;
                }

                if (parent->left == NULL) {
                    parent->left = newNode;
                }
                else if (parent->right == NULL) {
                    parent->right = newNode;
                }
                else {
                    delete newNode;
                    cout << "Parent with ID " << parent_id << " already has two children. Skipping member with ID " << member_id << endl;
                    continue;
                }
            }
        }

        infile.close();
    }
};

int main() {
    memeber_details L;
    L.make_head("Sukhmanjot");
    L.insert_for_parent_P(1, "Ripu");
    L.insert_for_parent_P(1, "Ekroop");
    L.insert_for_parent_P(3, "Ayush");
    L.insert_for_parent_P(3, "Ayushi");
    L.traverse(3);
    L.Display();

    // Save the multilevel membership data to file
    L.saveToFile("multilevel_membership.txt");

    // Read the multilevel membership data from the file
    memeber_details L2;
    L2.readFromFile("multilevel_membership.txt");
    L2.Display();

    return 0;
}

