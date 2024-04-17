#include
#include
using namespace std;

void functionGlobal(int num)
{
    cout << "Global Function: I am doing square: " << num * num << "\n";
}

class FuncObjectClass
{
public:
    void operator()(int num)
    {
        cout << "Function Object Class : I am tripling: " << 3 * num << "\n";
    }
};

class LocalStatic
{
public:
    static void functionStatic(int num)
    {
        cout << "Local Static Function: I am doubling: " << 2 * num << "\n";
    }
};

class LocalNonStaticMember
{
public:
    void functionMember(int num)
    {
        cout << "Non Static member function: I am dividing by 2: " << num / 2 << "\n";
    }
};

void (*funcPtr)(int) = functionGlobal;

class FunCallBackExample
{
public:
    using CallbackType = std::function;
    void setCallback(const CallbackType &cb)
    {
        callbackHandler = cb;
    }
    virtual void execute(int num)
    {
        callbackHandler(num);
    }

private:
    CallbackType callbackHandler;
};

int main()
{
    cout << "Hello World" << endl;
    FunCallBackExample ob;

    // calling FuncObjectClass
    FuncObjectClass foc;
    ob.setCallback(foc);
    ob.execute(10);

    // calling Local static method of a class
    ob.setCallback(LocalStatic::functionStatic);
    ob.execute(10);

    // calling using funtion pointer
    ob.setCallback(funcPtr);
    ob.execute(10);

    // calling a global function
    ob.setCallback(functionGlobal);
    ob.execute(10);

    // calling a non static member function of a class
    LocalNonStaticMember lns;
    /*
    You can't do something like this
    ob.setCallback(lns.functionMember);

    If you do, it will throw an error like below:
    main.cpp: In function ‘int main()’:
    main.cpp:82:37: error: invalid use of non-static member function ‘void LocalNonStatic::functionMember(int)’
    ob.setCallback(lns.functionMember);
    ^
    main.cpp:31:14: note: declared here
    void functionMember(int num)
    ^~~~~~~~~~~~~~
    */
    // For this, you have to use std::bind to bind a non static member function and 'this' comes
    // from the first argument
    ob.setCallback(std::bind(&LocalNonStaticMember::functionMember, lns, std::placeholders::_1));
    ob.execute(10);

    // Calling a closure
    ob.setCallback([](int num)
                   { cout << "From Closure: I am adding 5 to it: " << num + 5 << "\n"; });
    ob.execute(10);

    return 0;
}