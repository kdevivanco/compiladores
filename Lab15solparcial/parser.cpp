#include <iostream>
#include "token.h"
#include "scanner.h"
#include "exp.h"
#include "parser.h"

using namespace std;

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

Body* Parser::parseBody() {
    VarDecList* vdl = parseVarDecList();
    StatementList* sl = parseStatementList();
    return new Body(vdl, sl);
}


VarDecList* Parser::parseVarDecList() {
    VarDecList* vdl = new VarDecList();
    VarDec* aux;
    aux = parseVarDec();
    while (aux != nullptr) {
        vdl->add(aux);
        aux = parseVarDec();
    }
    return vdl;
}

FuncDecList* Parser::parseFuncDecList() {
    FuncDecList* fdl = new FuncDecList();
    FuncDec* aux;
    aux = parseFuncDec();
    while (aux != NULL) {
        fdl->add(aux);
        aux = parseFuncDec();
    }
    return fdl;
}

FuncDec* Parser::parseFuncDec() {
    FuncDec* fd = nullptr;
    if (match(Token::FUN)) {
        if (!match(Token::TYPE)) {
            cout << "Error: se esperaba un identificador después de 'fun'." << endl;
            exit(1);
        }
        string type = previous->text;
        if (!match(Token::ID)) {
            cout << "Error: se esperaba un identificador después de 'fun'." << endl;
            exit(1);
        }
        string name = previous->text;
        if (!match(Token::PI)) {
            cout << "Error: se esperaba un '(' después del nombre de la función." << endl;
            exit(1);
        }
        ParamDecList *params = parseParamDecList();
        if (!match(Token::PD)) {
            cout << "Error: se esperaba un ')' al final de la declaración de la función." << endl;
            exit(1);
        }
        Body* b = parseBody();
        if (!match(Token::ENDFUN)) {
            cout << "Error: se esperaba 'endfun' al final de la declaración." << endl;
            exit(1);
        }
        fd = new FuncDec(type, params,name,b);
    }
    return fd;
}

ParamDecList* Parser::parseParamDecList() {
    ParamDecList* pdl = new ParamDecList();
    if (match(Token::TYPE)) {
        string type = previous->text;
        if (!match(Token::ID)) {
            cout << "Error: se esperaba un identificador después del tipo." << endl;
            exit(1);
        }
        string id = previous->text;
        pdl->add(type, id);
        while (match(Token::COMA)) {
            if (!match(Token::TYPE)) {
                cout << "Error: se esperaba un tipo después de ','." << endl;
                exit(1);
            }
            type = previous->text;
            if (!match(Token::ID)) {
                cout << "Error: se esperaba un identificador después del tipo." << endl;
                exit(1);
            }
            id = previous->text;
            pdl->add(type, id);
        }
    }
    return pdl;
}

ReturnStatement* Parser::parseReturnStatement() {
    ReturnStatement* rs = new ReturnStatement();

    if (!match(Token::PI))
    {
        cout << "Error: se esperaba '(' después de 'return'." << endl;
        exit(1);
    }
    if (!match(Token::PD))
    {
        Exp *exp = parseCExp();
        if (exp) {
            rs->exp = exp;
        }

        if (!match(Token::PD))
        {
            cout << "Error: se esperaba ')' después de la expresión." << endl;
            exit(1);
        }
    }

    return rs;
}

VarDec* Parser::parseVarDec() {
    VarDec* vd = nullptr;
    if (match(Token::VAR)) {
        if (!match(Token::TYPE)) {
            cout << "Error: se esperaba un identificador después de 'TYPE'." << endl;
            exit(1);
        }
        string type = previous->text;
        list<string> ids;
        if (!match(Token::ID)) {
            cout << "Error: se esperaba un identificador después de 'var'." << endl;
            exit(1);
        }
        ids.push_back(previous->text);
        while (match(Token::COMA)) {
            if (!match(Token::ID)) {
                cout << "Error: se esperaba un identificador después de ','." << endl;
                exit(1);
            }
            ids.push_back(previous->text);
        }
        if (!match(Token::PC)) {
            cout << "Error: se esperaba un ';' al final de la declaración." << endl;
            exit(1);
        }
        vd = new VarDec(type, ids);
    }
    return vd;
}



StatementList* Parser::parseStatementList() {
    StatementList* sl = new StatementList();
    sl->add(parseStatement());
    while (match(Token::PC)) {
        sl->add(parseStatement());
    }
    return sl;
}





Program* Parser::parseProgram() {
    VarDecList* vdl = parseVarDecList();
    FuncDecList* fdl = parseFuncDecList();
    return new Program(vdl, fdl);
}

list<Stm*> Parser::parseStmList() {
    list<Stm*> slist;
    slist.push_back(parseStatement());
    while(match(Token::PC)) {
        slist.push_back(parseStatement());
    }
    return slist;
}

