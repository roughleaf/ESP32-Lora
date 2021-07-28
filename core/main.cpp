#include <stdio.h>
#include <string>
#include <iostream>

class test
{
    public:
    int test1 = 0;
    std::string testStr = "Test String";

    void writesomething(void)
    {
        std::cout << testStr << std::endl;
    };
};

extern "C" void app_main(void)
{
    test bla;

    while (true)
    {
        bla.writesomething();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
