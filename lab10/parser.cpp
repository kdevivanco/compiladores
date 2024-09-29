#include <iostream>
#include <stdexcept>
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
        Exp* condition = parseCExp(); // parseo a condicional 

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

