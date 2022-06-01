
class TransactionCCR:
    GFCID    = ""
    CAGID    = ""
    jtd      = 0
    notional = 0

    def __init__(self,
                 GFCID: str,
                 CAGID: str,
                 jtd: float,
                 notional: float) -> None:
        self.GFCID    = GFCID
        self.CAGID    = CAGID
        self.jtd      = jtd
        self.notional = notional

    def __repr__(self) -> str:
        return str(self.__class__.__name__) +\
            ": [" + \
            str(self.jtd) +\
            ", " + \
            str(self.notional) +\
            "]"

    @property
    def jtd(self) -> float:
        return self._jtd

    @jtd.setter
    def jtd(self,
            value: any) -> None:
        self._jtd = float(value)

    @property
    def notional(self) -> float:
        return self._notional

    @notional.setter
    def notional(self,
                 value: any) -> None:
        self._notional = float(value)

    def __eq__(self,
               rhs: "TransactionCCR") -> bool:
        if not isinstance(rhs, TransactionCCR): 
            return False
        
        return self.CAGID == rhs.CAGID and \
            self.GFCID == rhs.GFCID and \
            self.jtd == rhs.jtd and \
            self.notional == rhs.notional
