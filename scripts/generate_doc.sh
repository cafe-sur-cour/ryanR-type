#!/bin/bash
cd documentation
rm -rf doc/
doxygen Doxyfile
rm -rf ./doc/html
cd doc/latex
make
mv refman.pdf ../
cd ..
rm -rf latex/
mv refman.pdf R-Type-Doc.pdf
cp R-Type-Doc.pdf ../docusaurus/static/pdfs/R-Type-Doc.pdf

