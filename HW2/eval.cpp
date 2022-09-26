#include <string>
#include <iostream>
#include <stack>
#include <cassert>
using namespace std;

bool isValid(const string& s);
bool insideStatementCheck(const string& s);
int precedence(char op);
int evaluate(string infix, string& postfix, bool& result);

bool isValid(const string& s) {
// check that infix w/o spaces doesn't have any invalid chars
    for(size_t i = 0; i < s.length(); i++) {
        switch(s.at(i))
        {
            case 'T':
            case 'F':
            case '&':
            case '!':
            case '|':
            case '(':
            case ')':
                break;
            default:
                return false;
        }
    }
// checks for empty and one element strings
    if(s.length() == 0)
        return false;

    if(s.length() == 1) {
        if(s.at(0) != 'F' && s.at(0) != 'T')
            return false;
        else
            return true;
    }
// check parantheses - are there the proper number and are they in the proper order?
    stack<char> paranthesesOpen;

    for(size_t i = 0; i < s.length(); i++) {
        char ch = s.at(i);
        switch(ch)
        {
            case '(':
                paranthesesOpen.push(ch);
                break;
            case ')':
                if(paranthesesOpen.empty()) // no '(' for this ')'
                    return false;
                paranthesesOpen.pop();
                break;
            default:
                break;
        }
    }
    if(!paranthesesOpen.empty())
        return false;

// check that first element is valid - it's a (, T, or F
    if(s.at(0) != 'F' && s.at(0) != 'T' && s.at(0) != '(' && s.at(0) != '!')
        return false;

// check that last element is valid - it's a ), T, or F
    if(s.at(s.length()-1) != 'F' && s.at(s.length()-1) != 'T' && s.at(s.length()-1) != ')')
        return false;

// check that there aren't operands or binary operators next to each other
    string withoutParantheses = "";
    for(size_t i = 0; i < s.length(); i++) {
        switch(s.at(i))
        {
            case 'T':
            case 'F':
            case '&':
            case '!':
            case '|':
                withoutParantheses += s.at(i);
                break;
            default:
                break;
        }
    }
    if(withoutParantheses.length() == 0) // to handle cases like ()
        return false;
    if(withoutParantheses.length() == 1) { // to handle cases like ((T))
        if(withoutParantheses.at(0) != 'T' && withoutParantheses.at(0) != 'F')
           return false; // remember, you cannot return true just yet bc it might be something like ()T
    }
    for(size_t i = 0; i < withoutParantheses.length() - 1; i++) {
        switch(withoutParantheses.at(i))
        {
            case 'T':
            case 'F':
                if(withoutParantheses.at(i+1) == 'T' || withoutParantheses.at(i+1) == 'F' || withoutParantheses.at(i+1) == '!')
                    return false;
                break;
            case '&':
            case '|':
            case '!':
                if(withoutParantheses.at(i+1) == '&' || withoutParantheses.at(i+1) == '|')
                    return false;
                break;
            default:
                break;
        }
    }
    return true;
}

bool insideStatementCheck(const string& s) {
// function to check whatever is inside parantheses is valid: ie. ()T and T(&T) aren't valid, but (T|F)&(F) is valid
    // this is whatever is inside the parantheses we have
    for(size_t i = 0; i < s.length() - 1; i++) {
        if(s.at(i) == '(' && s.at(i+1) != '(') { // to check cases like ((T|F)), where there's multiple open parantheses
            string insideStatement = "";
            size_t j = i+1;
            while(s.at(j) != ')') {
                insideStatement += s.at(j);
                j++;
            }
            if(!isValid(insideStatement))
                return false;
        }
    }
    return true;
}

int precedence(char op) {
    if (op == '!')
        return 3;
    if (op == '&')
        return 2;
    return 1; // op is '|'
}

