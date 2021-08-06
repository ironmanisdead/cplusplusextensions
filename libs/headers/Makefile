.PHONY: All.hpp clean reset

All.hpp:
	.extra/depcheck

clean:
	$(if $(wildcard All.$(H_EXT)),rm All.hpp)
	$(if $(wildcard .dirhash),rm .dirhash)

reset: clean All.hpp
