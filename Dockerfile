ARG PHP_VERSION=8.4
ARG PHP_VARIANT=cli

FROM php:${PHP_VERSION}-${PHP_VARIANT} AS build

WORKDIR /usr/src/invariant_php

COPY . .

RUN if command -v apk >/dev/null 2>&1; then \
        apk add --no-cache --virtual .build-deps $PHPIZE_DEPS linux-headers; \
    fi \
    && phpize \
    && ./configure --enable-invariant_php \
    && make -j"$(nproc)"

FROM build AS test

RUN make test TESTS="-q tests"

FROM scratch AS artifact

COPY --from=test /usr/src/invariant_php/modules/invariant_php.so /invariant_php.so

FROM build AS dev

CMD ["make", "test", "TESTS=-q tests"]
