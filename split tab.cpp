#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;

// Person class to represent each individual
class Person
{
public:
    string name;
    double amountPaid;
    double balance;
    Person *next;

    Person(string n, double amt)
    {
        name = n;
        amountPaid = amt;
        balance = 0;
        next = nullptr;
    }
};

// Function to open an input file
void openInputFile(ifstream &inFile)
{
    string filename;
    cout << "Enter the filename? ";
    cin >> filename;
    inFile.open(filename);
    while (!inFile)
    {
        cout << "Bad filename" << endl;
        cout << "Try again? ";
        cin >> filename;
        inFile.clear();
        inFile.open(filename);
    }
}

int main()
{
    double total = 0;
    ifstream inFile;
    openInputFile(inFile);

    // Linked list setup
    Person *head = nullptr; // Start of the list
    Person *tail = nullptr; // End of the list

    double amount;
    string line, name;

    // Reading data into the linked list
    while (inFile >> amount)
    {
        getline(inFile, line); // Get rest of line for name
        name = line.substr(1); // Remove leading space

        // Create a new person node
        Person *newPerson = new Person(name, amount);
        total += amount;

        // Add the person to the linked list
        if (!head)
        {
            head = newPerson; // First node
            tail = newPerson;
        }
        else
        {
            tail->next = newPerson; // Link to the next person
            tail = newPerson;       // Move the tail
        }
    }

    // Calculate the average
    int count = 0;
    Person *current = head;
    while (current)
    {
        count++;
        current = current->next;
    }
    double average = total / count;

    // Calculate balances for each person
    current = head;
    while (current)
    {
        current->balance = average - current->amountPaid;
        current = current->next;
    }

    // Print balances and payments
    current = head;
    while (current)
    {
        cout << current->name << " paid $" << current->amountPaid << endl;
        current = current->next;
    }

    cout << "In the end, you should all have spent $" << total << endl;

    // Handle payments
    current = head;
    while (current)
    {
        if (abs(current->balance) < 0.01)
        {
            cout << current->name << ", you don't need to do anything" << endl;
        }
        current = current->next;
    }

    current = head;
    while (current)
    {
        if (current->balance > 0.01)
        {
            Person *receiver = head;
            while (receiver)
            {
                if (receiver->balance < -0.01)
                {
                    double payment = min(current->balance, -receiver->balance);
                    cout << current->name << " needs to pay " << receiver->name
                         << " $" << payment << endl;
                    current->balance -= payment;
                    receiver->balance += payment;
                    if (current->balance < 0.01)
                        break;
                }
                receiver = receiver->next;
            }
        }
        current = current->next;
    }
    cout << "In the end, all you should have spent is $" << average << endl;

    // Free the linked list memory
    current = head;
    while (current)
    {
        Person *temp = current;
        current = current->next;
        delete temp;
    }
}
/*
Let me explain this in a simpler way with a real-world example:

Imagine you're typing this into your program:
John Smith[Enter]

What actually gets sent to the computer is:
"John Smith\n"    (where \n is the Enter/Return key)

When you use getline(), it will only store:
"John Smith"      (notice the \n is gone)

This is like if you wrote a note on paper:
- The actual paper has "Hello" and then a crease where you folded it
- getline() only copies the "Hello" part, not the fold

This is different from cin >>, which:
1. Reads input up to a space or newline
2. Leaves everything else (including the \n) still waiting to be read

That's why if you do:
cin >> firstName;      // User types "John[Enter]"
getline(cin, address); // This will read an empty line because the \n is still there

The \n from the [Enter] key is still waiting to be read after cin >>,
but getline() won't include it in what it stores.
*/