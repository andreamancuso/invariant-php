# Invariant PHP

**Invariant PHP** is a native PHP extension that brings [Design by Contract (DbC)](https://en.wikipedia.org/wiki/Design_by_contract) principles to PHP. It **automatically enforces class invariants** _and ensures all typed properties are initialized_ without relying on reflection.

## License

Released under the [PHP License 3.01](https://www.php.net/license/3_01.txt).

## Features

- **Class Invariants (`__invariant()`)** – Ensures an object's state remains valid after successful outermost public method calls.
- **Enforced Initialization** – Detects **uninitialized typed properties** at invariant check time and raises a **fatal error** before `__invariant()` runs.
- **Native PHP Extension** – Written in C, avoiding overhead from reflection or AOP.
- **Transparent Execution** – Define a public instance `__invariant()` method and you are good to go.

## Why?

PHP lacks built-in support for **Design by Contract**. Existing solutions (like PHPDeal) rely on **runtime reflection** or **AOP hacks**, which can be slow or brittle. **Invariant PHP** hooks directly into the **Zend Engine**.

## Example Usage
```php
<?php

class BankAccount {
    // Typed properties must be initialized or you'll get a fatal error!
    private int $balance;

    public function __construct(int $balance) {
        $this->balance = $balance;
    }

    public function withdraw(int $amount) {
        $this->balance -= $amount;
    }

    public function __invariant(): void {
        if ($this->balance < 0) {
            throw new \LogicException("Invariant failed: Balance cannot be negative.");
        }
    }
}

$account = new BankAccount(100);
$account->withdraw(50); // Works fine
$account->withdraw(100); // Throws LogicException if the balance becomes negative
```

## Limitations

Invariant PHP does not automatically detect whether a method changed object state. Public read-only methods still trigger invariant checks. Destructors are skipped, and invariants are not run after methods that throw.

## Installation

Download a prebuilt NTS Linux extension from the GitHub releases page when one matches your platform:

- PHP 8.3 or 8.4
- x86_64 or arm64
- glibc or musl

Rename the downloaded artifact to `invariant_php.so`, place it in your PHP extension directory, and enable it in `php.ini`:
```ini
extension=invariant_php.so
```

Or build from source:
```sh
phpize
./configure
make -j$(nproc)
sudo make install
```
Then enable it in `php.ini`:
```ini
extension=invariant_php.so
```

Verify the loaded extension:

```sh
php --ri invariant_php
php -r "var_dump(phpversion('invariant_php'), Invariant\VERSION);"
```

## Runtime Semantics

Invariant PHP hooks the Zend Engine and checks object invariants at public object boundaries.

`__invariant()` runs after:

- successful outermost public instance method calls
- successful constructor calls, after `__construct()` has initialized the object
- magic public object calls such as `__invoke()` and calls handled by `__call()`

`__invariant()` does not run after:

- private or protected method calls
- static method calls
- destructors
- methods or constructors that throw
- nested public calls on the same object before the outermost call returns

Nested public calls on different objects each keep their own invariant boundary. A public method that calls another public method on the same object triggers one check when the outermost method returns.

Before calling `__invariant()`, the extension checks declared non-static typed properties. If any typed property is uninitialized, PHP raises a fatal error and `__invariant()` is not called.

`__invariant()` must be a public instance method to run. Private, protected, or static `__invariant()` methods are ignored.

Read-only public methods still trigger invariant checks. The extension does not inspect whether a method mutated object state.

## Version Information

The extension exposes its version in three ways:

```sh
php --ri invariant_php
```

```php
phpversion('invariant_php');
Invariant\VERSION;
```

Both PHP-level APIs return the same version string.

## How It Works

- **Hooks the Zend Engine** by overriding `zend_execute_ex`.
- **Checks typed properties** for initialization at successful public boundary checks before calling `__invariant()`.
- **Throws a fatal error** if any typed property is uninitialized.
- **Automatically calls `__invariant()`** after successful outermost public method execution.
- **Runs constructor checks after initialization**, not before `__construct()`.

## Roadmap

- Explicit opt-out controls for public methods that should skip invariant checks.
- Optimization and caching for better performance.
- PECL package for easy installation.
- Future Design by Contract features, including preconditions and postconditions, deserve a separate design pass.

---

**Want to contribute?** PRs and discussions are welcome! 🎯

