#ifndef VISITOR_H
#define VISITOR_H

class BinaryExp;
class NumberExp;
class BoolExp;
class IFExp;
class FcallExp;
class IdentifierExp;
class AssignStatement;
class PrintStatement;
class IfStatement;
class WhileStatement;
class ForStatement;
class VarDec;
class VarDecList;
class StatementList;
class Body;
class Program;
class ArgList;
class FuncDecList;
class FuncDec;
class ParamDecList;
class ReturnStatement;


class Visitor {
public:
    virtual int visit(IFExp* exp) = 0;
    virtual int visit(BinaryExp* exp) = 0;
    virtual int visit(NumberExp* exp) = 0;
    virtual int visit(FcallExp* exp) = 0;
    virtual int visit(BoolExp* exp) = 0;
    virtual int visit(IdentifierExp* exp) = 0;
    virtual void visit(AssignStatement* stm) = 0;
    virtual void visit(PrintStatement* stm) = 0;
    virtual void visit(IfStatement* stm) = 0;
    virtual void visit(WhileStatement* stm) = 0;
    virtual void visit(ForStatement* stm) = 0;
    virtual void visit(VarDec* stm) = 0;
    virtual void visit(VarDecList* stm) = 0;
    virtual void visit(StatementList* stm) = 0;
    virtual void visit(Body* b) = 0;
    virtual void visit(ArgList* argList) = 0; // Declaración de visit para ArgList
    virtual void visit(FuncDecList* funcDecList) = 0;
    virtual void visit(FuncDec* funcDec) = 0;
    virtual void visit(ParamDecList* paramDecList) = 0;
    virtual void visit(ReturnStatement* stm) = 0;

};

class PrintVisitor : public Visitor {
public:
    void imprimir(Program* program);
    int visit(IFExp* exp) override;
    int visit(BinaryExp* exp) override;
    int visit(NumberExp* exp) override;
    int visit(FcallExp* exp) override;
    int visit(BoolExp* exp) override;
    int visit(IdentifierExp* exp) override;
    void visit(AssignStatement* stm) override;
    void visit(PrintStatement* stm) override;
    void visit(IfStatement* stm) override;
    void visit(WhileStatement* stm) override;
    void visit(ForStatement* stm) override;
    void visit(VarDec* stm) override;
    void visit(VarDecList* stm) override;
    void visit(StatementList* stm) override;
    void visit(Body* b) override;
    void visit(ArgList* argList) override; // Declaración de visit para ArgList
    void visit(FuncDecList* funcDecList) override;
    void visit(FuncDec* funcDec) override;
    void visit(ParamDecList* paramDecList) override;
    void visit(ReturnStatement* stm) override;

};



#endif // VISITOR_H