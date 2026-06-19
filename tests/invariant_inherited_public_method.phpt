--TEST--
Inherited public methods trigger the child object's invariant
--EXTENSIONS--
invariant_php
--FILE--
<?php
class ParentAction {
    public function run(): void {
        echo "run\n";
    }
}

class ChildAction extends ParentAction {
    private int $value = 1;

    public function __invariant(): void {
        echo "child invariant {$this->value}\n";
    }
}

(new ChildAction())->run();
?>
--EXPECT--
run
child invariant 1
