import unittest
import idlpy

class TestUniform(unittest.TestCase):
    def test_upper(self):
        uniform = idlpy.distributions.Uniform(0, 1)
        self.assertEqual(len(uniform(1, 1)), 21)
        self.assertEqual(len(uniform(10, 1)), 10)
        self.assertEqual(uniform.cdf(0.5), 0.5)
        self.assertEqual(uniform.pdf(0.5), 1)


if __name__ == '__main__':
    unittest.main()