#include <iostream>
#include "exp.h"
#include "visitor.h"
#include <unordered_map>
using namespace std;


///////////////////////////////////////////////////////////////////////////////////
int BinaryExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}
int IFExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}
int NumberExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int BoolExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int IdentifierExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int AssignStatement::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int PrintStatement::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int IfStatement::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int WhileStatement::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}
int ForStatement::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int VarDec::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int VarDecList::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}
int StatementList::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}
int Body::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////

int PrintVisitor::visit(BinaryExp* exp) {
    exp->left->accept(this);
    cout << ' ' << Exp::binopToChar(exp->op) << ' ';
    exp->right->accept(this);
    return 0;
}

int PrintVisitor::visit(NumberExp* exp) {
    cout << exp->value;
    return 0;
}

int PrintVisitor::visit(BoolExp* exp) {
    if(exp->value) cout << "true";
    else cout << "false";
    return 0;
}

int PrintVisitor::visit(IdentifierExp* exp) {
    cout << exp->name;
    return 0;
}

void PrintVisitor::visit(AssignStatement* stm) {
    cout << stm->id << " = ";
    stm->rhs->accept(this);
    cout << ";";
}

void PrintVisitor::visit(PrintStatement* stm) {
    cout << "print(";
    stm->e->accept(this);
    cout << ");";
}

void PrintVisitor::visit(IfStatement* stm) {
    cout << "if ";
    stm->condition->accept(this);
    cout << " then" << endl;
    stm->then->accept(this);
    if(stm->els){
        cout << "else" << endl;
        stm->els->accept(this);
    }
    cout << "endif";
}

void PrintVisitor::imprimir(Program* program){
    program->body->accept(this);
};


int PrintVisitor::visit(IFExp* pepito) {
    cout<< "ifexp(";
    pepito->cond->accept(this);
    cout<< ",";
    pepito->left->accept(this);
    cout<< ",";
    pepito->right->accept(this);
    cout << ")" ;
    return 0;
}

void PrintVisitor::visit(WhileStatement* stm){
    cout << "while ";
    stm->condition->accept(this);
    cout << " do" << endl;
    stm->b->accept(this);
    cout << "endwhile";
}

void PrintVisitor::visit(ForStatement* stm){
    cout << "for ";
    stm->start->accept(this);
    cout << " to ";
    stm->end->accept(this);
    cout << " step ";
    stm->step->accept(this);
    cout << " do" << endl;
    stm->b->accept(this);
    cout << "endfor";
}

void PrintVisitor::visit(VarDec* stm){
    cout << "var ";
    cout << stm->type;
    cout << " ";
    for(auto i: stm->vars){
        cout << i;
        if(i != stm->vars.back()) cout << ", ";
    }
    cout << ";";
}

void PrintVisitor::visit(VarDecList* stm){
    for(auto i: stm->vardecs){
        i->accept(this);
        cout << endl;
    }
}

void PrintVisitor::visit(StatementList* stm){
    for(auto i: stm->stms){
        i->accept(this);
        cout << endl;
    }
}

void PrintVisitor::visit(Body* stm){
    stm->vardecs->accept(this);
    cout << endl;
    stm->slist->accept(this);
}

///////////////////////////////////////////////////////////////////////////////////
int EVALVisitor::visit(BinaryExp* exp) {
    int left = exp->left->accept(this);
    int right = exp->right->accept(this);
    switch(exp->op){
        case '+': return left + right;
        case '-': return left - right;
        case '*': return left * right;
        case '/': return left / right;
        case '%': return left % right;
        case '<': return left < right;
        case '>': return left > right;
        case '=': return left == right;
        case '&': return left && right;
        case '|': return left || right;
    }
    return 0;
}

int EVALVisitor::visit(NumberExp* exp) {
    return exp->value;
}

int EVALVisitor::visit(BoolExp* exp) {
    return exp->value;
}

int EVALVisitor::visit(IdentifierExp* exp) {
    return env[exp->name];
}

void EVALVisitor::visit(AssignStatement* stm) {
    env[stm->id] = stm->rhs->accept(this);
}

void EVALVisitor::visit(PrintStatement* stm) {
    cout << stm->e->accept(this);
}

