#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <cstring>

using namespace std;


// Clases //
class Token {
public:
    enum Type {PLUS_TK, MINUS_TK, MUL_TK, DIV_TK, NUM_TK, ERR_TK, END_TK, PO_TK, PC_TK, POW_TK}; //Agregamos POW_tk
    Type type;
    string text;
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

enum BinaryOp { PLUS, MINUS, MUL, DIV, POW }; // Agrega POW


class Exp {
public:
    virtual void print() = 0;
    virtual float eval() = 0;
    virtual ~Exp() = 0;
    static char binopToChar(BinaryOp op);
};


class BinaryExp : public Exp {
public:
    Exp *left, *right;
    BinaryOp op;
    BinaryExp(Exp* l, Exp* r, BinaryOp op);
    void print();
    float eval();
    ~BinaryExp();
};

class NumberExp : public Exp {
public:
    float value;
    NumberExp(float v);
    void print();
    float eval();
    ~NumberExp();
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
    Exp* parseFexp();
    bool tokenToOp(Token* tk, BinaryOp& op);
public:
    Parser(Scanner* scanner);
    Exp* parse();
};


Token::Token(Type type):type(type) { text = ""; }

Token::Token(Type type, char c):type(type) { text = c;}

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
    if (input[current] == '\0') return new Token(Token::END_TK);
    char c  = input[current];
    first = current;
    
    if (isdigit(c)) {
        current++;
        while (isdigit(input[current]) || input[current] == '.')
            current++;
        token = new Token(Token::NUM_TK,input,first,current-first);
    } 
    else if (c == '*' && input[current + 1] == '*') { // Token potencia
        current += 2;
        token = new Token(Token::POW_TK, '^');
    
    } else if (strchr("+-*/()", c)) {
        switch(c) {
            case '+': token = new Token(Token::PLUS_TK,c); break;
            case '-': token = new Token(Token::MINUS_TK,c); break;
            case '*': token = new Token(Token::MUL_TK,c); break;
            case '/': token = new Token(Token::DIV_TK,c); break;
            case '(': token = new Token(Token::PO_TK,c); break;
            case ')': token = new Token(Token::PC_TK,c); break;
            default: cout << "No deberia llegar aca" << endl;
        }
        current++;
    } else {
        token = new Token(Token::ERR_TK, c);
        current++;
    }
    return token;

}
Scanner::~Scanner() { }

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
        if (check(Token::ERR_TK)) {
            cout << "Parse error, unrecognised character: " << current->text << endl;
            exit(0);
        }
        return true;
    }
    return false;
}

bool Parser::isAtEnd() {
    return (current->type == Token::END_TK);
}

Parser::Parser(Scanner* sc):scanner(sc) {
    previous = current = NULL;
    return;
};

Exp* Parser::parseFexp() {
    Exp* left = parseFactor();
    if (match(Token::POW_TK)) {
        Exp* right = parseFexp(); // Asocatividad derecha 
        left = new BinaryExp(left, right, POW); 
    }
    return left;
}


Exp* Parser::parse() {
    current = scanner->nextToken();
    if (check(Token::ERR_TK)) {
        cout << "Error en scanner - caracter invalido" << endl;
        exit(0);
    }
    Exp* exp = parseExpression();
    if (current) delete current;
    return exp;
}

Exp* Parser::parseExpression() {

    Exp* left = parseTerm();

    while (match(Token::PLUS_TK) || match(Token::MINUS_TK)) {
        BinaryOp op;
        if (previous->type == Token::PLUS_TK){
            op = PLUS;
        }
        else if (previous->type == Token::MINUS_TK){
            op = MINUS;
        }
        Exp* right = parseTerm();
        left = new BinaryExp(left, right, op);
    }

    return left;
}



Exp* Parser::parseTerm() {
    Exp* left = parseFexp(); // cambiado a parseFexp

    while (match(Token::MUL_TK) || match(Token::DIV_TK)) {
        BinaryOp op;
        if (previous->type == Token::MUL_TK){
            op = MUL;
        }
        else if (previous->type == Token::DIV_TK){
            op = DIV;
        }
        Exp* right = parseFexp(); // actualizado a parse Fexp
        left = new BinaryExp(left, right, op);
    }
    return left;
}


Exp* Parser::parseFactor() {
    if (match(Token::NUM_TK)) {
        return new NumberExp(stof(previous->text));
    }
    else if (match(Token::PO_TK)){
    Exp* e = parseExpression();
        if (match(Token::PC_TK)){
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
        case POW: c='^'; break;
        default: c = '$';
    }
    return c;
}

// AST //


BinaryExp::BinaryExp(Exp* l, Exp* r, BinaryOp op):left(l),right(r),op(op) {}
NumberExp::NumberExp(float v):value(v) {}

Exp::~Exp() {}
BinaryExp::~BinaryExp() { delete left; delete right; }
NumberExp::~NumberExp() { }


void BinaryExp::print() {
    left->print();
    char c = binopToChar(this->op);;
    cout << ' ' << c << ' ';
    right->print();
}



void NumberExp::print() {
    cout << value;
}


float BinaryExp::eval() {
    float result;
    float v1 = left->eval();
    float v2 = right->eval();
    switch(this->op) {
        case PLUS: 
            result = v1 + v2; 
            
            break;
        case MINUS: result = v1 - v2; break;
        case MUL: result = v1 * v2; break;
        case DIV: result = v1 / v2; break;
        case POW: result = pow(v1, v2); break; // funcion pow de cmath
        default:
            cout << "Operador desconocido" << endl;
            result = 0;
    }
    return result;
}


float NumberExp::eval() {
    return value;
}


void test_scanner(Scanner * scanner) {
    Token* current;
    current = scanner->nextToken();
    while (current->type != Token::END_TK) {
        if (current->type == Token::ERR_TK) {
            cout << "Error en scanner - caracter invalido: " << current->text << endl;
            break;
        } else
            cout << current << endl;
        current = scanner->nextToken();
    }
    exit(1);

}

int main(int argc, const char* argv[]) {

    if (argc != 2) {
        cout << "Incorrect number of arguments" << endl;
        exit(1);
    }

    Scanner scanner(argv[1]);

    Parser parser(&scanner);

    Exp *exp = parser.parse();

    cout << "expr: ";
    exp->print();
    cout << endl;


    cout << "eval: ";
    cout << exp->eval() << endl;

    delete exp;
}