#include <iostream>
#include <vector>
#include <cmath>
#include <string>
using namespace std;

// Function to hash a key using the Division Method
int hashDivision(int key, int tableSize) {
    return key % tableSize;
}

// Function to hash a key using the Multiplication Method
int hashMultiplication(int key, int tableSize) {
    const double A = 0.6180339887;
    // Implement the formula: floor(m * (key * A mod 1))
    double fractionalPart = fmod(key * A, 1.0); // (key * A mod 1)
    return static_cast<int>(floor(tableSize * fractionalPart)); // floor(m * fractionalPart)
}

// Fold Shifting Hashing method
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

// Fold Boundary Hashing method
unsigned int foldBoundaryHash(const string &key, unsigned int boundary) {
    unsigned int hashValue = 0;
    int partSize = 2;

    unsigned int parts[4];

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

// Mid-Square Method
int midSquareMethod(int key, int tableSize) {
    long long squared = key * key;
    cout << "\nSquared value: " << squared << endl;
    string squaredStr = to_string(squared);
    int midStart = squaredStr.length() / 2 - 1;
    string midDigits = squaredStr.substr(midStart, 2);  // Extract the middle digits
    cout << "Middle digits: " << midDigits << endl;

    int midHashValue = stoi(midDigits);  // Convert middle digits to an integer
    return midHashValue % tableSize;     // Ensure it falls within the table size
}

int main() {
    int choice = -1, tableSize, numKeys, key;

    while (choice != 0){
        cout << "Hashing Techniques Menu:"              << '\n'
             << "1- Division Method."                   << '\n'
             << "2- Multiplication Method."             << '\n'
             << "3- Fold-Shifting & Boundary Hashing."  << '\n'
             << "4- Mid-Square Method"                  << '\n'
             << "ENTER 0 TO EXIT THE PROGRAM..."        << '\n'
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
                cout << "EXITING PROGRAM.." << '\n';
                break;
            default:
                cout << "INVALID CHOICE :(" << '\n';
                continue;
        }
    }

    return 0;
}
