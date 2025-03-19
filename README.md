# **Invariant PHP** 🚀  

**Invariant PHP** is a native PHP extension that brings **Design by Contract (DbC)** principles to PHP.  
It **automatically enforces class invariants** without relying on slow runtime reflection.

## **🔥 Features**  
✅ **Class Invariants (`__invariant()`)** – Ensures an object's state remains valid before and after method execution.  
✅ **Native PHP Extension** – Written in **C** for maximum performance, avoiding runtime overhead.  
✅ **Transparent Execution** – Works automatically on all class methods without modifying method calls.

## **🚀 Why?**  
PHP lacks **built-in support for contracts** like Eiffel. Existing solutions (like PHPDeal) rely on **slow runtime reflection and AOP hacks**.  
**Invariant PHP is a native extension** that makes contracts **fast, reliable, and easy to use.**  

## **📌 Example Usage**  
```php
class BankAccount {
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
$account->withdraw(100); // ❌ Throws LogicException (balance cannot be negative)
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
- **Automatically executes `__invariant()`** before and after method calls.  
- **Only applies to classes that define `__invariant()`**.  
- **Uses low-level Zend Engine hooks** to avoid performance overhead.  

## **⚡ Roadmap**  
- 🔄 **Support `requires()` (preconditions)**.  
- 🔄 **Support `ensures()` (postconditions)**.  
- 📈 **Optimization & caching** for better performance.  
- 📦 **PECL package for easy installation**.  

---

**Want to contribute?** PRs and discussions are welcome! 🎯

