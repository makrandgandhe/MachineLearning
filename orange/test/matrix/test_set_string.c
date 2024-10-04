#include<matrix.h>

set_string* set1;


void setup()
{
    set1 = set_string_create_new();
    set_string_add(set1, "Hello World");
    set_string_add(set1, "Hello  World");
    set_string_add(set1, "Hello World");
    set_string_add(set1, "Hello World");
    set_string_add(set1, "Fish");
    set_string_add(set1, "Mammal");
    set_string_add(set1, "Mammal");
    set_string_print(set1, "SET 1");
}

void test_basic_set()
{

}


int main()
{
    setup();
}