import unittest
import idlpy


class TestTransaction(unittest.TestCase):
    def test_transaction(self):
        transaction = idlpy.positions.TransactionCCR("1", "1", 900, 1000)
        self.assertEqual(transaction.CAGID, "1")
        self.assertEqual(transaction.GFCID, "1")
        self.assertEqual(transaction.notional, 1000)
        self.assertEqual(transaction.jtd, 900)

    @unittest.expectedFailure
    def test_transaction_constructor(self):
        idlpy.positions.TransactionCCR("1", "1", 900, "string")
        idlpy.positions.TransactionCCR("1", "1", "string", 1000)

