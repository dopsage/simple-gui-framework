
/********** SIMPLE GUI FRAMEWORK FUNCTION PROPERTIES MODULE **********/

#ifndef _FUNCTIONPROPERTIES_HPP
#define _FUNCTIONPROPERTIES_HPP

#include <map>
#include <string>

#include "SGF/Types.hpp"

namespace sgf
{

struct FunctionProperties
{
    // Constant
    std::wstring definition;    // Literal mathematical definition of the function
    
    // Dynamic (they may be externally changed)
    std::map<float, float>  mappingCache;   // Mapping of arguments to values computed using definition during the last refresh
    bool                    refreshFlag;    // If set, grapher updates the dynamic properties and resets the flag
    
    FunctionProperties();
    FunctionProperties(std::wstring definition);
    FunctionProperties(const FunctionProperties& other);
    FunctionProperties operator=(const FunctionProperties& other);
};

}
    
#endif	// _FUNCTIONPROPERTIES_HPP
