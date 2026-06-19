### Development Commands

- `phpize`
- `./configure`
- `make -j$(nproc)`
- `make test TESTS='-q tests'`

**Recompilation:**

- `make clean && make -j$(nproc)`

### Docker Development

Use Docker when local PHP extension tooling is not installed:

- `docker build -t invariant-php-dev .`
- `docker run --rm invariant-php-dev`

Or with Docker Compose:

- `docker compose build`
- `docker compose run --rm test`

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
