#ifndef EXP_H
#define EXP_H

#include <string>
#include <unordered_map>
#include <list>
#include "visitor.h"

using namespace std; 


enum BinaryOp { PLUS_OP, MINUS_OP, MUL_OP, DIV_OP };

class Exp {
public:
    virtual int  accept(Visitor* visitor) = 0;
    virtual ~Exp() = 0;
    static char binopToChar(BinaryOp op);
};

class BinaryExp : public Exp {
public:
    Exp *left, *right;
    BinaryOp op;
    BinaryExp(Exp* l, Exp* r, BinaryOp op);
    int accept(Visitor* visitor);
    ~BinaryExp();
};

class NumberExp : public Exp {
public:
    int value;
    NumberExp(int v);
    int accept(Visitor* visitor);
    ~NumberExp();
};

class IdentifierExp : public Exp {
public:
    std::string name;
    IdentifierExp(const std::string& n);
    int accept(Visitor* visitor);
    ~IdentifierExp();
};

class Stm {
public:
    virtual int accept(Visitor* visitor) = 0;
    virtual ~Stm() = 0;
};

class AssignStatement : public Stm {
public:
    std::string id;
    Exp* rhs;
    AssignStatement(std::string id, Exp* e);
    int accept(Visitor* visitor);
    ~AssignStatement();
};

class PrintStatement : public Stm {
public:
    Exp* e;
    PrintStatement(Exp* e);
    int accept(Visitor* visitor);
    ~PrintStatement();
};

class Program {
public:
    std::list<Stm*> slist;
    Program();
    void add(Stm* s);
    ~Program();
};

//Nueva clase if statement 
class IfStatement : public Stm {
public:
    Exp* condition;      // Expression condicional
    list<Stm*> thenList;   // Lista de sentencias para 'then'
    list<Stm*> elseList;   // Lista de sentencias para 'else'
    
    IfStatement(Exp* cond, list<Stm*> thenL, list<Stm*> elseL);
    int accept(Visitor* visitor);
    ~IfStatement();
};

//Nueva clase CExp:
class CExp : public Exp {
public:
    Exp* left;
    Exp* right;
    std::string op;  // Operador relacional: "<", "<=", "=="

    CExp(Exp* l, const std::string& op, Exp* r);
    int accept(Visitor* visitor) override;
    ~CExp();
};


#endif // EXP_H

