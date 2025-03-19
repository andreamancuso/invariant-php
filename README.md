# **Invariant PHP** 🚀

**Invariant PHP** is a native PHP extension that brings **Design by Contract (DbC)** principles to PHP. It **automatically enforces class invariants** _and ensures all typed properties are initialized_ without relying on slow runtime reflection.

## **🔥 Features**

✅ **Class Invariants (`__invariant()`)** – Ensures an object's state remains valid **before and after** each method call.
✅ **Enforced Initialization** – Detects **uninitialized typed properties** and raises a **fatal error** before `__invariant()` runs.
✅ **Native PHP Extension** – Written in C for **maximum performance**, avoiding overhead from reflection or AOP.
✅ **Transparent Execution** – Works automatically on all class methods **without** modifying user code.

## **🚀 Why?**

PHP lacks built-in support for **Design by Contract**. Existing solutions (like PHPDeal) rely on **runtime reflection** or **AOP hacks**, which can be slow or brittle. **Invariant PHP** is a **native extension** that makes contract checks **fast** and **reliable**, hooking directly into the **Zend Engine**.

## **📌 Example Usage**
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
$account->withdraw(50); // ✅ Works fine
$account->withdraw(100); // ❌ Fatal error or LogicException if below zero
```

## **📦 Installation**

Currently, you must **build from source**:
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

## **🛠 How It Works**

- **Hooks the Zend Engine** by overriding `zend_execute_ex`.
- **Checks typed properties** for initialization before calling `__invariant()`.
- **Throws a fatal error** if any typed property is uninitialized.
- **Automatically calls `__invariant()`** before & after each method execution.

## **⚡ Roadmap**

- 🔄 **Support `requires()` (preconditions)**.
- 🔄 **Support `ensures()` (postconditions)**.
- 📈 **Optimization & caching** for better performance.
- 📦 **PECL package for easy installation**.

---

**Want to contribute?** PRs and discussions are welcome! 🎯

