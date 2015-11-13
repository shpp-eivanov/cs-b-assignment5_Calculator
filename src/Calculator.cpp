#include <iostream>
#include <cmath>
#include "console.h"
#include "simpio.h"
#include "tokenscanner.h"
#include "myStack.h"

using namespace std;

/* Functions prototypes */
void scanCPlusPlusTokens(TokenScanner & scanner);

double formulaProcess(string formula, double xValue);
double formulaStringScanning(TokenScanner& scanner,                            
                             bool &bracketsOpened,
                             double xValue);
void sortTokenByStacks(string token,
                       MyStack<double>& stackNumbers,
                       MyStack<string>& stackOperators);
double singleCalculation(double num1, string oper, double num2);
double getFinalStacksResult(MyStack<double> stackNumbers,
                       MyStack<string> stackOperators);
void twoNumsProcess(MyStack<double>& numStack,
                    MyStack<string>& operStack);
int getOperPrecedence(string oper);

double powFunction(TokenScanner& scanner);
double sqrtFunction(TokenScanner& scanner, double xValue);

//Global variable
bool failFlag = false;

// Console settings
void consoleSettings();
const string programTitle = "Calculator";
const bool consoleEcho = false;

int main() {
    /*  */
    consoleSettings();
    /* User input  */
    string formula = getLine("Enter your formula: ");
    // string formula = "x + 6";
    /* Value for x-variable for current formula
     * It might be useful for equation greaphics */
    double xValue = 0;

    /* Simple calculation process */
    double result = formulaProcess(formula, xValue);
    cout << "YOUR RESULT IS " << result;
    return 0;
}

/* Create simple formula calculation process */
double formulaProcess(string formula, double xValue){
    TokenScanner scanner(formula);
    scanCPlusPlusTokens(scanner);
    bool bracketsOpenedFlag = false;//Are were brackets opened before function call
    double result = formulaStringScanning(scanner,
                                          bracketsOpenedFlag,
                                          xValue);
    if(failFlag){
        /* Shows if there were some errors due to calculation process */
        cout << "CALCULATION FAULT!" << endl;
    }
    return result;
}

/* Recursively scanns formula and sorts tokens through
 * stacks due to shunting-yard algorithm */
double formulaStringScanning(TokenScanner& scanner,
                             bool &bracketsOpenedBefore,
                             double xValue){
    if(failFlag){return 0;}
    else{
        MyStack<double> stackNumbers;
        MyStack<string> stackOperators;
        string token = "";
        bool bracketsOpenedHere = false;
        while(scanner.hasMoreTokens()){
            token = scanner.nextToken();
            if(token == "x"){
                /* Changes x-token by user xValue  */
                stackNumbers.push(xValue);
            }
            else if(token == "pow"){
                /* Solves library pow function  */
                stackNumbers.push(powFunction(scanner));
            }else if(token == "sqrt"){
                /* Solves library sqrt function  */
                stackNumbers.push(sqrtFunction(scanner, xValue));
            }else{
                    /* Brackets case */
                    if(token == "("){
                        bracketsOpenedHere = true;
                        /* Make new formula recursion */
                        stackNumbers.push(formulaStringScanning(scanner,
                                                                bracketsOpenedHere,
                                                                xValue));
                    }else if(token == ")"){
                            if(bracketsOpenedBefore){
                                /* It is just close brackets expression */
                                bracketsOpenedBefore = false;//it's end of this recursion
                                break;//Break to jump to end of calculation
                            }else{
                                /* This calculation start without opened brackets */
                                cout <<  "NOT OPENED BRACKETS! " << endl;
                                failFlag = true;
                                break;//Break to show  error to user
                            }
                    }else{
                        if(failFlag)break;//don't do any calculations
                        sortTokenByStacks(token, stackNumbers, stackOperators);
                    }
            }//end else statement
        }//end of while(scanner.hasMoreTokens())

        /* End of process */
        if(bracketsOpenedBefore){//Simple checking are brackets closed here
             cout <<  "NOT CLOSED BRACKETS! " << endl;
             failFlag = true;
             return 0;
        }else{
            if(failFlag){
                return 0;
            }else{
                 /* End of process calculation */
                return getFinalStacksResult(stackNumbers, stackOperators);
            }
        }
    }
}

/* Sort current tokens through
 * stacks due to shunting-yard algorithm */
