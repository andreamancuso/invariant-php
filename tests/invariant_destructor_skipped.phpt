--TEST--
Destructor does not trigger invariant checks
--EXTENSIONS--
invariant_php
--FILE--
<?php
class DestructorInvariant {
    private int $value = -1;

    public function __destruct() {
        echo "destruct\n";
    }

    public function __invariant(): void {
        echo "invariant\n";

        if ($this->value < 0) {
            throw new \LogicException("invalid");
        }
    }
}

$obj = new DestructorInvariant();
unset($obj);
echo "done\n";
?>
--EXPECT--
destruct
done
