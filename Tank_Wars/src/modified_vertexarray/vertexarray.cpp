#include "vertexarray.h"


vertexarray::vertexarray() :
    m_vertices(),
    m_primitiveType(sf::Points)
{
}



vertexarray::vertexarray(sf::PrimitiveType type, std::size_t vertexCount) :
    m_vertices(vertexCount),
    m_primitiveType(type)
{
}



std::size_t vertexarray::getVertexCount() const
{
    return m_vertices.size();
}



sf::Vertex& vertexarray::operator [](std::size_t index)
{
    return m_vertices[index];
}



const sf::Vertex& vertexarray::operator [](std::size_t index) const
{
    return m_vertices[index];
}



void vertexarray::clear()
{
    m_vertices.clear();
}



void vertexarray::resize(std::size_t vertexCount)
{
    m_vertices.resize(vertexCount);
}


void vertexarray::append(const sf::Vertex& vertex)
{
    m_vertices.push_back(vertex);
}


void vertexarray::setPrimitiveType(sf::PrimitiveType type)
{
    m_primitiveType = type;
}



sf::PrimitiveType vertexarray::getPrimitiveType() const
{
    return m_primitiveType;
}



sf::FloatRect vertexarray::getBounds() const
{
    if (!m_vertices.empty())
    {
        float left = m_vertices[0].position.x;
        float top = m_vertices[0].position.y;
        float right = m_vertices[0].position.x;
        float bottom = m_vertices[0].position.y;

        for (std::size_t i = 1; i < m_vertices.size(); ++i)
        {
            sf::Vector2f position = m_vertices[i].position;

            // Update left and right
            if (position.x < left)
                left = position.x;
            else if (position.x > right)
                right = position.x;

            // Update top and bottom
            if (position.y < top)
                top = position.y;
            else if (position.y > bottom)
                bottom = position.y;
        }

        return sf::FloatRect(left, top, right - left, bottom - top);
    }
    else
    {
        // Array is empty
        return sf::FloatRect();
    }
}

void vertexarray::erase(unsigned int start, unsigned int end)
{
    m_vertices.erase(m_vertices.begin() + start, m_vertices.begin() + end);
}



void vertexarray::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (!m_vertices.empty())
        target.draw(&m_vertices[0], m_vertices.size(), m_primitiveType, states);
}