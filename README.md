#INI configuration file parser

This is very simple parser written in `C++ using STL`. Parser will validate the correctness of the configuration file and allows you to query for the keys using template functions. You just need to include the header and .cpp file for using this.

Typical Usage is as follows 

```cpp
#include "IniParser.h"
int main( int argc, char ** argv){
        IniParser myparser;
        int intValue;
        try {
                myparser.Load(argv[1]);
                myparser.QueryKeyValue<int>("Section4","KeyInt",intValue);
        } catch ( IniParseException &e) {
                std::cout << e.what();
                exit (1);
        }
        return 0;
}
```

`IniParser::Load` take file name and parse the file, if found any error then it will throw `IniParseException` exception.

## Querying Key value

Following API will need to use for querying key. 

```cpp
template < typename ValueType> void QueryKeyValue(const std::string &section, 
                                                        const std::string &key, 
                                                        ValueType &defaultValue
                                                        const throw (IniParseException) 
```

Usage of the API is as follows

If you know the type of the value is int then call this function as follows 

```cpp
int value;
myparser.QueryKeyValue<int>("<SECTION>","<KEY>",intValue);
```
If this call does not throw any exception then `intValue` will have required value. 
                                                        
                                                        
