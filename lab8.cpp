#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <list>

using namespace std;

class ASTVisitor;

class Token {
public:
    enum Type {PLUS, MINUS, MUL,DIV, NUM, ERR, PD, PI, END, ASSIGN, SEMICOLON, LPAREN, RPAREN,PRINT, ID};
    Type type;
    string text;
    string lexema;
    Token(Type);
    Token(Type, char c);
    Token(Type, const string& source, int first, int last);
};

class Scanner {
private:
    string input;
    int first, current;
public:
    Scanner(const char* in_s);
    Token* nextToken();
    ~Scanner();

};

enum BinaryOp { PLUS, MINUS,MUL,DIV };


class Exp {
public:
    virtual ~Exp() = 0;
    static char binopToChar(BinaryOp op);
    virtual int accept(ASTVisitor* v) = 0;
};


class BinaryExp : public Exp {
public:
    Exp *left, *right;
    BinaryOp op;
    BinaryExp(Exp* l, Exp* r, BinaryOp op);
    ~BinaryExp();
    int accept(ASTVisitor* v);
//    virtual int accept(ASTVisitor* v) = 0;

};

class NumberExp : public Exp {
public:
    int value;
    NumberExp(int v);
    ~NumberExp();
    int accept(ASTVisitor* v);
};

class IdExp : public Exp {
    public:
        string id;
        IdExp(string id);
        int accept(ASTVisitor* v);
        ~IdExp();
};
class ParenthExp : public Exp {
    public:
        Exp *e;
        ParenthExp(Exp *e);
        int accept(ASTVisitor* v);
        ~ParenthExp();
};

class Stm {
public:
    virtual int accept(ASTVisitor* v) = 0;
    virtual ~Stm() = 0;
//    virtual int visit(Stm* s) = 0;
};

class AssignStatement : public Stm {
public:
    string id;
    Exp* rhs;
    AssignStatement(string id, Exp* e);
    int accept(ASTVisitor* v);
    ~AssignStatement();
};

class PrintStatement : public Stm {
public:
    Exp* e;
    PrintStatement(Exp* e);
    int accept(ASTVisitor* v);
    ~PrintStatement();
};


class Program {
public:
    list<Stm*> slist;
    Program();
    void add(Stm* s);
    int accept(ASTVisitor* v);
    ~Program();
};



class Parser {
private:
    Scanner* scanner;
    Token *current, *previous;

    bool match(Token::Type ttype);
    bool check(Token::Type ttype);
    bool advance();
    bool isAtEnd();
    Exp* parseExpression();
    Exp* parseTerm();
    Exp* parseFactor();
    bool tokenToOp(Token* tk, BinaryOp& op);
public:
    Parser(Scanner* scanner);
    Exp* parse();
    Program* parser();
    Stm* parseStatement();
    Program* parseProgram();
};



Token::Token(Type type):type(type) { text = ""; }

Token::Token(Type type, char c):type(type) { text = c; }

Token::Token(Type type, const string& source, int first, int last):type(type) {
    text = source.substr(first,last);
}

std::ostream& operator << ( std::ostream& outs, const Token & tok )
{
    if (tok.text.empty())
        return outs << tok.type;
    else
        return outs << "TOK" << "(" << tok.text << ")";
}

std::ostream& operator << ( std::ostream& outs, const Token* tok ) {
    return outs << *tok;
}

// SCANNER //

Scanner::Scanner(const char* s):input(s),first(0), current(0) { }

Token* Scanner::nextToken() {
    Token* token;
    while (input[current]==' ') current++;
    if (input[current] == '\0') return new Token(Token::END);
    char c  = input[current];
    first = current;
    //if is a character like x=5 then the x is an ID:
    if (isalpha(c)) {
        current++;
        while (isalnum(input[current]))
            current++;
        token = new Token(Token::ID,input,first,current-first);
    }
    //if its an = its ASSIGN:
   else if (c == '=') {
        token = new Token(Token::ASSIGN,c);
        current++;
    }
   //else if semicolon, its the end of that statment:
    else if (c == ';') {
        token = new Token(Token::SEMICOLON,c);
        current++;
    }
    else if (isdigit(c)) {
        current++;
        while (isdigit(input[current]))
            current++;
        token = new Token(Token::NUM,input,first,current-first);
    } else if (strchr("+-*/()", c)) {
        switch(c) {
            case '+': token = new Token(Token::PLUS,c); break;
            case '-': token = new Token(Token::MINUS,c); break;
            case '*': token = new Token(Token::MUL,c); break;
            case '/': token = new Token(Token::DIV,c); break;
            case '(': token = new Token(Token::PI,c); break;
            case ')': token = new Token(Token::PD,c); break;
            default: cout << "No deberia llegar aca" << endl;
        }
        current++;
    } else {
        token = new Token(Token::ERR, c);
        current++;
    }
    return token;
}

