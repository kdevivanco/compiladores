#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <cstring>
#include <cmath>
#include <unordered_map>
#include <vector>

using namespace std;




// Extender la calculadora a un lenguaje imperativo, para esto debe modificar la gramática: 
// - program: StmtList
// - StmList:= Smtmt (‘;’ Stmt)*
// - Stmt::= id ‘=‘ Exp | ‘print’ ‘(‘ Exp ‘)’
// - Exp::= Term ((‘+’|’-‘) Term)*
// - Term::= Factor ((‘*’|’/‘)Factor)* 
// - Factor ::= id 


// Clases de Token
class Token {
public:
    enum Type {PLUS_TK, MINUS_TK, MUL_TK, DIV_TK, NUM_TK, ERR_TK, END_TK, PO_TK, PC_TK, POW_TK, ID_TK, EQUAL_TK, PRINT_TK, SEMICOLON_TK}; // Agregados ID_TK, EQUAL_TK, PRINT_TK, SEMICOLON_TK
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

enum BinaryOp { PLUS, MINUS, MUL, DIV, POW };

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

class Stm {
public:
    virtual void execute() = 0;
    virtual ~Stm() = 0;
};

class AssignStatement : public Stm {
private:
    string id;
    Exp* rhs;
public:
    AssignStatement(const string& id, Exp* rhs);
    void execute();
    ~AssignStatement();
};

class PrintStatement : public Stm {
private:
    Exp* exp;
public:
    PrintStatement(Exp* exp);
    void execute();
    ~PrintStatement();
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
    Stm* parseStatement();
    bool tokenToOp(Token* tk, BinaryOp& op);
public:
    Parser(Scanner* scanner);
    void parse();
};

// Implementación de Token
Token::Token(Type type): type(type) { text = ""; }

Token::Token(Type type, char c): type(type) { text = c; }

Token::Token(Type type, const string& source, int first, int last): type(type) {
    text = source.substr(first, last - first);
}

// Implementación de Scanner
Scanner::Scanner(const char* s): input(s), first(0), current(0) { }

Token* Scanner::nextToken() {
    Token* token;
    while (input[current] == ' ') current++;
    if (input[current] == '\0') return new Token(Token::END_TK);
    char c = input[current];
    first = current;
    
    if (isdigit(c)) {
        current++;
        while (isdigit(input[current]) || input[current] == '.')
            current++;
        token = new Token(Token::NUM_TK, input, first, current);
    } 
    else if (c == '*' && input[current + 1] == '*') {
        current += 2;
        token = new Token(Token::POW_TK, '^');
    } 
    else if (isalpha(c)) {
        current++;
        while (isalnum(input[current])) current++;
        token = new Token(Token::ID_TK, input, first, current);
    }
    else if (c == '=') {
        current++;
        token = new Token(Token::EQUAL_TK, '=');
    }
    else if (c == ';') {
        current++;
        token = new Token(Token::SEMICOLON_TK, ';');
    }
    else if (c == '(') {
        current++;
        token = new Token(Token::PO_TK, '(');
    }
    else if (c == ')') {
        current++;
        token = new Token(Token::PC_TK, ')');
    }
    else if (c == '+') {
        current++;
        token = new Token(Token::PLUS_TK, '+');
    }
    else if (c == '-') {
        current++;
        token = new Token(Token::MINUS_TK, '-');
    }
    else if (c == '*') {
        current++;
        token = new Token(Token::MUL_TK, '*');
    }
    else if (c == '/') {
        current++;
        token = new Token(Token::DIV_TK, '/');
    }
    else if (c == 'p' && input.substr(current, 5) == "print") {
        current += 5;
        token = new Token(Token::PRINT_TK, "print");
    }
    else {
        token = new Token(Token::ERR_TK, c);
        current++;
    }
    return token;
}

Scanner::~Scanner() { }

// Implementación de Exp
char Exp::binopToChar(BinaryOp op) {
    char c = ' ';
    switch(op) {
        case PLUS: c = '+'; break;
        case MINUS: c = '-'; break;
        case MUL: c = '*'; break;
        case DIV: c = '/'; break;
        case POW: c = '^'; break;
        default: c = '$';
    }
    return c;
}

BinaryExp::BinaryExp(Exp* l, Exp* r, BinaryOp op): left(l), right(r), op(op) {}
NumberExp::NumberExp(float v): value(v) {}

Exp::~Exp() {}
BinaryExp::~BinaryExp() { delete left; delete right; }
NumberExp::~NumberExp() { }

void BinaryExp::print() {
    left->print();
    char c = binopToChar(this->op);
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
        case PLUS: result = v1 + v2; break;
        case MINUS: result = v1 - v2; break;
        case MUL: result = v1 * v2; break;
        case DIV: result = v1 / v2; break;
        case POW: result = pow(v1, v2); break;
        default: cout << "Operador desconocido" << endl; result = 0;
    }
    return result;
}

