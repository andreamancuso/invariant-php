### Development Commands

- `phpize`
- `./configure`
- `make -j$(nproc)`

**Recompilation:**

- `make clean && make -j$(nproc)`

### Development Links

- [PHP Internals Book: Zend Extensions](https://www.phpinternalsbook.com/php7/extensions_design/zend_extensions.html)

### VS Code Configuration (c_cpp_properties.json)

```json
{
    "configurations": [
        {
            "name": "Linux",
            "includePath": [
                "${workspaceFolder}/**",
                "/usr/include/php",
                "/usr/include/php/main",
                "/usr/include/php/Zend",
                "/usr/include/php/TSRM"
            ],
            "defines": [],
            "compilerPath": "/usr/bin/gcc",
            "cStandard": "c11",
            "cppStandard": "c++17",
            "intelliSenseMode": "linux-gcc-x64"
        }
    ],
    "version": 4
}