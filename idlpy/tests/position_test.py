from ctypes import pointer
import unittest
import idlpy

class TestHedge(unittest.TestCase):
    def test_hedge(self):
        hedge = idlpy.positions.Hedge(10, 9, 1)
        self.assertEqual(hedge.jtd, 10)
        self.assertEqual(hedge.notional, 9)
        self.assertEqual(hedge.rating, 1)

class TestPosition(unittest.TestCase):
    def test_constructor(self):
        self.assertEqual(idlpy.positions.Position(100, 90, 1, 2, 3, 123456789),
                         idlpy.positions.Position(100, 90, idlpy.WeightsDimension(1, 2, 3), 123456789))
    
    def test_attributes(self):
        position1 = idlpy.positions.Position(100, 90, 1, 2, 3, 123456789)
        self.assertEqual(position1.jtd, 100)
        self.assertEqual(position1.jtd_unhedged, 100)
        self.assertEqual(position1.notional, 90)
        self.assertEqual(position1.notional_unhedged, 90)
        self.assertEqual(position1.weight_dimension, idlpy.WeightsDimension(1, 2, 3))
        self.assertEqual(position1.rating, 1)
        self.assertEqual(position1.region, 2)
        self.assertEqual(position1.sector, 3)
        self.assertEqual(position1.idio_seed, 123456789)
        self.assertEqual(position1.hedges, [])

        position2 = idlpy.positions.Position(100, 90, idlpy.WeightsDimension(1, 2, 3), 123456789)
        self.assertEqual(position1.jtd, position2.jtd)
        self.assertEqual(position1.notional, position2.notional)
        self.assertEqual(position1.rating, position2.rating)
        self.assertEqual(position1.region, position2.region)
        self.assertEqual(position1.sector, position2.sector)
        self.assertEqual(position1.idio_seed, position2.idio_seed)

    def test_hedge_position(self):
        position1 = idlpy.positions.Position(100, 90, 1, 1, 1, 1,
                                             [idlpy.positions.Hedge(-10, -10, 1)])
        self.assertEqual(position1.jtd, 90)
        self.assertEqual(position1.jtd_unhedged, 100)
        self.assertEqual(position1.notional, 80)
        self.assertEqual(position1.notional_unhedged, 90)
        position2 = idlpy.positions.Position(100, 90, 1, 1, 1, 1,
                                             [idlpy.positions.Hedge(-10, -10, 1),
                                              idlpy.positions.Hedge(-10, -10, 1),
                                              idlpy.positions.Hedge(-10, -10, 1)])
        self.assertEqual(position2.jtd, 70)
        self.assertEqual(position2.jtd_unhedged, 100)
        self.assertEqual(position2.notional, 60)
        self.assertEqual(position2.notional_unhedged, 90)

    def test_add_transaction(self):
        position = idlpy.positions.PositionCCR(900, 1000, 5, 1, 3, "1", "1001", "ES", 1,
                                              [idlpy.positions.CDS(-100, -100, 1), idlpy.positions.CDS(-100, -100, 1)],
                                              tuple([idlpy.positions.TransactionCCR("1", "1", 900, 1000)]))

        self.assertEqual(len(position.transactions), 1)
        self.assertEqual(len(position.hedges), 2)

        position2 = position + idlpy.positions.TransactionCCR("1", "1", 900, 2000)
        self.assertEqual(len(position.transactions), 1)
        self.assertEqual(len(position2.transactions), 2)

        position += idlpy.positions.TransactionCCR("1", "1", 900, 2000)
        self.assertEqual(len(position.transactions), 2)

        self.assertEqual(position, position2)

        notional = 0
        jtd      = 0
        for it_transaction in position:
            notional += it_transaction.notional
            jtd      += it_transaction.jtd

        self.assertEqual(notional, 3000)
        self.assertEqual(jtd, 1800)
