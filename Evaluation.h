#include "Stack.h"
#include "LinkedList.h"
#include <math.h>

typedef std::string string ;

class Eval {
private:
    string currentExpression;
    double currentResult;
    LinkedList<Pair*> *logs;

public:
    Eval() {
        currentExpression = "";
        currentResult = 0;
        logs = new LinkedList<Pair*>();
    }

    ~Eval() {
        delete logs;
    }

    void setExpression(string raw) {
        this->currentExpression = raw;
        LinkedList<string> *list = this->infixToPostfix(this->reformat(raw));
        this->currentResult = this->DoEval(list);
        Pair *newLog = new Pair(raw, this->currentResult);
        this->logs->append(newLog);
        delete list;
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

    double DoEval(LinkedList<string> *list) {
        string symbol = "(+-*/^";
        Stack<double> *stack = new Stack<double>(); 
        Node<string> *temp = list->getHead();
        // list->print();
        while(temp) {
            // std::cout << "data: " << temp->getData() << "\n";
            if(isSubstring(symbol, temp->getData())) {
                // std::cout << "Do what ";
                double num1, num2;
                num2 = stack->peek();
                stack->pop();
                num1 = stack->peek();
                // std::cout << "num1 ... num2: " << num1 << " " << temp->getData() << " " << num2 << std::endl;
                stack->pop();
                stack->push(doArithmetic(num1, num2, temp->getData()));
                // std::cout << "num3: " << stack->peek() << std::endl;
            }
            else {
                stack->push(stringToDouble(temp->getData()));
            }
            temp = temp->getNext();
        }
        return stack->peek();
    }

    LinkedList<string> *infixToPostfix(string raw) {
        // std::cout << "Formated: " << raw << std::endl;
        int length = raw.length(), i = 0;
        Stack<string> *stack = new Stack<string>();
        LinkedList<string> *res = new LinkedList<string>();
        bool firstNum = true;
        while(i < length) {
            if(this->isNumber(raw[i])) { 
                string number = this->extractNumber(raw, i); // get a current number in string as substring
                i += number.length(); // move i to back of number
                res->append(number); // push number to back of linked list
                firstNum = false;
            }
            else if(raw[i] == '-') { // dealing with a negative number
                if(this->isNumber(raw[i + 1]) && firstNum) { // case negative number is first order
                    string number = "-" + this->extractNumber(raw, i + 1);
                    res->append(number);
                    i += number.length();
                    firstNum = false;   
                }
                else if(this->isNumber(raw[i + 1])) { // case next to '-' is number
                    string number = "-" + this->extractNumber(raw, i + 1);
                    if(this->isNumber(raw[i - 1]) || raw[i - 1] == ')') this->pushToStackLogic('+', res, stack); // if prev of '-' is not any operation, push '+' to stack
                    res->append(number);
                    i += number.length();
                }
                else { 
                    this->pushToStackLogic('-', res, stack); // case next to '-' is number in ( )
                    i++;    
                }   
            }   
            else {
                this->pushToStackLogic(raw[i], res, stack);
                i++;
            }
        }

        // if loop end but stack still remain item then pop all stack then append to linked list
        while(!stack->isEmpty()) {
            res->append(stack->peek());
            stack->pop();
        }
        return res;
    }

    void pushToStackLogic(char c, LinkedList<string> *res, Stack<string> *stack) {
        string symbol = "(+-*/^";
        int precedence[] = {0, 1, 2, 3, 4, 5}; // precedence pair with operations in symbol
        if(stack->isEmpty() || c == '(') {
            // std::cout << "case 1: " << c << "\n";
            stack->push(charToStr(c));
        }
        else if(c == ')') { // if current iteration is ')' we gonna pop stack and append in linked list until we find '('
            // std::cout << "case 2: " << c << "\n";
            while(!stack->isEmpty() && stack->peek() != "(") {
                res->append(stack->peek());
                stack->pop();
            }
            stack->pop();
        }
        else if(precedence[indexOf(symbol, c)] >= precedence[indexOf(symbol, stack->peek()[0])]) {
            // std::cout << "case 3: " << c << "\n";
            // std::cout << c << " " << stack->peek() << std::endl;
            stack->push(charToStr(c));
        }
        else {
            // std::cout << "case 4: " << c << "\n";
            while(!stack->isEmpty() && stack->peek() != "(" && (precedence[indexOf(symbol, c)] < precedence[indexOf(symbol, stack->peek()[0])])) {
                // std::cout << "peek: " << stack->peek() << "\n";
                res->append(stack->peek());
                stack->pop();
            }
            stack->push(charToStr(c)); 
        }
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

    string reformat(string s) {
        string res = "";
        for(int i = 0, size = s.length(); i < size; i++) { if(s[i] != ' ') res += s[i]; }
        return res;
    }
};