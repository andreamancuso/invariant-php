<?php

if (!extension_loaded('invariant_php')) {
    fwrite(STDERR, "extension not loaded\n");
    exit(10);
}

final class SmokeInvariant
{
    private int $value;

    public function __construct(int $value)
    {
        $this->value = $value;
        echo "constructed\n";
    }

    public function keepValid(): void
    {
        $this->value = 2;
        echo "valid\n";
    }

    public function breakIt(): void
    {
        $this->value = -1;
        echo "invalid\n";
    }

    public function __invariant(): void
    {
        if ($this->value < 0) {
            throw new LogicException('smoke invalid');
        }

        echo "invariant {$this->value}\n";
    }
}

$object = new SmokeInvariant(1);
$object->keepValid();
$object->breakIt();
echo "unreachable\n";
