

#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string>

using namespace std;

// Let's define our system parameters as constants
// This makes it easy to modify if needed
const int NUM_PROCESSES = 5;
const int NUM_RESOURCES = 3;

// Function to display a matrix in a nice, readable format
// Because we humans like to see data organized in tables!
void displayMatrix(const string& title, vector<vector<int>>& matrix) {
    cout << "\n" << title << ":" << endl;
    cout << "     A  B  C" << endl;  // Resource headers
    cout << "   ----------" << endl;
    
    for (int i = 0; i < NUM_PROCESSES; i++) {
        cout << "P" << i << " | ";
        for (int j = 0; j < NUM_RESOURCES; j++) {
            cout << matrix[i][j] << "  ";
        }
        cout << endl;
    }
}

// Function to display a vector (1D array) nicely
void displayVector(const string& title, vector<int>& vec) {
    cout << "\n" << title << ": ";
    for (int i = 0; i < NUM_RESOURCES; i++) {
        cout << vec[i] << " ";
    }
    cout << endl;
}

// The heart of our program - the Banker's Algorithm
// Returns true if the system is safe, false otherwise
bool isSafeState(vector<vector<int>>& allocation, 
                 vector<vector<int>>& max_need,
                 vector<int>& available,
                 vector<int>& safeSequence) {
    
    // First, let's calculate the Need matrix
    // Need = Max - Allocation (what each process still needs)
    vector<vector<int>> need(NUM_PROCESSES, vector<int>(NUM_RESOURCES));
    
    cout << "\n=== Calculating Need Matrix ===" << endl;
    cout << "Need = Max - Allocation (resources still needed by each process)" << endl;
    
    for (int i = 0; i < NUM_PROCESSES; i++) {
        for (int j = 0; j < NUM_RESOURCES; j++) {
            need[i][j] = max_need[i][j] - allocation[i][j];
        }
    }
    
    displayMatrix("Need Matrix", need);
    
    // Create a work vector - this represents resources available at each step
    // Initially, it's the same as available resources
    vector<int> work(NUM_RESOURCES);
    for (int i = 0; i < NUM_RESOURCES; i++) {
        work[i] = available[i];
    }
    
    // Keep track of which processes have finished
    // Think of this as a checklist - we mark off processes as they complete
    vector<bool> finish(NUM_PROCESSES, false);
    
    // This will store our safe sequence
    int safeIndex = 0;
    
    cout << "\n=== Starting Banker's Algorithm ===" << endl;
    cout << "Looking for a safe sequence of process execution..." << endl;
    
    // Keep trying to find processes that can run
    // We'll keep looping until we can't find any more processes to run
    int count = 0;
    while (count < NUM_PROCESSES) {
        // Flag to check if we found a process in this iteration
        bool found = false;
        
        // Check each process
        for (int p = 0; p < NUM_PROCESSES; p++) {
            // Skip if this process is already finished
            if (finish[p]) {
                continue;
            }
            
            // Check if this process can run with current available resources
            // A process can run if its needs can be satisfied
            bool canRun = true;
            for (int j = 0; j < NUM_RESOURCES; j++) {
                if (need[p][j] > work[j]) {
                    canRun = false;
                    break;
                }
            }
            
            // If this process can run, let it!
            if (canRun) {
                cout << "\n-> Process P" << p << " can execute!" << endl;
                cout << "   Current Work (available): ";
                for (int k = 0; k < NUM_RESOURCES; k++) {
                    cout << work[k] << " ";
                }
                cout << endl;
                
                // When the process finishes, it releases its allocated resources
                // So we add them back to our available pool
                for (int k = 0; k < NUM_RESOURCES; k++) {
                    work[k] += allocation[p][k];
                }
                
                cout << "   After P" << p << " completes, Work becomes: ";
                for (int k = 0; k < NUM_RESOURCES; k++) {
                    cout << work[k] << " ";
                }
                cout << endl;
                
                // Mark this process as finished and add to safe sequence
                safeSequence[safeIndex++] = p;
                finish[p] = true;
                found = true;
                count++;
            }
        }
        
        // If we couldn't find any process to run in this iteration,
        // we're stuck - the system is not in a safe state!
        if (!found) {
            cout << "\n!!! No process can execute with current resources !!!" << endl;
            cout << "System is NOT in a safe state - potential deadlock!" << endl;
            return false;
        }
    }
    
    // If we got here, all processes completed successfully!
    cout << "\n=== All processes can complete successfully! ===" << endl;
    return true;
}

