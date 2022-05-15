from typing import Iterator, List
from .._idlpy import Position, Hedge
from .transaction_ccr import TransactionCCR
from .cds import CDS
from .bond import Bond
from enum import Enum
from ..utils import Switch
from copy import copy

class PositionCCR(Position):
    industry = ""
    country  = ""
    CAGID    = ""

    transactions = ()

    def __init__(self,
                 jtd: float,
                 notional: float,
                 sector: int,
                 region: int,
                 rating: int,
                 CAGID: str,
                 industry: str,
                 country: str,
                 idio_seed: int,
                 hedges: List[Hedge] = [],
                 transactions: tuple = ()) -> None:
        super().__init__(jtd, notional, sector, region, rating, idio_seed, hedges)
        self.industry     = industry
        self.country      = country
        self.CAGID        = CAGID
        self.transactions = transactions

    def __copy__(self):
        return PositionCCR(self.jtd_unhedged,
                           self.notional_unhedged,
                           self.sector,
                           self.region,
                           self.rating,
                           self.CAGID,
                           self.industry,
                           self.country,
                           self.idio_seed,
                           self.hedges,
                           self.transactions)

    def __add__(self, value) -> "PositionCCR":
        copied = copy(self)
        return AddIDLPosition(AddIDLPositionEnum(type(value).__name__), copied, value)
        
    
    def __iadd__(self, value) -> None:
        AddIDLPosition(AddIDLPositionEnum(type(value).__name__), self, value)
        return self

    def __iter__(self) -> Iterator[TransactionCCR]:
        return iter(self.transactions)

    def __next__(self) -> Iterator[TransactionCCR]:
        while True:
            try:
                value = next(self)
            except StopIteration:
                break
        return value

class __addIDLPosition(Switch):
    def TransactionCCR(self, lhs: PositionCCR, rhs: TransactionCCR) -> None:
        lhs.transactions = (*lhs.transactions, rhs)
        return lhs

    def CDS(self, lhs: PositionCCR, rhs: CDS) -> None:
        lhs._add_hedge(rhs)
        return lhs

    def Bond(self, lhs: PositionCCR, rhs: Bond) -> None:
        lhs._add_hedge(rhs)
        return lhs
    
    def Hedge(self, lhs: PositionCCR, rhs: Hedge) -> None:
        lhs._add_hedge(rhs)
        return lhs

    def default(self, lhs: PositionCCR, *args) -> None:
        return lhs

class AddIDLPositionEnum(Enum):
    TransactionCCR = "TransactionCCR"
    CDS            = "CDS"
    Bond           = "Bond"
    Hedge          = "Hedge"

AddIDLPosition = __addIDLPosition(AddIDLPositionEnum)