#include <iostream>
#include "exp.h"
#include "visitor.h"
#include <unordered_map>

using namespace std;




int BinaryExp::accept(Visitor* visitor) {
    visitor->visit(this);
}

int NumberExp::accept(Visitor* visitor) {
    visitor->visit(this);
}

int IdentifierExp::accept(Visitor* visitor) {
    visitor->visit(this);
}

int AssignStatement::accept(Visitor* visitor) {
    visitor->visit(this);
}

int PrintStatement::accept(Visitor* visitor) {
    visitor->visit(this);
}

void PrintVisitor::visit(BinaryExp* exp) {
    exp->left->accept(this);
    cout << ' ' << Exp::binopToChar(exp->op) << ' ';
    exp->right->accept(this);
}

void PrintVisitor::visit(NumberExp* exp) {
    cout << exp->value;
}

void PrintVisitor::visit(IdentifierExp* exp) {
    cout << exp->name;
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

int PrintVisitor::visit(BinaryExp* exp)
{
    exp->left->accept(this);
    cout << ' ' << Exp::binopToChar(exp->op) << ' ';
    exp->right->accept(this);
    return 0;
}

int PrintVisitor::visit(NumberExp* exp)
{
    cout << exp->value;
    return 0;
}



int PrintVisitor::visit(IdentifierExp* exp){
    cout << exp->name;
    return 0;
}


int PrintVisitor::visit(AssignStatement* stm)
{
    cout << stm->id << " = ";
    stm->rhs->accept(this);
    cout << ";";
    return 0;
}




void EVALVisitor::visit(AssignStatement* stm) {
    cout << stm->id << " = ";
    stm->rhs->accept(this);
    cout << ";";
}

void EVALVisitor::visit(PrintStatement* stm) {
    cout << "print(";
    stm->e->accept(this);
    cout << ");";
}

int EVALVisitor::visit(BinaryExp* exp)
{
    int result;
    int v1 = exp->left->accept(this);
    int v2 = exp->right->accept(this);
    // cout << ' ' << Exp::binopToChar(exp->op) << ' ';
    switch(Exp::binopToChar(exp->op)) {
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


int EVALVisitor::visit(NumberExp* exp)
{
    return exp->value;
}



int EVALVisitor::visit(IdentifierExp* exp){
    retunr 1;
}


int EVALVisitor::visit(AssignStatement* stm)
{
    cout << stm->id << " = ";
    stm->rhs->accept(this);
    cout << ";";
    return 0;
}



void PrintVisitor::imprimir(Program* program){{
        for (Stm*  s : program ->slist){
            s->accept(this);
            // cout << endl;
        }
    }
}

void EVALVisitor::ejecutar(PRogram* program){
    for (Stm* s : program->slist){
        s->accept(this);
    }
}