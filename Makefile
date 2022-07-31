build:
	dub build

test:
	dub test

clean:
	dub --root=grammar clean
	dub --root=grammargen clean
	dub clean
	$(MAKE) -f Makefile.grammar clean
	rm -rf $(shell cat .gitignore)