Stm* Parser::parseStatement() {
    Stm* s = NULL;
    Exp* e = NULL;
    Body* tb = NULL; //true case
    Body* fb = NULL; //false case


    if (current == NULL) {
        cout << "Error: Token actual es NULL" << endl;
        exit(1);
    }

    if (match(Token::ID)) {
        string lex = previous->text;

        if (!match(Token::ASSIGN)) {
            cout << "Error: se esperaba un '=' después del identificador." << endl;
            exit(1);
        }
        e = parseCExp();
        s = new AssignStatement(lex, e);
    } else if (match(Token::PRINT)) {
        if (!match(Token::PI)) {
            cout << "Error: se esperaba un '(' después de 'print'." << endl;
            exit(1);
        }
        e = parseCExp();
        if (!match(Token::PD)) {
            cout << "Error: se esperaba un ')' después de la expresión." << endl;
            exit(1);
        }
        s = new PrintStatement(e);
    }
    else if (match(Token::IF)) {
        e = parseCExp();
        if (!match(Token::THEN)) {
            cout << "Error: se esperaba 'then' después de la expresión." << endl;
            exit(1);
        }
        
        tb = parseBody();

        if (match(Token::ELSE)) {
            fb = parseBody();
        }
        if (!match(Token::ENDIF)) {
            cout << "Error: se esperaba 'end' al final de la declaración." << endl;
            exit(1);
        }
        s = new IfStatement(e, tb, fb);

    }
    else if (match(Token::WHILE)) {
        e = parseCExp();
        if (!match(Token::DO)) {
            cout << "Error: se esperaba 'do' después de la expresión." << endl;
            exit(1);
        }
        tb = parseBody();
        if (!match(Token::ENDWHILE)) {
            cout << "Error: se esperaba 'endwhile' al final de la declaración." << endl;
            exit(1);
        }
        s = new WhileStatement(e, tb);

    }
    else if(match(Token::FOR)){
        if(!match(Token::PI)){
            cout << "Error: se esperaba '(' después de 'for'." << endl;
            exit(1);
        }
        Exp* start = parseCExp();
        if (!match(Token::COMA)) {
            cout << "Error: se esperaba ',' después de la expresión." << endl;
            exit(1);
        }
        Exp* end = parseCExp();
        if (!match(Token::COMA)) {
            cout << "Error: se esperaba ',' después de la expresión." << endl;
            exit(1);
        }
        Exp* step = parseCExp();
        if (!match(Token::PD)) {
            cout << "Error: se esperaba ')' después de la expresión." << endl;
            exit(1);
        }
        tb = parseBody();
        if (!match(Token::ENDFOR)) {
            cout << "Error: se esperaba 'endfor' al final de la declaración." << endl;
            exit(1);
        }
        s = new ForStatement(start, end, step, tb);
    }
    else if (match(Token::RETURN)) {
        s = parseReturnStatement();
    }
    else {
        cout << "Error: se esperaba un identificador o 'print', pero se encontró: " << *current << endl;
        exit(1);
    }
    return s;
}

Exp* Parser::parseCExp(){
    Exp* left = parseExpression();
    if (match(Token::LT) || match(Token::LE) || match(Token::EQ)){
        BinaryOp op;
        if (previous->type == Token::LT){
            op = LT_OP;
        }
        else if (previous->type == Token::LE){
            op = LE_OP;
        }
        else if (previous->type == Token::EQ){
            op = EQ_OP;
        }
        Exp* right = parseExpression();
        left = new BinaryExp(left, right, op);
    }
    return left;
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
    Exp* e1;
    Exp* e2;

    if (match(Token::TRUE)) {
        return new BoolExp(1);
    } else if (match(Token::FALSE)) {
        return new BoolExp(0);
    } else if (match(Token::NUM)) {
        return new NumberExp(stoi(previous->text));
    } else if (match(Token::ID)) {
        string nombre = previous->text;

        if (match(Token::PI))
        {
            // Llamada a función detectada
            Exp* e1;
            FcallExp* fcall = new FcallExp();
            if(!match(Token::PD)){
                e1 = parseCExp();

                fcall->nombre = nombre;
                fcall->entradas.push_back(e1);

                while (match(Token::COMA)) { // Continúa con más argumentos si los hay
                    e1 = parseCExp();
                    fcall->entradas.push_back(e1);
                }
                match(Token::PD);
            }

            return fcall;
        }
        else
        {
            return new IdentifierExp(nombre); // Variable, no función
        }
    } else if (match(Token::IFEXP)) {
        match(Token::PI);
        e = parseCExp();
        match(Token::COMA);
        e1 = parseCExp();
        match(Token::COMA);
        e2 = parseCExp();
        match(Token::PD);
        return new IFExp(e, e1, e2);
    } else if (match(Token::PI)) {
        e = parseCExp();
        if (!match(Token::PD)) {
            cout << "Falta paréntesis derecho" << endl;
            exit(0);
        }
        return e;
    }

    cout << "Error: se esperaba un número o identificador." << endl;
    exit(0);
}



// Nuevos metodos para manejar ArgList:

ArgList* Parser::parseArgList() {
    ArgList* argList = new ArgList();

    // Añadir el primer argumento
    argList->addArgument(parseCExp());

    // Continuar añadiendo argumentos mientras haya comas
    while (match(Token::COMA)) {
        argList->addArgument(parseCExp());
    }

    return argList;
}


