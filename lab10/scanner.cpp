#include <iostream>
#include <cstring>
#include "token.h"
#include "scanner.h"

using namespace std;

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