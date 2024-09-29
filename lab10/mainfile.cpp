#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <list>
#include <stdexcept>
#include <cstring>
#include <unordered_map>

using namespace std; 


class Visitor;
class Scanner;
class Token; 

enum BinaryOp { PLUS_OP, MINUS_OP, MUL_OP, DIV_OP };

class Token {
public:
    enum Type {
        PLUS, MINUS, MUL, DIV, NUM, ERR, PD, PI, END, ID, PRINT, ASSIGN, PC,  IF, ELSE, ENDIF, LT, LEQ, EQ, THEN
    };

    Type type;
    std::string text;

    Token(Type type);
    Token(Type type, char c);
    Token(Type type, const std::string& source, int first, int last);

    friend std::ostream& operator<<(std::ostream& outs, const Token& tok);
    friend std::ostream& operator<<(std::ostream& outs, const Token* tok);
};

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
public:
    Parser(Scanner* scanner);
    Program* parseProgram();
    Stm* parseStatement();
};


class Scanner {
private:
    std::string input;
    int first, current;
public:
    Scanner(const char* in_s);
    Token* nextToken();
    void reset();
    ~Scanner();
};




class BinaryExp;
class NumberExp;
class IdentifierExp;
class AssignStatement;
class PrintStatement;
class Program;
class IfStatement;

//Se agrega visit if statement a cada clase de visitor 
class Visitor {
public:
    virtual int visit(BinaryExp* exp) = 0;
    virtual int visit(NumberExp* exp) = 0;
    virtual int visit(IdentifierExp* exp) = 0;
    virtual void visit(AssignStatement* stm) = 0;
    virtual void visit(PrintStatement* stm) = 0;
    virtual void visit (IfStatement* stm) = 0; 
};

class PrintVisitor : public Visitor {
public:
    void imprimir(Program* program);
    int visit(BinaryExp* exp) override;
    int visit(NumberExp* exp) override;
    int visit(IdentifierExp* exp) override;
    void visit(AssignStatement* stm) override;
    void visit(PrintStatement* stm) override;
    void visit(IfStatement* stm) override;
};

class EVALVisitor : public Visitor {
public:
    void ejecutar(Program* program);
    int visit(BinaryExp* exp) override;
    int visit(NumberExp* exp) override;
    int visit(IdentifierExp* exp) override;
    void visit(AssignStatement* stm) override;
    void visit(PrintStatement* stm) override;
    void visit(IfStatement* stm) override;

};


void test_scanner(Scanner* scanner);


BinaryExp::BinaryExp(Exp* l, Exp* r, BinaryOp op):left(l),right(r),op(op) {}
NumberExp::NumberExp(int v):value(v) {}
IdentifierExp::IdentifierExp(const string& n):name(n) {}
Exp::~Exp() {}
BinaryExp::~BinaryExp() { delete left; delete right; }
NumberExp::~NumberExp() { }
IdentifierExp::~IdentifierExp() { }
AssignStatement::AssignStatement(string id, Exp* e): id(id), rhs(e) {}
AssignStatement::~AssignStatement() {
    delete rhs;
}
PrintStatement::PrintStatement(Exp* e): e(e) {}
PrintStatement::~PrintStatement() {
    delete e;
}
Program::Program() {}
void Program::add(Stm* s) {
    slist.push_back(s);
}
Program::~Program() {
    for (Stm* s : slist) {
        delete s;
    }
}
Stm::~Stm() {}
char Exp::binopToChar(BinaryOp op) {
    char  c=' ';
    switch(op) {
        case PLUS_OP: c = '+'; break;
        case MINUS_OP: c = '-'; break;
        case MUL_OP: c = '*'; break;
        case DIV_OP: c = '/'; break;
        default: c = '$';
    }
    return c;
}

//nuevos metodos IfStatement
IfStatement::IfStatement(Exp* cond, list<Stm*> thenL, list<Stm*> elseL) 
    : condition(cond), thenList(thenL), elseList(elseL) {}

IfStatement::~IfStatement() {
    delete condition;
    for (Stm* s : thenList) delete s;
    for (Stm* s : elseList) delete s;
}

int IfStatement::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}




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
        if (check(Token::ERR)) {
            cout << "Error de análisis, carácter no reconocido: " << current->text << endl;
            exit(1);
        }
        return true;
    }
    return false;
}

bool Parser::isAtEnd() {
    return (current->type == Token::END);
}

Parser::Parser(Scanner* sc):scanner(sc) {
    previous = NULL;
    current = scanner->nextToken();
    if (current->type == Token::ERR) {
        cout << "Error en el primer token: " << current->text << endl;
        exit(1);
    }
}

