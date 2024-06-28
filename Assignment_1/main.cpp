#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>

#include "imgui.h"
#include "imgui-SFML.h"

#include "SFML/Graphics.hpp"

sf::Font& readFont()
{
    static sf::Font myFont;
    if (!myFont.loadFromFile("fonts/georgia.ttf"))
    {
        std::cerr << "couldn't load font\n";
        exit(-1);
    }
    return myFont;
}

class Shape
{
protected:
    bool drawShape = true;
    bool drawText = true;
    float velocity[2] = { 2.f, 2.f };
    std::string name;
    sf::Text textName;
    char displayText[255] = "name";
    float m_color[3] = { 0.f, 0.f, 0.f };
    float meow_scale = 1.f;
public:
    Shape(const std::string& shapeName) : name(shapeName)
    {

        textName.setString(name);
        textName.setFont(readFont());
        textName.setCharacterSize(20);
    }

    virtual ~Shape() = default;

    virtual bool getDrawShape() const 
    { 
        return drawShape; 
    }
    virtual void setDrawShape(bool const dataDrawShape) 
    { 
        drawShape = dataDrawShape; 
    }

    virtual bool getDrawText() const 
    { 
        return drawText; 
    }
    virtual void setDrawText(bool const dataDrawText) 
    { 
        drawText = dataDrawText; 
    }

    virtual float getSpeedX() const 
    { 
        return velocity[0]; 
    }
    virtual void setSpeedX(float const dataSpeedX) 
    { 
        velocity[0] = dataSpeedX; 
    }

    virtual float getSpeedY() const 
    { 
        return velocity[1]; 
    }
    virtual void setSpeedY(float const dataSpeedY) 
    { 
        velocity[1] = dataSpeedY; 
    }

    virtual void setFillColor(const sf::Color& color) = 0;
    virtual void setPosition(float x, float y) = 0;
    virtual sf::Vector2f getPosition() const = 0;
    virtual sf::FloatRect getGlobalBounds() const = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual void renderImGuiControls()
    {
        ImGui::Checkbox("Draw Shape", &drawShape);
        ImGui::Checkbox("Draw Text", &drawText);
        ImGui::SliderFloat("Scale", &meow_scale, 1.0f, 4.0f);
        ImGui::SliderFloat2("Velocity", velocity, -4.0f, 4.0f);
        ImGui::ColorEdit3("Color", m_color);
        ImGui::InputText("Text", displayText, 255);
        if (ImGui::Button("setText"))
        {
            textName.setString(displayText);
        }
    };
    virtual void updateShape() = 0;
    std::string getName() const { return name; }
};


class Circle : public sf::CircleShape, public Shape
{
public:
    Circle(float circleRadius, int circleSegments, const std::string& name)
        : sf::CircleShape(circleRadius, circleSegments), Shape(name)
    {
        sf::CircleShape::setPosition(10.f, 10.f);
        textName.setPosition(getPosition());
    }

    void setFillColor(const sf::Color& color) override
    {
        sf::CircleShape::setFillColor(color);
        m_color[0] = (color.r) / 255.f;
        m_color[1] = (color.g) / 255.f;
        m_color[2] = (color.b) / 255.f;
    }

    void setPosition(float x, float y) override
    {
        sf::CircleShape::setPosition(x, y);
        updateTextPosition();
    }

    sf::Vector2f getPosition() const override
    {
        return sf::CircleShape::getPosition();
    }

    sf::FloatRect getGlobalBounds() const override
    {
        return sf::CircleShape::getGlobalBounds();
    }

    void draw(sf::RenderWindow& window) override
    {
        if (drawShape)
        {
            window.draw(*this);
            if (drawText) 
            {
                window.draw(textName);
            }
        }
    }

    void updateShape() override
    {
        setRadius(m_radius * meow_scale);
        setPointCount(m_segments);
        setFillColor(sf::Color((m_color[0] * 255), (m_color[1] * 255), (m_color[2] * 255)));
        
    }

private:
    float m_radius = 20.0f;
    int m_segments = 32;
    void updateTextPosition()
    {
        sf::FloatRect textBounds = textName.getLocalBounds();
        sf::Vector2f circlePosition = sf::CircleShape::getPosition();
        float circleRadius = getRadius();
        textName.setPosition(
            circlePosition.x + circleRadius - textBounds.width / 2.f,
            circlePosition.y + circleRadius - textBounds.height / 2.f - textBounds.top
        );
    }
};

class Rectangle : public sf::RectangleShape, public Shape
{
public:
    Rectangle(sf::Vector2f Vector, const std::string& name)
        : sf::RectangleShape(Vector), Shape(name), m_vector(Vector)
    {
        sf::RectangleShape::setPosition(20.f, 10.f);
        textName.setPosition(getPosition());
    }

