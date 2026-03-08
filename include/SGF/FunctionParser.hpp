
/********** SIMPLE GUI FRAMEWORK FUNCTION PARSER MODULE **********/

#ifndef _FUNCTIONPARSER_HPP
#define _FUNCTIONPARSER_HPP

#include <cmath>
#include <cstdint>
#include <unordered_map>
#include <stack>
#include <string>
#include <vector>

namespace sgf
{
    
enum class  FPError
{
    NONE                            = 0,
    ILLEGAL_DECLARATION_CHARACTER   = 1,
    INVALID_DECLARATION_SYNTAX      = 2,
    UNDEFINED_EXPRESSION_SYMBOL     = 3,
    INVALID_EXPRESSION_SYNTAX       = 4,
    ILLEGAL_EXPRESSION_CHARACTER    = 5
};

/* Utility class providing one with ability to work with functions that have
 * their declaration, in form of a wide string, given. Both function name and
 * argument name can be any combination of ascii letters, including capitalized ones.
 * Parser may support symbols different than the argument name, that are contained
 * in a functions expression part (after equal sign) - it is not hard to implement. */
class FunctionParser
{
private:
    struct ExpressionChunk
    {
        uint16_t    index;  // Position in definition string at which the chunk starts
        uint32_t    length; // Indicates a chunk length to right from position `index`
        float       number; // If chunk is of number type, in the end this gets its numeric value
        uint8_t     type;   // Type of the chunk, adheres to ECT_* class constants
    };

    static const wchar_t    CH_POINT;
    static const wchar_t    CH_EQUALS;
    static const wchar_t    CH_OPENING_PARENTHESIS;
    static const wchar_t    CH_CLOSING_PARENTHESIS;
    static const uint8_t    ECT_OPERATOR;
    static const uint8_t    ECT_NUMBER;
    static const uint8_t    ECT_SYMBOL;
    static const wchar_t    OP_ADD;
    static const wchar_t    OP_DIV;
    static const wchar_t    OP_MUL;
    static const wchar_t    OP_POW;
    static const wchar_t    OP_SUB;
    
    // Used to efficiently deal with precedence in chunks postfixization process
    static const std::unordered_map<wchar_t, int> OP_SCORES;
    
    std::wstring                    argName;
    void                            chunkize();
    std::vector<ExpressionChunk>    defChunks;
    std::wstring                    defString;
    FPError                         error;
    std::wstring                    funName;
    static bool                     isLetter(wchar_t ch);
    void                            postfixize();

public:
    FunctionParser(std::wstring defString);
    FunctionParser(const FunctionParser& other);
    FunctionParser operator=(const FunctionParser& other);
    std::wstring    getArgumentName()           const;
    std::wstring    getDefinitionString()       const;
    FPError         getError()                  const;
    std::wstring    getFunctionName()           const;
    float           getValue(float argument)    const;
};

}
    
#endif	// _FUNCTIONPARSER_HPP
