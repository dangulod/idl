import unittest
import idlpy

class TestDistribution(unittest.TestCase):
    def test_uniform(self):
        uniform = idlpy.distributions.Uniform(-1, 1)
        self.assertEqual(uniform.lower, -1)
        self.assertEqual(uniform.upper, 1)
        self.assertEqual(len(uniform(21, 1)), 21)
        self.assertEqual(len(uniform(10, 1)), 10)
        self.assertEqual(uniform.quantile(0.5), 0.5 * (uniform.upper - uniform.lower) + uniform.lower)
        self.assertEqual(uniform.cdf(0.5), (0.5 - uniform.lower) / (uniform.upper - uniform.lower))
        self.assertEqual(uniform.pdf(0.5), 1 / (uniform.upper - uniform.lower))
        self.assertEqual(uniform.mean(), 0)

    def test_normal(self):
        normal = idlpy.distributions.Normal(0, 1)
        self.assertEqual(normal.mu, 0)
        self.assertEqual(normal.sd, 1)
        self.assertEqual(len(normal(21, 1)), 21)
        self.assertEqual(len(normal(10, 1)), 10)
        self.assertEqual(normal.cdf(0), 0.5)
        self.assertAlmostEqual(normal.pdf(0), 0.3989422804014327, delta = 7)
        self.assertAlmostEqual(normal.quantile(0.5), 0.5, delta = 7)
        self.assertAlmostEqual(normal.quantile(0.9), 1.2815515655446008, delta = 7)
        self.assertAlmostEqual(normal.quantile(0.95), 1.6448536269514724, delta = 7)
        self.assertAlmostEqual(normal.quantile(0.99), 2.3263478740408405, delta = 7)
        self.assertEqual(normal.mean(), 0)

    def test_beta(self):
        beta = idlpy.distributions.Beta(0.5, 0.5, 0, 1)
        self.assertEqual(beta.a, 0)
        self.assertEqual(beta.b, 1)
        self.assertEqual(beta.shape1, 0.5)
        self.assertEqual(beta.shape2, 0.5)
        self.assertEqual(len(beta(21, 1)), 21)
        self.assertEqual(len(beta(10, 1)), 10)
        self.assertEqual(beta.cdf(0.5), 0.5)
        self.assertAlmostEqual(beta.pdf(0.5), 0.6366197723675814, delta = 7)
        self.assertAlmostEqual(beta.quantile(0.5), 0.5, delta = 7)
        self.assertAlmostEqual(beta.quantile(0.9), 0.9755282581475768, delta = 7)
        self.assertEqual(beta.mean(), 0.5)

    def test_lognormal(self):
        from math import exp

        lognormal = idlpy.distributions.LogNormal(0, 1)
        self.assertEqual(lognormal.mu, 0)
        self.assertEqual(lognormal.sd, 1)
        self.assertEqual(len(lognormal(21, 1)), 21)
        self.assertEqual(len(lognormal(10, 1)), 10)
        self.assertAlmostEqual(lognormal.cdf(0.5), 0.24410859578558275, delta = 7)
        self.assertAlmostEqual(lognormal.pdf(0.5), 0.6274960771159244, delta = 7)
        self.assertAlmostEqual(lognormal.mean(), exp(lognormal.mu + (lognormal.sd ** 2) / 2), delta = 7)

if __name__ == '__main__':
    unittest.main()