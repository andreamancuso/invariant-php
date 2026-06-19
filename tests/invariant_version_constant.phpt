--TEST--
Version is exposed through phpversion() and Invariant\VERSION
--EXTENSIONS--
invariant_php
--FILE--
<?php
$version = phpversion('invariant_php');

var_dump(is_string($version));
var_dump(defined('Invariant\\VERSION'));
var_dump(constant('Invariant\\VERSION') === $version);
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
