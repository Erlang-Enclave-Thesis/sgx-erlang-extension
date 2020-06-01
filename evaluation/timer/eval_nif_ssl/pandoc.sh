#!/bin/bash -e

#TODO move to Makefile

#this is how formated data in appendix were built
pandoc  -V fontsize=9pt --latex-engine xelatex -V geometry:"left=2cm,right=2cm,top=2cm,bottom=2cm" -o evaluation_ssl.pdf evaluation_ssl.pandoc
