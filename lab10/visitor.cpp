#include <iostream>
#include "exp.h"
#include "visitor.h"
#include <unordered_map>
using namespace std;
unordered_map<std::string, int> memoria;
///////////////////////////////////////////////////////////////////////////////////
int BinaryExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int NumberExp::accept(Visitor* visitor) {
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

void PrintVisitor::imprimir(Program* program){
    for (Stm* s : program->slist) {
        s->accept(this);
        cout << endl;
    }
};

///////////////////////////////////////////////////////////////////////////////////
int EVALVisitor::visit(BinaryExp* exp) {
    int result;
    int v1 = exp->left->accept(this);
    int v2 = exp->right->accept(this);
    switch(Exp::binopToChar(exp->op)) {
        case '+': result = v1 + v2; break;
        case '-': result = v1 - v2; break;
        case '*': result = v1 * v2; break;
        case '/':
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

int EVALVisitor::visit(NumberExp* exp) {
    return exp->value;
}

int EVALVisitor::visit(IdentifierExp* exp) {
    return memoria[exp->name];
}

void EVALVisitor::visit(AssignStatement* stm) {
    memoria[stm->id] = stm->rhs->accept(this);
}

void EVALVisitor::visit(PrintStatement* stm) {
    cout << stm->e->accept(this);
}
void EVALVisitor::ejecutar(Program* program){
    for (Stm* s : program->slist) {
        s->accept(this);
    }
};

// Print visitor para if statement: 
void PrintVisitor::visit(IfStatement* stm) {
    cout << "if ";
    stm->condition->accept(this);
    cout << " then" << endl;
    for (Stm* s : stm->thenList) {
        s->accept(this);
        cout << endl;
    }
    if (!stm->elseList.empty()) {
        cout << "else" << endl;
        for (Stm* s : stm->elseList) {
            s->accept(this);
            cout << endl;
        }
    }
    cout << "endif";
}

//Eval Visitor 

void EVALVisitor::visit(IfStatement* stm) {
    int cond = stm->condition->accept(this);
    if (cond) {
        for (Stm* s : stm->thenList) {
            s->accept(this);
        }
    } else {
        for (Stm* s : stm->elseList) {
            s->accept(this);
        }
    }
}




