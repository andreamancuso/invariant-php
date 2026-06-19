--TEST--
Static methods do not trigger invariant checks
--EXTENSIONS--
invariant_php
--FILE--
<?php
class StaticMethodInvariant {
    private int $value = -1;

    public static function run(): void {
        echo "static\n";
    }

    public function __invariant(): void {
        echo "invariant\n";

        if ($this->value < 0) {
            throw new \LogicException("invalid");
        }
    }
}

StaticMethodInvariant::run();
?>
--EXPECT--
static