Scanner::~Scanner() { }


Program* Parser::parser() {
    current = scanner->nextToken();
    //debug
    cout << "current: " << current << endl;
    if (check(Token::ERR)) {
        //debug variable
        cout << "Error en scanner - caracter invalido" << endl;
        exit(0);
    }
    Program* p = parseProgram();
    cout << p->slist.size() << endl;
    if (current->type != Token::
    END) {
        delete p;
        p = NULL;
    }
    if (current) delete current;
    return p;
}


Program* Parser::parseProgram() {
    Program* p = new Program();
    p->add(parseStatement());
    while(match(Token::SEMICOLON)) {
        p->add(parseStatement());
    }
    return p;
}
Stm* Parser::parseStatement() {
    cout<< "holaaa"<<endl;
    Stm* s = NULL;
    Exp* e;
    if (match(Token::ID)) {
        string lex = previous->lexema;
        if (!match(Token::ASSIGN)) {
            exit(0);
        }
        s = new AssignStatement(lex, parseExpression());
    } else if (match(Token::PRINT)) {
        if (!match(Token::LPAREN)) {
            exit(0);
        }
        e = parseExpression();
        if (!match(Token::RPAREN)) {
            exit(0);
        }
        s = new PrintStatement(e);
    } else {
        exit(0);
    }
    return s;
}



class ASTVisitor {
public:
    virtual int visit(BinaryExp* e) = 0;
    virtual int visit(NumberExp* e) = 0;
//    virtual int visit(AssignStatement* s) = 0;
//    virtual int visit(PrintStatement* s) = 0;
//    virtual int visit(Program* p) = 0;
//    virtual int visit(IdExp* e) = 0;
//    virtual int visit(ParenthExp* e) = 0;



};


class ASTPrinter : public ASTVisitor{
public:
    void print(Exp*);
    int visit(BinaryExp* e);
    int visit(NumberExp* e);
};

class ASTEvaluator : public ASTVisitor {
public:
    int eval(Exp*);
    int visit(BinaryExp* e);
    int visit(NumberExp* e);
};

// PARSER //

