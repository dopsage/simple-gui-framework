
#include "SGF/FunctionParser.hpp"

using namespace sgf;

const wchar_t   FunctionParser::CH_POINT                = L'.';
const wchar_t   FunctionParser::CH_EQUALS               = L'=';
const wchar_t   FunctionParser::CH_OPENING_PARENTHESIS  = L'(';
const wchar_t   FunctionParser::CH_CLOSING_PARENTHESIS  = L')';
const uint8_t   FunctionParser::ECT_OPERATOR            = 1;
const uint8_t   FunctionParser::ECT_NUMBER              = 0;
const uint8_t   FunctionParser::ECT_SYMBOL              = 2;
const wchar_t   FunctionParser::OP_ADD                  = L'+';
const wchar_t   FunctionParser::OP_DIV                  = L'/';
const wchar_t   FunctionParser::OP_MUL                  = L'*';
const wchar_t   FunctionParser::OP_POW                  = L'^';
const wchar_t   FunctionParser::OP_SUB                  = L'-';

const std::unordered_map<wchar_t, int> FunctionParser::OP_SCORES = 
{
    { OP_ADD, 0 },
    { OP_SUB, 0 },
    { OP_MUL, 1 },
    { OP_DIV, 1 },
    { OP_POW, 2 }
};

bool FunctionParser::isLetter(wchar_t ch)
{
// NOTICE: Only ASCII letters are considered, but unicode ones may be implemented too
    return (ch > 0x40 && ch < 0x5b) || (ch > 0x60 && ch < 0x7b);
}

