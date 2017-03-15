# hyrax

Hyrax is a quantitative phase model to study hydrogen (as hydrogen in solution, and delta-hydrides) in alpha-zirconium.

#Major Changelog

15 March 2017
- Changed CALPHAD H2 gas energy expression to the one available in Thermocalc's PURE5 database.  This information has been added to all Hyrax input files.  All user input files that use CalphadAB1CD1Material and CalphadAB1CD2Material need to be updated (should error out if not updated).