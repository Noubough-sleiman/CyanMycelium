## Style

- CamelCase naming is used EXCEPT for C function names.
- C function names use lower case snake_case.
- C functions belonging to libraries are prefixed with the library identifier (all capitals, may be an abbreviation).
- Static C functions are declared at the top of the source file and are prefixed with `_`.
- Types start with capitals.
- Variables and function parameters start with lowercase.
- Method names start with uppercase.
- Class declaration should follow the order: public, protected, private.
- Constants and Defines are SNAKE_CASE in all capital letters.
- Private and Protected object data is prefixed with `_`.
- Tabs are forbidden, and the indentation unit is double-space.
- C headers use the `.h` extension.
- C++ headers use the `.hpp` extension.
- C sources use the `.c` extension.
- C++ sources use the `.cpp` extension.
- All files are named using lower case snake_case.
- Never use the `using` keyword in a header file.
- Include guards in all headers. Guard format must follow the example.
- Omission of `{}` for blocks is only allowed in single-line format.
- Keep line length below 80 characters when possible.
- Initialize member variables using an initializer list.
- Forward declare when possible.
- Use namespaces as much as possible.
- Use `<>` only for system includes.
- Use `""` for including local files.
- Brackets `{}` are always placed on their own line, except for single-line blocks format, which is allowed for accessors.
- Variables declared on the stack are prefixed with `l`. An exception is made for single-letter index variables.
- C++ enum items must always be scoped. The simplest way is using `enum class`.
- C enum items must be prefixed with the enum name.
- Enum item names are constants and use SNAKE_CASE in all capital letters.

### Special considerations for embedded (BluePanda)

- Use C `const char *` for strings (std::string is forbidden).
- C++ exceptions must not be used.
- Use of goto patterns for error handling is encouraged.
- Avoid the use of C++ standard libraries that make extensive use of dynamic allocation.
- BluePanda headers (drivers, libraries, core, net) should be considered system includes and included using `<>`, except for local includes within a module.
- Inside methods, always scope class variables using `this->` prefix.
- Methods that can be safely called from an ISR context must be prefixed with `ISR_`.

### cpp_header.hpp

```cpp
#ifndef __CPP_HEADER_HPP__    // C++ Guards format
#define __CPP_HEADER_HPP__    // Use file name

class MyOtherClass;           // Forward-declare instead of including
                              // when possible (reduce compile time)

namespace MyNamespace         // Use namespaces
{

class MyClass                 // Type starts with capitals
{
public:                       // Public elements go first
    MyClass(int dataValue)    // Parameters start with lowercase
        : _data(t_data)       // Use initializer list
    {
        _data = dataValue;    // Forbidden
    }

    int GetData() const       // Methods start with uppercase
    {
        return this->data;
    }

    // Single-line format is allowed for inline methods
    void SetData(int value) { _data = value; }

    const int DEF_VALUE = 1;  // Constants are SNAKE_CASE

protected:                    // Protected elements go after public

private:                      // Private elements are the last
    int _data;                // Private data is prefixed with _
    int _dataExample;

    // Private data is prefixed with _
    int _dataExample;
    MyOtherClass* _pointer;  // Forward-declaration is sufficient here
};

}  // namespace MyNamespace

#endif
```

### cpp_file.cpp

```cpp
#include <cstring>       // Only use <> for system includes
#include <osal.h>        // BluePanda headers are considered
                         // as system includes
#include "cpp_header.hpp"  // Use "" for including local files

using MyNamespace;       // Using only allowed in source files

enum class MyEnum        // Enum items are scoped thanks to
{                        // enum class declaration
    ITEM_ONE,            // Enum items names are constants
    ITEM_TWO,            // and use SNAKE_CASE
};

```

### c_header.h

```c
#ifndef __C_HEADER_H__     // C Guards format
#define __C_HEADER_H__     // Use file name

#ifdef __cplusplus
extern "C" {
#endif

#define VALUE 1            // Defines are SNAKE_CASE

// C library functions are prefixed with the library identifier
int MYLIB_do_something(int someParameter);

#ifdef __cplusplus
}
#endif

#endif
```

### c_file.c

```c
#include <string.h>   // Only use <> for system includes
#include <osal.h>     // BluePanda headers are considered
                       // as system includes
#include "c_header.h"  // Use "" for including local files

// Static C functions are declared at the top of the source file
static int _print_number(int number);

typedef enum
{
    MyCEnum_ITEM_ONE,  // C enum items are prefixed with
    MyCEnum_ITEM_TWO,  // enum name and use SNAKE_CASE
} MyCEnum;

int MYLIB_do_something(int someParameter)
{
    // Variables declared on the stack are prefixed with l
    int ls;

    // Allowed missing {} for single-line format
    if ((ls = some_function()) < 0)
        return ls;

    // Forbidden - No {} around block
    for (int i = 0; i < someParameter; i++)
        printf("%d", i);

    // Index variables (i) are allowed to drop the l prefix
    for (int i = 0; i < someParameter; i++)
    {
        printf("%d", i);
    }
}

// Static C functions are prefixed with _
static int _print_number(int number)
{
    printf("%d", number);
}
```
