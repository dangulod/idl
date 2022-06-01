from typing import Iterator, List
from .._idlpy import Position, Hedge
from .transaction_ccr import TransactionCCR
from .cds import CDS
from .bond import Bond
from enum import Enum
from .utils import equal_position
from ..utils import Switch
from copy import copy
from typing import Union

class PositionCCR(Position):
    industry = ""
    country  = ""
    CAGID    = ""

    def __init__(self,
                 jtd: float,
                 notional: float,
                 rating: int,
                 region: int,
                 sector: int,
                 CAGID: str,
                 industry: str,
                 country: str,
                 idio_seed: int,
                 hedges: List[Hedge] = [],
                 transactions: Union[TransactionCCR, tuple] = ()) -> None:
        Position.__init__(self,
                          jtd       = float(jtd),
                          notional  = float(notional),
                          sector    = int(sector),
                          region    = int(region),
                          rating    = int(rating),
                          idio_seed = int(idio_seed),
                          hedges    = hedges)
        self.industry     = industry
        self.country      = country
        self.CAGID        = CAGID
        self.transactions = transactions

    def __copy__(self):
        return PositionCCR(jtd          = self.jtd_unhedged,
                           notional     = self.notional_unhedged,
                           rating       = self.rating,
                           region       = self.region,
                           sector       = self.sector,
                           CAGID        = self.CAGID,
                           industry     = self.industry,
                           country      = self.country,
                           idio_seed    = self.idio_seed,
                           hedges       = self.hedges,
                           transactions = self.transactions)

    @property
    def transactions(self) -> tuple:
        return self._transactions

    @transactions.setter
    def transactions(self,
                     value: Union[TransactionCCR, tuple]) -> None:
        if isinstance(value, TransactionCCR):
            self._transactions = tuple([value])
        else:
            self._transactions = value
    
    def __add__(self,
                value : Union[TransactionCCR, Hedge, CDS, Bond]) -> "PositionCCR":
        copied = copy(self)
        return AddIDLPosition(AddIDLPositionEnum(type(value).__name__), copied, value)
        
    
    def __iadd__(self,
                 value : Union[TransactionCCR, Hedge, CDS, Bond]) -> None:
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
    
    def __eq__(self,
               rhs: "PositionCCR") -> bool:

        for it_lhs, it_rhs in zip(self, rhs):
            if it_lhs != it_rhs:
                return False
        
        if not equal_position(self, rhs):
            return False

        return self.industry == rhs.industry and \
            self.country == rhs.country and \
            self.CAGID == rhs.CAGID


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