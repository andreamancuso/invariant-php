--TEST--
Nested public calls on different objects each trigger their own invariant checks
--EXTENSIONS--
invariant_php
--FILE--
<?php
class InnerObject {
    public function work(): void {
        echo "inner work\n";
    }

    public function __invariant(): void {
        echo "inner invariant\n";
    }
}

class OuterObject {
    public function run(InnerObject $inner): void {
        echo "outer start\n";
        $inner->work();
        echo "outer end\n";
    }

    public function __invariant(): void {
        echo "outer invariant\n";
    }
}

(new OuterObject())->run(new InnerObject());
?>
--EXPECT--
outer start
inner work
inner invariant
outer end
outer invariant
