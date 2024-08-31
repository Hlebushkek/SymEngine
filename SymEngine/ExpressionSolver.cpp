//
//  ExpressionSolver.cpp
//  SymEngine
//
//  Created by Hlib Sobolevskyi on 2024-08-12.
//

#include <iostream>
#include <symengine/solve.h>
#include <symengine/expression.h>
#include <symengine/parser.h>
#include <symengine/printers.h>
#include "ExpressionSolver.hpp"

using SymEngine::Expression;
using SymEngine::FiniteSet;
using SymEngine::ConditionSet;
using SymEngine::Interval;
using SymEngine::RCP;
using SymEngine::Basic;
using SymEngine::symbol;
using SymEngine::integer;

void print_set(RCP<const SymEngine::Set> set) {
    if (is_a<FiniteSet>(*set)) {
        const FiniteSet &fset = down_cast<const FiniteSet &>(*set);
        
        for (const auto &s : fset.get_container())
            std::cout << *s << "\n";
    } else if (is_a<Interval>(*set)) {
        auto interval_set = rcp_dynamic_cast<const Interval>(set);
        std::cout << interval_set->__str__() << "\n";
    } else if (is_a<ConditionSet>(*set)) {
        auto condition_set = rcp_dynamic_cast<const ConditionSet>(set);
        std::cout << condition_set->__str__() << std::endl;
    } else {
        std::cout << "Unsupported type" << "\n";
    }
}

std::vector<std::string> extract_result(RCP<const SymEngine::Set> set) {
    std::vector<std::string> result;
    
    if (is_a<FiniteSet>(*set)) {
        const FiniteSet &fset = down_cast<const FiniteSet &>(*set);
        
        for (const auto &s : fset.get_container())
            result.push_back(s->__str__());
    } else if (is_a<Interval>(*set)) {
        auto interval_set = rcp_dynamic_cast<const Interval>(set);
        result.push_back(interval_set->__str__());
    } else if (is_a<ConditionSet>(*set)) {
        auto condition_set = rcp_dynamic_cast<const ConditionSet>(set);
        result.push_back(condition_set->__str__());
    } else {
        std::cout << "Unsupported type" << "\n";
    }
    
    return result;
}

// Simplify (exp)^power to exp, if power is integer
RCP<const Basic> simplify_power_equation(const RCP<const Basic> &equation) {
    if (is_a<SymEngine::Pow>(*equation)) {
        const SymEngine::Pow &power_expr = static_cast<const SymEngine::Pow &>(*equation);
        
        RCP<const Basic> base = power_expr.get_base();
        RCP<const Basic> exponent = power_expr.get_exp();

        if (SymEngine::is_a<SymEngine::Integer>(*exponent)) {
            return base;
        }
    }
    
    return equation;
}

RCP<const SymEngine::Set> solve_simplify(const RCP<const Basic> &ex, const RCP<const SymEngine::Symbol> &symbol) {
    return SymEngine::solve(simplify_power_equation(ex), symbol);
}

ExpressionSolver::ExpressionSolver() {
    auto x = symbol("x");
    Expression x_ex(x);
    
    auto simple_ex1 = SymEngine::parse("x**2 - 4");
    auto simple_sol1 = SymEngine::solve(simple_ex1, x);
    print_set(simple_sol1);
    
    auto simple_ex2 = SymEngine::parse("x^2 - 4");
    auto simple_sol2 = SymEngine::solve(simple_ex2, x);
    print_set(simple_sol2);
    
    auto complex_ex = pow(x_ex + sqrt(Expression(2)), 5);
    auto expanded_complex_ex = expand(complex_ex);

    std::cout << complex_ex << "\n";
    std::cout << expanded_complex_ex << "\n";
    auto complex_solutions = solve_simplify(complex_ex, x);
    print_set(complex_solutions);
    
    auto equation = pow(x_ex, 2) - x_ex * 5 + 6;
    auto solutions = SymEngine::solve(equation, x);
    std::cout << equation << std::endl;
    print_set(solutions);
    
    // Note: need to solve corener cases
    auto equation_leq = Le(pow(x_ex, 2), SymEngine::real_double(4));
    auto solutions_leq = SymEngine::solve(equation_leq, x);
    std::cout << equation_leq->__str__() << std::endl;
    print_set(solutions_leq);
    
    auto equation_basic = SymEngine::parse("(4^2 - 4)/(4)");
    std::cout << equation_basic->__str__() << std::endl;
}

std::string ExpressionSolver::evaluate(const std::string &exp_str) const {
    std::cout << "Evaluating: " << exp_str <<  "\n";
    auto ex = SymEngine::parse(exp_str);
    
    return ex->__str__();
}

std::vector<std::string> ExpressionSolver::solve(const std::string &exp_str, const std::string &sym_str) const {
    std::cout << "Solving: " << exp_str << ", symbol: " << sym_str <<  "\n";
    auto ex = SymEngine::parse(exp_str);
    std::cout << "Parsed as: " << ex->__str__() << "\n";
    
    auto sym = symbol(sym_str);
    auto solution = SymEngine::solve(ex, sym);
    print_set(solution);
    
    return extract_result(solution);
}
