BIN := bin
assetsbin := Assets/$(BIN)
outfile := HideNSeek

DUMMY != mkdir -p $(BIN)

default:
	@$(MAKE) --no-print-directory -C src -f ../Makefile_Clang.mk
	cp src/build/$(outfile)_P.rel $(BIN)/$(outfile)_P.rel
	cp src/build/$(outfile)_E.rel $(BIN)/$(outfile)_E.rel
	cp src/build/$(outfile)_J.rel $(BIN)/$(outfile)_J.rel
	cp src/build/$(outfile)_K.rel $(BIN)/$(outfile)_K.rel

clean:
	rm -rf loader/build
	rm -rf src/build
	rm -rf $(BIN)

fresh:
	@$(MAKE) --no-print-directory clean
	@$(MAKE) --no-print-directory default