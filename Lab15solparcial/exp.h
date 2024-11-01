#ifndef EXP_H
#define EXP_H

#include <string>
#include <list>
#include "visitor.h"
#include <vector>
using namespace std;
enum BinaryOp { PLUS_OP, MINUS_OP, MUL_OP, DIV_OP,LT_OP, LE_OP, EQ_OP };

class Body;
//class ArgList;

class Exp {
public:
    virtual int  accept(Visitor* visitor) = 0;
    virtual ~Exp() = 0;
    static string binopToChar(BinaryOp op);
};
class IFExp : public Exp {
public:
    Exp *cond,*left, *right;
    IFExp(Exp *cond, Exp* l, Exp* r);
    int accept(Visitor* visitor);
    ~IFExp();
};

class ArgList {
public:
    std::list<Exp*> args;

    // Constructor vacío
    ArgList() = default;

    // Método para añadir un argumento
    void addArgument(Exp* exp) {
        args.push_back(exp);
    }

    // Método para aceptar el visitor
    int accept(Visitor* visitor) {
        for (Exp* arg : args) {
            arg->accept(visitor); // Llama al visitor en cada argumento
        }
        return 0;
    }

    // Destructor para liberar memoria
    ~ArgList() {
        for (Exp* exp : args) {
            delete exp;
        }
    }
};

class FcallExp : public Exp {
public:
    string nombre;
    list<Exp* > entradas;
    int accept(Visitor* visitor);
    FcallExp();
    // Sobrecarga de operador `=` para aceptar ArgList
    FcallExp& operator=(const ArgList& argList);

    // Destructor para liberar memoria en `entradas`
    ~FcallExp() {
        for (Exp* exp : entradas) {
            delete exp;
        }
    }
//    ~FcallExp();
};
class BinaryExp : public Exp {
public:
    Exp *left, *right;
    string type;
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

class BoolExp : public Exp {
public:
    int value;
    BoolExp(bool v);
    int accept(Visitor* visitor);
    ~BoolExp();
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


class IfStatement : public Stm {
public:
    Exp* condition;
    Body* then;
    Body* els;
    IfStatement(Exp* condition, Body* then, Body* els);
    int accept(Visitor* visitor);
    ~IfStatement();
};
class WhileStatement : public Stm {
public:
    Exp* condition;
    Body* b;
    WhileStatement(Exp* condition, Body* b);
    int accept(Visitor* visitor);
    ~WhileStatement();
};

//FOR(int,int,int) stmlist endfor
class ForStatement : public Stm {
public:
    Exp* start;
    Exp* end;
    Exp* step;
    Body* b;
    ForStatement(Exp* start, Exp* end, Exp* step, Body* b);
    int accept(Visitor* visitor);
    ~ForStatement();
};

class ReturnStatement : public Stm
{
public:
    Exp* exp;
    ReturnStatement() {};
    int accept(Visitor* visitor);
    ~ReturnStatement(){};
};

class VarDec {
public:
    string type;
    list<string> vars;
    VarDec(string type, list<string> vars);
    int accept(Visitor* visitor);
    ~VarDec();
};

class VarDecList{
public:
    list<VarDec*> vardecs;
    VarDecList();
    void add(VarDec* vardec);
    int accept(Visitor* visitor);
    ~VarDecList();
};

class StatementList {
public:
    list<Stm*> stms;
    StatementList();
    void add(Stm* stm);
    int accept(Visitor* visitor);
    ~StatementList();
};


class ParamDecList
{
public:
    vector<string> types;
    vector<string> params;
    ParamDecList(){};
    void add(const string& type, const string& param) {
        types.push_back(type);
        params.push_back(param);
    }
    int accept(Visitor* visitor);
    ~ParamDecList(){};
};

class FuncDec
{
public:
    string type;
    string name;
    ParamDecList* params;
    Body* body;
    FuncDec(string type, ParamDecList* params, string name, Body* body): type(std::move(type)), params(params), name(std::move(name)), body(body) {};
    int accept(Visitor* visitor);
    ~FuncDec()
    {
        delete params;
    };
};

class FuncDecList
{
public:
    list<FuncDec*> funcdecs;
    FuncDecList()= default;
    void add(FuncDec* funcdec)
    {
        funcdecs.push_back(funcdec);
    };
    int accept(Visitor* visitor);
    ~FuncDecList()
    {
        for (auto f : funcdecs)
        {
            delete f;
        }
    };
};

class Body{
public:
    VarDecList* vardecs;
    StatementList* slist;
    Body(VarDecList* vardecs, StatementList* stms);
    int accept(Visitor* visitor);
    ~Body();
};

class Program {
public:
    VarDecList* vardecs;
    FuncDecList* funcdecs;
    Program(VarDecList* vardecs, FuncDecList* funcdecs);
    ~Program();
};



#endif // EXP_H