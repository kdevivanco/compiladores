#ifndef VISITOR_H
#define VISITOR_H

#include "exp.h"
#include "scanner.h"
#include "token.h"


class BinaryExp;
class NumberExp;
class IdentifierExp;
class AssignStatement;
class PrintStatement;
class Program;


class Visitor {
public:
    virtual void visit(BinaryExp* exp) = 0;
    virtual void visit(NumberExp* exp) = 0;
    virtual void visit(IdentifierExp* exp) = 0;
    virtual void visit(AssignStatement* stm) = 0;
    virtual void visit(PrintStatement* stm) = 0;
};

class PrintVisitor : public Visitor {
public:
    void imprimir(Program* program);
    void visit(BinaryExp* exp) override;
    void visit(NumberExp* exp) override;
    void visit(IdentifierExp* exp) override;
    void visit(AssignStatement* stm) override;
    void visit(PrintStatement* stm) override;
};

class EVALVisitor: public Visitor{
public:
    void ejecutar(Program* program);
    void visit(AssignStatement* stm) override;
    void visit(PrintStatement* stm) override;
    void visit(BinaryExp* exp) override;
    void visit(NumberExp* exp) override;
    void visit(IdentifierExp* exp) override;
};

#endif // VISITOR_H