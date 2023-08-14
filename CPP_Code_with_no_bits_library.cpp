#include <windows.h>
#include <string>
#include<list>
#include <iostream>
#include <iomanip>
using namespace std;

class Expression
{
private:
    string expression;                                                  // THE STRING THAT IS GOING TO CONTAIN THE EXPRESSION TO BE PROCESSED, SEPARATELY.
    list <string> expressionElementsContainer;        // THE LIST TO CONTAIN THE DIFFERENT SEGMENTS OF THE EXPRESSION
    list<string> postFixStackBuffer;                         // ALL THIS IS MEANT FOR REMEMBERING THE STEPS
    list<string> postFixStack;                                //  THE STACK TO CONTAIN THE POST FIX REPRESENTATION OF THE EXPRESSION
    list<string> outputStack;                                // THIS STACK TO CONTAIN THE TEMPORARY OUTPUTS

    // FUNCTION TO SEPARATE THE NUMERICS AND SYMBOLS CONTAINED IN THE EXPRESSION AND STORE IN A STACK
    void separateNumericsAndSymbols(string expression) {
        // PUSHING THE EXPRESSION CONTENTS TO STACK ONE BY ONE  ||  Here the top of stack is its first index, so push_start should be used
        string bufferString = "";

        for (auto character : expression) {

            if (isdigit(character)) {
                bufferString += string(1, character);
                continue;
            }

            if (!isdigit(character)) {
                if (!bufferString.empty()) {
                    this->expressionElementsContainer.push_back({ bufferString });
                    bufferString = "";
                }
                bufferString = string(1, character);   // pick this operator
                this->expressionElementsContainer.push_back({ bufferString });
                bufferString = "";
            }


            if (!bufferString.empty()) {
                this->expressionElementsContainer.push_back({ bufferString });
                bufferString = "";
            }
        }

        if (!bufferString.empty()) {
            expressionElementsContainer.push_back({ bufferString });
        }
    }

    // CONSTRUCTING POST FIX STACK
    int getPrecedence(string operatorr) {
        switch (operatorr[0]) {
        case '/':
            return 5;
        case '*':
            return 4;
        case '-':
            return 3;
        case '+':
            return 2;
        case ')':
            return 1;
        default:
            return 0;
        }
    }

    string printAllElementsOfStack(list<string> Stack) {
        string stringBuffer;
        for (auto element : Stack) {
            stringBuffer += element + ", ";
        }
        return stringBuffer.substr(0, stringBuffer.length() - 2);     // the substr is used to remove the last comma in the stack
    }

