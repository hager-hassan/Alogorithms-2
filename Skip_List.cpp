#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include <valarray>
#include <algorithm>
using namespace std;

// Skip List Node Definition
struct Node {
    string name;
    int score;
    vector<Node*> forward; // Array of forward pointers for each level
    Node(string name, int value) {
        this->name = name;
        this->score = value;
    }
    Node(string player, int value, int level) : name(player), score(value), forward(level, nullptr) {}
};

// Skip List Class Definition
class SkipList {
private:
    Node* header;      // The header (sentinel) node
    int maxLevel;      // The maximum level of the skip list
    int currentLevel;  // Current level of the skip list
    int nodeCount = 0;

public:
    SkipList(int maxLevel = 16) : maxLevel(maxLevel), currentLevel(0) {
        header = new Node("", -1, maxLevel);  // Initialize the header with a dummy value (-1)
        srand(time(0));  // Seed the random number generator
    }

    // Generate a random level for a new node
    int randomLevel() {
        int level = 1;
        while ((rand() % 2) && level < maxLevel) { // Randomly decide if the node should go to a higher level
            level++;
        }
        return level;
    }

    // Insert a value into the skip list
    void insert(string name, int value) {
        vector<Node*> update(maxLevel, nullptr);  // Store the nodes where we need to update pointers
        Node* current = header;

        // Start from the highest level and move downwards
        for (int i = currentLevel - 1; i >= 0; i--) {
            while (current->forward[i] != nullptr && current->forward[i]->score < value) {
                current = current->forward[i];
            }
            update[i] = current;  // Store the node where the forward pointer needs to be updated
        }

        current = current->forward[0];  // Move to the next level (lowest level)

        // If the value doesn't exist, insert it
        if (current == nullptr || current->score != value) {
            int level = randomLevel();  // Determine the level for the new node
            if (level > currentLevel) {
                for (int i = currentLevel; i < level; i++) {
                    update[i] = header;  // Extend the update array
                }
                currentLevel = level;
            }

            Node* newNode = new Node(name, value, level);
            for (int i = 0; i < level; i++) {
                newNode->forward[i] = update[i]->forward[i];  // Set the forward pointers
                update[i]->forward[i] = newNode;  // Update the forward pointers at the levels
            }
            nodeCount++;
        }
    }

    // Delete a node
    void deleteNode(string name) {
        vector<Node*> update(maxLevel, nullptr);
        Node* current = header;

        // Search for the node with the specified value and store the relevant nodes in update[]
        for (int i = currentLevel - 1; i >= 0; i--) {
            while (current->forward[i] != nullptr && current->forward[i]->name != name) {
                current = current->forward[i];
            }
            update[i] = current;
        }

        // Move to the lowest level (0-level)
        current = current->forward[0];

        if (current == nullptr || current->name != name) {
            cout << "Player not found" << endl;
        } else {
            // Update the forward pointers at each level
            for (int i = 0; i < currentLevel; i++) {
                if (update[i]->forward[i] == current) {
                    update[i]->forward[i] = current->forward[i];
                }
            }

            // If the highest level becomes empty, reduce the current level
            while (currentLevel > 1 && header->forward[currentLevel - 1] == nullptr) {
                currentLevel--;
            }

            delete current;
            nodeCount--;
        }
    }

    // Search for a node by name
    Node* searchNode(string name) {
        vector<Node*> update(maxLevel, nullptr); // Vector to store the nodes where pointers need to be updated
        Node* current = header;  // Start from the header node

        // Search for the node with the specified value and store the relevant nodes in update[]
        for (int i = currentLevel - 1; i >= 0; i--) {
            while (current->forward[i] != nullptr && current->forward[i]->name != name) {
                current = current->forward[i];
            }
            update[i] = current; // Store the node where we need to update the forward pointer
        }

        // Move to the lowest level (0-level)
        current = current->forward[0];
        if (current == nullptr || current->name != name) {
            return nullptr;
        }
        return current;
    }

    // Display the Skip List (for testing and visualization)
    void display() {
        for (int i = 0; i < currentLevel; i++) {
            Node* node = header->forward[i];
            cout << "Level " << i + 1 << ": ";
            while (node != nullptr) {
                cout << node->name << ":" << node->score << " ";
                node = node->forward[i];
            }
            cout << endl;
        }
    }

    // View the score of a specific player
    void viewScore(string name) {
        vector<Node*> update(maxLevel, nullptr);
        Node* current = header;
        for (int i = currentLevel - 1; i >= 0; i--) {
            while (current->forward[i] != nullptr && current->forward[i]->name != name) {
                current = current->forward[i];
            }
            update[i] = current;
        }
        current = current->forward[0];
        if (current == nullptr || current->name != name) {
            cout << "Player not found" << endl;
        }
        else {
            cout << "Player score: " << current->score << endl;
        }
    }

    // Show the top N players
    void topN_player(int n) {
        if (n <= 0) {
            cout << "Invalid number of top players." << endl;
            return;  // Exit the function if the input is invalid
        }

        vector<Node*> node;
        Node* current = header->forward[0];

        while (current != nullptr) {
            node.push_back(current);
            current = current->forward[0];
        }

        sort(node.begin(), node.end(), [](Node* a, Node* b) { return a->score > b->score; });

        int topPlayers = min(n, (int)node.size());  // Handle if n is greater than available players
        cout << "Top " << topPlayers << " player scores:" << endl;
        for (int i = 0; i < topPlayers; i++) {
            cout << node[i]->name << ":" << node[i]->score << endl;
        }
    }

    // Increase the score of a player
    void scoreUp(string name, int value) {
        Node* current = searchNode(name);
        if (current != nullptr) {
            current->score += value;
            deleteNode(name);  // Delete the old node
            insert(name, current->score);  // Insert the new node with the updated score
        } else {
            cout << "No such player" << endl;
        }
    }

    // Decrease the score of a player
    void scoreDown(string name, int value) {
        Node* current = searchNode(name);
        if (current != nullptr) {
            current->score -= value;
            deleteNode(name);
            insert(name, current->score);
        } else {
            cout << "No such player" << endl;
        }
    }

};

// Main function
int main() {
    SkipList s(10);
    int choice = -1;

    while (choice != 0) {
        cout << "1- Insert player\n"
             << "2- Delete player\n"
             << "3- Update score\n"
             << "4- View player score\n"
             << "5- Show top N players\n"
             << "ENTER 0 TO EXIT FROM PROGRAM\n"
             << "Enter your choice: \n";

        cin >> choice;

        string name;
        int score, n;

        switch (choice) {
            case 1:
            {
                cout << "Enter player name: \n";
                cin >> name;
                cout << "Enter score: \n";
                cin >> score;
                s.insert(name, score);
                continue;
            }
            case 2:
            {
                cout << "Enter player name to delete: \n";
                cin >> name;
                s.deleteNode(name);
                continue;
            }
            case 3:
            {
                cout << "Enter player name: ";
                cin >> name;
                cout << "Enter score change (+ve to increase, -ve to decrease): \n";
                cin >> score;
                if (score > 0) {
                    s.scoreUp(name, score);
                } else {
                    s.scoreDown(name, -score);
                }
                continue;
            }
            case 4:
            {
                cout << "Enter player name: \n";
                cin >> name;
                s.viewScore(name);
                continue;
            }
            case 5:
            {
                cout << "Enter N (number of top players to show): \n";
                cin >> n;
                s.topN_player(n);
                continue;
            }
            case 0:
                cout << "EXITING PROGRAM....\n";
                break;
            default:
                cout << "INVALID CHOICE :(\n";
                continue;
        }
    }

    return 0;
}
