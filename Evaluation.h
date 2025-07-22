#include "Stack.h"
#include "LinkedList.h"
#include <math.h>

typedef std::string string ;

class Eval {
private:
    string currentExpression;
    double currentResult;
    Stack<string> *stack;
    LinkedList<string> *list;
    LinkedList<Pair*> *logs;

public:
    Eval() {
        currentExpression = "";
        currentResult = 0;
        stack = new Stack<string>();
        list = new LinkedList<string>();
        logs = new LinkedList<Pair*>();
    }

    void setExpression(string raw) {
        currentExpression = raw;
        list = infixToPostfix(currentExpression);
        currentResult = DoEval();
        Pair *newLog = new Pair(raw, currentResult);
        logs->append(newLog);
    }

    double getResult() { return currentResult; }

    int logSize() {
        return logs->getSize();
    }

    void showLogs() { 
        Node<Pair*> *temp = logs->getHead();
        int count = 1;
        while(temp) {
            std::cout << count << ": " << temp->getData()->getData() << std::endl;
            temp = temp->getNext();
            count++;
        }
    }

    double DoEval() {
        string symbol = "(+-*/^";
        Stack<double> *stack2 = new Stack<double>(); 
        Node<string> *temp = list->getHead();
        list->print();
        while(temp) {
            // std::cout << "data: " << temp->getData() << "\n";
            if(isSubstring(symbol, temp->getData())) {
                // std::cout << "Do what ";
                double num1, num2;
                num2 = stack2->peek();
                stack2->pop();
                num1 = stack2->peek();
                // std::cout << "num1 ... num2: " << num1 << " " << temp->getData() << " " << num2 << std::endl;
                stack2->pop();
                stack2->push(doArithmetic(num1, num2, temp->getData()));
                // std::cout << "num3: " << stack2->peek() << std::endl;
            }
            else {
                stack2->push(stringToDouble(temp->getData()));
            }
            temp = temp->getNext();
        }
        return stack2->peek();
    }

    LinkedList<string> *infixToPostfix(string raw) {
        int length = raw.length(), i = 0;
        stack->clear(), list->clear();
        Stack<string> *negativeLogicStack = new Stack<string>();
        LinkedList<string> *res = new LinkedList<string>();
        string symbol = "(+-*/^";
        int precedence[] = {0, 1, 2, 3, 4, 5};
        while(i < length) {
            if(isNumber(raw[i])) {
                string number = extractNumber(raw, i);
                i += number.length();
                res->append(number);
            }
            else if(raw[i] == ' ') {
                i++; 
                continue;
            }
            else {
                // logic for - sign
                if(raw[i] == '-') {
                    for(int j = i + 1; j < length; j++) {
                        if(raw[j] == ' ') continue;
                        if(isNumber(raw[j])) {
                            string number = extractNumber(raw, j);
                            number = "-" + number;
                            bool logic = res->isEmpty();
                            res->append(number);
                            if(!logic) res->append("+");
                            i = j + number.length() - 1;
                            break;
                        }
                        else {
                            negativeLogicStack->push("+");
                            negativeLogicStack->push("*");
                            negativeLogicStack->push("-1");
                            i += 1;
                            break;
                        }
                    }
                    continue;
                }

                if(stack->isEmpty() || raw[i] == '(') {
                    // std::cout << "case 1: " << raw[i] << "\n";
                    stack->push(charToStr(raw[i]));
                }
                else if(raw[i] == ')') {
                    // std::cout << "case 2: " << raw[i] << "\n";
                    while(!stack->isEmpty() && stack->peek() != "(") {
                        res->append(stack->peek());
                        stack->pop();
                    }
                    stack->pop();
                    if(!negativeLogicStack->isEmpty()) {
                        for(int count = 0; count < 3; count++) {
                            res->append(negativeLogicStack->peek());
                            negativeLogicStack->pop();
                        }
                    }
                }
                else if(precedence[indexOf(symbol, raw[i])] >= precedence[indexOf(symbol, stack->peek()[0])]) {
                    // std::cout << "case 3: " << raw[i] << "\n";
                    // std::cout << raw[i] << " " << stack->peek() << std::endl;
                    stack->push(charToStr(raw[i]));
                }
                else {
                    // std::cout << "case 4: " << raw[i] << "\n";
                    while(!stack->isEmpty() && stack->peek() != "(" && (precedence[indexOf(symbol, raw[i])] < precedence[indexOf(symbol, stack->peek()[0])])) {
                        // std::cout << "peek: " << stack->peek() << "\n";
                        res->append(stack->peek());
                        stack->pop();
                    }
                    stack->push(charToStr(raw[i])); 
                }
                i++;
            }
        }
        while(!stack->isEmpty()) {
            res->append(stack->peek());
            stack->pop();
        }
        return res;
    }

    double doArithmetic(string s1, string s2, string symbol) {
        double res = 0;
        if(symbol == "+") res = stringToDouble(s1) + stringToDouble(s2);
        else if(symbol == "-") res = stringToDouble(s1) - stringToDouble(s2);
        else if(symbol == "*") res = stringToDouble(s1) * stringToDouble(s2);
        else if(symbol == "/") res = stringToDouble(s1) / stringToDouble(s2);
        else if(symbol == "^") res = pow(stringToDouble(s1), stringToDouble(s2));
        return res;
    }
    
    double doArithmetic(double s1, double s2, string symbol) {
        double res = 0;
        // std::cout << "s1, s2: " << s1 << ", " << s2 << "\n";
        if(symbol == "+") res = (s1) + (s2);
        else if(symbol == "-") res = (s1) - (s2);
        else if(symbol == "*") res = (s1) * (s2);
        else if(symbol == "/") res = (s1) / (s2);
        else if(symbol == "^") res = pow((s1), (s2));
        // std::cout << "res: " << res << "\n";
        return res;
    }

    string extractNumber(string raw, int startIndex) {
        int length = raw.length();
        string res = "";
        if(raw[startIndex] == '-') res = "-", startIndex++;
        for(int i = startIndex; i < length; i++) {
            // std::cout << raw[i] << " ";  
            if(isNumber(raw[i]) || raw[i] == '.') {
                res += raw[i];
            }
            else return res;
        }
        return res;
    }

    int indexOf(string s, char l) {
        int length = s.length();
        for(int i = 0; i < length; i++) {
            // std::cout << s[i] << ", " << i << std::endl;
            if(s[i] == l) return i;
        }
        return -1;
    }

    bool isInStr(string s, char l) {
        int length = s.length();
        for(int i = 0; i < length; i++) {
            if(s[i] == l) return true;
        }
        return false;
    }

    bool isSubstring(string s, string subS) {
        int length = s.length();
        // std::cout << "s, subS: " << s << ", " << subS << "/\n";
        for(int i = 0; i < length; i++) {
            // std::cout << "s[i], subS[i]: " << s[i] << ", " << subS[i] << std::endl;
            if(s[i] == subS[0]) {
                int length2 = subS.length();
                for(int j = i; j < i + length2; j++) {
                    if(s[j] != subS[j - i]) return false;
                }
                // std::cout << subS << " ;\n";
                return true;
            }
        }
        return false;
    }

    string charToStr(char l) { 
        string res = {l};
        return res;
    }

    bool isNumber(char l) { return '0' <= l && l <= '9'; }  

    double stringToDouble(string s) {
        return std::stod(s);
    }

    string doubleToString(double num) {
        return std::to_string(num);
    }
};