from .._idlpy import Hedge

class CDS(Hedge):
    def __init__(self,
                 jtd: float,
                 notional:float,
                 rating: int) -> None:
        super().__init__(jtd, notional, rating)
