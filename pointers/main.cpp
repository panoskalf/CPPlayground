#include "demo_object.h"

int main(void)
{
    // 1.
    demo_object first("first", 10);

    // 2.
    demo_object second(first);

    // 3.
    demo_object temp("second", 20);

    // 4.
    second = temp;

    // 5. and 6. and 7. destructor
    temp = demo_object("third", 30);

    // 8. and 9. and 10. destructor
    demo_object moved(std::move(demo_object("temp_moved", 40)));

    temp = std::move(demo_object("temp_moved", 42));

    return 0;
}