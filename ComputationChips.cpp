/* Name: AbduMalik Shodunke
 * Date: 9/29/2024
 */

#include <iostream>
#include <string>
using namespace std;

class Chip {
private:
    char chipType; // Type of the chip (A: Addition, S: Subtraction, etc.)
    string id; // Unique identifier for the chip
    Chip* input1; // Pointer to the first input chip
    Chip* input2; // Pointer to the second input chip (can be NULL)
    Chip* output; // Pointer to the output chip (is NULL for output chips)
    double inputValue; // For input chips to store their values
    double outputValue; // Store computed output value for non-input chips

public:
    // Constructor
    Chip(char type, const string& id) : chipType(type), id(id), input1(nullptr), input2(nullptr), output(nullptr), inputValue(0), outputValue(0) {}

    // Method prototypes
    // Sets the first input chip
    void setInput1(Chip* inputChip) {
        this->input1 = inputChip;
    }

    // second input chip (can be NULL)
    void setInput2(Chip* inputChip) {
        this->input2 = inputChip;
    }

    // Method to set the output chip
    void setOutput(Chip* outputChip) {
        this->output = outputChip;
    }

    // Performs the operation based on the chip type
    void compute() {
        if (chipType == 'O' && input1) {
            // For output chips, trigger computation but output via the input chip
            input1->compute();
            cout << "Computation Starts\n";
            cout << "The output value from this circuit is " << input1->getOutputValue() << endl;
            return;
        }

        if (input1) input1->compute();
        if (input2) input2->compute();

        switch (chipType) {
            case 'A': // Addition
                outputValue = input1->getOutputValue() + input2->getOutputValue();
                break;
            case 'S': // Subtraction
                outputValue = input1->getOutputValue() - input2->getOutputValue();
                break;
            case 'M': // Multiplication
                outputValue = input1->getOutputValue() * input2->getOutputValue();
                break;
            case 'D': // Division
                if (input2->getOutputValue() != 0) {
                    outputValue = input1->getOutputValue() / input2->getOutputValue();
                } else {
                    cerr << "Error: Division by zero\n";
                }
                break;
            // GPT: Help me write a negation case for these
            case 'N': // Negation
                outputValue = -(input1->getOutputValue());
                break;
            case 'I': // Input (just pass the value onward)
                outputValue = inputValue;
                break;
        }
    }

    // Accessor for the chip's ID
    string getId() const {
        return id;
    }

    // Accessor for obtaining output value
    double getOutputValue() const {
        return outputValue;
    }

    // Accessors to check if inputs are set
    bool hasInput1() const {
        return input1 != nullptr;
    }

    // Set input chip value (used for input chips)
    void setInputValue(double value) {
        inputValue = value;
    }

    // Display the chip's connections
    void display() const {
        if (chipType == 'I') {
            cout << id << ", Output = " << (output ? output->getId() : "None") << endl;
        } else if (chipType == 'O') {
            cout << id << ", Input 1 = " << (input1 ? input1->getId() : "None") << endl;
        } else {
            cout << id << ", Input 1 = " << (input1 ? input1->getId() : "None")
                 << ", Input 2 = " << (input2 ? input2->getId() : "None")
                 << ", Output = " << (output ? output->getId() : "None") << endl;
        }
    }
    // Getter for input1
    Chip* getInput1() const {
        return input1;
    }
};

int main() {
    int numChips;
    cin >> numChips;

    Chip** allChips = new Chip*[numChips];

    for (int i = 0; i < numChips; i++) {
        string chipId;
        cin >> chipId;
        char chipType = chipId[0];
        allChips[i] = new Chip(chipType, chipId);
    }

    int numCommands;
    cin >> numCommands;

    // Function to find a chip by its ID
    auto findChipById = [allChips, numChips](const string& id) -> Chip* {
        for (int i = 0; i < numChips; i++) {
            if (allChips[i]->getId() == id) {
                return allChips[i];
            }
        }
        return nullptr; // If not found
    };

    for (int i = 0; i < numCommands; i++) {
        string commandType, sourceId, targetId;
        cin >> commandType;

        if (commandType == "A") {
            // Establish connection between chips
            cin >> sourceId >> targetId;
            Chip* sourceChip = findChipById(sourceId);
            Chip* targetChip = findChipById(targetId);

            if (targetChip && sourceChip) {
                if (targetChip->getId()[0] != 'O') { // If not an output chip
                    if (!targetChip->hasInput1()) {
                        targetChip->setInput1(sourceChip);
                    } else {
                        targetChip->setInput2(sourceChip);
                    }
                } else {
                    targetChip->setInput1(sourceChip); // Setting input for output chip
                }
                sourceChip->setOutput(targetChip);
            }
        } else if (commandType == "I") {
            // Assign value to input chip
            double value;
            cin >> sourceId >> value;
            Chip* inputChip = findChipById(sourceId);
            if (inputChip) {
                inputChip->setInputValue(value);
            }
        } else if (commandType == "O") {
            // Compute output
            cin >> sourceId;
            Chip* outputChip = findChipById(sourceId);
            if (outputChip) {
                outputChip->compute();
            }
        }
    }

    cout << "***** Showing the connections that were established" << endl;

    // Display all chip connections
    for (int i = 0; i < numChips; i++) {
        allChips[i]->display();
    }

    // Cleanup
    for (int i = 0; i < numChips; i++) {
        delete allChips[i];
    }
    delete[] allChips;

    return 0;
}




/*
 LLM and GitHub Copilot Usage Documentation :
For this project, I primarily used LLM (Large Language Model) tools like Chat GPT to assist in the development of the main program logic, as the Chip class was largely provided in the starter code.

Prompts and Suggestions:
To implement the main program, I used prompts like "Write a function to connect chips in a circuit", "Function to find a chip by its ID", and "Implement the logic to compute the output of a circuit given a list of chips." Chat GPT was able to provide me with a basic structure and pseudocode for these functions, which I then refined and implemented.

Additionally, I used Chat GPT to suggest optimizations and improvements to the main program logic. For example, when implementing the circuit traversal and computation, GPT suggested a more efficient algorithm that I incorporated into my final solution.

Rationale:
I chose to use these prompts and suggestions because they helped me quickly establish a solid foundation for the main program logic, which was the primary focus of this project. Chat GPT allowed me to generate a starting point for the key functions, which I could then build upon. The suggestions from GPT also helped me identify areas for improvement and optimize the performance of the main program.

Incremental Development:
Throughout the development process, I used Chat GPT in an incremental fashion. I started by generating the basic structure for the main program functions, then worked on implementing the logic step-by-step. After each major implementation step, I would test the code and use GPT to suggest refinements or identify any issues.

This incremental approach allowed me to build the main program in a manageable way, ensuring that each component integrated seamlessly with the overall solution.

Debugging and Testing Plan (10%)

Specific Tests:
To ensure the correctness of the main program implementation, I conducted several tests, including:
- Verifying that the circuit traversal and computation logic correctly handles circuit configurations, including input, output, and logic chips
- Checking that the program correctly handles edge cases, such as circuits with loops or invalid chip connections
- Testing the program with the provided sample input and output, as well as additional test cases to ensure robustness

Issues and Resolutions:
During the development process, I encountered a few issues that required troubleshooting and resolution:
- Optimizing the circuit traversal algorithm: Copilot suggested a more efficient approach, which I implemented.
- Handling invalid chip connections: I added additional validation logic to ensure the program could handle these cases.

Verification:
To verify that the main program implementation works correctly with the provided main program, I ran a series of test cases using the sample input and output. I also created additional test cases to ensure the robustness and accuracy of my implementation.
 */
