//
//  ExpressionSolver.cpp
//  SymEngine
//
//  Created by Hlib Sobolevskyi on 2024-08-12.
//

#include <iostream>
#include <symengine/expression.h>
#include <symengine/printers.h>
#include "ExpressionSolver.hpp"

using SymEngine::Expression;

ExpressionSolver::ExpressionSolver() {
    Expression x("x");
    
    auto ex = pow(x+sqrt(Expression(2)), 6);
    auto expanded_ex = expand(ex);

    std::cout << ex << std::endl;
    std::cout << expanded_ex << std::endl;
}
