#pragma once

#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <vector>


class vertexarray : public sf::Drawable
{
public:

   
    vertexarray();

    explicit vertexarray(sf::PrimitiveType type, std::size_t vertexCount = 0);

    std::size_t getVertexCount() const;

    sf::Vertex& operator [](std::size_t index);

    const sf::Vertex& operator [](std::size_t index) const;

    void clear();

    void resize(std::size_t vertexCount);

    void append(const sf::Vertex& vertex);

    void setPrimitiveType(sf::PrimitiveType type);

    sf::PrimitiveType getPrimitiveType() const;

    sf::FloatRect getBounds() const;

    void erase(unsigned int start, unsigned int end);

    auto get_init_iterator() { return m_vertices.begin(); }

    auto get_end_iterator() { return m_vertices.end(); }

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:

    std::vector<sf::Vertex>     m_vertices;      ///< Vertices contained in the array
    sf::PrimitiveType       m_primitiveType; ///< Type of primitives to draw

};