Program* Parser::parseProgram() {
    Program* p = new Program();
    try {
        while (!isAtEnd()) {
            p->add(parseStatement());
            if (!isAtEnd() && !match(Token::PC)) {
                throw runtime_error("Error: se esperaba ';' al final de la declaración.");
            }
        }
    } catch (const exception& e) {
        cout << "Error durante el parsing: " << e.what() << endl;
        delete p;
        exit(1);
    }
    return p;
}\


//Modificacion a parser: 

Stm* Parser::parseStatement() {
    Stm* s = nullptr;
    Exp* e = nullptr;

    // 
    if (match(Token::ID)) {
        std::string lex = previous->text;

        if (!match(Token::ASSIGN)) {
            throw std::runtime_error("Error: se esperaba un '=' después del identificador.");
        }
        e = parseExpression();
        s = new AssignStatement(lex, e);
    } 
    
    else if (match(Token::PRINT)) {
        if (!match(Token::PI)) { // '(' después de 'print'
            throw std::runtime_error("Error: se esperaba un '(' después de 'print'.");
        }
        e = parseExpression();
        if (!match(Token::PD)) { // ')' despues de la expresion
            throw std::runtime_error("Error: se esperaba un ')' después de la expresión.");
        }
        s = new PrintStatement(e);
    } 
    // is token if??
    else if (match(Token::IF)) {
        Exp* condition = parseExpression(); // parseo a condicional 

        if (!match(Token::THEN)) {
            throw std::runtime_error("Error: se esperaba 'then' después de la condición.");
        }

        // parse de declaracions dentro del then
        std::list<Stm*> thenList;
        thenList.push_back(parseStatement()); // Parsear al menos una declaración
        while (!isAtEnd() && !match(Token::ENDIF) && !check(Token::ELSE)) {
            thenList.push_back(parseStatement());
        }

        // parseo de declaraciones dentro del else
        std::list<Stm*> elseList;
        if (match(Token::ELSE)) {
            elseList.push_back(parseStatement());
            while (!isAtEnd() && !match(Token::ENDIF)) {
                elseList.push_back(parseStatement());
            }
        }

        //Error, no encontramos endif 
        if (!match(Token::ENDIF)) {
            throw std::runtime_error("Error: se esperaba 'endif' al final del bloque 'if'.");
        }

        s = new IfStatement(condition, thenList, elseList);
    } 
    // token inesperado ---> ERROR 
    else {
        throw std::runtime_error("Error: se esperaba un identificador, 'print' o 'if', pero se encontró: " + current->text);
    }

    return s;
}


