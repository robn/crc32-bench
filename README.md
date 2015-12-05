# crc32-bench

A bench test for various CRC32 implementations.

This was built so we could test CRC32 performance with a view to replacing the
implementation used inside [Cyrus IMAP](https://cyrusimap.org/). For more
information, read https://blog.fastmail.com/2015/12/03/the-search-for-a-faster-crc32/

## implementations

- *oldcyrus*: The old, slow fallback implementation from Cyrus.
- *cyrus*: The current Cyrus implementation, a combo slice-by-16/slice-by-8 depending on input buffer size.
- *kernel*: Kernel implementation via the Crypto API. Internally it uses the PLCMULQDQ CPU instruction, with fallback to slice-by-8.
- *slice4*, *slice8*, *slice16*: table-based slice-by-N, all from [Stephan Brumme's CRC32 collection](http://create.stephan-brumme.com/crc32/). Each increase in N adds more tables and unrolls loops further, relying more and more on a modern CPU's ability to parallelise work.
- *slice16-prefetch*: same as *slice16* but with a strategically-placed instruction to force the CPU to bring the next 256 bytes into the CPU cache while it's doing the math.
- *zlib*: system zlib.

## adding your own

You can build the bench against any CRC32 implementation that has a `crc32()` function with prototype:

    unsigned long crc32(unsigned long crc, unsigned char *buf, unsigned long len);

This is standard for zlib, so its easy to drop in any zlib-based implementation, such as [zlib-cloudflare](https://github.com/cloudflare/zlib).

If you need setup and cleanup functions, implement 

    void crc32_init(void);
    void crc32_shutdown(void);

If you don't, link `bench-init.o`, which has stubs for these functions.

See `Makefile` for more info.

## copyright and license

- The *slice* implementations are [Copyright © 2014 Stephan Brumme](http://create.stephan-brumme.com/disclaimer.html).
- The *cyrus* implementation is Copyright © 1986 Gary S Brown.
- *crc32-kernel* and the bench itself is by me (Rob N) for FastMail, and is placed in the public domain.
