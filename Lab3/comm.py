#!/usr/bin/python
#NEED TO FIX PROBLEM WITH DUPLICATES. FOR EXAMPLE: 
# A     A counts as matching
# A     B new A is considered unique
"""
Implementation of the POSIX comm command

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

Please see <http://www.gnu.org/licenses/> for a copy of the license.

$Id: randline.py,v 1.4 2017/10/19 16:58:00 sheldon Exp $
"""

import sys, string, locale
from optparse import OptionParser

class comm:
    #Initialize comm
    def __init__(self, dict, parser):
        self.key = dict
        file1 = self.key["file1"]
        file2 = self.key["file2"]
        
        try:
            if file1 == "-":
                f1 = sys.stdin
                f2 = open(file2, 'r')

            elif file2 == "-":
                f1 = open(file1, 'r')
                f2 = sys.stdin

            else:
                f1 = open(file1, 'r')
                f2 = open(file2, 'r')

            #Reads line without the \r\n 
            self.lines1 = f1.read().splitlines() #f1.read().splitlines()
            self.lines2 = f2.read().splitlines() #f2.read().splitlines()

            #add the \r\n because can't assume last line in text will be ended with enter
            self.lines1 = [x + "\n" for x in self.lines1]
            self.lines2 = [x + "\n" for x in self.lines2]

            self.column1 = []
            self.column2 = []
            self.column3 = []
            
            f1.close()
            f2.close()

        #If there is a problem reading one of the files
        except IOError as (errno, strerror):
            parser.error("I/O error({0}): {1}".format(errno, strerror))

        #If there is a -u option
        if self.key["unsort"]:
            self.unsortedComp()

        # there is no -u option
        elif not self.key["unsort"]:
            self.regComp()

    #Check if content of lists are sorted
    def inorder(self):
        sort1 = sorted(self.lines1)
        sort2 = sorted(self.lines2)
        ordered = True

        #If the sorted version of file1 is not the same as the original, file1 isn't sorted
        if sort1 != self.lines1:
            ordered = False
            sys.stderr.write("FILE1 not sorted\n") 

        #If the sorted version of file2 is not the same as the original, file2 isn't sorted
        if sort2 != self.lines2:
            ordered = False
            sys.stderr.write("FILE2 not sorted\n") 

        return ordered

    #compare if there is -u
    def unsortedComp(self):

        copy1 = self.lines1
        copy2 = self.lines2
        #get all words common to column 3

        self.column3 = set(copy1) & set(copy2)
        self.column3 = list(self.column3)

#        self.column3 = [lines for lines in copy1 if lines in copy2]
        #remove common words from lists
        for lines in self.column3:
            if lines in copy2:
                copy2.remove(lines)
#            if lines in copy1:
 #               copy1.remove(lines)

        self.column1 = copy1
        self.column2 = copy2

        self.writerUnsort()

    def regComp(self):
        if not self.inorder():
            return

        copy1 = self.lines1
        copy2 = self.lines2
#        self.column3 = set(copy1) & set(copy2)
#        self.column3 = list(self.column3)
        self.column3 = [lines for lines in copy1 if lines in copy2]

 #       print self.column3
        
        for lines in self.column3:
            if lines in copy1:
#                print "removed" + lines
                copy1.remove(lines)
            if lines in copy2:
                copy2.remove(lines)

        self.column1 = copy1
        self.column2 = copy2

        self.combined = sorted(copy1 + copy2 + self.column3)

        self.writerSort()


    def writerSort(self):
        #Look through the lines to figure out correct format
        for lines in self.combined:
            #if lies in column1 and column 1 is not surpressed
            if lines in self.column1 and not self.key["one"]:
           #     self.column1.remove(lines)
                print lines,

            
            elif lines in self.column2 and not self.key["two"]:                
                if not self.key["one"]:
                    #Print it in the second column
                    print "\t" + lines,
               
                elif self.key["one"]:
                    print lines,

            
            elif lines in self.column3 and not self.key["three"]:                
                if not self.key["one"]:                   
                    if not self.key["two"]:
                        print "\t\t"+lines,
                    
                    elif self.key["two"]:
                        print "\t"+lines,
                
                elif self.key["one"]:                    
                    if not self.key["two"]:
                        print "\t"+lines,
                    
                    elif self.key["two"]:
                       print lines,

    def writerUnsort(self):
        for lines in self.column1:

           if lines in self.column3:
               self.column3.remove(lines)
               if not self.key["one"]:
                   if not self.key["two"] and not self.key["three"]:
                       print "\t\t" + lines,

                   elif self.key["two"] and not self.key["three"]:
                       print "\t" + lines,

               elif self.key["one"]:
                   if not self.key["two"] and not self.key["three"]:
                       print "\t" + lines,

                   elif self.key["two"] and not self.key["three"]:
                       print lines,
           elif lines not in self.column3 and not self.key["one"]:
                print lines,

        for lines in self.column2:
            if not self.key["two"]:
                if not self.key["one"]:
                    print "\t" + lines,
                elif self.key["one"]:
                    print lines,


def main():
    locale.setlocale(locale.LC_ALL, 'C')
    version_msg = "%prog 2.0"
    usage_msg = """%prog [OPTION]... FILE1 FILE2
compare two sorted files line by line."""

    parser = OptionParser(version=version_msg, usage=usage_msg)
    parser.add_option("-1", action="store_true", dest="one", default=False, help="Suppress the output column of lines unique to file1")

    parser.add_option("-2", action="store_true", dest="two", default=False, help="Suppress the output column of lines unique to file2")

    parser.add_option("-3", action="store_true", dest="three", default=False, help="Suppress the output column of lines duplicated in file1 and file2")

    parser.add_option("-u", action="store_true", dest="u", default=False, help="Run comparison on unsorted files")
    
    options, args = parser.parse_args(sys.argv[1:])

    if len(args) <= 0:
        parser.error("missing operand")
    elif len(args) < 2:
        parser.error("missing operand after " + "'" + args[0] + "'")
    elif len(args) > 2:
        parser.error("extra operand " + "'" + args[2] + "'")

    d = {}
    d["one"] = options.one
    d["two"] = options.two
    d["three"] = options.three
    d["unsort"] = options.u
    d["file1"] = args[0]
    d["file2"] = args[1]

    commObj = comm(d, parser)

if __name__ == "__main__":
    main()
