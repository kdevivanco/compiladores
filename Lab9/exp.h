#ifndef EXP_H
#define EXP_H

#include <string>
#include <unordered_map>
#include <list>
#include "visitor.h"

enum BinaryOp { PLUS_OP, MINUS_OP, MUL_OP, DIV_OP };

class Exp {
public:
    virtual void print() = 0;
    virtual int eval(std::unordered_map<std::string, int>& symbolTable) = 0;
    virtual int accept(Visitor* visitor) = 0;
    virtual ~Exp() = 0;
    static char binopToChar(BinaryOp op);
};

class BinaryExp : public Exp {
public:
    Exp *left, *right;
    BinaryOp op;
    BinaryExp(Exp* l, Exp* r, BinaryOp op);
    void print();
    int eval(std::unordered_map<std::string, int>& symbolTable);
    int accept(Visitor* visitor);
    ~BinaryExp();
};

class NumberExp : public Exp {
public:
    int value;
    NumberExp(int v);
    void print();
    int eval(std::unordered_map<std::string, int>& symbolTable);
    int accept(Visitor* visitor);
    ~NumberExp();
};

class IdentifierExp : public Exp {
public:
    std::string name;
    IdentifierExp(const std::string& n);
    void print();
    int eval(std::unordered_map<std::string, int>& symbolTable);
    int accept(Visitor* visitor);
    ~IdentifierExp();
};

class Stm {
public:
    virtual void execute(std::unordered_map<std::string, int>& symbolTable) = 0;
    virtual int accept(Visitor* visitor) = 0;
    virtual ~Stm() = 0;
};

class AssignStatement : public Stm {
public:
    std::string id;
    Exp* rhs;
    AssignStatement(std::string id, Exp* e);
    void execute(std::unordered_map<std::string, int>& symbolTable);
    int accept(Visitor* visitor);
    ~AssignStatement();
};

class PrintStatement : public Stm {
public:
    Exp* e;
    PrintStatement(Exp* e);
    void execute(std::unordered_map<std::string, int>& symbolTable);
    int accept(Visitor* visitor);
    ~PrintStatement();
};

class Program {
public:
    std::list<Stm*> slist;
    std::unordered_map<std::string, int> memoria;
    Program();
    void add(Stm* s);
    void interprete();
    ~Program();
};

#endif // EXP_H