void EVALVisitor::ejecutar(Program* program){
    program->body->accept(this);
}

void EVALVisitor::visit(IfStatement* stm) {
    if(stm->condition->accept(this)){
        stm->then->accept(this);
    } else if(stm->els){
        stm->els->accept(this);
    }
}

void EVALVisitor::visit(WhileStatement* stm) {
    while(stm->condition->accept(this)){
        stm->b->accept(this);
    }
}


int EVALVisitor::visit(IFExp* pepito) {
    if(pepito->cond->accept(this)){
        return pepito->left->accept(this);
    } else {
        return pepito->right->accept(this);
    }
}

void EVALVisitor::visit(ForStatement* stm){
    for(int i = stm->start->accept(this); i <= stm->end->accept(this); i += stm->step->accept(this)){
        stm->b->accept(this);
    }
}



void EVALVisitor::visit(VarDec* stm){
    for(auto i: stm->vars){
        env[i] = 0;
    }
}

void EVALVisitor::visit(VarDecList* stm){
    for(auto i: stm->vardecs){
        i->accept(this);
    }
}

void EVALVisitor::visit(StatementList* stm){
    for(auto i: stm->stms){
        i->accept(this);
    }
}

void EVALVisitor::visit(Body* b){
    b->vardecs->accept(this);
    b->slist->accept(this);
}

///////////////////////////////////////////////////////////////////////////////////

//0 = undefined
//1 = int
//2 = bool

void TypeVisitor::check(Program* program){
    program->body->accept(this);
}

int TypeVisitor::visit(BinaryExp* exp) {
    int left = exp->left->accept(this);
    int right = exp->right->accept(this);
    if(left == 0 || right == 0) return 0;
    if(left == 1 && right == 1){
        if(exp->op == '+' || exp->op == '-' || exp->op == '*' || exp->op == '/' || exp->op == '%') return 1;
        if(exp->op == '<' || exp->op == '>' || exp->op == '=') return 2;
    }
    if(left == 2 && right == 2){
        if(exp->op == '&' || exp->op == '|') return 2;
    }
    return 0;
}

int TypeVisitor::visit(NumberExp* exp) {


    return 1;
}

int TypeVisitor::visit(BoolExp* exp) {

    return 2;
}

int TypeVisitor::visit(IdentifierExp* exp) {
    return env[exp->name];
}

int TypeVisitor::visit(IFExp* exp) {
    int cond = exp->cond->accept(this);
    int left = exp->left->accept(this);
    int right = exp->right->accept(this);
    if(cond == 2 && left == right) return left;
    return 0;
}

void TypeVisitor::visit(AssignStatement* stm) {
    int rhs = stm->rhs->accept(this);
    if(env[stm->id] == 0) env[stm->id] = rhs;
    else if(env[stm->id] != rhs) env[stm->id] = 0;
}

void TypeVisitor::visit(PrintStatement* stm) {
    stm->e->accept(this);
}

void TypeVisitor::visit(IfStatement* stm) {
    int cond = stm->condition->accept(this);
    if(cond != 2) return;
    stm->then->accept(this);
    if(stm->els) stm->els->accept(this);
}

void TypeVisitor::visit(WhileStatement* stm) {
    int cond = stm->condition->accept(this);
    if(cond != 2) return;
    stm->b->accept(this);
}

void TypeVisitor::visit(ForStatement* stm) {
    int start = stm->start->accept(this);
    int end = stm->end->accept(this);
    int step = stm->step->accept(this);
    if(start != 1 || end != 1 || step != 1) return;
    stm->b->accept(this);
}

void TypeVisitor::visit(VarDec* stm) {
    for(auto i: stm->vars){
        env[i] = stm->type == "int" ? 1 : 2;
    }
}

void TypeVisitor::visit(VarDecList* stm) {
    for(auto i: stm->vardecs){
        i->accept(this);
    }
}

void TypeVisitor::visit(StatementList* stm) {
    for(auto i: stm->stms){
        i->accept(this);
    }
}

void TypeVisitor::visit(Body* b) {
    b->vardecs->accept(this);
    b->slist->accept(this);
}