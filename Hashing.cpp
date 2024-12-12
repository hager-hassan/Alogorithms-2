#include <iostream>
#include <vector>
#include <list>
#include <cmath>
#include <string>
using namespace std;

// Hashing Techniques
int hashDivision(int key, int tableSize) {
    return key % tableSize;
}

int hashMultiplication(int key, int tableSize) {
    const double A = 0.6180339887;
    double fractionalPart = fmod(key * A, 1.0);
    return static_cast<int>(floor(tableSize * fractionalPart));
}

unsigned int foldShiftingHash(const string &key, unsigned int boundary) {
    unsigned int hashValue = 0;
    int partSize = 2;
    for (size_t i = 0; i < key.length(); i += partSize) {
        unsigned int part = 0;
        for (int j = i; j < i + partSize && j < key.length(); ++j) {
            part = part * 10 + (key[j] - '0');
        }
        hashValue += part;
    }
    return hashValue % boundary;
}

unsigned int foldBoundaryHash(const string &key, unsigned int boundary) {
    unsigned int hashValue = 0;
    int partSize = 2;
    unsigned int parts[4] = {0};
    for (size_t i = 0; i < key.length(); i += partSize) {
        unsigned int part = 0;
        for (int j = i; j < i + partSize && j < key.length(); ++j) {
            part = part * 10 + (key[j] - '0');
        }
        parts[i / partSize] = part;
    }
    swap(parts[0], parts[3]);
    swap(parts[1], parts[2]);
    for (int i = 0; i < 4; ++i) {
        hashValue += parts[i];
    }
    return hashValue % boundary;
}

int midSquareMethod(int key, int tableSize) {
    long long squared = key * key;
    string squaredStr = to_string(squared);
    int midStart = squaredStr.length() / 2 - 1;
    string midDigits = squaredStr.substr(midStart, 2);
    int midHashValue = stoi(midDigits);
    return midHashValue % tableSize;
}

// Collision Resolution Techniques
class Chaining {
    int size;
    vector<list<int>> table;
public:
    Chaining(int size) {
        this->size = size;
        table.resize(size);
    }

    int getKey(int value) {
        return value % size;
    }

    void insert(int value) {
        int key = getKey(value);
        table[key].push_back(value);
    }

    void search(int value) {
        int key = getKey(value);
        int index = 0;
        for (int val : table[key]) {
            if (val == value) {
                cout << "Number " << value << " found at index " << key << ", position " << index << endl;
                return;
            }
            index++;
        }
        cout << "Number " << value << " not found." << endl;
    }

    void deletion(int value) {
        int key = getKey(value);
        for (auto it = table[key].begin(); it != table[key].end(); ) {
            if (*it == value) {
                it = table[key].erase(it);
                cout << "Number " << value << " deleted." << endl;
                return;
            } else {
                ++it;
            }
        }
        cout << "Number " << value << " not found." << endl;
    }

    void display() {
        for (int i = 0; i < size; i++) {
            cout << "index " << i << ": ";
            if (table[i].empty())
                cout << "empty";
            else {
                for (int x : table[i])
                    cout << x << "-->";
            }
            cout << endl;
        }
    }
};

class Linear_Probing {
    int size;
    vector<int> table;
    const int EMPTY = -1;

public:
    Linear_Probing(int size) {
        this->size = size;
        table.resize(size, EMPTY);
    }

    int getKey(int value) {
        return value % size;
    }

    void insert(int value) {
        int key = getKey(value);
        int start = key;
        while (table[key] != EMPTY) {
            key = (key + 1) % size;
            if (key == start) {
                cout << "Hash table is full." << endl;
                return;
            }
        }
        table[key] = value;
    }

    void search(int value) {
        int key = getKey(value);
        int start = key;
        while (table[key] != EMPTY) {
            if (table[key] == value) {
                cout << value << " found at index " << key << endl;
                return;
            }
            key = (key + 1) % size;
            if (key == start) break;
        }
        cout << value << " not found" << endl;
    }

    void deletion(int value) {
        int key = getKey(value);
        int start = key;
        while (table[key] != EMPTY) {
            if (table[key] == value) {
                table[key] = EMPTY;
                cout << value << " deleted from index " << key << endl;
                return;
            }
            key = (key + 1) % size;
            if (key == start) break;
        }
        cout << value << " not found" << endl;
    }

    void display() {
        for (int i = 0; i < size; i++) {
            cout << "index " << i << ": ";
            if (table[i] == EMPTY)
                cout << "empty";
            else
                cout << table[i];
            cout << endl;
        }
    }
};

