DUB ?= dub

build:
	$(DUB) build

test:
	$(DUB) test

clean:
	$(DUB) --root=grammar clean
	$(DUB) --root=grammargen clean
	$(DUB) clean
	+$(MAKE) -f Makefile.grammar clean
	rm -rf $(shell cat .gitignore)
