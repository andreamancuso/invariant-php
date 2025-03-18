# **Invariant PHP** 🚀  

**Invariant PHP** is a native PHP extension that introduces **Design by Contract (DbC)** principles, allowing developers to enforce **class invariants, preconditions, and postconditions** at the language level.  

## **🔥 Features**  
✅ **Class Invariants** – Ensure an object's state remains valid throughout its lifecycle.  
✅ **Preconditions (`requires()`)** – Define conditions that must be met before a method executes.  
✅ **Postconditions (`ensures()`)** – Define conditions that must hold after a method executes.  
✅ **Native PHP Extension** – Written in **C** for maximum performance, avoiding runtime overhead.  

## **🚀 Why?**  
PHP lacks **built-in support for contracts** like Eiffel. Existing solutions (like PHPDeal) rely on **slow runtime reflection and AOP hacks**. Invariant PHP is a **native extension** that makes contracts **fast, reliable, and easy to use.**  

## **📌 Example Usage**  
```php
class BankAccount {
    private int $balance;

    public function __construct(int $balance) {
        $this->balance = $balance;
    }

    public function withdraw(int $amount) {
        requires($amount > 0, "Amount must be positive.");
        $this->balance -= $amount;
        ensures($this->balance >= 0, "Balance should never be negative.");
    }

    public function __invariant(): void {
        if ($this->balance < 0) {
            throw new \LogicException("Invariant failed: Balance cannot be negative.");
        }
    }
}
