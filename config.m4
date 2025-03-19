PHP_ARG_ENABLE(invariant_php, whether to enable invariant_php support,
[  --enable-invariant_php   Enable invariant_php extension])

if test "$PHP_INVARIANT_PHP" != "no"; then
    AC_DEFINE(HAVE_INVARIANT_PHP, 1, [Have invariant_php])
    PHP_NEW_EXTENSION(invariant_php, invariant_php.c, $ext_shared)
fi
