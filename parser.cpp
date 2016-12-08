#include <vector>
#include <iostream>
#include <stack>
#include <cstdio>

using namespace std;

vector<string> tokens;
vector<string>::iterator vit;

enum non_terminal {start,function,params
,body,param,paramm,statements,statement,exp
,nexp};

bool result = true;

bool match(string a){
    cout << "matching : " << *vit << endl;
    if(vit == tokens.end()){
        cout << "Nothing left to match" << endl;
        result = 0;
        return result;    
    }
    if(a == "") a = *vit; // match anything
    result &= (*(vit) == a);
    if(!result) {
        cout << "expected \"" << a << "\" got \"" << *vit << "\""<<endl;
    }
    vit++;
    return result;
}

bool f(non_terminal current){
    switch (current){
        case start : 
            match("function");
            f(function);
            f(params);
            f(body);
            break;
        case function :
            // assume function_name can be anything
            match("");
            break;
        case params :
            f(param);
            match("stop");
            break;
        case param :
            match(""); // name
            f(paramm);
            break;
        case paramm :
            if(*vit == "and"){
                match("and");
                f(param);
            }
            break;
        case body :
            match("block");
            f(statements);
            match("stop");
            break;
        case statements :
            f(statement);
            if(*vit != "end") f(statements);
            else match("end");
            break;
        case statement :
            if(*vit == "return"){
                match("return");
                f(exp);
                match("stop");
            }
            if(*vit == "if"){
                match("if");
                f(exp);
                match("then");
                f(statements);
                match("else");
                f(statements);
                match("stop");
            }
            if(*vit == "assign"){
                f(exp);
                match("to");
                match("");
                match("stop");
            }
            break;
        case exp :
            match("");
            if(*vit == "op"){
                match("op");
                f(exp);
            } 
    };
}

int main(){
    string s;
    cout << "input program : ";
    while(true){
        cin >> s;
        if(s == "EOP") break;
        tokens.push_back(s);
        cout << s << " ";
    }
    cout << endl;
    vit = tokens.begin();
    f(start);
    if (result) cout << "parse OK!" <<endl;
    else cout << "parse not OK :(" << endl;
}


/*

Start -> function params body .

function -> function_name .
params -> param stop .
param -> name param* .
param* -> and param .
param* -> .

body -> block statements stop .
statements -> statement statements end .
statements -> .
statement -> return exp stop.
statement -> if exp then statements else statements stop.
statement -> assign exp to name stop.

exp -> name nexp .
exp -> function param .
nexp -> op exp .
nexp -> .

op -> + .
op -> - .
op -> * .
op -> / .
op -> == .
op -> != .
*/