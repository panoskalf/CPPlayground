#include "DummyRaw.h"

int main(void)
{
    // 1.
    DummyRaw first("first", 10);

    // 2.
    DummyRaw second(first);

    // 3.
    DummyRaw temp("second", 20);

    // 4.
    second = temp;

    // 5. and 6. and 7. destructor
    temp = DummyRaw("third", 30);

    // 8. and 9. and 10. destructor
    DummyRaw moved(std::move(DummyRaw("temp_moved", 40)));

    temp = std::move(DummyRaw("temp_moved", 42));

    return 0;
}