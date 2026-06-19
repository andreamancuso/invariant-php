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
- `docker build --build-arg PHP_VERSION=8.3 -t invariant-php-dev:8.3 .`
- `docker build --build-arg PHP_VERSION=8.4 --build-arg PHP_VARIANT=cli-alpine -t invariant-php-dev:8.4-musl .`

Or with Docker Compose:

- `docker compose build`
- `docker compose run --rm test`

### Release Artifacts

GitHub Actions builds NTS Linux extension binaries for:

- PHP 8.3 and 8.4
- glibc and musl
- x86_64 and arm64

The x86_64 jobs run on `ubuntu-latest`; arm64 jobs run natively on
`ubuntu-24.04-arm`.

Tagged pushes matching `v*` attach the generated `.so` files to the GitHub release.

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
