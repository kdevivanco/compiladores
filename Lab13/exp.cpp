#include <iostream>
#include "exp.h"
using namespace std;
IFExp::IFExp(Exp* cond,Exp* l, Exp* r): cond(cond),left(l),right(r){}
BinaryExp::BinaryExp(Exp* l, Exp* r, BinaryOp op):left(l),right(r),op(op) {}
NumberExp::NumberExp(int v):value(v) {}
IdentifierExp::IdentifierExp(const string& n):name(n) {}
Exp::~Exp() {}
BinaryExp::~BinaryExp() { delete left; delete right; }
IFExp::~IFExp() {delete cond, delete left; delete right; }
NumberExp::~NumberExp() { }
IdentifierExp::~IdentifierExp() { }
AssignStatement::AssignStatement(string id, Exp* e): id(id), rhs(e) {}
AssignStatement::~AssignStatement() {
    delete rhs;
}
PrintStatement::PrintStatement(Exp* e): e(e) {}
PrintStatement::~PrintStatement() {
    delete e;
}

IfStatement::IfStatement(Exp* c, list<Stm*> t, list<Stm*> e): condition(c), then(t), els(e) {}
IfStatement::~IfStatement() {
    delete condition;
    for (Stm* s : then) {
        delete s;
    }
    for (Stm* s : els) {
        delete s;
    }
}
WhileStatement::WhileStatement(Exp* c, list<Stm*> t): condition(c), dolist(t) {}
WhileStatement::~WhileStatement() {
    delete condition;
    for (Stm* s : dolist) {
        delete s;
    }
}
Program::Program() {}

Body::Body(){}

Body::~Body() {
    for (Stm* s : slist) {
        delete s;
    }
}


void Body::add(Stm* s) {
    slist.push_back(s);
    // declist
    declist.push_back(d)
}
Program::~Program() {
    // delete body;
}

Dec::Dec(){}

Dec::~Dec() {}


Stm::~Stm() {}
string Exp::binopToChar(BinaryOp op) {
    string  c;
    switch(op) {
        case PLUS_OP: c = "+"; break;
        case MINUS_OP: c = "-"; break;
        case MUL_OP: c = "*"; break;
        case DIV_OP: c = "/"; break;
        case LT_OP: c = "<"; break;
        case LE_OP: c = "<="; break;
        case EQ_OP: c = "=="; break;
        default: c = "$";
    }
    return c;
}