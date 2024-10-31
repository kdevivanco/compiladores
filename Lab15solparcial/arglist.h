// ArgList.h
#ifndef ARGLIST_H
#define ARGLIST_H

#include <list>
#include "exp.h" // Incluye la clase Exp como tipo base

//class Exp;;
//class ArgList {
//public:
//    std::list<Exp*> args;
//
//    // Constructor vacío
//    ArgList() = default;
//
//    // Método para añadir un argumento
//    void addArgument(Exp* exp) {
//        args.push_back(exp);
//    }
//
//    // Método para aceptar el visitor
//    int accept(Visitor* visitor) {
//        for (Exp* arg : args) {
//            arg->accept(visitor); // Llama al visitor en cada argumento
//        }
//        return 0;
//    }
//
//    // Destructor para liberar memoria
//    ~ArgList() {
//        for (Exp* exp : args) {
//            delete exp;
//        }
//    }
//};

#endif

