# Next Steps

## 1. Watch v0.1.1 for breakage

Give the semantics cleanup release some time before adding more behavior.

The useful feedback now is whether the new rules feel predictable in real code:

- invariants run after successful outermost public object operations
- constructors are checked after successful construction
- destructors, static methods, private/protected helper calls, and throwing methods are skipped
- nested same-object public calls do not trigger intermediate checks
- read-only public methods still trigger checks

Avoid adding new contract features immediately. The priority is confidence in the core behavior.

## 2. Add release-artifact smoke tests

The GitHub Actions matrix builds and tests inside each target image, but the release assets should also be smoke-tested as final `.so` files.

Add a job or step that:

1. Builds/exports the `.so` artifact.
2. Copies or mounts that exact artifact into the matching PHP image.
3. Runs a minimal script with `extension=/path/to/invariant_php.so`.
4. Confirms the extension loads and a tiny invariant example passes.

This proves the uploaded release asset itself is usable, not only that the build stage passed.

## 3. Add explicit opt-out controls

The next user-facing feature should probably be ergonomics for public methods that are known to be read-only.

Prefer explicit opt-out over automatic mutation detection. Automatic state-change detection is likely to be expensive, incomplete, or wrong in subtle PHP cases.

Possible API:

```php
#[Invariant\Skip]
public function getBalance(): int {
    return $this->balance;
}
```

Potential alternatives:

- INI setting to disable all invariant checks globally.
- Attribute to disable checks for a whole class.
- Attribute to mark a method as pure/read-only.

## Later

- Add `php --ri invariant_php` module info output.
- Expose extension version information.
- Clean up PECL packaging.
- Add a README section dedicated to exact runtime semantics.
- Consider preconditions/postconditions only after invariant behavior is stable.

The project should focus on trust and ergonomics before adding larger contract features.