Exp* Parser::parseExpression() {
    Exp* left = parseTerm();
    while (match(Token::PLUS) || match(Token::MINUS)) {
        BinaryOp op;
        if (previous->type == Token::PLUS){
            op = PLUS_OP;
        }
        else if (previous->type == Token::MINUS){
            op = MINUS_OP;
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
            op = MUL_OP;
        }
        else if (previous->type == Token::DIV){
            op = DIV_OP;
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
    else if (match(Token::ID)) {
        return new IdentifierExp(previous->text);
    }
    else if (match(Token::PI)){
        e = parseExpression();
        if (!match(Token::PD)){
            cout << "Falta paréntesis derecho" << endl;
            exit(0);
        }
        return e;
    }
    cout << "Error: se esperaba un número o identificador." << endl;
    exit(0);
}




Scanner::Scanner(const char* s):input(s),first(0), current(0) { }

Token* Scanner::nextToken() {
    Token* token;
    // Ignorar espacios y saltos de línea
    while (current < input.length() && (input[current] == ' ' || input[current] == '\n')) current++;
    if (current >= input.length()) return new Token(Token::END);
    
    char c = input[current];
    first = current;
    
    // Detectar números
    if (isdigit(c)) {
        current++;
        while (current < input.length() && isdigit(input[current]))
            current++;
        token = new Token(Token::NUM, input, first, current - first);
    } 
    // Detectar identificadores o palabras clave
    else if (isalpha(c)) {
        current++;
        while (current < input.length() && isalnum(input[current]))
            current++;
        std::string word = input.substr(first, current - first);
        
        // Palabras clave: print, if, else, endif
        if (word == "print") {
            token = new Token(Token::PRINT, word, 0, word.length());
        } else if (word == "if") {
            token = new Token(Token::IF, word, 0, word.length());
        } else if (word == "else") {
            token = new Token(Token::ELSE, word, 0, word.length());
        } else if (word == "endif") {
            token = new Token(Token::ENDIF, word, 0, word.length());
        } else {
            token = new Token(Token::ID, word, 0, word.length());
        }
    }
    // Detectar operadores y puntuación
    else if (strchr("+-*/()=;<", c)) {
        switch (c) {
            case '+': token = new Token(Token::PLUS, c); break;
            case '-': token = new Token(Token::MINUS, c); break;
            case '*': token = new Token(Token::MUL, c); break;
            case '/': token = new Token(Token::DIV, c); break;
            case '(': token = new Token(Token::PI, c); break;
            case ')': token = new Token(Token::PD, c); break;
            case '=': token = new Token(Token::ASSIGN, c); break;
            case ';': token = new Token(Token::PC, c); break;
            case '<': 
                current++;
                if (input[current] == '=') {
                    token = new Token(Token::LEQ, "<=", 0, 2);
                } else {
                    token = new Token(Token::LT, "<", 0, 1);
                }
                break;
            default: 
                token = new Token(Token::ERR, c);
        }
        current++;
    }
    // Caso de error (carácter no reconocido)
    else {
        token = new Token(Token::ERR, c);
        current++;
    }
    return token;
}


void Scanner::reset() {
    first = 0;
    current = 0;
}

Scanner::~Scanner() { }

void test_scanner(Scanner* scanner) {
    Token* current;
    cout << "Iniciando Scanner:" << endl<< endl;
    while ((current = scanner->nextToken())->type != Token::END) {
        if (current->type == Token::ERR) {
            cout << "Error en scanner - carácter inválido: " << current->text << endl;
            break;
        } else {
            cout << *current << endl;
        }
        delete current;
    }
    cout << "TOKEN(END)" << endl;
    delete current;
}


Token::Token(Type type):type(type) { text = ""; }

Token::Token(Type type, char c):type(type) { text = string(1, c); }

Token::Token(Type type, const string& source, int first, int last):type(type) {
    text = source.substr(first, last);
}

std::ostream& operator << ( std::ostream& outs, const Token & tok )
{
    switch (tok.type) {
        case Token::PLUS: outs << "TOKEN(PLUS)"; break;
        case Token::MINUS: outs << "TOKEN(MINUS)"; break;
        case Token::MUL: outs << "TOKEN(MUL)"; break;
        case Token::DIV: outs << "TOKEN(DIV)"; break;
        case Token::NUM: outs << "TOKEN(NUM)"; break;
        case Token::ERR: outs << "TOKEN(ERR)"; break;
        case Token::PD: outs << "TOKEN(PD)"; break;
        case Token::PI: outs << "TOKEN(PI)"; break;
        case Token::END: outs << "TOKEN(END)"; break;
        case Token::ID: outs << "TOKEN(ID)"; break;
        case Token::PRINT: outs << "TOKEN(PRINT)"; break;
        case Token::ASSIGN: outs << "TOKEN(ASSIGN)"; break;
        case Token::PC: outs << "TOKEN(PC)"; break;
        default: outs << "TOKEN(UNKNOWN)"; break;
    }
    return outs;
}

std::ostream& operator << ( std::ostream& outs, const Token* tok ) {
    return outs << *tok;
}

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




int main(int argc, const char* argv[]) {
    if (argc != 2) {
        cout << "Numero incorrecto de argumentos. Uso: " << argv[0] << " <archivo_de_entrada>" << endl;
        exit(1);
    }

    ifstream infile(argv[1]);
    if (!infile.is_open()) {
        cout << "No se pudo abrir el archivo: " << argv[1] << endl;
        exit(1);
    }

    string input;
    string line;
    while (getline(infile, line)) {
        input += line + '\n';
    }
    infile.close();

    Scanner scanner(input.c_str());

    string input_copy = input;
    Scanner scanner_test(input_copy.c_str());
    test_scanner(&scanner_test);
    cout << "Scanner exitoso" << endl;
    cout << endl;
    cout << "Iniciando parsing:" << endl;
    Parser parser(&scanner); 
    try {
        Program* program = parser.parseProgram();
        cout << "Parsing exitoso" << endl << endl;
        cout << "Iniciando Visitor:" << endl;
        PrintVisitor printVisitor;
        EVALVisitor evalVisitor;
        cout << "IMPRIMIR:" << endl;
        printVisitor.imprimir(program);
        cout  << endl;
        cout << "EJECUTAR:" << endl;
        evalVisitor.ejecutar(program);
        delete program;
    } catch (const exception& e) {
        cout << "Error durante la ejecución: " << e.what() << endl;
        return 1;
    }

    return 0;
}