void FunctionParser::chunkize()
{
	static const int ECT_CPU = 100;	// Closing Parenthesis Unload trigger
		
    int         collectedNames	= 0;
    int 		prevChunkType   = ECT_OPERATOR;	// This setting triggers appropriates
    int         openParentheses = 0;
    uint32_t    chunkLength     = 0;
    uint16_t    chunkPos 		= 0;
    bool        wasEqualSign    = false;
    bool        wasPoint        = false;
    uint16_t    defLength       = defString.length();
    for(uint16_t currPos = 0; currPos <= defLength; currPos++)
    {
        /* Dollar symbol is used to let all checks run, and in the end lets
         * the parser unload itself of any chunk processing occurring. */
        wchar_t ch = (currPos == defLength ? L'$' : defString[currPos]);
        
        if(wasEqualSign)
        {
            /*** Process function expression ***/

            bool        isCharacterIllegal  = true;
            uint32_t    currChunkType;

            // [ 1 ] Check for a symbol
            if(isLetter(ch))
            {
                if(prevChunkType == ECT_NUMBER)
                {
                    // [ ! ] Letter just after a number
                    error = FPError::INVALID_EXPRESSION_SYNTAX;
                    break;
                }
                else if(prevChunkType == ECT_OPERATOR)
                {
					// Started reading some symbol ...
                    chunkPos    	= currPos;
                    chunkLength 	= 0;
                    currChunkType	= ECT_SYMBOL;
                }
                
                isCharacterIllegal = false;
            }
            else if(prevChunkType == ECT_SYMBOL)
            {
                /* Until now, some symbol was being collected. Now validate
                 * the collection and, in positive outcome, save it. */
                
                std::wstring symbol = defString.substr(chunkPos, chunkLength);
                if(symbol != argName)
                {
                    /* For now only argument is considered the right symbol, but in
                     * the future custom symbols like pi or e can be introduced effortlessly. */
                    error = FPError::UNDEFINED_EXPRESSION_SYMBOL;
                    break;
                }
                
                // Register the chunk of type number
                defChunks.push_back({ chunkPos, chunkLength, 0.0f, ECT_SYMBOL });
            }
            
            // [ 2 ] Check for a number
            bool isPoint = (ch == CH_POINT);
            if(isPoint || (ch > 0x2f && ch < 0x3a))
            {
                if(isPoint)
                {
                    if(wasPoint)
                    {
                        // [ ! ] Two points in a single number chunk
                        error = FPError::INVALID_EXPRESSION_SYNTAX;
                        break;
                    }
                    
                    wasPoint = true;
                }
				
                if(prevChunkType == ECT_SYMBOL)
                {
                    // [ ! ] Number just after a symbol
                    error = FPError::INVALID_EXPRESSION_SYNTAX;
                    break;
                }
                else if(prevChunkType == ECT_OPERATOR)
                {
                    // Started reading some number
                    chunkPos    	= currPos;
                    chunkLength 	= 0;
                    currChunkType  	= ECT_NUMBER;
                }
                
                isCharacterIllegal = false;
            }
            else if(prevChunkType == ECT_NUMBER)
            {
                if(wasPoint && chunkLength == 1)
                {
                    // [ ! ] Lone point
                    error = FPError::INVALID_EXPRESSION_SYNTAX;
                    break; 
                }
                
                // Until now, some numeric was being collected, try saving it.
                std::wstring number = defString.substr(chunkPos, chunkLength);
                defChunks.push_back({ chunkPos, chunkLength, std::stof(number), ECT_NUMBER });
                wasPoint            = false;
            }
            
            // [ 3 ] Check for an operator
            bool isOpeningPar  	= (ch == CH_OPENING_PARENTHESIS);
            bool isClosingPar 	= (ch == CH_CLOSING_PARENTHESIS);
            if( isOpeningPar   	||
                isClosingPar   	||
                ch == OP_ADD	||
                ch == OP_DIV   	||
                ch == OP_MUL   	||
                ch == OP_POW   	||
                ch == OP_SUB)
            {
                if(( isOpeningPar && prevChunkType == ECT_CPU) ||
					!isOpeningPar && !isClosingPar && prevChunkType == ECT_OPERATOR)
                {
					// [ ! ] Opening parenthesis just after closing the other
                    // [ ! ] Or operator just after operator
                    error = FPError::INVALID_EXPRESSION_SYNTAX;
                    break;
                }
                
                // It is immediatelly known that processed character is an operator
                defChunks.push_back({ currPos, 1, 0.0f, ECT_OPERATOR });
                
                /* After closing parenthessis, there must be some operator.
                 * Use special type in order to not trigger checks 1 & 2 (above). */
				currChunkType 		= isClosingPar ? ECT_CPU : ECT_OPERATOR;
                
                isCharacterIllegal  = false;
                openParentheses    += isOpeningPar ? +1 : (isClosingPar ? -1 : 0);
            }
            
            if(isCharacterIllegal && currPos != defLength)
            {
                // [ ! ] Illegal character, unless it is last (unloading) iteration
                error = FPError::ILLEGAL_EXPRESSION_CHARACTER;
                break;
            }
            else if(currPos == defLength && (defChunks.size() == 0 || openParentheses != 0 || currChunkType == ECT_OPERATOR))
			{
                // [ ! ] Empty expression
                // [ ! ] Or unloading is performed, but parentheses are impaired
                // [ ! ] Or there is operator with nothing on right
                error = FPError::INVALID_EXPRESSION_SYNTAX;
                break;
            }
            else
            {
				// Everything is fine, continue processing next expression character
                prevChunkType = currChunkType;
                chunkLength++;
            }
        }
        else
        {
            /*** Process function definition ***/
            
            if(!isLetter(ch))
            {
                if(ch == CH_OPENING_PARENTHESIS)
                {
                    if(collectedNames > 0 || chunkLength == 0)
                    {
                        /* [ ! ] Argument name is being (or is already) collected
                         * [ ! ] Length of a function name is missing */
                        error = FPError::INVALID_DECLARATION_SYNTAX;
                        break;
                    }
                    
                    /* Until now, the function name was being collected.
                     * Save it and setup chunk for argument name collection. */
                    collectedNames++;
                    funName         = defString.substr(chunkPos, chunkLength);
                    chunkLength     = 0;
                    chunkPos        	= currPos + 1;
                    continue;
                }
                else if(ch == CH_CLOSING_PARENTHESIS)
                {
					if(collectedNames < 1 || chunkLength == 0)
					{
						/* [ ! ] Did not collect function name (impossible at this point)
                         * [ ! ] Length of an argument name is missing */
                        error = FPError::INVALID_DECLARATION_SYNTAX;
                        break;
					}
						
					// This time, the argument name is already collected.
					collectedNames++;
					argName 		= defString.substr(chunkPos, chunkLength);
					continue;
                }
                else if(ch == CH_EQUALS)
                {
                    if(collectedNames < 2)
                    {
						// [ ! ] Did not collect both names (of function and of argument)
                        error = FPError::INVALID_DECLARATION_SYNTAX;
                        break;
					}
					
					// Prepare for expression processing and continue to it
					chunkLength     = 0;
					chunkPos        = currPos + 1;
					wasEqualSign    = true;
					continue;
                }
                else
                {
                    /* Characters different than letters, parenthesses and equals are
                     * not allowed in function declaration. */
                    error = FPError::ILLEGAL_DECLARATION_CHARACTER;
                    break;
                }
            }
            else if(collectedNames == 2)
            {
                // [ ! ] Letters after closing parenthesis (declaration end)
                error = FPError::INVALID_DECLARATION_SYNTAX;
                break;
            }
            
            chunkLength++;
        }
    }
}

