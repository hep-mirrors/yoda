#! /usr/bin/env python

import yoda
import unittest

class TestReader(unittest.TestCase):
    def setUp(self):
        print "Set Up"

    def test_reader(self):
        reader = yoda.ReaderAIDA()

if __name__=='__main__':
    unittest.main()
