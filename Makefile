gen-docs: # Generate new documentation
	doxygen Doxyfile

clean: # Clean up old generated documentation
	rm -rfv docs/html docs/latex
