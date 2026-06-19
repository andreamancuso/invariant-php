FROM php:8.4-cli

WORKDIR /usr/src/invariant_php

COPY . .

RUN phpize \
    && ./configure --enable-invariant_php \
    && make -j"$(nproc)"

CMD ["make", "test", "TESTS=-q tests"]
