#include <algorithm>
#include <cassert>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <functional>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <vector>

using namespace std;
using Symbol = char;
using Word = std::vector<Symbol>;

struct Grammar {
    std::set<Symbol> m_Nonterminals;
    std::set<Symbol> m_Terminals;
    std::vector<std::pair<Symbol, std::vector<Symbol>>> m_Rules;
    Symbol m_InitialSymbol;
};

map<pair<size_t, size_t>, map<Symbol, vector<size_t>>> firstIteration(const Word &word, const Grammar &grammar){
    map<pair<size_t, size_t>, map<Symbol, vector<size_t>>> table;
    for(size_t i = 0; i < word.size(); i++){
        for(size_t j = 0; j < grammar.m_Rules.size(); j++){
            if(grammar.m_Rules[j].second.size() == 1){
                Symbol literal = grammar.m_Rules[j].second[0];
                Symbol key = grammar.m_Rules[j].first;
                if(literal == word[i]){
                    table[make_pair(1, i + 1)][key].emplace_back(j);
                }
            }
        }
    }
    return table;
}

int invalidInput(const Grammar &grammar, size_t size){
    for(size_t i = 0; i < grammar.m_Rules.size(); i++){
        if( grammar.m_Rules[i].first == grammar.m_InitialSymbol && grammar.m_Rules[i].second.empty() && size == 0){
            return (int)i;
        }
    }
    return -1;
}