// Function to read input data from a file
// Returns true if successful, false otherwise
bool readInputFile(const string& filename,
                   vector<vector<int>>& allocation,
                   vector<vector<int>>& max_need,
                   vector<int>& available) {
    
    ifstream inputFile(filename);
    
    if (!inputFile.is_open()) {
        cout << "Error: Could not open file " << filename << endl;
        return false;
    }
    
    cout << "\nReading input from file: " << filename << endl;
    
    // Read allocation matrix
    cout << "Reading Allocation Matrix..." << endl;
    for (int i = 0; i < NUM_PROCESSES; i++) {
        for (int j = 0; j < NUM_RESOURCES; j++) {
            inputFile >> allocation[i][j];
        }
    }
    
    // Read max need matrix
    cout << "Reading Max Matrix..." << endl;
    for (int i = 0; i < NUM_PROCESSES; i++) {
        for (int j = 0; j < NUM_RESOURCES; j++) {
            inputFile >> max_need[i][j];
        }
    }
    
    // Read available resources
    cout << "Reading Available Resources..." << endl;
    for (int i = 0; i < NUM_RESOURCES; i++) {
        inputFile >> available[i];
    }
    
    inputFile.close();
    cout << "Input file read successfully!" << endl;
    return true;
}

int main() {
    // Print a nice header for our program
    cout << "================================================" << endl;
    cout << "     BANKER'S ALGORITHM IMPLEMENTATION         " << endl;
    cout << "     Deadlock Avoidance System Checker         " << endl;
    cout << "================================================" << endl;
    
    // Initialize our data structures
    vector<vector<int>> allocation(NUM_PROCESSES, vector<int>(NUM_RESOURCES));
    vector<vector<int>> max_need(NUM_PROCESSES, vector<int>(NUM_RESOURCES));
    vector<int> available(NUM_RESOURCES);
    vector<int> safeSequence(NUM_PROCESSES);
    
    // Read input from file
    if (!readInputFile("input.txt", allocation, max_need, available)) {
        cout << "Failed to read input file. Exiting..." << endl;
        return 1;
    }
    
    // Display the input data so we can see what we're working with
    cout << "\n=== Current System State ===" << endl;
    displayMatrix("Allocation Matrix", allocation);
    displayMatrix("Max Matrix", max_need);
    displayVector("Available Resources", available);
    
    // Display total resources in the system
    cout << "\n=== System Resources ===" << endl;
    cout << "Total instances: A=10, B=5, C=7" << endl;
    
    // Now let's run the Banker's Algorithm!
    cout << "\n================================================" << endl;
    cout << "     RUNNING BANKER'S ALGORITHM                " << endl;
    cout << "================================================" << endl;
    
    if (isSafeState(allocation, max_need, available, safeSequence)) {
        // Yay! The system is safe
        cout << "\n================================================" << endl;
        cout << "           RESULT: SYSTEM IS SAFE!             " << endl;
        cout << "================================================" << endl;
        
        cout << "\nSafe Sequence Found: < ";
        for (int i = 0; i < NUM_PROCESSES; i++) {
            cout << "P" << safeSequence[i];
            if (i < NUM_PROCESSES - 1) {
                cout << " -> ";
            }
        }
        cout << " >" << endl;
        
        cout << "\nThis means processes can execute in this order without deadlock!" << endl;
    } else {
        // Oh no! The system is not safe
        cout << "\n================================================" << endl;
        cout << "        RESULT: SYSTEM IS NOT SAFE!            " << endl;
        cout << "================================================" << endl;
        cout << "\nThe system may enter a deadlock state!" << endl;
        cout << "Resource allocation should be reconsidered." << endl;
    }
    
    cout << "\n================================================" << endl;
    cout << "Program execution completed successfully!" << endl;
    
    return 0;
}
