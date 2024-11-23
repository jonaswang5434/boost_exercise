#include <iostream>
#include <memory>
// 产品接口
class Shape{
    public:
    // 虚函数：在基类中声明，在派生类中重新，常用于动态绑定，即运行时再确定调用哪个函数，而不是在编译时。
    virtual void draw()=0; 
    virtual ~Shape()=default; //类中定义了虚函数draw(),在析构时也需要虚函数，否则通过指针删除派生类对象时，析构函数不会被调用。
};
// 具体产品类
class Cricle:public Shape // 调用基类Shape
{ 
    public:
    void draw() override{ // override:重写了draw()函数
        std::cout<<"Drawing circle" << std::endl;
    }
};
class Square:public Shape
{
    public:
    void draw() override{
        std::cout<<"Drawing Square" << std::endl;
    }
};
//创建者（工厂方法）
class ShapeFactory
{
    public:
    virtual std::shared_ptr<Shape> createShape()=0;
    virtual ~ShapeFactory()=default;
};

//具体创建者
class CricleFactory:public ShapeFactory
{
    public:
    std::shared_ptr<Shape> createShape() override  //override 重写了createShape函数，返回shape类型的指针
    {
        return std::make_shared<Cricle>();  // 调用了Cricle默认构造函数，返回Cricle类的的共享指针，后面的()中的内容是传递Circle构造函数的参数列表，此处为空。
        /* make_shared将基类型指针Shape指向Cricle，可参考：Shape * shape =& Cricle  这里是隐式转换，可以将基类型指针Shape 指向派生类Cricle对象 */
    }
};
class SquareFactory:public ShapeFactory
{
    public:
    std::shared_ptr<Shape> createShape() override
    {
        return std::make_shared<Square>();  // 创建square对象
    }
};
int main()
{
    std::shared_ptr<ShapeFactory> factory=std::make_shared<CricleFactory>(); //
    std::shared_ptr<Shape> shape=factory->createShape();//使用工厂方法创建对象
    shape->draw(); // 通过指针调用了shape中的draw()函数，输出Drawing Circle

    factory=std::make_shared<SquareFactory>();
    shape=factory->createShape();
    shape->draw(); // 通过指针调用了draw函数，输出Drawing squre
    return 0;

}