class Quad_Probing {
    int size;
    double none = 0.0/0;
    vector<double> table;
public:
    Quad_Probing(int size) {
        this->size = size;
        table.resize(size, none);
    }
    int getKey(int value) {
        return value % size;
    }
    void insert(int value) {
        int key = getKey(value);
        int stop = key;
        int i = 1;
        if(isnan(table[key]))
            table[key] = value;
        else {
            while(!isnan(table[key])) {
                key = (key + i * i) % size;
                i++;
                if(stop == key) return;
            }
            table[key] = value;
        }
    }
    void search(int value) {
        int key = getKey(value);
        bool found = false;
        int start = key;
        int i = 1;
        while(!isnan(table[key])) {
            if(table[key] == value) {
                found = true;
                cout<<value<<" found at index "<<key<<endl;
                return;
            }
            key = (key + i * i) % size;
            i++;
        }
        if(!found)
            cout<<value<<" not found"<<endl;
    }
    bool found(int value) {
        int key = getKey(value);
        int i = 1;
        while(!isnan(table[key])) {
            if(table[key] == value)
                return true;
            key = (key + i * i) % size;
            i++;
        }
        return false;
    }
    void deletion(int value) {
        int key = getKey(value);
        if(found(value)) {
            table[key] = none;
            cout<<value<<" deleted from index "<<key<<endl;
        } else
            cout<<value<<" not found"<<endl;
    }
    void display() {
        for (int i = 0; i < size; i++) {
            cout<<"index "<<i<<": ";
            if(isnan(table[i]))
                cout<<"empty\n";
            else
                cout<<table[i]<<endl;
        }
    }
};

class Double_Hashing {
    int size;
    int hash2;
    vector<int> table;
    vector<bool> isDeleted; // Marks slots that are deleted

    const int EMPTY = -1; // Sentinel value for empty slots

public:
    Double_Hashing(int size, int hash2) {
        this->size = size;
        this->hash2 = hash2;
        table.resize(size, EMPTY);
        isDeleted.resize(size, false); // Initialize all slots as not deleted
    }

    int getKey1(int value) {
        return value % size;
    }

    int getKey2(int value) {
        return hash2 - (value % hash2);
    }

    void insert(int value) {
        int key = getKey1(value);
        int key2 = getKey2(value);
        int attempts = 0;

        while (attempts < size) {
            if (table[key] == EMPTY || isDeleted[key]) {
                table[key] = value;
                isDeleted[key] = false; // Clear deleted marker on insertion
                cout << "Inserted " << value << " at index " << key << endl;
                return;
            }
            key = (key + key2) % size;
            attempts++;
        }
        cout << "Hash table is full. Could not insert " << value << endl;
    }

    void search(int value) {
        int key = getKey1(value);
        int key2 = getKey2(value);
        int attempts = 0;

        while (attempts < size && table[key] != EMPTY) {
            if (table[key] == value) {
                cout << value << " found at index " << key << endl;
                return;
            }
            key = (key + key2) % size;
            attempts++;
        }
        cout << value << " not found" << endl;
    }

    void deletion(int value) {
        int key = getKey1(value);
        int key2 = getKey2(value);
        int attempts = 0;

        while (attempts < size && table[key] != EMPTY) {
            if (table[key] == value) {
                table[key] = EMPTY;
                isDeleted[key] = true; // Mark slot as deleted
                cout << value << " deleted from index " << key << endl;
                return;
            }
            key = (key + key2) % size;
            attempts++;
        }
        cout << value << " not found" << endl;
    }

    void display() {
        for (int i = 0; i < size; i++) {
            cout << "index " << i << ": ";
            if (table[i] == EMPTY) {
                if (isDeleted[i]) {
                    cout << "deleted";
                } else {
                    cout << "empty";
                }
            } else {
                cout << table[i];
            }
            cout << endl;
        }
    }
};

