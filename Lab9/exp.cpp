#include <iostream>
#include "exp.h"

using namespace std;

BinaryExp::BinaryExp(Exp* l, Exp* r, BinaryOp op):left(l),right(r),op(op) {}
NumberExp::NumberExp(int v):value(v) {}
IdentifierExp::IdentifierExp(const string& n):name(n) {}

Exp::~Exp() {}
BinaryExp::~BinaryExp() { delete left; delete right; }
NumberExp::~NumberExp() { }
IdentifierExp::~IdentifierExp() { }

void BinaryExp::print() {
    left->print();
    char c = binopToChar(this->op);
    cout << ' ' << c << ' ';
    right->print();
}

void NumberExp::print() {
    cout << value;
}

void IdentifierExp::print() {
    cout << name;
}

int BinaryExp::eval(unordered_map<string, int>& symbolTable) {
    int result;
    int v1=left->eval(symbolTable);
    int v2=right->eval(symbolTable);
    switch(this->op) {
        case PLUS_OP: result = v1 + v2; break;
        case MINUS_OP: result = v1 - v2; break;
        case MUL_OP: result = v1 * v2; break;
        case DIV_OP: 
            if(v2 != 0) result = v1 / v2; 
            else {
                cout << "Error: división por cero" << endl;
                result = 0;
            }
            break;
        default:
            cout << "Operador desconocido" << endl;
            result = 0;
    }
    return result;
}

int NumberExp::eval(unordered_map<string, int>& symbolTable) {
    return value;
}

int IdentifierExp::eval(unordered_map<string, int>& symbolTable) {
    if (symbolTable.find(name) != symbolTable.end()) {
        return symbolTable[name];
    } else {
        cout << "Error: variable no definida: " << name << endl;
        return 0;
    }
}

AssignStatement::AssignStatement(string id, Exp* e): id(id), rhs(e) {}

void AssignStatement::execute(unordered_map<string, int>& symbolTable) {
    symbolTable[id] = rhs->eval(symbolTable);
}

AssignStatement::~AssignStatement() {
    delete rhs;
}

PrintStatement::PrintStatement(Exp* e): e(e) {}

void PrintStatement::execute(unordered_map<string, int>& symbolTable) {
    cout << e->eval(symbolTable) << endl;
}

PrintStatement::~PrintStatement() {
    delete e;
}

Program::Program() {}

void Program::add(Stm* s) {
    slist.push_back(s);
}

void Program::interprete() {
    for (Stm* s : slist) {
        s->execute(memoria);
    }
}

Program::~Program() {
    for (Stm* s : slist) {
        delete s;
    }
}

Stm::~Stm() {}

char Exp::binopToChar(BinaryOp op) {
    char  c=' ';
    switch(op) {
        case PLUS_OP: c = '+'; break;
        case MINUS_OP: c = '-'; break;
        case MUL_OP: c = '*'; break;
        case DIV_OP: c = '/'; break;
        default: c = '$';
    }
    return c;
}