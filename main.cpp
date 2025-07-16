#include "Evaluation.h"
using namespace std;

int main() {
    Eval ev;
    string exp, commandBuffer;
    exp = commandBuffer = "";
    string help = "Enter an expression -> exp\nShow log -> log\nShow Result -> res\nTo exit -> quit\n\n";
    cout << help;
    while(true) {
        getline(cin, commandBuffer);
        if(commandBuffer == "help") cout << help;
        else if(commandBuffer == "exp") {
            cout << ": ";
            getline(cin, exp);
            ev.setExpression(exp);
            cout << "result is: " << ev.getResult() << endl << endl; 
        }
        else if(commandBuffer == "res") {
            if(ev.logSize() == 0) {
                cout << "Please enter your expression before request a result\n\n";
            }
            else cout << "result is: " << ev.getResult() << endl << endl;
        }
        else if(commandBuffer == "log") {
            if(ev.logSize() == 0) {
                cout << "Log is empty\n\n";
            }
            else ev.showLogs(); cout << endl;
        }
        else if(commandBuffer == "quit") break;
    }
    return 0;
}