    void preparePostFixStack() {
        cout << "\nI prepared the postfix expression from your input, as follows :- " << endl
            << setw(5) << "Token     " << setw(15) << "Stack      " << setw(35) << "Output    " << endl;

        Sleep(1000);

        for (auto element : expressionElementsContainer) {
            cout << setw(5) << element;
            Sleep(500);

            // if token is number
            if (isdigit(element[0])) {
                outputStack.push_back(element);
                cout << setw(15) << printAllElementsOfStack(postFixStack) << setw(35) << printAllElementsOfStack(outputStack) << endl;
                Sleep(500);
            }
            else {

                // token is operator and has high precedence

                if (element == "(") {
                    postFixStack.push_back(element);
                    postFixStackBuffer.push_back(element);
                    cout << setw(15) << printAllElementsOfStack(postFixStackBuffer) << setw(35) << printAllElementsOfStack(outputStack) << endl;
                    Sleep(500);
                }
                else if (element == ")") {
                    while (!postFixStack.empty() && postFixStack.back() != "(") {
                        outputStack.push_back(postFixStack.back());
                        postFixStackBuffer.pop_back();
                        postFixStack.pop_back();
                        cout << setw(15) << printAllElementsOfStack(postFixStackBuffer) << setw(35) << printAllElementsOfStack(outputStack) << endl;
                        Sleep(500);
                    }
                    postFixStackBuffer.pop_back(); // Remove the open parenthesis from buffer
                    postFixStack.pop_back(); // Remove the open parenthesis from stack
                    cout << setw(5) << "" << setw(15) << printAllElementsOfStack(postFixStackBuffer) << setw(35) << printAllElementsOfStack(outputStack) << endl;
                    Sleep(500);
                }

                else if (!postFixStack.empty() && (getPrecedence(postFixStack.back()) < getPrecedence(element) || getPrecedence(postFixStack.back()) == getPrecedence(element))) {
                    postFixStack.push_back(element);
                    postFixStackBuffer.push_back(element);
                    cout << setw(15) << printAllElementsOfStack(postFixStackBuffer) << setw(35) << printAllElementsOfStack(outputStack) << endl;
                    Sleep(500);
                }
                else if (postFixStack.empty()) {
                    postFixStack.push_back(element);
                    postFixStackBuffer.push_back(element);
                    cout << setw(15) << printAllElementsOfStack(postFixStackBuffer) << setw(35) << printAllElementsOfStack(outputStack) << endl;
                    Sleep(500);
                }
                else {
                    // token is operator and has low precedence
                    // EMPTY THE POSTFIX STACK UNTILL ALL THE OPERATORS ARRIVE IN THE OUTPUT STACK
                    while (postFixStack.size() > 0) {
                        if (postFixStack.back() != "(") {
                            outputStack.push_back(postFixStack.back());
                        }
                        else {
                            postFixStackBuffer.pop_back();
                            postFixStack.pop_back();
                            cout << setw(15) << printAllElementsOfStack(postFixStackBuffer) << setw(35) << printAllElementsOfStack(outputStack) << endl;
                            Sleep(500);
                            break;
                        }
                        postFixStackBuffer.pop_back();
                        postFixStack.pop_back();
                        cout << setw(15) << printAllElementsOfStack(postFixStackBuffer) << setw(35) << printAllElementsOfStack(outputStack) << endl;
                        Sleep(500);
                    }
                    if (element != "(" && element != ")") {
                        postFixStack.push_back(element);
                        postFixStackBuffer.push_back(element);
                    }
                    cout << setw(5) << "" << setw(15) << printAllElementsOfStack(postFixStackBuffer) << setw(35) << printAllElementsOfStack(outputStack) << endl;
                    Sleep(500);
                }
            }
        }

        // NOW FINALLY CHECK IF THE STACK IS NOT EMPTY AND THEN IF IT IS NOT, TRANSFER ALL THE ELEMENTS FROM THE TOP, TO THE OUTPUT STACK; 
        while (postFixStack.size() > 0) {
            if (postFixStack.back() != "(" && postFixStack.back() != ")") {
                outputStack.push_back(postFixStack.back());
            }
            postFixStack.pop_back();
            cout << setw(5) << "" << setw(15) << printAllElementsOfStack(postFixStack) << setw(35) << printAllElementsOfStack(outputStack) << endl;
            Sleep(500);
        }

        // THIS WAS FOR DEBUGGING PURPOSES

        // cout << "\n\nI prepared this postfix stack from your expression : [ ";
        // for ( auto element : outputStack ) {
        //     cout << element << " | ";
        // }
        // cout << " ]";


    }

public:
    void setExpressionFromString(string express) {
        this->expression = express;
        separateNumericsAndSymbols(express);
        preparePostFixStack();
    }

    void displayThePostfixMade() {

        cout << endl << endl;
        for (auto element : outputStack) {
            cout << element << " ";
        }
        cout << " is the postfix expression I obtained.";
    }

    void solveTheMadePostfix() {
        list <string> resultStack;
        // Sleep(1300);
        cout << "\n\nSolving the postfix expression - \n" << setw(5) << "Token" << setw(27) << "Output Stack" << endl;
        // Sleep(1000);
        for (auto element : outputStack) {
            if (isdigit(element[0])) {
                resultStack.push_back(element);
                cout << setw(5) << element << setw(27) << printAllElementsOfStack(resultStack) << endl;
                Sleep(500);
            }
            else {
                cout << setw(5) << element << setw(27) << printAllElementsOfStack(resultStack) << endl;
                Sleep(500);
                if (!resultStack.empty() && resultStack.size() >= 2) {
                    int a = stof(resultStack.back());
                    resultStack.pop_back();
                    int b = stof(resultStack.back());
                    resultStack.pop_back();
                    switch (element[0]) {
                    case '*':
                        resultStack.push_back(to_string(a * b));
                        break;
                    case '+':
                        resultStack.push_back(to_string(a + b));
                        break;
                    case '-':
                        resultStack.push_back(to_string(b - a));
                        break;
                    case '/':
                        resultStack.push_back(to_string(b / a));
                        break;
                    case '^':
                        resultStack.push_back(to_string(pow(b, a)));
                        break;
                    }
                }
                cout << setw(5) << "" << setw(27) << printAllElementsOfStack(resultStack) << endl;
                Sleep(500);
            }
        }
        cout << "\n\nThe answer is : ";
        Sleep(800);
        cout << setprecision(2) << resultStack.back();
    }
};

int main(void) {
    // START UP PROMPT
    cout << "\nTHIS PROGRAM IMPLEMENTS POSTFIX CONVERTER AND CALCULATOR..\nIt shows how a mechanical calculator converts the expressions given and solves them.\n\n>> By Ashwin Sharma" << endl << "Enter the infix (normal : a+b-c) expression : ";

    // TAKING INPUT OF THE EXPRESSION
    string expression;
    getline(cin, expression);

    Expression expressionHolder;
    expressionHolder.setExpressionFromString(expression);
    expressionHolder.displayThePostfixMade();
    expressionHolder.solveTheMadePostfix();

    cout << "\nHit Enter to Exit...";
    string useless;
    getline(cin, useless);
    return 0;   // --> executed with no errors
}
