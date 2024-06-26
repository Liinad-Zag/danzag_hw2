#include <iostream>
#include <vector>
#include <memory>
#include <array>
#include <variant>
#include <unordered_map>

// (1 + 2) * 3 / 4 + 5 * (6 - 7)

// 1. Токенизация
// 2. Парсер (построение дерева разбора выражения)

// +, -, *, /, %

// 1. Дополнить решение токенами: max, sqr
struct OpeningBracket {};

struct ClosingBracket {};

struct Number {
    int value;
};

struct UnknownToken {
    std::string value;
};

struct MinToken {};

struct AbsToken {};

// Доп. токен max. В нем аргументы будут разделяться пробелом, поэтому дополнительно не храним ничего
struct MaxToken {};
// Доп. токен sqrt
struct SqrToken {};

struct Plus {};

struct Minus {};

struct Multiply {};

struct Modulo {};

struct Divide {};

// 2. Добавить все необходимые классы в определение Token
using Token = std::variant<Number, Plus, Minus, Multiply, Divide, Modulo, OpeningBracket, ClosingBracket,
                           AbsToken, MinToken, MaxToken, SqrToken, UnknownToken>;

// 1234

const std::unordered_map<char, Token> kSymbol2Token{
    { '+', Plus{} }, { '-', Minus{} }, { '*' , Multiply{} }, { '/', Divide{} }, { '%', Modulo{} }, { '(', OpeningBracket{} },
    { ')', ClosingBracket{} }};
// Хешсет для тектовых функций
const std::unordered_map<std::string, Token> kString2Token{ {"abs",  AbsToken{} }, { "min",  MinToken{} },
    { "max", MaxToken{} }, { "sqr", SqrToken{} }};

int ToDigit(unsigned char symbol) {
    return symbol - '0';
}

Number ParseNumber(const std::string& input, size_t& pos) {
    int value = 0;
    auto symbol = static_cast<unsigned char>(input[pos]);
    while (std::isdigit(symbol)) {
        value = value * 10 + ToDigit(symbol);
        if (pos == input.size() - 1) {
            ++pos;
            break;
        }
        symbol = static_cast<unsigned char>(input[++pos]);
    }
    return Number{ value };
}
// 3. Реализовать ParseName, который читает последовательность букв и возвращает токен соответствующей функции или UnknownToken
Token ParseName(const std::string& input, size_t& pos) {
    std::string token = "";
    auto symbol = static_cast<unsigned char>(input[pos]);
    while (std::isalpha(symbol)) {
        token += symbol;
        if (pos == input.size() - 1) {
            ++pos;
            break;
        }
        symbol = static_cast<unsigned char>(input[++pos]);
    }
    if (auto it = kString2Token.find(token); it != kString2Token.end())
    {
        return it->second;
    }
    else
        return UnknownToken{ token };
}
//4. Закончить Tokenize
std::vector<Token> Tokenize(const std::string& input) {
    std::vector<Token> tokens;
    const size_t size = input.size();
    size_t pos = 0;
    std::string value;
    while (pos < size) {
        const auto symbol = static_cast<unsigned char>(input[pos]);
        if (std::isspace(symbol)) {
            ++pos;
        }
        else if (std::isdigit(symbol)) {
            tokens.emplace_back(ParseNumber(input, pos));
        }
        else if (auto it = kSymbol2Token.find(symbol); it != kSymbol2Token.end()) {
            tokens.emplace_back(it->second);
            ++pos;
        }
        // Проверка на то, что это английская буква 
        else if (std::isalpha(symbol)){
            tokens.emplace_back(ParseName(input, pos));
        }
        else {
            tokens.emplace_back(UnknownToken{ value=symbol });
            pos++;
        }
    }
    return tokens;
}
int main()
{
    std::string input = "min(123) + abS(2) - Max66 альфа";
    std::vector<Token> tokens = Tokenize(input);
    return 0;
}
