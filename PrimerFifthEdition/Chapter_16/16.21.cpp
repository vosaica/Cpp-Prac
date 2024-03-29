#include <iostream>
#include <memory>

class DebugDelete
{
public:
    DebugDelete(std::ostream& s = std::cerr) : os(s)
    {
    }

    template <typename T>
    void operator()(T* p) const
    {
        os << "deleting unique_ptr" << std::endl;
        delete p;
    }

private:
    std::ostream& os;
};

int main()
{
    auto* p = new double;
    DebugDelete d;
    d(p);

    auto* ip = new int;
    DebugDelete()(ip);

    return 0;
}