bool Parser::match(Token::Type ttype) {
    if (check(ttype)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(Token::Type ttype) {
    if (isAtEnd()) return false;
    return current->type == ttype;
}

bool Parser::advance() {
    if (!isAtEnd()) {
        Token* temp =current;
        if (previous) delete previous;
        current = scanner->nextToken();
        previous = temp;
        if (check(Token::ERR)) {
            cout << "Parse error, unrecognised character: " << current->text << endl;
            exit(0);
        }
        return true;
    }
    return false;
}

bool Parser::isAtEnd() {
    return (current->type == Token::END);
}

Parser::Parser(Scanner* sc):scanner(sc) {
    previous = current = NULL;
    return;
};

Exp* Parser::parse() {
    current = scanner->nextToken();
    if (check(Token::ERR)) {
        cout << "Error en scanner - caracter invalido" << endl;
        exit(0);
    }
    Exp* exp = parseExpression();
    if (current) delete current;
    return exp;
}

//Program* Parser::parser()
//{
//    current = scanner->nextToken();
//    if (check(Token::ERR)) {
//        cout << "Error en scanner - caracter invalido" << endl;
//        exit(0);
//    }
//    Program* p = parseProgram();
//    if (current->type != Token::END) {
//        delete p;
//        p = NULL;
//    }
//    if (current) delete current;
//    return p;
//}

Exp* Parser::parseExpression() {

    Exp* left = parseTerm();

    while (match(Token::PLUS) || match(Token::MINUS)) {
        BinaryOp op;
        if (previous->type == Token::PLUS){
            op = PLUS;
        }
        else if (previous->type == Token::MINUS){
            op = MINUS;
        }
        Exp* right = parseTerm();
        left = new BinaryExp(left, right, op);
    }

    return left;
}

Exp* Parser::parseTerm() {

    Exp* left = parseFactor();

    while (match(Token::MUL) || match(Token::DIV)) {
        BinaryOp op;
        if (previous->type == Token::MUL){
            op = MUL;
        }
        else if (previous->type == Token::DIV){
            op = DIV;
        }
        Exp* right = parseFactor();
        left = new BinaryExp(left, right, op);
    }
    return left;
}

Exp* Parser::parseFactor() {
    Exp* e;
    if (match(Token::NUM)) {
        return new NumberExp(stoi(previous->text));
    }
    else if (match(Token::PI)){
        e = parseExpression();
        if (!match(Token::PD)){
            cout << "Falta parentesis derecho" << endl;
        }
        return e;
    }
    cout << "Error: se esperaba un número." << endl;
    exit(0);
}

char Exp::binopToChar(BinaryOp op) {
    char  c=' ';
    switch(op) {
        case PLUS: c = '+'; break;
        case MINUS: c = '-'; break;
        case MUL: c = '*'; break;
        case DIV: c = '/'; break;
        default: c = '$';
    }
    return c;
}

// AST //



void ASTPrinter::print(Exp* e) {
    cout << "expression: ";
    e->accept(this);
    cout << endl;
    return;
}

int ASTPrinter::visit(BinaryExp* e) {
    e->left->accept(this);
    cout << ' ' << Exp::binopToChar(e->op) << ' ';
    e->right->accept(this);
    return 0;
}

int ASTPrinter::visit(NumberExp* e) {
    cout << e->value;
    return 0;
}

int ASTEvaluator::eval(Exp* e) {
    return e->accept(this);
}

int ASTEvaluator::visit(BinaryExp* e) {
    int v1 = e->left->accept(this);
    int v2 = e->right->accept(this);
    int result = 0;
    switch(e->op) {
        case PLUS: result = v1+v2; break;
        case MINUS: result = v1-v2; break;
        case MUL: result = v1 * v2; break;
        case DIV: result = v1 / v2; break;
        }
    return result;
}


int BinaryExp::accept(ASTVisitor* v) {
    return v->visit(this);
}
int NumberExp::accept(ASTVisitor* v) {
    return v->visit(this);
}


int ASTEvaluator::visit(NumberExp* e) {
    return e->value;
}

BinaryExp::BinaryExp(Exp* l, Exp* r, BinaryOp op):left(l),right(r),op(op) {}
NumberExp::NumberExp(int v):value(v) {}

Exp::~Exp() {}
BinaryExp::~BinaryExp() { delete left; delete right; }
NumberExp::~NumberExp() { }

Stm::~Stm() {}
Program::~Program() {
    for (list<Stm*>::iterator it = slist.begin(); it != slist.end(); it++) {
        delete *it;
    }
}

void Program::add(Stm *s) {
    slist.push_back(s);
}
Program::Program() {
    slist = list<Stm*>();
}
AssignStatement::AssignStatement(string id, Exp* e):id(id),rhs(e) {}
PrintStatement::PrintStatement(Exp* e):e(e) {}
PrintStatement::~PrintStatement() { delete e; }
AssignStatement::~AssignStatement() { delete rhs; }



int AssignStatement::accept(ASTVisitor* v) {
    return v->visit(reinterpret_cast<BinaryExp *>(this));
}

int PrintStatement::accept(ASTVisitor* v) {
    return v->visit(reinterpret_cast<BinaryExp *>(this));
}



void test_scanner(Scanner * scanner) {
    Token* current;
    current = scanner->nextToken();
    while (current->type != Token::END) {
        if (current->type == Token::ERR) {
            cout << "Error en scanner - caracter invalido: " << current->text << endl;
            break;
        } else
            cout << current << endl;
        current = scanner->nextToken();
    }
    exit(1);

}

int Program::accept(ASTVisitor* v) {
    for (list<Stm*>::iterator it = slist.begin(); it != slist.end(); it++) {
        (*it)->accept(v);
    }
    return 0;
}

int main(int argc, const char* argv[]) {

    if (argc != 2) {
        cout << "Incorrect number of arguments" << endl;
        exit(1);
    }
    cout << "holaaaa" << endl;

    Scanner scanner(argv[1]);

    cout << "scanner " <<endl;

    //test_scanner(&scanner);

    Parser parser(&scanner);

    cout << "Parser " <<endl;

    // Exp *exp = parser.parse();
    Program *programa = parser.parser();
    cout << "programa: " << endl;

    ASTPrinter printer;
    ASTEvaluator evaluator;

    cout << "print: ";

    programa->accept(&printer);
    cout << "eval: " << programa->accept(&evaluator) << endl;
    cout << endl;


//    Exp *exp = parser.parse();
//    ASTPrinter printer;
//    printer.print(exp);
//    ASTEvaluator evaluator;
//    cout << evaluator.eval(exp);

//    cout << "expr: ";
//    cout << endl;
//    cout << "eval: ";
//    delete exp;
}