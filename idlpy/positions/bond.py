from .._idlpy import Hedge

class Bond(Hedge):
    def __init__(self,
                 jtd: float,
                 notional: float,
                 rating: int) -> None:
        Hedge.__init__(self,
                       jtd      = float(jtd),
                       notional = float(notional),
                       rating   = int(rating))