//#include<iostream>
void FunctionParser::postfixize()
{
    // Rearange expression chunks in vector to postfix form, and convert numerics in
    // number typed ECs, so it is ready for getValue method to use for value calculation ...
    
    std::vector<ExpressionChunk> 	postfixedChunks;
    std::stack<ExpressionChunk> 	operatorStack;
    std::stack<int> 				operatorContexts;
    operatorContexts.push(0);
    
    for(int eci = 0; eci < defChunks.size(); eci++)
    {
        ExpressionChunk ec = defChunks[eci];
        
        if(ec.type == ECT_OPERATOR)
        {
            wchar_t currOperChar = defString[ec.index];
            
            if(currOperChar == CH_OPENING_PARENTHESIS)
            {
                // Open new operator context by creating new record of its length (initially 0)
                operatorContexts.push(0);
                
                continue;
            }
            else if(currOperChar == CH_CLOSING_PARENTHESIS)
            {
                // Unload the whole operator context opened earlier
                int ocLength = operatorContexts.top();
				while(ocLength--)
				{
					postfixedChunks.push_back(operatorStack.top());
					operatorStack.pop();
				}
				
				// Root context is not popped, but zeroed instead
				if(operatorContexts.size() == 1)	operatorContexts.top() = 0;
				else 								operatorContexts.pop();
				
				continue;
            }
            else while(!operatorStack.empty() && operatorContexts.top() > 0)
            {
                wchar_t topOperChar    = defString[operatorStack.top().index];
                int     currScore      = OP_SCORES.at(currOperChar);
                int     topScore       = OP_SCORES.at(topOperChar);
                
                if(currScore <= topScore)
                {
                    postfixedChunks.push_back(operatorStack.top());
                    operatorStack.pop();
                    operatorContexts.top()--;
                }
                else
                    break;
            }

			// If execution made here, it means the operator needs to be simply pushed
			operatorStack.push(ec);
			operatorContexts.top()++;			
        }
        else
        {
			// Symbols and numbers are simply pushed
            postfixedChunks.push_back(ec);
        }
    }
    
    // Unload remainders in the operator stack 
    while(!operatorStack.empty())
    {
        postfixedChunks.push_back(operatorStack.top());
        operatorStack.pop();
    }

    // Print postfix string
    //for(int i=0;i<postfixedChunks.size();i++)
    //    std::wcout<<defString.substr(postfixedChunks[i].index,postfixedChunks[i].length)<<L" ";
    //std::wcout << L"\n\n";
    
    defChunks = postfixedChunks;
}

FunctionParser::FunctionParser(std::wstring defString) :
                argName(L""),
                defChunks(),
                defString(defString),
                error(FPError::NONE),
                funName(L"")
{
    chunkize();
    postfixize();
}

FunctionParser::FunctionParser(const FunctionParser& other) :
                FunctionParser(other.getDefinitionString())
{
    
}

FunctionParser FunctionParser::operator=(const FunctionParser& other)
{
    defString = other.getDefinitionString();
    chunkize();
    postfixize();
    
    return *this;
}

std::wstring FunctionParser::getArgumentName() const
{
    return argName;
}

FPError FunctionParser::getError() const
{
    return error;
}

std::wstring FunctionParser::getDefinitionString() const
{
    return defString;
}

std::wstring FunctionParser::getFunctionName() const
{
    return funName;
}

float FunctionParser::getValue(float argument) const
{
    if(error != FPError::NONE)
		return 14.052025f;
    
    // Single chunk case
    if(defChunks.size() == 1)
        return defChunks[0].type == ECT_SYMBOL ? argument : defChunks[0].number;
    
    // We need to work with the leftmost valid chunk triplets
    std::vector<ExpressionChunk> defChunksCopy = defChunks;
    int tripletOffset = 0;
    while(defChunksCopy.size() != 1)
    {
        ExpressionChunk* triplet[3] =
        {
            &defChunksCopy[tripletOffset + 0],
            &defChunksCopy[tripletOffset + 1],
            &defChunksCopy[tripletOffset + 2]
        };
        
        if(triplet[0]->type != ECT_OPERATOR && triplet[1]->type != ECT_OPERATOR && triplet[2]->type == ECT_OPERATOR)
        {
			// Detected triplet: symbol/number | symbol/number | operator
            float leftNumber    = triplet[0]->type == ECT_SYMBOL ? argument : triplet[0]->number;
            float rightNumber   = triplet[1]->type == ECT_SYMBOL ? argument : triplet[1]->number;
            float answer        = 0.0f;
            
            // Match operation for the triplet
            switch(defString[triplet[2]->index])
            {
                case OP_ADD:
                    answer = leftNumber+rightNumber;
                    break;
                case OP_DIV:
                    answer = leftNumber/rightNumber;
                    break;
                case OP_MUL:
                    answer = leftNumber*rightNumber;
                    break;
                case OP_POW:
                    answer = std::pow(leftNumber, rightNumber);
                    break;
                case OP_SUB:
                    answer = leftNumber-rightNumber;
                    break;
            }
            
            /* Exchange calculation with answer, then start whole processing
             * again by resetting the triplet offset. */
            defChunksCopy.erase (defChunksCopy.begin() + tripletOffset, defChunksCopy.begin() + tripletOffset + 3);
            defChunksCopy.insert(defChunksCopy.begin() + tripletOffset, { 0, 0, answer, ECT_NUMBER });
            tripletOffset = 0;
        }
        else
        {
			tripletOffset++;
		}
    }
    
    // The last chunk remaining is the function output
    return defChunksCopy[0].number;
}
