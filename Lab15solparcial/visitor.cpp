#include <iostream>
#include "exp.h"
#include "visitor.h"
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
int FcallExp::accept(Visitor* visitor) {
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

int FuncDec::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int FuncDecList::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int ParamDecList::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int ReturnStatement::accept(Visitor* visitor) {
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
int PrintVisitor::visit(FcallExp* exp) {
    cout << exp->nombre;
    cout << "(";
    for(auto i: exp->entradas){
        i->accept(this);
        cout << ",";
    }
    cout << ")";
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
    program->vardecs->accept(this);
    program->funcdecs->accept(this);
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

void PrintVisitor::visit(Body* b){
    b->vardecs->accept(this);
    cout << endl;
    b->slist->accept(this);
}


void PrintVisitor::visit(ArgList* argList) {
    std::cout << "(";
    bool first = true;
    for (Exp* arg : argList->args) {
        if (!first) std::cout << ", ";
        first = false;
        arg->accept(this); // Imprime cada argumento usando el `visit` adecuado
    }
    std::cout << ")";
}


void PrintVisitor::visit(FuncDec* funcDec) {
    std::cout << "fun " << funcDec->type << " " << funcDec->name << "(";
    funcDec->params->accept(this); // Imprime los parámetros
    std::cout << ")\n";
    funcDec->body->accept(this); // Imprime el cuerpo
    std::cout << std::endl;
}

void PrintVisitor::visit(FuncDecList* funcDecList) {
    for (FuncDec* funcDec : funcDecList->funcdecs) {
        funcDec->accept(this); // Imprime cada función
    }
    cout << endl;
}

void PrintVisitor::visit(ParamDecList* paramDecList) {
    bool first = true;
    for (size_t i = 0; i < paramDecList->types.size(); ++i) {
        if (!first) std::cout << ", ";
        first = false;
        std::cout << paramDecList->types[i] << " " << paramDecList->params[i]; // Imprime cada parámetro
    }
}

void PrintVisitor::visit(ReturnStatement* stm) {
    std::cout << "return (";
    if (stm->exp)
        stm->exp->accept(this); // Imprime la expresión de retorno
    cout << ")";
}
