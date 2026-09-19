#include "Stack.h"
#include <math.h>
#include <vector>
#include <iostream>
#include <cstdio>
#include <unordered_set>
#include <unordered_map>

using namespace std;

class Eval {
private:
	string current_expression;
	double current_result;
	vector<pair<string, double>> logs;

public:
	Eval() : current_expression(""), current_result(0) {}

	void set_expression(string raw) {
		current_expression = raw;
		vector<string> arr = infix_to_postfix(reformat(raw));
		current_result = do_eval(arr);
		logs.push_back(pair<string, double>(raw, current_result));
	}

	double get_result() { return current_result; }

	int log_size() { return logs.size(); }

	void show_logs() {
		for(int i = 0; i < logs.size(); ++i) {
			printf("%d : %s -> %f\n", i+1, logs[i].first.c_str(), logs[i].second);	
		}
	}

private:

	double do_eval(vector<string>& arr) {
		unordered_set<char> symbol({'(', '+', '-', '*', '/', '^'});
		Stack<double> stack;

		for(string e : arr) {
			if(e.length() == 1 && symbol.count(e[0])) {
				double 
					num2 = stack.pop(), 
					num1 = stack.pop();

				stack.push(do_arithmetic(num1, num2, e[0])); // get num1 and num2 from stack peek then operate them with current operation
			}
			else 
				stack.push(stringToDouble(e));
		}

		double res = stack.peek();

		return res;
	}

	vector<string> infix_to_postfix(string raw) {
		int 
			length 	= raw.length(), 
			i 		= 0;
		bool first_num = true;
		Stack<char> stack;
		vector<string> res;

		while(i < length) {
			if(is_number(raw[i])) { 
				string number = extract_number(raw, i); // get a current number in string as substring
				i += number.length(); // move i to back of number
				res.push_back(number); // push number to back of linked list
				first_num = false;
			}
			else if(raw[i] == '-') { // dealing with a negative number
				if(is_number(raw[i + 1]) && first_num) { // case negative number is first order
					string number = "-"+extract_number(raw, i+1);
					res.push_back(number);
					i += number.length();
					first_num = false;   
				}
				else if(is_number(raw[i + 1])) { // case next to '-' is number
					string number = "-"+extract_number(raw, i+1);
					if(is_number(raw[i - 1]) || raw[i - 1] == ')') push_to_stack_logic('+', res, stack); // if prev of '-' is not any operation, push '+' to stack
					res.push_back(number);
					i += number.length();
				}
				else { 
					push_to_stack_logic('-', res, stack); // case next to '-' is number in ( )
					i++;    
				}   
			}   
			else 
				push_to_stack_logic(raw[i++], res, stack); // case operation is "+*/^"
		}

		// if loop end but stack still remain item then pop all stack then append to array
		while(!stack.is_empty()) {
			res.push_back(string("")+stack.peek());
			stack.pop();
		}

		return res;
	}

	void push_to_stack_logic(char c, vector<string> &res, Stack<char> &stack) {
		unordered_map<char, int> precedence = {
			{ '(', 0 },
			{ '+', 1 },
			{ '-', 2 },
			{ '*', 3 },
			{ '/', 4 },
			{ '^', 5 }
		}; // precedence pair with operations in symbol

		if(stack.is_empty() || c == '(') 
			stack.push(c);
		
		// if current iteration is ')' we gonna pop stack and append in array until we find '('
		else if(c == ')') { 			
			while(!stack.is_empty() && stack.peek() != '(') {
				res.push_back(string("")+stack.peek());
				stack.pop();
			}
			stack.pop();
		}
		else if(precedence[c] >= precedence[stack.peek()])
			stack.push(c);
		else {
			while(!stack.is_empty() && stack.peek() != '(' && (precedence[c] < precedence[stack.peek()])) {
				res.push_back(string("")+stack.peek());
				stack.pop();
			}
			stack.push(c); 
		}
	}
	
	double do_arithmetic(double s1, double s2, char symbol) {
		double res = 0;
		switch(symbol) {
			case '+':
				res = s1+s2;
				break;
			case '-':
				res = s1-s2;
				break;
			case '*':
				res = s1*s2;
				break;
			case '/':
				res = s1/s2;
				break;
			case '^':
				res = pow(s1, s2);
				break;
			default:
				break;
		}
		return res;
	}

	string extract_number(string& raw, int idx) {
		int length = raw.length();
		string res = "";
		if(raw[idx] == '-') res = "-", idx++;
		for(int i = idx; i < length; i++) {
			if(is_number(raw[i]) || raw[i] == '.') {
				res += raw[i];
			}
			else return res;
		}
		return res;
	}

	bool is_number(char l) { return '0' <= l && l <= '9'; }  

	double stringToDouble(string& s) {
		return stod(s);
	}

	string doubleToString(double num) {
		return std::to_string(num);
	}

	string reformat(string& s) {
		string res = "";
		for(int i = 0, size = s.length(); i < size; i++) { if(s[i] != ' ') res += s[i]; }
		return res;
	}
};