void sortTokenByStacks(string token,
                MyStack<double> &stackNumbers,
                MyStack<string> &stackOperators){
    if(stringIsDouble(token)){ /* Token is number */
        double num = stringToDouble(token);
        stackNumbers.push(num);
    }else{/* Token is operator or ")" */
        /* If operators stack empty - push there without conditions */
        if(stackOperators.isEmpty()){
            stackOperators.push(token);
        }else{
            string topOper = stackOperators.peek();
            if(getOperPrecedence(topOper) < getOperPrecedence(token)){
                /* Last operator in stack precednce  is
                 * weaker then this token operator */
                stackOperators.push(token);
            }else{
                /* Last operator in stack precednce  is higher -
                 * evaluate two last numbers from stack */
                if(!failFlag){
                 twoNumsProcess(stackNumbers, stackOperators);
                 sortTokenByStacks(token, stackNumbers, stackOperators);
                }
            }
        }
    }
}

/* End of program function - returns result of end stacks values processing */
double getFinalStacksResult(MyStack<double> stackNumbers,
                       MyStack<string> stackOperators){
    /* "End of string" or brackets closed process */
    while(!stackOperators.isEmpty()){
        if(failFlag) {
            break;
        }
        /* End of calculation process  */
        twoNumsProcess(stackNumbers, stackOperators);
    }
    /* End result returning */
    if(!stackNumbers.isEmpty()){
        return stackNumbers.pop();
    }else{
        return 0;
    }
}

/* Library pow function execution
 * Believe that it's normalized function - like pow(1,9)
 * without internal brackets and evaquations */
double powFunction(TokenScanner& scanner){
    string token = "";
    string num1 = "";
    string num2 = "";
    while(scanner.hasMoreTokens()){
        token = scanner.nextToken();
        if(token == "("){
            num1 = scanner.nextToken();//pow function param
            token = scanner.nextToken();//waiting coma at this place
            if(token == ","){
                num2 = scanner.nextToken();//pow function param
                token = scanner.nextToken();//waiting for brackets at this place
                if(token == ")"){
                    break;
                }
            }
        }
    }
    /* Calculate pow function result */
    return pow(stringToDouble(num1), stringToDouble(num2));
}

/* Library sqrt function execution
 * Resolves equation after "sqrt" token and returns value of
 * sqrt library function for obtained param */
double sqrtFunction(TokenScanner& scanner, double xValue){
    string token = "";
    double num1 = 0;
    while(scanner.hasMoreTokens()){
        token = scanner.nextToken();
        if(token == "("){
            bool bracketsOpened = true;
            num1 = formulaStringScanning(scanner,
                                         bracketsOpened,
                                         xValue);
            break;
        }
    }
    return sqrt((num1));
}

/* Takes to last digits and one last operator
 * Returns operation result */
void twoNumsProcess(MyStack<double> &numStack, MyStack<string> &operStack){
    /* Stacks elements validation checking for calculating process */
    if((numStack.size() - operStack.size()) != 1){       
        cout << "CHECK YOUR ENTER!" << endl;
        failFlag = true;
    }else{
        /* Calculating process */
        double num2 = numStack.pop();
        double num1 = numStack.pop();
        string thisOper = operStack.pop();
        double result = singleCalculation(num1, thisOper, num2);
        numStack.push(result);
    }
}

/* Returns precedence of operators to use in shunting-yard algorithm */
int getOperPrecedence (string oper){
    int result = 0;
    if(oper == "-")result = 0;
    else if(oper == "+")result = 0;
    else if(oper == "*")result = 1;
    else if(oper == "/")result = 1;
    else if(oper == "%")result = 1;
    else if(oper == "^")result = 2;
    else if(oper == "pow")result = 2;
    else if(oper == "sqrt")result = 2;
    else{
        failFlag = true;
        cout << "UNKNOWN OPERATOR" << endl;
    }
    return result;
}

/* Executes single operation with two numbers */
double singleCalculation(double num1, string oper, double num2){
    double result = 0;
    if(oper == "+"){
        result = num1 + num2;
    }else if(oper == "-"){
        result = num1 - num2;
    }else if(oper == "*"){
        result = num1 * num2;
    }else if(oper == "/"){
        result = num1 / num2;
    }else if(oper == "%"){
        result = (int)num1 % (int)num2;
    }else if(oper == "^"){
        result = pow(num1, num2);
    }else if(oper == "pow"){
        result = pow(num1, num2);
    }else{
        cout << "OPERATOR FAIL!" << endl;
        result = 0;
    }
    return result;
}

/* Sets token scanner features */
void scanCPlusPlusTokens(TokenScanner & scanner) {
    scanner.ignoreWhitespace();
    /* Return numbers */
    scanner.scanNumbers();
    /* To recognize symbol "..." like end of sentence */
    scanner.addOperator("%");
    scanner.addOperator("/");
    scanner.addOperator("*");
    scanner.addOperator("(");
    scanner.addOperator(")");
}

/* Make console output window more convenient */
void consoleSettings(){
    setConsoleWindowTitle(programTitle);
    setConsoleEcho(consoleEcho);
}
