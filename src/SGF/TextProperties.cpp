
#include "SGF/TextProperties.hpp"

using namespace sgf;

TextProperties::TextProperties() :
                TextProperties({ 0, 0, 0 }, L"", 0)
{
    
}

TextProperties::TextProperties(Color3D color, std::wstring content, int size)
{
    this->color   = color;
    this->content = content;
    this->size    = size;
}

TextProperties::TextProperties(const TextProperties& other) :
                TextProperties(other.color, other.content, other.size)
{
    
}

TextProperties TextProperties::operator=(const TextProperties& other)
{
    this->color   = other.color;
    this->content = other.content;
    this->size    = other.size;
    
    // Trigger refresh to neatly update the copied structure with realtime data
    this->refreshFlag = true;
    
    return *this;
}