void fillTable(map<pair<size_t, size_t>, map<Symbol, vector<size_t>>> &table, const Word &word, const Grammar &grammar){
    size_t n = word.size();
    for(size_t i = 2; i <= n; i++){
        for(size_t j = 1; j <= n - i + 1; j++){
            for(size_t k = 1; k <= i - 1; k++){
                set<Symbol>tmp1;
                for(const auto &left: table[{k, j}]){
                    for(const auto &right: table[{i - k, j + k}]){
                        for(size_t p = 0; p < grammar.m_Rules.size(); p++){
                            if(grammar.m_Rules[p].second.size() == 2){
                                if(left.first == grammar.m_Rules[p].second[0] && right.first == grammar.m_Rules[p].second[1]) {
                                    vector<size_t> new_values = right.second;
                                    new_values.insert(new_values.end(), left.second.begin(), left.second.end());
                                    new_values.emplace_back(p);
                                    auto& target = table[{i, j}][grammar.m_Rules[p].first];
                                    target = new_values;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

std::vector<size_t> trace(const Grammar& grammar, const Word& word) {
    int isInvalid = invalidInput(grammar, word.size());
    size_t n = word.size();
    if(isInvalid != -1){
        return vector<size_t>{(size_t)isInvalid};
    }
    map<pair<size_t, size_t>, map<Symbol, vector<size_t>>>table = firstIteration(word, grammar);
    fillTable(table, word, grammar);
    if(table[{n, 1}].count(grammar.m_InitialSymbol) == 0){
        return vector<size_t>{};
    }
    vector<size_t>result = table[{n,1}][grammar.m_InitialSymbol];
    std::reverse(result.begin(), result.end());
    return result;
}

int main() {
    Grammar g0{
            {'A', 'B', 'C', 'S'},
            {'a', 'b'},
            {
                    {'S', {'A', 'B'}},
                    {'S', {'B', 'C'}},
                    {'A', {'B', 'A'}},
                    {'A', {'a'}},
                    {'B', {'C', 'C'}},
                    {'B', {'b'}},
                    {'C', {'A', 'B'}},
                    {'C', {'a'}},
            },
            'S'};

        assert(trace(g0, {'b', 'a', 'a', 'b', 'a'}) == std::vector<size_t>({0, 2, 5, 3, 4, 6, 3, 5, 7}));
        assert(trace(g0, {'b'}) == std::vector<size_t>({}));
        assert(trace(g0, {'a'}) == std::vector<size_t>({}));
        assert(trace(g0, {}) == std::vector<size_t>({}));
        assert(trace(g0, {'a', 'a', 'a', 'a', 'a'}) == std::vector<size_t>({1, 4, 6, 3, 4, 7, 7, 7, 7}));
        assert(trace(g0, {'a', 'b'}) == std::vector<size_t>({0, 3, 5}));
        assert(trace(g0, {'b', 'a'}) == std::vector<size_t>({1, 5, 7}));
        assert(trace(g0, {'c', 'a'}) == std::vector<size_t>({}));

    Grammar g1{
            {'A', 'B'},
            {'x', 'y'},
            {
                    {'A', {}},
                    {'A', {'x'}},
                    {'B', {'x'}},
                    {'A', {'B', 'B'}},
                    {'B', {'B', 'B'}},
            },
            'A'};

    assert(trace(g1, {}) == std::vector<size_t>({0}));
    assert(trace(g1, {'x'}) == std::vector<size_t>({1}));
    assert(trace(g1, {'x', 'x'}) == std::vector<size_t>({3, 2, 2}));
    assert(trace(g1, {'x', 'x', 'x'}) == std::vector<size_t>({3, 4, 2, 2, 2}));
    assert(trace(g1, {'x', 'x', 'x', 'x'}) == std::vector<size_t>({3, 4, 4, 2, 2, 2, 2}));
    assert(trace(g1, {'x', 'x', 'x', 'x', 'x'}) == std::vector<size_t>({3, 4, 4, 4, 2, 2, 2, 2, 2}));
    assert(trace(g1, {'x', 'x', 'x', 'x', 'x', 'x'}) == std::vector<size_t>({3, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2}));
    assert(trace(g1, {'x', 'x', 'x', 'x', 'x', 'x', 'x'}) == std::vector<size_t>({3, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2, 2}));
    assert(trace(g1, {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'}) == std::vector<size_t>({3, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2, 2, 2}));
    assert(trace(g1, {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'}) == std::vector<size_t>({3, 4, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2}));

    Grammar g2{
            {'A', 'B'},
            {'x', 'y'},
            {
                    {'A', {'x'}},
                    {'B', {'x'}},
                    {'A', {'B', 'B'}},
                    {'B', {'B', 'B'}},
            },
            'B'};

    assert(trace(g2, {}) == std::vector<size_t>({}));
    assert(trace(g2, {'x'}) == std::vector<size_t>({1}));
    assert(trace(g2, {'x', 'x'}) == std::vector<size_t>({3, 1, 1}));
    assert(trace(g2, {'x', 'x', 'x'}) == std::vector<size_t>({3, 3, 1, 1, 1}));

    Grammar g3{
            {'A', 'B', 'C', 'D', 'E', 'S'},
            {'a', 'b'},
            {
                    {'S', {'A', 'B'}},
                    {'S', {'S', 'S'}},
                    {'S', {'a'}},
                    {'A', {'B', 'S'}},
                    {'A', {'C', 'D'}},
                    {'A', {'b'}},
                    {'B', {'D', 'D'}},
                    {'B', {'b'}},
                    {'C', {'D', 'E'}},
                    {'C', {'b'}},
                    {'C', {'a'}},
                    {'D', {'a'}},
                    {'E', {'S', 'S'}},
            },
            'S'};

    assert(trace(g3, {}) == std::vector<size_t>({}));
    assert(trace(g3, {'b'}) == std::vector<size_t>({}));
    assert(trace(g3, {'a', 'b', 'a', 'a', 'b'}) == std::vector<size_t>({1, 2, 0, 3, 7, 1, 2, 2, 7}));
    assert(trace(g3, {'a', 'b', 'a', 'a', 'b', 'a', 'b', 'a', 'b', 'a', 'a'}) == std::vector<size_t>({1, 1, 0, 4, 8, 11, 12, 0, 5, 6, 11, 11, 0, 4, 9, 11, 7, 11, 7, 2, 2}));
}