float NumberExp::eval() {
    return value;
}

// Implementación de Stm
AssignStatement::AssignStatement(const string& id, Exp* rhs): id(id), rhs(rhs) {}

void AssignStatement::execute() {
    // Implementar la lógica para almacenar el valor de 'id' en un mapa
    // Ejemplo de uso: map<string, float> memory;
    // memory[id] = rhs->eval();
    // Utiliza 'memory' para almacenar el valor
}

AssignStatement::~AssignStatement() { delete rhs; }

PrintStatement::PrintStatement(Exp* exp): exp(exp) {}

void PrintStatement::execute() {
    cout << exp->eval() << endl;
}

PrintStatement::~PrintStatement() { delete exp; }

// Implementación de Parser
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
        Token* temp = current;
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

Parser::Parser(Scanner* sc): scanner(sc) {
    previous = current = NULL;
}

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
    vector<Stm*> statements;
    while (!isAtEnd()) {
        statements.push_back(parseStatement());
        if (!match(Token::END_TK)) {
            if (!match(Token::SEMICOLON_TK)) {
                cout << "Error: Se esperaba ';' después de instrucción." << endl;
                exit(0
                exit(0);
            }
        }
    }
    for (auto statement : statements) {
        statement->execute();
        delete statement; // Limpiar memoria después de ejecutar
    }
}

Exp* Parser::parseExpression() {
    Exp* left = parseTerm();
    while (match(Token::PLUS_TK) || match(Token::MINUS_TK)) {
        BinaryOp op = (previous->type == Token::PLUS_TK) ? PLUS : MINUS;
        Exp* right = parseTerm();
        left = new BinaryExp(left, right, op);
    }
    return left;
}

Exp* Parser::parseTerm() {
    Exp* left = parseFexp();
    while (match(Token::MUL_TK) || match(Token::DIV_TK)) {
        BinaryOp op = (previous->type == Token::MUL_TK) ? MUL : DIV;
        Exp* right = parseFexp();
        left = new BinaryExp(left, right, op);
    }
    return left;
}

Exp* Parser::parseFactor() {
    if (match(Token::NUM_TK)) {
        return new NumberExp(stof(previous->text));
    }
    if (match(Token::ID_TK)) {
        // Aquí puedes implementar el manejo de variables si es necesario
        return new NumberExp(0); // Solo como placeholder
    }
    if (match(Token::PO_TK)) { // Abre paréntesis
        Exp* expr = parseExpression();
        if (!match(Token::PC_TK)) {
            cout << "Error: Falta paréntesis de cierre." << endl;
            exit(0);
        }
        return expr;
    }
    cout << "Error: Token inesperado: " << current->text << endl;
    exit(0);
}

Stm* Parser::parseStatement() {
    if (match(Token::ID_TK)) {
        string id = previous->text;
        if (!match(Token::EQUAL_TK)) {
            cout << "Error: Se esperaba '=' después del identificador." << endl;
            exit(0);
        }
        Exp* exp = parseExpression();
        return new AssignStatement(id, exp);
    } else if (match(Token::PRINT_TK)) {
        Exp* exp = parseExpression();
        return new PrintStatement(exp);
    }
    cout << "Error: Instrucción no válida." << endl;
    exit(0);
}

// Programa principal
int main() {
    const char* input = "a = 2; b = 3; c = a ** b; print c;";
    Scanner scanner(input);
    Parser parser(&scanner);
    parser.parse();
    return 0;
}
