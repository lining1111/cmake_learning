//
// Created by lining on 3/20/25.
//

#include <iostream>
#include <memory>

class Rect{
public:
    Rect(int width, int height) : m_width(width), m_height(height) {
        std::cout << "Rect constructor" << std::endl;
    }

    ~Rect() {
        std::cout << "Rect destructor" << std::endl;
    }

    int getWidth() const {
        return m_width;
    }

    int getHeight() const {
        return m_height;
    }

private:
    int m_width;
    int m_height;
};

int main() {
    {
        std::shared_ptr<Rect> rect1 = std::make_shared<Rect>(10, 20);
        std::shared_ptr<Rect> rect2 = rect1;
        std::shared_ptr<Rect> rect3(rect1);
        std::cout << "rect1 use count: " << rect1.use_count() << std::endl;
        rect1.reset();
        std::cout << "rect1 use count: " << rect1.use_count() << std::endl;
    }

};
