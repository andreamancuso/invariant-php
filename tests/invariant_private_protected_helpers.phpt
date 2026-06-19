--TEST--
Private and protected helper methods do not trigger intermediate invariant checks
--EXTENSIONS--
invariant_php
--FILE--
<?php
class HelperRepair {
    private int $value = 1;

    public function run(): void {
        $this->breakIt();
        $this->repairIt();
        echo "done\n";
    }

    private function breakIt(): void {
        $this->value = -1;
        echo "broken\n";
    }

    protected function repairIt(): void {
        $this->value = 1;
        echo "repaired\n";
    }

    public function __invariant(): void {
        if ($this->value < 0) {
            throw new \LogicException("invalid");
        }

        echo "invariant\n";
    }
}

(new HelperRepair())->run();
?>
--EXPECT--
broken
repaired
done
invariant
