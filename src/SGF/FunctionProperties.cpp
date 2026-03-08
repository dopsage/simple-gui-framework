
#include "SGF/FunctionProperties.hpp"

using namespace sgf;

FunctionProperties::FunctionProperties() :
                    FunctionProperties(L"")
{
    
}

FunctionProperties::FunctionProperties(std::wstring definition)
{
    this->definition = definition;
}

FunctionProperties::FunctionProperties(const FunctionProperties& other) :
                    FunctionProperties(other.definition)
{
    
}

FunctionProperties FunctionProperties::operator=(const FunctionProperties& other)
{
    this->definition  = other.definition;
    
    /* The properties must be updates immediatelly after copying. This is clearer
     * way than just mindlessly copying cached data. */
    this->refreshFlag = true;
    
    return *this;
}