    void setFillColor(const sf::Color& color) override
    {
        sf::RectangleShape::setFillColor(color);
        m_color[0] = (color.r) / 255.f;
        m_color[1] = (color.g) / 255.f;
        m_color[2] = (color.b) / 255.f;
    }

    void setPosition(float x, float y) override
    {
        sf::RectangleShape::setPosition(x, y);
        updateTextPosition();
    }

    sf::Vector2f getPosition() const override
    {
        return sf::RectangleShape::getPosition();
    }

    sf::FloatRect getGlobalBounds() const override
    {
        return sf::RectangleShape::getGlobalBounds();
    }

    void draw(sf::RenderWindow& window) override
    {
        if (drawShape)
        {
            window.draw(*this);
            if (drawText) 
            {
                window.draw(textName);
            }
        }
    }
    void updateShape() override
    {
        sf::RectangleShape::setSize(sf::Vector2f(m_vector.x * meow_scale, m_vector.y * meow_scale));
        setFillColor(sf::Color((m_color[0] * 255), (m_color[1] * 255), (m_color[2] * 255)));
    }

private:
    sf::Vector2f m_vector;
    void updateTextPosition()
    {
        sf::FloatRect textBounds = textName.getLocalBounds();
        sf::FloatRect rectBounds = sf::RectangleShape::getGlobalBounds();

        textName.setPosition(
            rectBounds.left + (rectBounds.width / 2.f) - (textBounds.width / 2.f),
            rectBounds.top + (rectBounds.height / 2.f) - (textBounds.height / 2.f) - textBounds.top
        );
    }
};

void readFile(const std::string& filename, std::vector<std::shared_ptr<Shape>>& shapes)
{
    std::ifstream fileIn(filename);
    if (!fileIn.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(fileIn, line)) 
    {
        std::istringstream iss(line);
        std::string shapeType;
        std::string name;
        float position[2];
        float velocity[2];
        int color[3];
        float size[2];
        float radius;
        if ((iss >> shapeType))
        {
            if (shapeType == "Rectangle") 
            {
                iss >> name >> position[0] >> position[1] >> velocity[0] >> velocity[1] >> color[0] >> color[1] >> color[2] >> size[0] >> size[1];
                auto rectangle = std::make_shared<Rectangle>(sf::Vector2f(size[0], size[1]), name);
                rectangle->setPosition(position[0], position[1]);
                rectangle->setFillColor(sf::Color(color[0], color[1], color[2]));
                shapes.push_back(rectangle);
            }
            if (shapeType == "Circle")
            {
                iss >> name >> position[0] >> position[1] >> velocity[0] >> velocity[1] >> color[0] >> color[1] >> color[2] >> radius;
                auto circle = std::make_shared<Circle>(radius, 32, name);
                circle->setPosition(position[0], position[1]);
                circle->setFillColor(sf::Color(color[0], color[1], color[2]));
                shapes.push_back(circle);
            }
        }
        
    }

    fileIn.close();
}
int main() {
    int wHeight = 720;
    int wWidth = 1280;
    sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "Window Title");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    std::vector<std::shared_ptr<Shape>> shapes;

    readFile("shapes.txt", shapes);
    int currentShapeIndex = 0;
    sf::Clock deltaClock;
    while (window.isOpen()) 
    {
        sf::Event event;
        while (window.pollEvent(event)) 
        {
            ImGui::SFML::ProcessEvent(window, event);
            if (event.type == sf::Event::Closed) 
            {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());
       
        ImGui::Begin("Shape Controls", NULL, ImGuiWindowFlags_AlwaysAutoResize);
        std::vector<std::string> shapeNames;
        for (const auto& shape : shapes) 
        {
            shapeNames.push_back(shape->getName());
        }
        std::vector<const char*> shapeNamesCStr;
        for (const auto& name : shapeNames) 
        {
            shapeNamesCStr.push_back(name.c_str());
        }
        ImGui::Combo("Select Shape", &currentShapeIndex, shapeNamesCStr.data(), shapeNamesCStr.size());
        shapes[currentShapeIndex]->renderImGuiControls();
        shapes[currentShapeIndex]->updateShape();
        ImGui::End();

        for (auto& tempShape : shapes)
        {
            if (std::round(tempShape->getGlobalBounds().left + tempShape->getGlobalBounds().width) >= wWidth
                || std::round(tempShape->getGlobalBounds().left) <= 0)
            {
                tempShape->setSpeedX(-tempShape->getSpeedX());
            }
            if (std::round(tempShape->getGlobalBounds().top + tempShape->getGlobalBounds().height) >= wHeight
                || std::round(tempShape->getGlobalBounds().top) <= 0)
            {
                tempShape->setSpeedY(-tempShape->getSpeedY());
            }
            tempShape->setPosition(tempShape->getPosition().x + tempShape->getSpeedX(), tempShape->getPosition().y + tempShape->getSpeedY());
        }

        window.clear(sf::Color::Black);

        for (const auto& shape : shapes) 
        {
            shape->draw(window);
        }

        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}