
class TransactionCCR:
    GFCID    = ""
    CAGID    = ""
    jtd      = 0
    notional = 0

    def __init__(self,
                 GFCID: str,
                 CAGID: str,
                 jtd: float,
                 notional:float) -> None:
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

    def __eq__(self, rhs: "TransactionCCR") -> str:
        if not isinstance(rhs, TransactionCCR): 
            return False
        
        return self.CAGID == rhs.CAGID and \
            self.CAGID == rhs.CAGID and \
            self.jtd == rhs.jtd and \
            self.notional == rhs.notional
