//
//  ExpressionSolver.hpp
//  SymEngine
//
//  Created by Hlib Sobolevskyi on 2024-08-12.
//

#pragma once

#include <vector>
#include <string>

class ExpressionSolver {
public:
    ExpressionSolver();
    
    std::string evaluate(const std::string &exp_str) const;
    std::vector<std::string> solve(const std::string &exp_str, const std::string &sym_str) const;
};
