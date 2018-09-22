import os.path as path
import unittest
from unsafe_client import http_get, http_post, Response
from unittest import TestCase


class Test(TestCase):

    # @skip
    def test_get_foo(self):
        responses = [
            http_get(url='http://localhost:8080/foo'),
            http_get(url='http://localhost:8080/foo'),
        ]
        for response in responses:
            self.assertEqual("hello path GET foo", response.body)
            #self.assertEqual("there", respo nse.headers['hi'])
            self.assertEqual(200, response.status)

    # @skip
    def test_post_foo(self):
        responses = [
            http_post(url='http://localhost:8080/foo', data={}),
            http_post(url='http://localhost:8080/foo', data={}),
        ]
        for response in responses:
            self.assertEqual("hello path POST foo", response.body)
            #self.assertEqual("there", response.headers['hi'])
            self.assertEqual(200, response.status)


if __name__ == '__main__':
    unittest.main()
