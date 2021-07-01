.PHONY: All.hpp clean reset

All.hpp:
	.extra/depcheck

clean:
	$(if $(wildcard All.hpp),rm All.hpp)
	$(if $(wildcard .dirhash),rm .dirhash)

reset: clean All.hpp