int evaluate(string infix, string& postfix, bool& result)
{
    // eliminate all spaces
    string s = "";
    for(size_t i = 0; i < infix.length(); i++) {
        switch(infix.at(i))
        {
            case ' ':
                break;
            default:
                s += infix.at(i);
                break;
        }
    }


    // check if infix without spaces is valid
    if(!isValid(s))
        return 1;

    if(!insideStatementCheck(s))
        return 1;

//    // delete all unnecessary exclamation points
//    string s = "";
//    for(size_t i = 0; i < t.length(); i++) {
//        if (t.at(i) == '!') {
//            int numExclamationsInARow = 0;
//            for(size_t j = i; j < t.length(); j++) {
//                if(t.at(j) == '!')
//                    numExclamationsInARow++;
//                else
//                    break;
//            }
//            if (numExclamationsInARow%2 == 1) {
//                s+='!';
//            }
//            i+=numExclamationsInARow - 1; // bc if the number of exclamations are even it just cancels out
//        } else {
//            s += t.at(i);
//        }
//    }

    // convert infix to postfix
    postfix = "";
    stack<char> operators;
    for(size_t i = 0; i < s.length(); i++) {
        char ch = s.at(i);
        switch(ch)
        {
            case 'T':
            case 'F':
                postfix += ch;
                break;
            case '(':
                operators.push(ch);
                break;
            case ')':
                // pop stack until matching '('
                while(operators.top() != '(') {
                    postfix += operators.top();
                    operators.pop();
                }
                operators.pop(); // remove the '('
                break;
            case '|':
            case '&':
            case '!':
                while(!operators.empty() && operators.top() != '(') {
                    if (precedence(ch) <= precedence(operators.top())) {
                        postfix += operators.top();
                        operators.pop();
                    } else {
                        break;
                    }
                }
                operators.push(ch);
                break;
        }
    }
    while(!operators.empty()) {
        postfix += operators.top();
        operators.pop();
    }

//    cout << postfix << endl;

    // evaluate postfix
    stack<char> operands;
    for(size_t i = 0; i < postfix.length(); i++) {
        char ch = postfix.at(i);
        if(ch == 'F' || ch == 'T')
            operands.push(ch);
        else if(ch == '!') {
            if(operands.top() == 'F') {
                operands.pop();
                operands.push('T');
            } else { // 'T'
                operands.pop();
                operands.push('F');
            }
        }
        else { // ch is a binary operator (&, |)
            char operand2 = operands.top();
            operands.pop();
            char operand1 = operands.top();
            operands.pop();
            char newOperand;
            if(ch == '&') {
                if (operand1 == 'T' && operand2 == 'T')
                    newOperand = 'T';
                else
                    newOperand = 'F';
            } else { // '|'
                if (operand1 == 'T' || operand2 == 'T')
                    newOperand = 'T';
                else
                    newOperand = 'F';
            }
            operands.push(newOperand);
        }
    }

    if(operands.top() == 'T')
        result = true;
    else
        result = false;
    return 0;
}

int main()
{
    // remember to check cases with double parantheses
    string pf;
            bool answer;
            assert(evaluate("T| F", pf, answer) == 0  &&  pf == "TF|"  &&  answer);
            assert(evaluate("T|", pf, answer) == 1);
            assert(evaluate("F F", pf, answer) == 1);
            assert(evaluate("TF", pf, answer) == 1);
            assert(evaluate("()", pf, answer) == 1);
            assert(evaluate("()T", pf, answer) == 1);
            assert(evaluate("T(F|T)", pf, answer) == 1);
            assert(evaluate("T(&T)", pf, answer) == 1);
            assert(evaluate("(T&(F|F)", pf, answer) == 1);
            assert(evaluate("T+F", pf, answer) == 1);
            assert(evaluate("", pf, answer) == 1);
            assert(evaluate("F  |  !F & (T&F) ", pf, answer) == 0
                                   &&  pf == "FF!TF&&|"  &&  !answer);
            assert(evaluate(" F  ", pf, answer) == 0 &&  pf == "F"  &&  !answer);
            assert(evaluate("((T))", pf, answer) == 0 &&  pf == "T"  &&  answer);
}
