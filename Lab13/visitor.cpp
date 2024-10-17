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
int IFExp::accept(Visitor* visitor) {
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

int IfStatement::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int WhileStatement::accept(Visitor* visitor) {
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

void PrintVisitor::visit(IfStatement* stm) {
    cout << "if ";
    stm->condition->accept(this);
    cout << " then" << endl;
    for (Stm* s : stm->then) {
        cout << "    ";
        s->accept(this);
        cout << endl;
    }
    if(!stm->els.empty()) cout << "else" << endl;

    for (Stm* s : stm->els) {
        cout << "    ";
        s->accept(this);
        cout << endl;
    }

    cout << "endif";
}

void PrintVisitor::imprimir(Program* program){
    for(Stm* s : program->body->dlist){
        s->accept(this);
        cout << endl;
    }
    for (Stm s : program->body->slist) {
        s->accept(this);
        cout << endl;
    }
};

void PrintVisitor::visit(Dec* d){
    cout << d->tipo << " ";
    for (string s : d->variables) {
        cout << s << " ";
    }
    cout << ";";
}

///////////////////////////////////////////////////////////////////////////////////
int EVALVisitor::visit(BinaryExp* exp) {
    int result;
    int v1 = exp->left->accept(this);
    int v2 = exp->right->accept(this);
    switch(exp->op) {
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
        case LT_OP: result = v1 < v2; break;
        case LE_OP: result = v1 <= v2; break;
        case EQ_OP: result = v1 == v2; break;
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
    cout<< endl;
}
void EVALVisitor::ejecutar(Program* program){
    for (Stm* s : program->body->slist) {
        s->accept(this);
    }
};

void EVALVisitor::visit(IfStatement* stm) {
    if(stm->condition->accept(this)){
        for (Stm* s : stm->then) {
            s->accept(this);
        }
    }
    else{
        for (Stm* s : stm->els) {
            s->accept(this);
        }
    }
}
void EVALVisitor::visit(WhileStatement* stm) {
    while(stm->condition->accept(this)){
        for (Stm* s : stm->dolist) {
            s->accept(this);
        }
    }
}
void PrintVisitor::visit(WhileStatement* stm){
    cout << "while ";
    stm->condition->accept(this);
    cout << " do";
    cout << endl;
    for (Stm* s : stm->dolist) {
        cout << "    ";
        s->accept(this);
    }
}

int EVALVisitor::visit(IFExp* pepito) {
    if(pepito->cond->accept(this)){
        return pepito->left->accept(this);
    }
    else{
        return pepito->right->accept(this);
    }
}
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

void EVALVisitor::visit(Dec* d){
    // for (string s : d->variables) {
    //     memoria[s] = 0;
    // }
}