int main() {
    int x = -1;

    while (x != 0) {
        cout << "Main Menu:" << '\n'
             << "1- Hashing Techniques" << '\n'
             << "2- Collision Resolution Techniques" << '\n'
             << "0- Exit" << endl;

        cin >> x;

        switch (x) {
            case 1:
            {
                int choice = -1, tableSize, numKeys, key;

                while (choice != 0){
                    cout << "Hashing Techniques Menu:"              << '\n'
                         << "1- Division Method."                   << '\n'
                         << "2- Multiplication Method."             << '\n'
                         << "3- Fold-Shifting & Boundary Hashing."  << '\n'
                         << "4- Mid-Square Method"                  << '\n'
                         << "ENTER 0 TO BACK TO MAIN MENU.."        << '\n'
                         << "Enter your choice: "                   << '\n';

                    cin >> choice;

                    switch (choice) {
                        case 1: {
                            cout << "Enter the size of the hash table: " << '\n';
                            cin >> tableSize;

                            cout << "Enter the number of keys: " << '\n';
                            cin >> numKeys;

                            vector<int> keys(numKeys);
                            cout << "Enter the keys:" << '\n';
                            for (int i = 0; i < numKeys; i++) {
                                cout << "Key " << (i + 1) << ": \n";
                                cin >> keys[i];
                            }

                            cout << "Hash values using Division Method:" << '\n';
                            for (int i = 0; i < numKeys; i++) {
                                int hashValue = hashDivision(keys[i], tableSize);
                                cout << "Key: " << keys[i] << " -> Hash Value: " << hashValue << '\n';
                            }
                            cout << '\n';
                            continue;
                        }
                        case 2:
                        {
                            cout << "Enter the size of the hash table: " << '\n';
                            cin >> tableSize;

                            cout << "Enter the number of keys: " << '\n';
                            cin >> numKeys;

                            vector<int> keys(numKeys);
                            cout << "Enter the keys:" << '\n';
                            for (int i = 0; i < numKeys; i++) {
                                cout << "Key " << (i + 1) << ": \n";
                                cin >> keys[i];
                            }

                            cout << "Hash values using Multiplication Method:" << '\n';
                            for (int i = 0; i < numKeys; i++) {
                                int hashValue = hashMultiplication(keys[i], tableSize);
                                cout << "Key: " << keys[i] << " -> Hash Value: " << hashValue << '\n';
                            }
                            cout << '\n';
                            continue;
                        }
                        case 3:
                        {
                            string key;
                            unsigned int boundary;

                            // Get the user input
                            cout << "Enter a string key for hashing (numbers or characters): " << '\n';
                            cin >> key;

                            cout << "Enter a boundary value (like 100 for table size): " << '\n';
                            cin >> boundary;

                            // Compute and display the results for both methods
                            unsigned int foldShiftingResult = foldShiftingHash(key, boundary);
                            cout << "Final Fold-Shifting Hash value: " << foldShiftingResult << '\n';

                            unsigned int foldBoundaryResult = foldBoundaryHash(key, boundary);
                            cout << "Final Fold-Boundary Hash value: " << foldBoundaryResult << '\n';

                            cout << '\n';
                            continue;
                        }
                        case 4:
                        {
                            cout << "Enter the size of the hash table: " << '\n';
                            cin >> tableSize;

                            cout << "Enter the number of keys: " << '\n';
                            cin >> numKeys;

                            vector<int> keys(numKeys);
                            cout << "Enter the keys:" << '\n';
                            for (int i = 0; i < numKeys; i++) {
                                cout << "Key " << (i + 1) << ": \n";
                                cin >> keys[i];
                            }

                            cout << "Hash values using Mid-Square Method:" << '\n';
                            for (int i = 0; i < numKeys; i++) {
                                int hashValue = midSquareMethod(keys[i], tableSize);
                                cout << "Key: " << keys[i] << " -> Hash Value: " << hashValue << '\n';
                            }
                            cout << '\n';
                            continue;
                        }
                        case 0:
                            cout << "BACK TO MAIN MENU.." << '\n';
                            break;
                        default:
                            cout << "INVALID CHOICE :(" << '\n';
                            continue;
                    }
                }
                continue;
            }
            case 2:
            {
                int size, value, hash2 , choice = -1;

                while(choice != 0)
                {
                    cout << "Collision Resolution Techniques Menu:"  << '\n'
                         << "1- Chaining."                           << '\n'
                         << "2- Linear Probing."                     << '\n'
                         << "3- Quadratic Probing."                  << '\n'
                         << "4- Double Hashing"                      << '\n'
                         << "ENTER 0 TO EXIT THE PROGRAM..."         << '\n'
                         << "Enter your choice: "                    << '\n';

                    cin >> choice;

                    switch (choice) {
                        case 1:
                        {
                            cout << "Enter the size of the hash table: " << '\n';
                            cin >> size;
                            Chaining chaining(size);

                            int operationChoice = -1;
                            while (operationChoice != 0) {
                                cout << "Choose an operation:" << '\n'
                                     << "1- Insert" << '\n'
                                     << "2- Search" << '\n'
                                     << "3- Delete" << '\n'
                                     << "4- Display" << '\n'
                                     << "Enter 0 to back to main menu" << '\n'
                                     << "Enter your choice: " << '\n';

                                cin >> operationChoice;

                                switch (operationChoice) {
                                    case 1: {
                                        cout << "Enter value to insert: " << '\n';
                                        cin >> value;
                                        chaining.insert(value);
                                        continue;
                                    }
                                    case 2: {
                                        cout << "Enter value to search: " << '\n';
                                        cin >> value;
                                        chaining.search(value);
                                        continue;
                                    }
                                    case 3: {
                                        cout << "Enter value to delete: " << '\n';
                                        cin >> value;
                                        chaining.deletion(value);
                                        continue;
                                    }
                                    case 4: {
                                        cout << "Displaying hash table:" << '\n';
                                        chaining.display();
                                        continue;
                                    }
                                    case 0:
                                        cout << "Returning to main menu..." << '\n';
                                        continue;
                                    default:
                                        cout << "Invalid choice :(" << '\n';
                                        continue;
                                }
                            }
                            continue;
                        }
                        case 2:{
                            cout << "Enter the size of the hash table: " << '\n';
                            cin >> size;
                            Linear_Probing linearProbing(size);

                            int operationChoice = -1;
                            while (operationChoice != 0) {
                                cout << "1- Insert\n2- Search\n3- Delete\n4- Display\n0- Return to main menu\nChoose an operation: \n";
                                cin >> operationChoice;

                                switch (operationChoice) {
                                    case 1: {
                                        cout << "Enter the value to insert: " << '\n';
                                        cin >> value;
                                        linearProbing.insert(value);  // Insert using Linear Probing
                                        continue;
                                    }
                                    case 2: {
                                        cout << "Enter the value to search: \n";
                                        cin >> value;
                                        linearProbing.search(value);  // Search using Linear Probing
                                        continue;
                                    }
                                    case 3: {
                                        cout << "Enter the value to delete: \n";
                                        cin >> value;
                                        linearProbing.deletion(value);  // Delete using Linear Probing
                                        continue;
                                    }
                                    case 4: {
                                        cout << "Displaying hash table:\n";
                                        linearProbing.display();  // Display using Linear Probing
                                        continue;
                                    }
                                    case 0:
                                        cout << "Returning to main menu...\n";
                                        continue;
                                    default:
                                        cout << "Invalid choice. Try again.\n";
                                        continue;
                                }
                            }
                            continue;
                        }
                        case 3:
                        {
                            cout << "Enter the size of the hash table: " << '\n';
                            cin >> size;
                            Quad_Probing quadProbing(size);

                            int operationChoice = -1;
                            while (operationChoice != 0) {
                                cout << "1- Insert\n2- Search\n3- Delete\n4- Display\n0- Return to main menu\nChoose an operation: \n";
                                cin >> operationChoice;

                                switch (operationChoice) {
                                    case 1: {
                                        cout << "Enter the value to insert: " << '\n';
                                        cin >> value;
                                        quadProbing.insert(value);
                                        continue;
                                    }
                                    case 2: {
                                        cout << "Enter the value to search: \n";
                                        cin >> value;
                                        quadProbing.search(value);
                                        continue;
                                    }
                                    case 3: {
                                        cout << "Enter the value to delete: \n";
                                        cin >> value;
                                        quadProbing.deletion(value);
                                        continue;
                                    }
                                    case 4: {
                                        cout << "Displaying hash table:\n";
                                        quadProbing.display();
                                        continue;
                                    }
                                    case 0:
                                        cout << "Returning to main menu...\n";
                                        continue;
                                    default:
                                        cout << "Invalid choice. Try again.\n";
                                        continue;
                                }
                            }
                            continue;
                        }
                        case 4:
                        {
                            cout << "Enter the size of the hash table: " << '\n';
                            cin >> size;
                            cout << "Enter the secondary hash size: " << '\n';
                            cin >> hash2;
                            Double_Hashing doubleHashing(size, hash2);

                            int operationChoice = -1;
                            while (operationChoice != 0) {
                                cout << "1- Insert\n2- Search\n3- Delete\n4- Display\n0- Return to main menu\nChoose an operation: \n";
                                cin >> operationChoice;

                                switch (operationChoice) {
                                    case 1: {
                                        cout << "Enter the value to insert: " << '\n';
                                        cin >> value;
                                        doubleHashing.insert(value);  // Insert using Double Hashing
                                        continue;
                                    }
                                    case 2: {
                                        cout << "Enter the value to search: \n";
                                        cin >> value;
                                        doubleHashing.search(value);  // Search using Double Hashing
                                        continue;
                                    }
                                    case 3: {
                                        cout << "Enter the value to delete: \n";
                                        cin >> value;
                                        doubleHashing.deletion(value);  // Delete using Double Hashing
                                        continue;
                                    }
                                    case 4: {
                                        cout << "Displaying hash table:\n";
                                        doubleHashing.display();  // Display using Double Hashing
                                        continue;
                                    }
                                    case 0:
                                        cout << "Returning to main menu...\n";
                                        continue;
                                    default:
                                        cout << "Invalid choice. Try again.\n";
                                        continue;
                                }
                            }
                            continue;
                        }
                        case 0:
                            cout << "BACK TO MAIN MENU.." << '\n';
                            break;
                        default:
                            cout << "INVALID CHOICE :(" << '\n';
                            continue;
                    }
                }
                continue;
            }
            case 0:
                cout << "EXITING PROGRAM..." << '\n';
                break;
            default:
                cout << "INVALID CHOICE :(" << '\n';
                continue;
        }
    }

    return 0;
}
