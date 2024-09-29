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

    if (match(Token::ID)) {
        std::string lex = previous->text;

        if (!match(Token::ASSIGN)) {
            throw std::runtime_error("Error: se esperaba un '=' después del identificador.");
        }
        e = parseCExp();  // Usa parseCExp para permitir expresiones condicionales
        s = new AssignStatement(lex, e);
    } 
    else if (match(Token::PRINT)) {
        if (!match(Token::PI)) {
            throw std::runtime_error("Error: se esperaba un '(' después de 'print'.");
        }
        e = parseCExp();  // Usa parseCExp para permitir expresiones condicionales
        if (!match(Token::PD)) {
            throw std::runtime_error("Error: se esperaba un ')' después de la expresión.");
        }
        s = new PrintStatement(e);
    } 
    else if (match(Token::IF)) {
        Exp* condition = parseCExp();  // Condición ahora puede ser una CExp

        if (!match(Token::THEN)) {
            throw std::runtime_error("Error: se esperaba 'then' después de la condición.");
        }

        std::list<Stm*> thenList;
        thenList.push_back(parseStatement());
        while (!isAtEnd() && !match(Token::ENDIF) && !check(Token::ELSE)) {
            thenList.push_back(parseStatement());
        }

        std::list<Stm*> elseList;
        if (match(Token::ELSE)) {
            elseList.push_back(parseStatement());
            while (!isAtEnd() && !match(Token::ENDIF)) {
                elseList.push_back(parseStatement());
            }
        }

        if (!match(Token::ENDIF)) {
            throw std::runtime_error("Error: se esperaba 'endif' al final del bloque 'if'.");
        }

        s = new IfStatement(condition, thenList, elseList);
    } 
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



Exp* Parser::parseCExp() {
    Exp* left = parseExpression();  // Primero parsea una expresión normal

    // Verifica si hay un operador relacional
    if (match(Token::LT) || match(Token::LEQ) || match(Token::EQ)) {
        std::string op = previous->text;  // Guarda el operador relacional
        Exp* right = parseExpression();   // Parsea la expresión de la derecha
        return new CExp(left, op, right);  // Devuelve un CExp
    }

    return left;  // Si no hay operador relacional, simplemente devuelve la expresión
}